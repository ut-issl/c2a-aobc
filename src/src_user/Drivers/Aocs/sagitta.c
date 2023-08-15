#pragma section REPRO
/**
* @file   sagitta.c
* @brief  SAGITTAのDriver
*/
#include "sagitta.h"

#include <src_core/Library/print.h>
#include <string.h> // for memcpy

#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/xxhash.h>
#include <src_user/Library/physical_constants.h>

#define DRIVER_SAGITTA_DEBUG_SHOW_REC_DATA

#define SAGITTA_STREAM_TLM_CMD        (0)
#define SAGITTA_TX_MAX_FRAME_SIZE     (80)  //!< TXフレームの最大バイト数。
#define SAGITTA_RX_MAX_FRAME_SIZE     (350) //!< RXフレームの最大バイト数。

#define SAGITTA_TX_HEADER_SIZE        (1)  //!< TXのヘッダーのサイズ。
#define SAGITTA_RX_HEADER_SIZE        (2)  //!< RXのヘッダーのサイズ。
#define SAGITTA_FOOTER_SIZE           (1)  //!< フッターのサイズ。TX,RXともに同じフッターを用いる。
#define SAGITTA_ESCAPE_SIZE           (2)  //!< ヘッダー・フッターと文字が被った際のエスケープバイト数。
#define SAGITTA_XXHASH_SIZE           (4)  //!< チェックサムに用いられるxxHashのバイト数

static const uint8_t SAGITTA_kTxHeader_[SAGITTA_TX_HEADER_SIZE] = { 0xc0 }; //!< TXのヘッダー
static const uint8_t SAGITTA_kRxHeader_[SAGITTA_RX_HEADER_SIZE] = { 0xc0, 0x21 }; //!< RXのヘッダー
static const uint8_t SAGITTA_kFooter_[SAGITTA_FOOTER_SIZE]      = { 0xc0 }; //!< TX,RXのフッター
static const uint8_t SAGITTA_kEscapeEnd_[SAGITTA_ESCAPE_SIZE] = { 0xdb, 0xdc };    //!< データ内に0xc0が含まれる場合はこの値を用いる
static const uint8_t SAGITTA_kEscapeEsc_[SAGITTA_ESCAPE_SIZE] = { 0xdb, 0xdd };    //!< データ内に0xdcが含まれる場合はこの値を用いる
static uint8_t SAGITTA_tx_frame_[SAGITTA_TX_MAX_FRAME_SIZE];      //!< 送信するデータを格納するTXフレーム
static uint8_t SAGITTA_tx_data_frame_[SAGITTA_TX_MAX_FRAME_SIZE]; //!< エンコード前のヘッダー・フッター以外のTXデータを格納する
static uint8_t SAGITTA_tx_frame_length_;
static uint8_t SAGITTA_rx_frame_decoded_[SAGITTA_RX_MAX_FRAME_SIZE]; //!< デコード後のRXデータを格納する
static const uint8_t SAGITTA_kAddress_             = 0x21;  //!< テレコマに含まれるSTTのアドレス。複数のSTTを用いる場合はそれぞれ異なるアドレスが付与される。
static const uint8_t SAGITTA_kCmdSetParameter_     = 0x00;
static const uint8_t SAGITTA_kCmdRequestParameter_ = 0x01;
static const uint8_t SAGITTA_kCmdRequestTelem_     = 0x02;
static const uint8_t SAGITTA_kCmdAction_           = 0x03;

static const uint8_t SAGITTA_kParameterRegion_ = 0x01;

static const uint8_t SAGITTA_kTlmOffsetTlmType_   = 2;
static const uint8_t SAGITTA_kTlmOffsetTlmID_     = 3;
static const uint8_t SAGITTA_kTlmOffsetTlmStatus_ = 4;
static const uint8_t SAGITTA_kTlmOffsetUnixTime_  = 9;
static const uint8_t SAGITTA_kTlmOffsetData_      = 17;
static const uint8_t SAGITTA_kTlmSizeSetParameterReply_  = 8;

static const uint16_t SAGITTA_kXxhashSeed_ = 1425; //!< Reference: DATA INTERFACE CONTROL DOCUMENT Ver.1.2: 2.3 Transport layer

static DS_ERR_CODE SAGITTA_load_driver_super_init_settings_(DriverSuper* p_super);
static DS_ERR_CODE SAGITTA_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver);

// parameter
static DS_ERR_CODE SAGITTA_analyze_rec_data_set_parameter_reply_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_reply_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_log_level_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_limits_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_mounting_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_camera_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_image_processor_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_centroiding_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_lisa_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_matching_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_tracking_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_validation_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_algo_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_subscription_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_auto_threshold_(SAGITTA_Driver* sagitta_driver);
// action
static DS_ERR_CODE SAGITTA_analyze_rec_data_action_reply_(SAGITTA_Driver* sagitta_driver);
// telemetry
static DS_ERR_CODE SAGITTA_analyze_rec_data_telemetry_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_unix_time_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_power_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_quaternion_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_temperature_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_histogram_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_blobs_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_centroids_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_auto_blob_(SAGITTA_Driver* sagitta_driver);
static DS_ERR_CODE SAGITTA_analyze_rec_data_matched_centroids_(SAGITTA_Driver* sagitta_driver);

static void SAGITTA_analyze_rec_data_xxhash_(SAGITTA_Driver* sagitta_driver, uint16_t rx_data_len);
static SAGITTA_REC_ERR_CODE SAGITTA_decode_rx_frame_(SAGITTA_Driver* sagitta_driver,
                                                     const uint8_t* sagitta_rx_data, const uint16_t sagitta_rx_data_length);
static DS_CMD_ERR_CODE SAGITTA_send_cmd_(SAGITTA_Driver* sagitta_driver, const uint8_t cmd_data_len);
static DS_CMD_ERR_CODE SAGITTA_request_tlm_(SAGITTA_Driver* sagitta_driver);

static DS_CMD_ERR_CODE SAGITTA_set_tx_frame_(uint8_t cmd_data_len);
static int SAGITTA_set_tx_frame_header_();
static int SAGITTA_set_tx_frame_xxhash_(uint8_t cmd_data_len);
static int SAGITTA_set_tx_frame_footer_(uint8_t cmd_offset_len);
static int SAGITTA_encode_tx_frame_(uint8_t cmd_data_len);
static void SAGITTA_memcpy_float_from_rx_frame_decoded_(float* output, uint16_t offset);
static void SAGITTA_memcpy_u32_from_rx_frame_decoded_(uint32_t* output, uint16_t offset);
static void SAGITTA_memcpy_s16_from_rx_frame_decoded_(int16_t* output, uint16_t offset);
static void SAGITTA_memcpy_u16_from_rx_frame_decoded_(uint16_t* output, uint16_t offset);
static void SAGITTA_memcpy_u8_from_rx_frame_decoded_(uint8_t* output, uint16_t offset);

DS_INIT_ERR_CODE SAGITTA_init(SAGITTA_Driver* sagitta_driver, uint8_t ch, DS_StreamRecBuffer* rx_buffer)
{
  DS_ERR_CODE ret;

  sagitta_driver->driver.uart_config.ch = ch;
  sagitta_driver->driver.uart_config.baudrate = 115200;
  sagitta_driver->driver.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  sagitta_driver->driver.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  sagitta_driver->driver.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  ret = DS_init(&(sagitta_driver->driver.super),
                &(sagitta_driver->driver.uart_config),
                rx_buffer,
                SAGITTA_load_driver_super_init_settings_);
  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;

  sagitta_driver->info.tlm_type = SAGITTA_TLM_TYPE_SET_PARAMETER_REPLY;
  sagitta_driver->info.tlm_id = (uint8_t)SAGITTA_TLM_ID_TEMPERATURE;
  sagitta_driver->info.tlm_status = 0;
  sagitta_driver->info.unix_time_ms = 0;
  sagitta_driver->info.err_status = SAGITTA_REC_ERR_CODE_OK;
  sagitta_driver->info.xxhash = 0;
  sagitta_driver->info.xxhash_state = SAGITTA_XXHASH_STATE_OK;

  // Initialize telemetry
  memset(&(sagitta_driver->info.telemetry), 0x00, sizeof(sagitta_driver->info.telemetry));

  sagitta_driver->info.telemetry.solution.quaternion_i2c = QUATERNION_make_unit();
  sagitta_driver->info.telemetry.solution.track_quaternion_i2c = QUATERNION_make_unit();
  sagitta_driver->info.telemetry.solution.lisa_quaternion_i2c = QUATERNION_make_unit();
  sagitta_driver->info.frame_transform_c2b = QUATERNION_make_unit();
  sagitta_driver->info.quaternion_i2b = QUATERNION_product(sagitta_driver->info.telemetry.solution.quaternion_i2c,
                                                           sagitta_driver->info.frame_transform_c2b);

  // Initialize read_parameter
  memset(&(sagitta_driver->info.read_parameter), 0x00, sizeof(sagitta_driver->info.read_parameter));

  // Initialize set_parameter
  memset(&(sagitta_driver->info.set_parameter.log_level), 0x00, sizeof(sagitta_driver->info.set_parameter.log_level));
  sagitta_driver->info.set_parameter.limits.action = 0;
  memset(&(sagitta_driver->info.set_parameter.limits.max_value), 0x00, sizeof(sagitta_driver->info.set_parameter.limits.max_value));
  sagitta_driver->info.set_parameter.mounting.scalar_part = 1.0f;
  memset(&(sagitta_driver->info.set_parameter.mounting.vector_part), 0x00, sizeof(sagitta_driver->info.set_parameter.mounting.vector_part));
  sagitta_driver->info.set_parameter.camera.mode = 2;
  sagitta_driver->info.set_parameter.camera.focal_length_mm = 25.2f;
  sagitta_driver->info.set_parameter.camera.exposure_s = 0.2f;
  sagitta_driver->info.set_parameter.camera.interval_s = 0.2f;
  sagitta_driver->info.set_parameter.camera.offset_pix = 0;
  sagitta_driver->info.set_parameter.camera.pga_gain = 3;
  sagitta_driver->info.set_parameter.camera.adc_gain = 58;
  memset(&(sagitta_driver->info.set_parameter.camera.override_register), 0x00,
         sizeof(sagitta_driver->info.set_parameter.camera.override_register));
  sagitta_driver->info.set_parameter.camera.freq = 3;
  sagitta_driver->info.set_parameter.image_processor.mode = 3;
  sagitta_driver->info.set_parameter.image_processor.store = 0;
  sagitta_driver->info.set_parameter.image_processor.signal_threshold_pix = 15;
  sagitta_driver->info.set_parameter.image_processor.dark_threshold_pix = 0;
  sagitta_driver->info.set_parameter.image_processor.background_compensation = 0;
  sagitta_driver->info.set_parameter.centroiding.enable_filter = 0;
  sagitta_driver->info.set_parameter.centroiding.max_quality = 9999.0f;
  sagitta_driver->info.set_parameter.centroiding.dark_threshold_pix = 0.0f;
  sagitta_driver->info.set_parameter.centroiding.min_quality = -9999.0f;
  sagitta_driver->info.set_parameter.centroiding.max_intensity = 9999.0f;
  sagitta_driver->info.set_parameter.centroiding.min_intensity = -9999.0f;
  sagitta_driver->info.set_parameter.centroiding.max_magnitude = 9999.0f;
  sagitta_driver->info.set_parameter.centroiding.gaussian_cmax_pix = 9999.0f;
  sagitta_driver->info.set_parameter.centroiding.gaussian_cmin_pix = -9999.0f;
  sagitta_driver->info.set_parameter.centroiding.transmatrix_00 = 0.0f;
  sagitta_driver->info.set_parameter.centroiding.transmatrix_01 = 1.0f;
  sagitta_driver->info.set_parameter.centroiding.transmatrix_10 = -1.0f;
  sagitta_driver->info.set_parameter.centroiding.transmatrix_11 = 0.0f;
  sagitta_driver->info.set_parameter.lisa.mode = 2;
  sagitta_driver->info.set_parameter.lisa.prefilter_distance_threshold_mm = 0.20f;
  sagitta_driver->info.set_parameter.lisa.prefilter_angle_threshold_rad = 0.10f;
  sagitta_driver->info.set_parameter.lisa.field_of_view_width_rad = 0.443161868784811f;
  sagitta_driver->info.set_parameter.lisa.field_of_view_height_rad = 0.443161868784811f;
  sagitta_driver->info.set_parameter.lisa.float_star_limit_mm = 0.08f;
  sagitta_driver->info.set_parameter.lisa.close_star_limit_mm = 0.08f;
  sagitta_driver->info.set_parameter.lisa.rating_weight_close_star_count = 100.0f;
  sagitta_driver->info.set_parameter.lisa.rating_weight_fraction_close = 10.0f;
  sagitta_driver->info.set_parameter.lisa.rating_weight_mean_sum = 0.0f;
  sagitta_driver->info.set_parameter.lisa.rating_weight_db_star_count = 0.0f;
  sagitta_driver->info.set_parameter.lisa.max_combinations = 12;
  sagitta_driver->info.set_parameter.lisa.nr_stars_stop = 6;
  sagitta_driver->info.set_parameter.lisa.fraction_close_stop = 0.5f;
  sagitta_driver->info.set_parameter.matching.squared_distance_limit_mm = 0.03f;
  sagitta_driver->info.set_parameter.matching.squared_shift_limit_mm = 0.03f;
  sagitta_driver->info.set_parameter.tracking.thin_limit_deg = 0.01f;
  sagitta_driver->info.set_parameter.tracking.outlier_threshold_mm = 0.000272f;
  sagitta_driver->info.set_parameter.tracking.outlier_threshold_quest_mm = 0.000272f;
  sagitta_driver->info.set_parameter.tracking.tracker_choice = 2;
  sagitta_driver->info.set_parameter.validation.stable_count = 5;
  sagitta_driver->info.set_parameter.validation.max_difference_deg = 0.500000f;
  sagitta_driver->info.set_parameter.validation.min_tracker_confidence = 0.00500000f;
  sagitta_driver->info.set_parameter.validation.min_matched_stars = 6;
  sagitta_driver->info.set_parameter.algo.mode = 12;
  sagitta_driver->info.set_parameter.algo.l2t_min_confidence = 0.00500000f;
  sagitta_driver->info.set_parameter.algo.l2t_min_matched = 6;
  sagitta_driver->info.set_parameter.algo.t2l_min_confidence = 0.00500000f;
  sagitta_driver->info.set_parameter.algo.t2l_min_matched = 6;
  uint8_t default_subscription_length = 0;
  sagitta_driver->info.set_parameter.subscription[default_subscription_length] = 24;
  default_subscription_length++;
  memset(&(sagitta_driver->info.set_parameter.subscription[default_subscription_length]), 0x00,
         SAGITTA_PARAMETER_SUBSCRIPTION_LENGTH - default_subscription_length);
  sagitta_driver->info.set_parameter.auto_threshold.mode = 1;
  sagitta_driver->info.set_parameter.auto_threshold.desired_blobs_count = 30;
  sagitta_driver->info.set_parameter.auto_threshold.min_threshold = 1;
  sagitta_driver->info.set_parameter.auto_threshold.max_threshold = 255;
  sagitta_driver->info.set_parameter.auto_threshold.threshold_kp = 0.10;

  return DS_INIT_OK;
}

// ---------- UART Telemetry ----------
DS_REC_ERR_CODE SAGITTA_rec(SAGITTA_Driver* sagitta_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* stream_config;

  DS_clear_rx_buffer(&(sagitta_driver->driver.super)); // ここでクリアしないとずれる

  ret = DS_receive(&(sagitta_driver->driver.super));

#ifdef DRIVER_SAGITTA_DEBUG_SHOW_REC_DATA
  if (sagitta_driver->driver.super.config.info.rec_status_.ret_from_if_rx > 0)
  {
    Printf("sagitta_rx_data: %d Bytes \n", sagitta_driver->driver.super.config.info.rec_status_.ret_from_if_rx);
    for (int i = 0; i < sagitta_driver->driver.super.config.info.rec_status_.ret_from_if_rx; i++)
    {
      stream_config = &(sagitta_driver->driver.super.stream_config[SAGITTA_STREAM_TLM_CMD]);
      const uint8_t* sagitta_rx_data = DSSC_get_rx_frame(stream_config);
      Printf("%02x ", sagitta_rx_data[i]);
      if (i % 4 == 3) Printf("   ");
    }
    Printf("\n");
  }
#endif

  if (ret != DS_ERR_CODE_OK)
  {
    DS_clear_rx_buffer(&(sagitta_driver->driver.super)); // ここでクリアしないとずれる
    return DS_REC_DS_RECEIVE_ERR;
  }

  stream_config = &(sagitta_driver->driver.super.stream_config[SAGITTA_STREAM_TLM_CMD]);
#ifdef DRIVER_SAGITTA_DEBUG_SHOW_REC_DATA
  if (sagitta_driver->driver.super.config.info.rec_status_.ret_from_if_rx > 0)
  {
    Printf("DSSC_get_rec_status(stream_config)->status_code: %d\n", DSSC_get_rec_status(stream_config)->status_code);
  }
#endif
  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    DS_clear_rx_buffer(&(sagitta_driver->driver.super));
    return DS_REC_OTHER_ERR;
  }

  ret = DS_analyze_rec_data(&(sagitta_driver->driver.super), SAGITTA_STREAM_TLM_CMD, sagitta_driver);

  if (ret != DS_ERR_CODE_OK)
  {
    return DS_REC_ANALYZE_ERR;
  }

  return DS_REC_OK;
}

DS_CMD_ERR_CODE SAGITTA_boot(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t kCmdDataLength = 4;

  SAGITTA_tx_data_frame_[0] = SAGITTA_kAddress_;
  SAGITTA_tx_data_frame_[1] = SAGITTA_kCmdAction_;
  SAGITTA_tx_data_frame_[2] = SAGITTA_ACTION_ID_BOOT;
  SAGITTA_tx_data_frame_[3] = SAGITTA_kParameterRegion_;

  return SAGITTA_send_cmd_(sagitta_driver, kCmdDataLength);
}

DS_CMD_ERR_CODE SAGITTA_set_unix_time_us(SAGITTA_Driver* sagitta_driver, uint32_t const unix_time_us_upper, uint32_t const unix_time_us_lower)
{
  const uint8_t kCmdDataLength = 11;
  SAGITTA_tx_data_frame_[0] = SAGITTA_kAddress_;
  SAGITTA_tx_data_frame_[1] = SAGITTA_kCmdAction_;
  SAGITTA_tx_data_frame_[2] = SAGITTA_ACTION_ID_SETTIME;
  memcpy(&(SAGITTA_tx_data_frame_[3]), &unix_time_us_upper, (uint8_t)sizeof(uint32_t));
  memcpy(&(SAGITTA_tx_data_frame_[7]), &unix_time_us_lower, (uint8_t)sizeof(uint32_t));

  return SAGITTA_send_cmd_(sagitta_driver, kCmdDataLength);
}

DS_CMD_ERR_CODE SAGITTA_set_log_level(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t* log_level = &(sagitta_driver->info.set_parameter.log_level[0]);
  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_LOG_LEVEL;
  offset += (uint8_t)sizeof(uint8_t);
  for (int i = 0; i < SAGITTA_PARAMETER_LOG_LEVEL_LENGTH; i++)
  {
    memcpy(&(SAGITTA_tx_data_frame_[offset]), &(log_level[i]), sizeof(log_level[i]));
    offset += (uint8_t)sizeof(log_level[i]);
  }

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_limits(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t action = sagitta_driver->info.set_parameter.limits.action;
  const float* max_value = &(sagitta_driver->info.set_parameter.limits.max_value[0]);
  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_LIMITS;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(action), sizeof(action));
  offset += (uint8_t)sizeof(action);
  for (int i = 0; i < SAGITTA_PARAMETER_LIMITS_LENGTH; i++)
  {
    memcpy(&(SAGITTA_tx_data_frame_[offset]), &(max_value[i]), sizeof(max_value[i]));
    offset += (uint8_t)sizeof(max_value[i]);
  }

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_mounting(SAGITTA_Driver* sagitta_driver)
{
  uint8_t offset = 0;
  const float qw = sagitta_driver->info.set_parameter.mounting.scalar_part;
  const float qx = sagitta_driver->info.set_parameter.mounting.vector_part[0];
  const float qy = sagitta_driver->info.set_parameter.mounting.vector_part[1];
  const float qz = sagitta_driver->info.set_parameter.mounting.vector_part[2];

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_MOUNTING;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(qw), sizeof(qw));
  offset += (uint8_t)sizeof(qw);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(qx), sizeof(qx));
  offset += (uint8_t)sizeof(qx);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(qy), sizeof(qy));
  offset += (uint8_t)sizeof(qy);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(qz), sizeof(qz));
  offset += (uint8_t)sizeof(qz);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_camera(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t  mode = sagitta_driver->info.set_parameter.camera.mode;
  const float    focal_length_mm = sagitta_driver->info.set_parameter.camera.focal_length_mm;
  const float    exposure_s = sagitta_driver->info.set_parameter.camera.exposure_s;
  const float    interval_s = sagitta_driver->info.set_parameter.camera.interval_s;
  const int16_t  offset_pix = sagitta_driver->info.set_parameter.camera.offset_pix;
  const uint8_t  pga_gain = sagitta_driver->info.set_parameter.camera.pga_gain;
  const uint8_t  adc_gain = sagitta_driver->info.set_parameter.camera.adc_gain;
  const uint8_t* override_register = &(sagitta_driver->info.set_parameter.camera.override_register[0]);
  const uint32_t freq = sagitta_driver->info.set_parameter.camera.freq;

  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_CAMERA;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(mode), sizeof(mode));
  offset += (uint8_t)sizeof(mode);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(focal_length_mm), sizeof(focal_length_mm));
  offset += (uint8_t)sizeof(focal_length_mm);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(exposure_s), sizeof(exposure_s));
  offset += (uint8_t)sizeof(exposure_s);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(interval_s), sizeof(interval_s));
  offset += (uint8_t)sizeof(interval_s);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(offset_pix), sizeof(offset_pix));
  offset += (uint8_t)sizeof(offset_pix);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(pga_gain), sizeof(pga_gain));
  offset += (uint8_t)sizeof(pga_gain);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(adc_gain), sizeof(adc_gain));
  offset += (uint8_t)sizeof(adc_gain);
  for (int i = 0; i < SAGITTA_PARAMETER_CAMERA_OVERRIDE_REGISTER_LENGTH; i++)
  {
    memcpy(&(SAGITTA_tx_data_frame_[offset]), &(override_register[i]), sizeof(override_register[i]));
    offset += (uint8_t)sizeof(override_register[i]);
  }
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(freq), sizeof(freq));
  offset += (uint8_t)sizeof(freq);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_image_processor(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t mode = sagitta_driver->info.set_parameter.image_processor.mode;
  const uint8_t store = sagitta_driver->info.set_parameter.image_processor.store;
  const uint16_t signal_threshold_pix = sagitta_driver->info.set_parameter.image_processor.signal_threshold_pix;
  const uint16_t dark_threshold_pix = sagitta_driver->info.set_parameter.image_processor.dark_threshold_pix;
  const uint8_t background_compensation = sagitta_driver->info.set_parameter.image_processor.background_compensation;
  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(mode), sizeof(mode));
  offset += (uint8_t)sizeof(mode);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(store), sizeof(store));
  offset += (uint8_t)sizeof(store);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(signal_threshold_pix), sizeof(signal_threshold_pix));
  offset += (uint8_t)sizeof(signal_threshold_pix);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(dark_threshold_pix), sizeof(dark_threshold_pix));
  offset += (uint8_t)sizeof(dark_threshold_pix);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(background_compensation), sizeof(background_compensation));
  offset += (uint8_t)sizeof(background_compensation);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_centroiding(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t enable_filter = sagitta_driver->info.set_parameter.centroiding.enable_filter;
  const float max_quality = sagitta_driver->info.set_parameter.centroiding.max_quality;
  const float dark_threshold_pix = sagitta_driver->info.set_parameter.centroiding.dark_threshold_pix;
  const float min_quality = sagitta_driver->info.set_parameter.centroiding.min_quality;
  const float max_intensity = sagitta_driver->info.set_parameter.centroiding.max_intensity;
  const float min_intensity = sagitta_driver->info.set_parameter.centroiding.min_intensity;
  const float max_magnitude = sagitta_driver->info.set_parameter.centroiding.max_magnitude;
  const float gaussian_cmax_pix = sagitta_driver->info.set_parameter.centroiding.gaussian_cmax_pix;
  const float gaussian_cmin_pix = sagitta_driver->info.set_parameter.centroiding.gaussian_cmin_pix;
  const float transmatrix_00 = sagitta_driver->info.set_parameter.centroiding.transmatrix_00;
  const float transmatrix_01 = sagitta_driver->info.set_parameter.centroiding.transmatrix_01;
  const float transmatrix_10 = sagitta_driver->info.set_parameter.centroiding.transmatrix_10;
  const float transmatrix_11 = sagitta_driver->info.set_parameter.centroiding.transmatrix_11;
  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_CENTROIDING;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(enable_filter), sizeof(enable_filter));
  offset += (uint8_t)sizeof(enable_filter);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(max_quality), sizeof(max_quality));
  offset += (uint8_t)sizeof(max_quality);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(dark_threshold_pix), sizeof(dark_threshold_pix));
  offset += (uint8_t)sizeof(dark_threshold_pix);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(min_quality), sizeof(min_quality));
  offset += (uint8_t)sizeof(min_quality);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(max_intensity), sizeof(max_intensity));
  offset += (uint8_t)sizeof(max_intensity);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(min_intensity), sizeof(min_intensity));
  offset += (uint8_t)sizeof(min_intensity);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(max_magnitude), sizeof(max_magnitude));
  offset += (uint8_t)sizeof(max_magnitude);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(gaussian_cmax_pix), sizeof(gaussian_cmax_pix));
  offset += (uint8_t)sizeof(gaussian_cmax_pix);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(gaussian_cmin_pix), sizeof(gaussian_cmin_pix));
  offset += (uint8_t)sizeof(gaussian_cmin_pix);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(transmatrix_00), sizeof(transmatrix_00));
  offset += (uint8_t)sizeof(transmatrix_00);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(transmatrix_01), sizeof(transmatrix_01));
  offset += (uint8_t)sizeof(transmatrix_01);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(transmatrix_10), sizeof(transmatrix_10));
  offset += (uint8_t)sizeof(transmatrix_10);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(transmatrix_11), sizeof(transmatrix_11));
  offset += (uint8_t)sizeof(transmatrix_11);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_lisa(SAGITTA_Driver* sagitta_driver)
{
  const uint32_t mode = sagitta_driver->info.set_parameter.lisa.mode;
  const float prefilter_distance_threshold_mm = sagitta_driver->info.set_parameter.lisa.prefilter_distance_threshold_mm;
  const float prefilter_angle_threshold_rad = sagitta_driver->info.set_parameter.lisa.prefilter_angle_threshold_rad;
  const float field_of_view_width_rad = sagitta_driver->info.set_parameter.lisa.field_of_view_width_rad;
  const float field_of_view_height_rad = sagitta_driver->info.set_parameter.lisa.field_of_view_height_rad;
  const float float_star_limit_mm = sagitta_driver->info.set_parameter.lisa.float_star_limit_mm;
  const float close_star_limit_mm = sagitta_driver->info.set_parameter.lisa.close_star_limit_mm;
  const float rating_weight_close_star_count = sagitta_driver->info.set_parameter.lisa.rating_weight_close_star_count;
  const float rating_weight_fraction_close = sagitta_driver->info.set_parameter.lisa.rating_weight_fraction_close;
  const float rating_weight_mean_sum = sagitta_driver->info.set_parameter.lisa.rating_weight_mean_sum;
  const float rating_weight_db_star_count = sagitta_driver->info.set_parameter.lisa.rating_weight_db_star_count;
  const uint8_t max_combinations = sagitta_driver->info.set_parameter.lisa.max_combinations;
  const uint8_t nr_stars_stop = sagitta_driver->info.set_parameter.lisa.nr_stars_stop;
  const float fraction_close_stop = sagitta_driver->info.set_parameter.lisa.fraction_close_stop;
  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_LISA;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(mode), sizeof(mode));
  offset += (uint8_t)sizeof(mode);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(prefilter_distance_threshold_mm), sizeof(prefilter_distance_threshold_mm));
  offset += (uint8_t)sizeof(prefilter_distance_threshold_mm);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(prefilter_angle_threshold_rad), sizeof(prefilter_angle_threshold_rad));
  offset += (uint8_t)sizeof(prefilter_angle_threshold_rad);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(field_of_view_width_rad), sizeof(field_of_view_width_rad));
  offset += (uint8_t)sizeof(field_of_view_width_rad);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(field_of_view_height_rad), sizeof(field_of_view_height_rad));
  offset += (uint8_t)sizeof(field_of_view_height_rad);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(float_star_limit_mm), sizeof(float_star_limit_mm));
  offset += (uint8_t)sizeof(float_star_limit_mm);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(close_star_limit_mm), sizeof(close_star_limit_mm));
  offset += (uint8_t)sizeof(close_star_limit_mm);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(rating_weight_close_star_count), sizeof(rating_weight_close_star_count));
  offset += (uint8_t)sizeof(rating_weight_close_star_count);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(rating_weight_fraction_close), sizeof(rating_weight_fraction_close));
  offset += (uint8_t)sizeof(rating_weight_fraction_close);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(rating_weight_mean_sum), sizeof(rating_weight_mean_sum));
  offset += (uint8_t)sizeof(rating_weight_mean_sum);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(rating_weight_db_star_count), sizeof(rating_weight_db_star_count));
  offset += (uint8_t)sizeof(rating_weight_db_star_count);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(max_combinations), sizeof(max_combinations));
  offset += (uint8_t)sizeof(max_combinations);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(nr_stars_stop), sizeof(nr_stars_stop));
  offset += (uint8_t)sizeof(nr_stars_stop);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(fraction_close_stop), sizeof(fraction_close_stop));
  offset += (uint8_t)sizeof(fraction_close_stop);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_matching(SAGITTA_Driver* sagitta_driver)
{
  const float squared_distance_limit_mm = sagitta_driver->info.set_parameter.matching.squared_distance_limit_mm;
  const float squared_shift_limit_mm = sagitta_driver->info.set_parameter.matching.squared_shift_limit_mm;

  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_MATCHING;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(squared_distance_limit_mm), sizeof(squared_distance_limit_mm));
  offset += (uint8_t)sizeof(squared_distance_limit_mm);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(squared_shift_limit_mm), sizeof(squared_shift_limit_mm));
  offset += (uint8_t)sizeof(squared_shift_limit_mm);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_tracking(SAGITTA_Driver* sagitta_driver)
{
  const float thin_limit_deg = sagitta_driver->info.set_parameter.tracking.thin_limit_deg;
  const float outlier_threshold_mm = sagitta_driver->info.set_parameter.tracking.outlier_threshold_mm;
  const float outlier_threshold_quest_mm = sagitta_driver->info.set_parameter.tracking.outlier_threshold_quest_mm;
  const uint8_t tracker_choice = sagitta_driver->info.set_parameter.tracking.tracker_choice;

  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_TRACKING;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(thin_limit_deg), sizeof(thin_limit_deg));
  offset += (uint8_t)sizeof(thin_limit_deg);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(outlier_threshold_mm), sizeof(outlier_threshold_mm));
  offset += (uint8_t)sizeof(outlier_threshold_mm);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(outlier_threshold_quest_mm), sizeof(outlier_threshold_quest_mm));
  offset += (uint8_t)sizeof(outlier_threshold_quest_mm);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(tracker_choice), sizeof(tracker_choice));
  offset += (uint8_t)sizeof(tracker_choice);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_validation(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t stable_count = sagitta_driver->info.set_parameter.validation.stable_count;
  const float max_difference_deg = sagitta_driver->info.set_parameter.validation.max_difference_deg;
  const float min_tracker_confidence = sagitta_driver->info.set_parameter.validation.min_tracker_confidence;
  const uint8_t min_matched_stars = sagitta_driver->info.set_parameter.validation.min_matched_stars;

  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_VALIDATION;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(stable_count), sizeof(stable_count));
  offset += (uint8_t)sizeof(stable_count);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(max_difference_deg), sizeof(max_difference_deg));
  offset += (uint8_t)sizeof(max_difference_deg);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(min_tracker_confidence), sizeof(min_tracker_confidence));
  offset += (uint8_t)sizeof(min_tracker_confidence);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(min_matched_stars), sizeof(min_matched_stars));
  offset += (uint8_t)sizeof(min_matched_stars);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_algo(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t mode = sagitta_driver->info.set_parameter.algo.mode;
  const float l2t_min_confidence = sagitta_driver->info.set_parameter.algo.l2t_min_confidence;
  const uint8_t l2t_min_matched = sagitta_driver->info.set_parameter.algo.l2t_min_matched;
  const float t2l_min_confidence = sagitta_driver->info.set_parameter.algo.t2l_min_confidence;
  const uint8_t t2l_min_matched = sagitta_driver->info.set_parameter.algo.t2l_min_matched;

  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_ALGO;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(mode), sizeof(mode));
  offset += (uint8_t)sizeof(mode);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(l2t_min_confidence), sizeof(l2t_min_confidence));
  offset += (uint8_t)sizeof(l2t_min_confidence);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(l2t_min_matched), sizeof(l2t_min_matched));
  offset += (uint8_t)sizeof(l2t_min_matched);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(t2l_min_confidence), sizeof(t2l_min_confidence));
  offset += (uint8_t)sizeof(t2l_min_confidence);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(t2l_min_matched), sizeof(t2l_min_matched));
  offset += (uint8_t)sizeof(t2l_min_matched);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_subscription(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t* subscription = &(sagitta_driver->info.set_parameter.subscription[0]);

  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_SUBSCRIPTION;
  offset += (uint8_t)sizeof(uint8_t);

  for (int i = 0; i < SAGITTA_PARAMETER_SUBSCRIPTION_LENGTH; i++)
  {
    memcpy(&(SAGITTA_tx_data_frame_[offset]), &(subscription[i]), sizeof(subscription[i]));
    offset += (uint8_t)sizeof(subscription[i]);
  }

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_set_auto_threshold(SAGITTA_Driver* sagitta_driver)
{
  const uint8_t mode = sagitta_driver->info.set_parameter.auto_threshold.mode;
  const uint8_t desired_blobs_count = sagitta_driver->info.set_parameter.auto_threshold.desired_blobs_count;
  const uint16_t min_threshold = sagitta_driver->info.set_parameter.auto_threshold.min_threshold;
  const uint16_t max_threshold = sagitta_driver->info.set_parameter.auto_threshold.max_threshold;
  const float threshold_kp = sagitta_driver->info.set_parameter.auto_threshold.threshold_kp;

  uint8_t offset = 0;

  SAGITTA_tx_data_frame_[offset] = SAGITTA_kAddress_;
  offset += (uint8_t)sizeof(SAGITTA_kAddress_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_kCmdSetParameter_;
  offset += (uint8_t)sizeof(SAGITTA_kCmdSetParameter_);
  SAGITTA_tx_data_frame_[offset] = SAGITTA_PARAMETER_ID_AUTO_THRESHOLD;
  offset += (uint8_t)sizeof(uint8_t);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(mode), sizeof(mode));
  offset += (uint8_t)sizeof(mode);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(desired_blobs_count), sizeof(desired_blobs_count));
  offset += (uint8_t)sizeof(desired_blobs_count);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(min_threshold), sizeof(min_threshold));
  offset += (uint8_t)sizeof(min_threshold);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(max_threshold), sizeof(max_threshold));
  offset += (uint8_t)sizeof(max_threshold);
  memcpy(&(SAGITTA_tx_data_frame_[offset]), &(threshold_kp), sizeof(threshold_kp));
  offset += (uint8_t)sizeof(threshold_kp);

  return SAGITTA_send_cmd_(sagitta_driver, offset);
}

DS_CMD_ERR_CODE SAGITTA_change_log_level(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= SAGITTA_PARAMETER_LOG_LEVEL_LENGTH) return DS_CMD_ILLEGAL_PARAMETER;

  sagitta_driver->info.set_parameter.log_level[param_idx] = (uint8_t)value;

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_limits(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 11) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.limits.action = (uint8_t)value;
    break;
  default:
    sagitta_driver->info.set_parameter.limits.max_value[param_idx - 1] = value;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_mounting(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 4) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.mounting.scalar_part = value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.mounting.vector_part[0] = value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.mounting.vector_part[1] = value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.mounting.vector_part[2] = value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_camera(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 24) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.camera.mode = (uint8_t)value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.camera.focal_length_mm = value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.camera.exposure_s = value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.camera.interval_s = value;
    break;
  case 4:
    sagitta_driver->info.set_parameter.camera.offset_pix = (int16_t)value;
    break;
  case 5:
    sagitta_driver->info.set_parameter.camera.pga_gain = (uint8_t)value;
    break;
  case 6:
    sagitta_driver->info.set_parameter.camera.adc_gain = (uint8_t)value;
    break;
  case 23:
    sagitta_driver->info.set_parameter.camera.freq = (uint32_t)value;
    break;
  default:
    sagitta_driver->info.set_parameter.camera.override_register[param_idx - 7] = (uint8_t)value;
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_image_processor(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 5) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.image_processor.mode = (uint8_t)value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.image_processor.store = (uint8_t)value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.image_processor.signal_threshold_pix = (uint16_t)value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.image_processor.dark_threshold_pix = (uint16_t)value;
    break;
  case 4:
    sagitta_driver->info.set_parameter.image_processor.background_compensation = (uint8_t)value;
    break;
  default:
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_centroiding(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 13) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.centroiding.enable_filter = (uint8_t)value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.centroiding.max_quality = value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.centroiding.dark_threshold_pix = value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.centroiding.min_quality = value;
    break;
  case 4:
    sagitta_driver->info.set_parameter.centroiding.max_intensity = value;
    break;
  case 5:
    sagitta_driver->info.set_parameter.centroiding.min_intensity = value;
    break;
  case 6:
    sagitta_driver->info.set_parameter.centroiding.max_magnitude = value;
    break;
  case 7:
    sagitta_driver->info.set_parameter.centroiding.gaussian_cmax_pix = value;
    break;
  case 8:
    sagitta_driver->info.set_parameter.centroiding.gaussian_cmin_pix = value;
    break;
  case 9:
    sagitta_driver->info.set_parameter.centroiding.transmatrix_00 = value;
    break;
  case 10:
    sagitta_driver->info.set_parameter.centroiding.transmatrix_01 = value;
    break;
  case 11:
    sagitta_driver->info.set_parameter.centroiding.transmatrix_10 = value;
    break;
  case 12:
    sagitta_driver->info.set_parameter.centroiding.transmatrix_11 = value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_lisa(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  // TODO
  if (param_idx >= 14) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.lisa.mode = (uint32_t)value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.lisa.prefilter_distance_threshold_mm = value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.lisa.prefilter_angle_threshold_rad = value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.lisa.field_of_view_width_rad = value;
    break;
  case 4:
    sagitta_driver->info.set_parameter.lisa.field_of_view_height_rad = value;
    break;
  case 5:
    sagitta_driver->info.set_parameter.lisa.float_star_limit_mm = value;
    break;
  case 6:
    sagitta_driver->info.set_parameter.lisa.close_star_limit_mm = value;
    break;
  case 7:
    sagitta_driver->info.set_parameter.lisa.rating_weight_close_star_count = value;
    break;
  case 8:
    sagitta_driver->info.set_parameter.lisa.rating_weight_fraction_close = value;
    break;
  case 9:
    sagitta_driver->info.set_parameter.lisa.rating_weight_mean_sum = value;
    break;
  case 10:
    sagitta_driver->info.set_parameter.lisa.rating_weight_db_star_count = value;
    break;
  case 11:
    sagitta_driver->info.set_parameter.lisa.max_combinations = (uint8_t)value;
    break;
  case 12:
    sagitta_driver->info.set_parameter.lisa.nr_stars_stop = (uint8_t)value;
    break;
  case 13:
    sagitta_driver->info.set_parameter.lisa.fraction_close_stop = value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_matching(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 2) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.matching.squared_distance_limit_mm = value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.matching.squared_shift_limit_mm = value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_tracking(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 4) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.tracking.thin_limit_deg = value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.tracking.outlier_threshold_mm = value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.tracking.outlier_threshold_quest_mm = value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.tracking.tracker_choice = (uint8_t)value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_validation(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 4) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.validation.stable_count = (uint8_t)value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.validation.max_difference_deg = value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.validation.min_tracker_confidence = value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.validation.min_matched_stars = (uint8_t)value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_algo(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 5) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.algo.mode = (uint8_t)value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.algo.l2t_min_confidence = value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.algo.l2t_min_matched = (uint8_t)value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.algo.t2l_min_confidence = value;
    break;
  case 4:
    sagitta_driver->info.set_parameter.algo.t2l_min_matched = (uint8_t)value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_subscription(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= SAGITTA_PARAMETER_SUBSCRIPTION_LENGTH) return DS_CMD_ILLEGAL_PARAMETER;

  sagitta_driver->info.set_parameter.subscription[param_idx] = (uint8_t)value;

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE SAGITTA_change_auto_threshold(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value)
{
  if (param_idx >= 5) return DS_CMD_ILLEGAL_PARAMETER;

  switch (param_idx)
  {
  case 0:
    sagitta_driver->info.set_parameter.auto_threshold.mode = (uint8_t)value;
    break;
  case 1:
    sagitta_driver->info.set_parameter.auto_threshold.desired_blobs_count = (uint8_t)value;
    break;
  case 2:
    sagitta_driver->info.set_parameter.auto_threshold.min_threshold = (uint16_t)value;
    break;
  case 3:
    sagitta_driver->info.set_parameter.auto_threshold.max_threshold = (uint16_t)value;
    break;
  case 4:
    sagitta_driver->info.set_parameter.auto_threshold.threshold_kp = value;
    break;
  default:
    // NOT REACHED
    break;
  }

  return DS_CMD_OK;
}

C2A_MATH_ERROR SAGITTA_set_frame_transform_c2b(SAGITTA_Driver* sagitta_driver, const Quaternion q_c2b)
{
  C2A_MATH_ERROR ret;
  ret = QUATERNION_is_normalized(q_c2b);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  sagitta_driver->info.frame_transform_c2b = q_c2b;
  return ret;
}

DS_CMD_ERR_CODE SAGITTA_read_parameter(SAGITTA_Driver* sagitta_driver, const uint8_t parameter_id)
{
  const uint8_t kCmdDataLength = 3;

  SAGITTA_tx_data_frame_[0] = SAGITTA_kAddress_;
  SAGITTA_tx_data_frame_[1] = SAGITTA_kCmdRequestParameter_;
  SAGITTA_tx_data_frame_[2] = parameter_id;

  return SAGITTA_send_cmd_(sagitta_driver, kCmdDataLength);
}

static DS_ERR_CODE SAGITTA_load_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* stream_config;

  p_super->interface = UART;

  stream_config = &(p_super->stream_config[SAGITTA_STREAM_TLM_CMD]);
  DSSC_enable(stream_config);

  DSSC_set_tx_frame(stream_config, SAGITTA_tx_frame_);
  DSSC_set_tx_frame_size(stream_config, SAGITTA_tx_frame_length_);

  // 定期的な受信はする
  DSSC_set_rx_header(stream_config, SAGITTA_kRxHeader_, SAGITTA_RX_HEADER_SIZE);
  DSSC_set_rx_footer(stream_config, SAGITTA_kFooter_, SAGITTA_FOOTER_SIZE);
  DSSC_set_rx_frame_size(stream_config, -1);      // 可変なので負
  DSSC_set_rx_framelength_pos(stream_config, -1); // フレーム長データが存在しないので負

  DSSC_set_data_analyzer(stream_config, SAGITTA_analyze_rec_data_);

  return DS_ERR_CODE_OK;
}

static DS_CMD_ERR_CODE SAGITTA_send_cmd_(SAGITTA_Driver* sagitta_driver, const uint8_t cmd_data_len)
{
  DS_ERR_CODE ret_super;
  DS_CMD_ERR_CODE ret_cmd;
  DS_StreamConfig* stream_config;
  stream_config = &(sagitta_driver->driver.super.stream_config[SAGITTA_STREAM_TLM_CMD]);

  ret_cmd = SAGITTA_set_tx_frame_(cmd_data_len);
  if (ret_cmd != DS_CMD_OK) return ret_cmd;

  DSSC_set_tx_frame_size(stream_config, SAGITTA_tx_frame_length_);
  // コマンドを送ったらテレメが返ってくることを想定
  ret_super = DS_send_req_tlm_cmd(&(sagitta_driver->driver.super), SAGITTA_STREAM_TLM_CMD);

#ifdef DRIVER_SAGITTA_DEBUG_SHOW_REC_DATA
  Printf("SAGITTA_tx_frame_: %d Bytes \n", SAGITTA_tx_frame_length_);
  for (int i = 0; i < SAGITTA_tx_frame_length_; i++)
  {
    Printf("%02x ", SAGITTA_tx_frame_[i]);
    if (i % 4 == 3) Printf("   ");
  }
  Printf("\n");
#endif

  if (ret_super == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

static DS_CMD_ERR_CODE SAGITTA_set_tx_frame_(uint8_t cmd_data_len)
{
  uint8_t cmd_size = cmd_data_len;
  uint8_t cmd_escape_size = 0;

  cmd_size += (uint8_t)SAGITTA_set_tx_frame_header_();
  cmd_size += (uint8_t)SAGITTA_set_tx_frame_xxhash_(cmd_data_len);
  cmd_escape_size = (uint8_t)SAGITTA_encode_tx_frame_(cmd_data_len);
  if (cmd_escape_size < 0) return DS_CMD_ILLEGAL_LENGTH; // TXで送るバイト数がバッファを超えた場合
  cmd_size += cmd_escape_size;
  cmd_size += (uint8_t)SAGITTA_set_tx_frame_footer_(cmd_size);
  SAGITTA_tx_frame_length_ = cmd_size;

  return DS_CMD_OK;
}

static int SAGITTA_set_tx_frame_header_()
{
  for (uint8_t i = 0; i < SAGITTA_TX_HEADER_SIZE; i++)
  {
    SAGITTA_tx_frame_[i] = SAGITTA_kTxHeader_[i];
  }
  return SAGITTA_TX_HEADER_SIZE;
}

static int SAGITTA_set_tx_frame_xxhash_(uint8_t cmd_data_len)
{
  uint32_t calced_hash = XXH32(SAGITTA_tx_data_frame_, cmd_data_len, SAGITTA_kXxhashSeed_);

  memcpy(&(SAGITTA_tx_data_frame_[cmd_data_len]), &(calced_hash), SAGITTA_XXHASH_SIZE);

  return SAGITTA_XXHASH_SIZE;
}

static int SAGITTA_set_tx_frame_footer_(uint8_t cmd_offset_len)
{
  for (uint8_t i = 0; i < SAGITTA_FOOTER_SIZE; i++)
  {
    SAGITTA_tx_frame_[cmd_offset_len + i] = SAGITTA_kFooter_[i];
  }
  return SAGITTA_FOOTER_SIZE;
}

static int SAGITTA_encode_tx_frame_(uint8_t cmd_data_len)
{
  uint8_t num_escape = 0;
  for (uint8_t i = 0; i < cmd_data_len + SAGITTA_XXHASH_SIZE; i++)
  {
    if (SAGITTA_tx_data_frame_[i] == SAGITTA_kTxHeader_[0])
    {
      SAGITTA_tx_frame_[i + num_escape + SAGITTA_TX_HEADER_SIZE] = SAGITTA_kEscapeEnd_[0];
      num_escape++;
      SAGITTA_tx_frame_[i + num_escape + SAGITTA_TX_HEADER_SIZE] = SAGITTA_kEscapeEnd_[1];
    }
    else if (SAGITTA_tx_data_frame_[i] == SAGITTA_kEscapeEnd_[0])
    {
      SAGITTA_tx_frame_[i + num_escape + SAGITTA_TX_HEADER_SIZE] = SAGITTA_kEscapeEsc_[0];
      num_escape++;
      SAGITTA_tx_frame_[i + num_escape + SAGITTA_TX_HEADER_SIZE] = SAGITTA_kEscapeEsc_[1];
    }
    else
    {
      SAGITTA_tx_frame_[i + num_escape + SAGITTA_TX_HEADER_SIZE] = SAGITTA_tx_data_frame_[i];
    }
    // TXフレームのサイズが確保しているbyte数を超えるようであれば終了
    if (cmd_data_len + num_escape + SAGITTA_TX_HEADER_SIZE + SAGITTA_FOOTER_SIZE + SAGITTA_XXHASH_SIZE >
        SAGITTA_TX_MAX_FRAME_SIZE)
    {
      return -1;
    }
  }

  return num_escape;
}

DS_CMD_ERR_CODE SAGITTA_request_tlm(SAGITTA_Driver* sagitta_driver, uint8_t tlm_id)
{
  const uint8_t kCmdDataLength = 3;

  SAGITTA_tx_data_frame_[0] = SAGITTA_kAddress_;
  SAGITTA_tx_data_frame_[1] = SAGITTA_kCmdRequestTelem_;
  SAGITTA_tx_data_frame_[2] = tlm_id;

  return (SAGITTA_send_cmd_(sagitta_driver, kCmdDataLength));
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver)
{
  const uint8_t* sagitta_rx_data = DSSC_get_rx_frame(stream_config);
  SAGITTA_Driver* sagitta_driver = (SAGITTA_Driver*)p_driver;
  sagitta_driver->info.err_status = SAGITTA_decode_rx_frame_(sagitta_driver, sagitta_rx_data, SAGITTA_RX_MAX_FRAME_SIZE);
  uint8_t tlm_type = 0;
  uint8_t tlm_id = 0;
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&tlm_type, (uint16_t)SAGITTA_kTlmOffsetTlmType_);
  sagitta_driver->info.tlm_type = (SAGITTA_TLM_TYPE)tlm_type;
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&tlm_id, (uint16_t)SAGITTA_kTlmOffsetTlmID_);
  sagitta_driver->info.tlm_id = tlm_id;

  switch (sagitta_driver->info.tlm_type)
  {
  case SAGITTA_TLM_TYPE_SET_PARAMETER_REPLY:
    return SAGITTA_analyze_rec_data_set_parameter_reply_(sagitta_driver);
  case SAGITTA_TLM_TYPE_PARAMETER_REPLY:
    return SAGITTA_analyze_rec_data_parameter_reply_(sagitta_driver);
  case SAGITTA_TLM_TYPE_ACTION_REPLY:
    return SAGITTA_analyze_rec_data_action_reply_(sagitta_driver);
  case SAGITTA_TLM_TYPE_TELEMETRY_REPLY:
    return SAGITTA_analyze_rec_data_telemetry_(sagitta_driver);
  case SAGITTA_TLM_TYPE_ASYNCHRONOUS_TELEMETRY_REPLY:
    return SAGITTA_analyze_rec_data_telemetry_(sagitta_driver);
  default:
    return DS_ERR_CODE_ERR;
  }
}

static SAGITTA_REC_ERR_CODE SAGITTA_decode_rx_frame_(SAGITTA_Driver* sagitta_driver,
  const uint8_t* sagitta_rx_data, const uint16_t sagitta_rx_data_length)
{
  uint8_t num_escape = 0;

  if (sagitta_rx_data_length <= SAGITTA_RX_HEADER_SIZE + SAGITTA_FOOTER_SIZE) return SAGITTA_REC_ERR_CODE_ILLEGAL_LENGTH;

  for (uint16_t i = 0; i < SAGITTA_RX_HEADER_SIZE; i++)
  {
    SAGITTA_rx_frame_decoded_[i] = sagitta_rx_data[i];
  }

  for (uint16_t i = SAGITTA_RX_HEADER_SIZE; i < sagitta_rx_data_length - SAGITTA_FOOTER_SIZE; i++)
  {
    if (sagitta_rx_data[i + num_escape] == SAGITTA_kEscapeEnd_[0])
    {
      num_escape++;
      if (sagitta_rx_data[i + num_escape] == SAGITTA_kEscapeEnd_[1])
      {
        SAGITTA_rx_frame_decoded_[i] = SAGITTA_kRxHeader_[0]; // ESC2byteを0xc0に変換
      }
      else if (sagitta_rx_data[i + num_escape] == SAGITTA_kEscapeEsc_[1])
      {
        SAGITTA_rx_frame_decoded_[i] = SAGITTA_kEscapeEsc_[0]; // ESC2byteを0xdbに変換
      }
      else
      {
        // 0xdbの後に0xdcか0xddがなければエラー
        return SAGITTA_REC_ERR_CODE_ILLEGAL_ENCODE;
      }
    }
    else
    {
      SAGITTA_rx_frame_decoded_[i] = sagitta_rx_data[i + num_escape];
    }

    if (i + num_escape >= sagitta_rx_data_length - SAGITTA_FOOTER_SIZE) break;
  }

  return SAGITTA_REC_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_unix_time_(SAGITTA_Driver* sagitta_driver)
{
  uint32_t unix_time_us_upper = 0;
  uint32_t unix_time_us_lower = 0;

  SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(unix_time_us_upper), (uint16_t)SAGITTA_kTlmOffsetUnixTime_);
  SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(unix_time_us_lower), (uint16_t)(SAGITTA_kTlmOffsetUnixTime_ + sizeof(uint32_t)));

  // uint64が実装されていないので無理やり計算
  sagitta_driver->info.unix_time_ms = (uint32_t)((unix_time_us_upper) / 1000);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_set_parameter_reply_(SAGITTA_Driver* sagitta_driver)
{
  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, (uint16_t)SAGITTA_kTlmSizeSetParameterReply_);

  uint8_t result = 0;
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&result, (uint16_t)SAGITTA_kTlmOffsetTlmStatus_);
  sagitta_driver->info.tlm_status = result;

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_reply_(SAGITTA_Driver* sagitta_driver)
{
  // parameter reply にstatusコードは含まれない

  switch ((SAGITTA_PARAMETER_ID)sagitta_driver->info.tlm_id)
  {
  case SAGITTA_PARAMETER_ID_LOG_LEVEL:
    return SAGITTA_analyze_rec_data_parameter_log_level_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_LIMITS:
    return SAGITTA_analyze_rec_data_parameter_limits_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_MOUNTING:
    return SAGITTA_analyze_rec_data_parameter_mounting_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_CAMERA:
    return SAGITTA_analyze_rec_data_parameter_camera_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR:
    return SAGITTA_analyze_rec_data_parameter_image_processor_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_CENTROIDING:
    return SAGITTA_analyze_rec_data_parameter_centroiding_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_LISA:
    return SAGITTA_analyze_rec_data_parameter_lisa_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_MATCHING:
    return SAGITTA_analyze_rec_data_parameter_matching_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_TRACKING:
    return SAGITTA_analyze_rec_data_parameter_tracking_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_VALIDATION:
    return SAGITTA_analyze_rec_data_parameter_validation_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_ALGO:
    return SAGITTA_analyze_rec_data_parameter_algo_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_SUBSCRIPTION:
    return SAGITTA_analyze_rec_data_parameter_subscription_(sagitta_driver);
  case SAGITTA_PARAMETER_ID_AUTO_THRESHOLD:
    return SAGITTA_analyze_rec_data_parameter_auto_threshold_(sagitta_driver);
  default:
    return DS_ERR_CODE_ERR;
  }

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_log_level_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  for (int i = 0; i < SAGITTA_PARAMETER_LOG_LEVEL_LENGTH; i++)
  {
    SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.log_level[i]), offset);
    offset += (uint16_t)sizeof(uint8_t);
  }

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_limits_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.limits.action), offset);
  offset += (uint16_t)sizeof(uint8_t);
  for (int i = 0; i < SAGITTA_PARAMETER_LIMITS_LENGTH; i++)
  {
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.limits.max_value[i]), offset);
    offset += (uint16_t)sizeof(float);
  }

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_mounting_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.mounting.scalar_part), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.mounting.vector_part[0]), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.mounting.vector_part[1]), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.mounting.vector_part[2]), offset);
  offset += (uint16_t)sizeof(float);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_camera_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.mode), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.focal_length_mm), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.exposure_s), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.interval_s), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_s16_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.offset_pix), offset);
  offset += (uint16_t)sizeof(int16_t);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.pga_gain), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.adc_gain), offset);
  offset += (uint16_t)sizeof(uint8_t);
  for (int i = 0; i < SAGITTA_PARAMETER_CAMERA_OVERRIDE_REGISTER_LENGTH; i++)
  {
    SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.override_register[i]), offset);
    offset += (uint16_t)sizeof(uint8_t);
  }
  SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.camera.freq), offset);
  offset += (uint16_t)sizeof(uint32_t);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_image_processor_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.image_processor.mode), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.image_processor.store), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.image_processor.signal_threshold_pix), offset);
  offset += (uint16_t)sizeof(uint16_t);
  SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.image_processor.dark_threshold_pix), offset);
  offset += (uint16_t)sizeof(uint16_t);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.image_processor.background_compensation), offset);
  offset += (uint16_t)sizeof(uint8_t);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_centroiding_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.enable_filter), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.max_quality), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.dark_threshold_pix), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.min_quality), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.max_intensity), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.min_intensity), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.max_magnitude), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.gaussian_cmax_pix), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.gaussian_cmin_pix), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.transmatrix_00), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.transmatrix_01), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.transmatrix_10), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.centroiding.transmatrix_11), offset);
  offset += (uint16_t)sizeof(float);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_lisa_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.mode), offset);
  offset += (uint16_t)sizeof(uint32_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.prefilter_distance_threshold_mm), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.prefilter_angle_threshold_rad), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.field_of_view_width_rad), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.field_of_view_height_rad), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.float_star_limit_mm), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.close_star_limit_mm), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.rating_weight_close_star_count), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.rating_weight_fraction_close), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.rating_weight_mean_sum), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.rating_weight_db_star_count), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.max_combinations), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.nr_stars_stop), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.lisa.fraction_close_stop), offset);
  offset += (uint16_t)sizeof(float);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_matching_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.matching.squared_distance_limit_mm), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.matching.squared_shift_limit_mm), offset);
  offset += (uint16_t)sizeof(float);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_tracking_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.tracking.thin_limit_deg), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.tracking.outlier_threshold_mm), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.tracking.outlier_threshold_quest_mm), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.tracking.tracker_choice), offset);
  offset += (uint16_t)sizeof(uint8_t);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_validation_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.validation.stable_count), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.validation.max_difference_deg), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.validation.min_tracker_confidence), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.validation.min_matched_stars), offset);
  offset += (uint16_t)sizeof(uint8_t);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_algo_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.algo.mode), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.algo.l2t_min_confidence), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.algo.l2t_min_matched), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.algo.t2l_min_confidence), offset);
  offset += (uint16_t)sizeof(float);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.algo.t2l_min_matched), offset);
  offset += (uint16_t)sizeof(uint8_t);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_subscription_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  for (int i = 0; i < SAGITTA_PARAMETER_SUBSCRIPTION_LENGTH; i++)
  {
    SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.subscription[i]), offset);
    offset += (uint16_t)sizeof(uint8_t);
  }

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_parameter_auto_threshold_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetTlmID_ + (uint16_t)(sizeof(sagitta_driver->info.tlm_id));

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.auto_threshold.mode), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.auto_threshold.desired_blobs_count), offset);
  offset += (uint16_t)sizeof(uint8_t);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.auto_threshold.min_threshold), offset);
  offset += (uint16_t)sizeof(uint16_t);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.auto_threshold.max_threshold), offset);
  offset += (uint16_t)sizeof(uint16_t);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.read_parameter.auto_threshold.threshold_kp), offset);
  offset += (uint16_t)sizeof(float);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_action_reply_(SAGITTA_Driver* sagitta_driver)
{
  uint8_t status;
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&status, (uint16_t)SAGITTA_kTlmOffsetTlmStatus_);
  sagitta_driver->info.tlm_status = status;

  // TODO_L actionごとにxxhashをチェック

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_telemetry_(SAGITTA_Driver* sagitta_driver)
{
  uint8_t status;
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&status, (uint16_t)SAGITTA_kTlmOffsetTlmStatus_);
  sagitta_driver->info.tlm_status = status;

  SAGITTA_analyze_rec_data_unix_time_(sagitta_driver);

  switch ((SAGITTA_TLM_ID)sagitta_driver->info.tlm_id)
  {
  case SAGITTA_TLM_ID_POWER:
    return SAGITTA_analyze_rec_data_power_(sagitta_driver);
  case SAGITTA_TLM_ID_SOLUTION:
    return SAGITTA_analyze_rec_data_quaternion_(sagitta_driver);
  case SAGITTA_TLM_ID_TEMPERATURE:
    return SAGITTA_analyze_rec_data_temperature_(sagitta_driver);
  case SAGITTA_TLM_ID_HISTOGRAM:
    return SAGITTA_analyze_rec_data_histogram_(sagitta_driver);
  case SAGITTA_TLM_ID_BLOBS:
    return SAGITTA_analyze_rec_data_blobs_(sagitta_driver);
  case SAGITTA_TLM_ID_CENTROIDS:
    return SAGITTA_analyze_rec_data_centroids_(sagitta_driver);
  case SAGITTA_TLM_ID_AUTO_BLOB:
    return SAGITTA_analyze_rec_data_auto_blob_(sagitta_driver);
  case SAGITTA_TLM_ID_MATCHED_CENTROIDS:
    return SAGITTA_analyze_rec_data_matched_centroids_(sagitta_driver);
  default:
    return DS_ERR_CODE_ERR;
  }
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_power_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.mcu_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.mcu_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.mcu_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.mcu_voltage_V);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.fpga_core_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.fpga_core_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.fpga_core_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.fpga_core_voltage_V);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.fpga_18_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.fpga_18_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.fpga_18_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.fpga_18_voltage_V);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.fpga_25_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.fpga_25_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.fpga_25_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.fpga_25_voltage_V);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_21_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_21_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_21_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_21_voltage_V);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_pix_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_pix_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_pix_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_pix_voltage_V);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_33_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_33_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_33_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_33_voltage_V);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_res_current_A), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_res_current_A);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.power.cmv_res_voltage_V), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.power.cmv_res_voltage_V);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_quaternion_(SAGITTA_Driver* sagitta_driver)
{
  float quaternion_array_i2c[PHYSICAL_CONST_QUATERNION_DIM];
  float track_quaternion_array_i2c[PHYSICAL_CONST_QUATERNION_DIM];
  float lisa_quaternion_array_i2c[PHYSICAL_CONST_QUATERNION_DIM];
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  for (uint8_t i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(quaternion_array_i2c[i]), offset);
    offset += (uint16_t)sizeof(quaternion_array_i2c[i]);
  }
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.track_confidence), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.track_confidence);
  for (uint8_t i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(track_quaternion_array_i2c[i]), offset);
    offset += (uint16_t)sizeof(track_quaternion_array_i2c[i]);
  }
  QUATERNION_make_from_array(&sagitta_driver->info.telemetry.solution.track_quaternion_i2c, track_quaternion_array_i2c, QUATERNION_SCALAR_POSITION_FIRST);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.num_stars_removed), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.num_stars_removed);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.num_stars_centroided), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.num_stars_centroided);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.num_stars_matched), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.num_stars_matched);
  for (uint8_t i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(lisa_quaternion_array_i2c[i]), offset);
    offset += (uint16_t)sizeof(lisa_quaternion_array_i2c[i]);
  }
  QUATERNION_make_from_array(&sagitta_driver->info.telemetry.solution.lisa_quaternion_i2c, lisa_quaternion_array_i2c, QUATERNION_SCALAR_POSITION_FIRST);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.lisa_percentage_close), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.lisa_percentage_close);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.num_stars_lisa_close), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.num_stars_lisa_close);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.star_tracker_mode), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.star_tracker_mode);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.is_valid_quaternion), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.is_valid_quaternion);
  SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.solution.stable_count), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.stable_count);
  SAGITTA_memcpy_u8_from_rx_frame_decoded_((uint8_t*)(&(sagitta_driver->info.telemetry.solution.solution_strategy)), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.solution.solution_strategy);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  C2A_MATH_ERROR ret;
  ret = QUATERNION_make_from_array(&sagitta_driver->info.telemetry.solution.quaternion_i2c, quaternion_array_i2c, QUATERNION_SCALAR_POSITION_FIRST);
  if (sagitta_driver->info.telemetry.solution.is_valid_quaternion == 1 && ret != C2A_MATH_ERROR_OK) return DS_ERR_CODE_ERR;

  sagitta_driver->info.quaternion_i2b = QUATERNION_product(sagitta_driver->info.telemetry.solution.quaternion_i2c,
                                                           sagitta_driver->info.frame_transform_c2b);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_temperature_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.temperature.mcu_degC), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.temperature.mcu_degC);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.temperature.cmos_degC), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.temperature.cmos_degC);
  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.temperature.fpga_degC), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.temperature.fpga_degC);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_histogram_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  for (uint8_t i = 0; i < SAGITTA_TELEMETRY_HISTOGRAM_LENGTH; i++)
  {
    SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.histogram_a_pix[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.histogram_a_pix[i]);
  }
  for (uint8_t i = 0; i < SAGITTA_TELEMETRY_HISTOGRAM_LENGTH; i++)
  {
    SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.histogram_b_pix[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.histogram_b_pix[i]);
  }
  for (uint8_t i = 0; i < SAGITTA_TELEMETRY_HISTOGRAM_LENGTH; i++)
  {
    SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.histogram_c_pix[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.histogram_c_pix[i]);
  }
  for (uint8_t i = 0; i < SAGITTA_TELEMETRY_HISTOGRAM_LENGTH; i++)
  {
    SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.histogram_d_pix[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.histogram_d_pix[i]);
  }

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_blobs_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.blobs.count), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.blobs.count);
  SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.blobs.count_used), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.blobs.count_used);
  SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.blobs.four_lines_skipped), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.blobs.four_lines_skipped);
  for (uint8_t i = 0; i < SAGITTA_TELEMETRY_BLOBS_LENGTH; i++)
  {
    SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.blobs.x_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.blobs.x_coordinate[i]);
    SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.blobs.y_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.blobs.y_coordinate[i]);
  }

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_centroids_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  SAGITTA_memcpy_u16_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.centroids.count), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.centroids.count);
  for (uint8_t i = 0; i < SAGITTA_TELEMETRY_CENTROIDS_LENGTH; i++)
  {
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.centroids.x_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.centroids.x_coordinate[i]);
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.centroids.y_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.centroids.y_coordinate[i]);
    SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.centroids.magnitude[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.centroids.magnitude[i]);
  }

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_auto_blob_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.auto_blob_threshold), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.auto_blob_threshold);

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE SAGITTA_analyze_rec_data_matched_centroids_(SAGITTA_Driver* sagitta_driver)
{
  uint16_t offset = (uint16_t)SAGITTA_kTlmOffsetData_;

  SAGITTA_memcpy_u8_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.matched_centroids.count), offset);
  offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.matched_centroids.count);
  for (uint8_t i = 0; i < SAGITTA_TELEMETRY_MATCHED_CENTROIDS_LENGTH; i++)
  {
    SAGITTA_memcpy_u32_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.matched_centroids.id[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.matched_centroids.id[i]);
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.matched_centroids.x_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.matched_centroids.x_coordinate[i]);
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.matched_centroids.y_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.matched_centroids.y_coordinate[i]);
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.matched_centroids.error_x_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.matched_centroids.error_x_coordinate[i]);
    SAGITTA_memcpy_float_from_rx_frame_decoded_(&(sagitta_driver->info.telemetry.matched_centroids.error_y_coordinate[i]), offset);
    offset += (uint16_t)sizeof(sagitta_driver->info.telemetry.matched_centroids.error_y_coordinate[i]);
  }

  SAGITTA_analyze_rec_data_xxhash_(sagitta_driver, offset + SAGITTA_XXHASH_SIZE - 1);

  return DS_ERR_CODE_OK;
}

static void SAGITTA_analyze_rec_data_xxhash_(SAGITTA_Driver* sagitta_driver, uint16_t rx_data_len)
{
  memcpy(&(sagitta_driver->info.xxhash),
         &(SAGITTA_rx_frame_decoded_[SAGITTA_TX_HEADER_SIZE + rx_data_len - SAGITTA_XXHASH_SIZE]),
         SAGITTA_XXHASH_SIZE);

  uint32_t calced_hash = XXH32(&(SAGITTA_rx_frame_decoded_[SAGITTA_TX_HEADER_SIZE]), rx_data_len - SAGITTA_XXHASH_SIZE, SAGITTA_kXxhashSeed_);

  if (sagitta_driver->info.xxhash != calced_hash)
  {
    sagitta_driver->info.xxhash_state = SAGITTA_XXHASH_STATE_NG;
  }
  else
  {
    sagitta_driver->info.xxhash_state = SAGITTA_XXHASH_STATE_OK;
  }
}

static void SAGITTA_memcpy_float_from_rx_frame_decoded_(float* output, uint16_t offset)
{
  memcpy(output, &(SAGITTA_rx_frame_decoded_[offset]), (uint8_t)sizeof(float));
}

static void SAGITTA_memcpy_u32_from_rx_frame_decoded_(uint32_t* output, uint16_t offset)
{
  memcpy(output, &(SAGITTA_rx_frame_decoded_[offset]), (uint8_t)sizeof(uint32_t));
}

static void SAGITTA_memcpy_s16_from_rx_frame_decoded_(int16_t* output, uint16_t offset)
{
  memcpy(output, &(SAGITTA_rx_frame_decoded_[offset]), (uint8_t)sizeof(int16_t));
}

static void SAGITTA_memcpy_u16_from_rx_frame_decoded_(uint16_t* output, uint16_t offset)
{
  memcpy(output, &(SAGITTA_rx_frame_decoded_[offset]), (uint8_t)sizeof(uint16_t));
}

static void SAGITTA_memcpy_u8_from_rx_frame_decoded_(uint8_t* output, uint16_t offset)
{
  memcpy(output, &(SAGITTA_rx_frame_decoded_[offset]), (uint8_t)sizeof(uint8_t));
}

#pragma section
