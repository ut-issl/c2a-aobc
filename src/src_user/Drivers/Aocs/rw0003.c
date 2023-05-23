#pragma section REPRO
/**
* @file   rw0003.c
* @brief  RW0003のDriver
*/

#ifndef SILS_FW
#include <Arduino.h>
#endif

#include "./rw0003.h"
#include <src_core/Library/print.h>
#include <string.h>

#include <src_core/Library/crc.h>
#include "../../Library/vector3.h"
#include "../../Library/c2a_math.h"

// #define DRIVER_RW0003_DEBUG_SHOW_REC_DATA

/**
 * @enum   RW0003_POLL
 * @brief  POLL bitの設定
 * @note   uint8_tを想定
 */
typedef enum
{
  RW0003_POLL_WITHOUT_REPLY = 0,
  RW0003_POLL_WITH_REPLY    = 1,
} RW0003_POLL;


#define RW0003_WRITE_MODE_FILE_DATA_LENGTH (4)  //!< Mode file データ書き込み長 [byte]
#define RW0003_READ_OVER_HAED              (4)  //!< TLMに付加される情報のバイト数 [byte]
#define RW0003_NSP_HEADER_SIZE             (2)  //!< Nano Satellite Protcol header: MCF(Message Control Field) + Source Address = 2 byte
#define RW0003_NSP_CRC_SIZE                (2)  //!< Nano Satellite Protcol CRC size
#define RW0003_FOOTER_SIZE                 (1)  //!< TX/RX共通
#define RW0003_RX_MAX_FRAME_SIZE           (15) //!< SLIPを考慮した(現時点での)最大テレメフレーム長( 9 + 6 )
#define RW0003_STREAM_TLM_CMD              (0)


static const uint8_t RW0003_kSlipFend_  = 0xc0;  //!< SLIP FEND
static const uint8_t RW0003_kSlipFesc_  = 0xdb;  //!< SLIP FESC
static const uint8_t RW0003_kSlipTfend_ = 0xdc;  //!< SLIP TFEND
static const uint8_t RW0003_kSlipTfesc_ = 0xdd;  //!< SLIP TFESC

static const uint8_t RW0003_kSrcAddress_ = 0x11;  //!< I2C source address. 実際は0xc0以外の適当な値でよくこの値は仕様書で例として与えられているもの

static const uint8_t RW0003_kCmdIdInit_      = 0x01;
static const uint8_t RW0003_kCmdIdDiagnostic_= 0x04;
static const uint8_t RW0003_kCmdIdReadFile_  = 0x07;
static const uint8_t RW0003_kCmdIdWriteFile_ = 0x08;
static const uint8_t RW0003_kCmdIdMax_       = 0x1f;

static const uint8_t RW0003_kWriteAddressIdle_   = 0x00;
static const uint8_t RW0003_kWriteAddressSpeed_  = 0x03;
static const uint8_t RW0003_kWriteAddressTorque_ = 0x12;

static const uint8_t RW0003_kDiagnosticsChannel_ = 0x00;

static const uint8_t RW0003_kReadAddressVDD_         = 0x02;
static const uint8_t RW0003_kReadAddressTemperature_ = 0x03;
static const uint8_t RW0003_kReadAddressSpeed_       = 0x15;
static const uint8_t RW0003_kReadAddressSEUCount_    = 0x18;
static const uint8_t RW0003_kReadAddressFaultState_  = 0x19;
static const uint8_t RW0003_kReadAddressLimitSpeed1_ = 0x33;
static const uint8_t RW0003_kReadAddressLimitSpeed2_ = 0x34;

static const uint8_t RW0003_kMcfReadEdac_ = 0xa7;   //!< EDAC memory読み出し返答テレメのMessage Control Field
static const uint8_t RW0003_kMcfDiagnostics_ = 0xa4;//!< Diagnostics返答テレメのMessage Control Field

static const int      RW0003_kCrcRevFlag_ = 0;      //!< CRC関数の反転フラグ
static const uint16_t RW0003_kCrcInitial_ = 0xffff; //!< CRC計算初期値


const float RW0003_kMaxTorqueNm         = 4.0e-3f;   //!< RW0003 max torque [Nm]
const float RW0003_kMomentOfInertiaKgm2 = 3.33e-6f;  //!< RW0003 moment of inertia [kg m2]


static DS_ERR_CODE RW0003_load_driver_super_init_settings_(DriverSuper* super);
static DS_ERR_CODE RW0003_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver);

static DS_CMD_ERR_CODE RW0003_convert_to_mcf_(const RW0003_POLL poll, const uint8_t cmd_id, uint8_t* mcf);
static void RW0003_convert_to_nsp_with_data_(const uint8_t i2c_address,
                                             const uint8_t mcf,
                                             const uint8_t* payload,
                                             const size_t payload_length,
                                             uint8_t* nsp_data);
static void RW0003_decode_slip_(const uint8_t* in, const size_t in_length, uint8_t* out, size_t* out_length);
static void RW0003_encode_slip_(const uint8_t* in, const size_t in_length, uint8_t* out, size_t* out_length);

static DS_CMD_ERR_CODE RW0003_write_mode_file_(RW0003_Driver* rw0003_driver,
                                                   const uint8_t cmd_id,
                                                   const uint8_t write_data[RW0003_WRITE_MODE_FILE_DATA_LENGTH]);
static DS_CMD_ERR_CODE RW0003_read_edac_(RW0003_Driver* rw0003_driver, const uint8_t file_addr);
static DS_CMD_ERR_CODE RW0003_send_cmd_(RW0003_Driver* rw0003_driver,
                                            uint8_t* write_data,
                                            const size_t write_data_length);
static DS_CMD_ERR_CODE RW0003_send_read_(RW0003_Driver* rw0003_driver,
                                             uint8_t* write_data,
                                             const size_t write_data_length,
                                             const size_t read_data_length);


DS_INIT_ERR_CODE RW0003_init(RW0003_Driver* rw0003_driver, uint8_t ch, uint8_t i2c_address, DS_StreamRecBuffer* rx_buffer)
{
  rw0003_driver->info.speed_rad_s = 0.0f;
  rw0003_driver->info.torque_Nm   = 0.0f;
  rw0003_driver->info.temperature_degC = 0.0f;
  rw0003_driver->info.vdd_V = -1.0f;
  rw0003_driver->info.seu_count = -1.0f;
  rw0003_driver->info.fault_state = -1.0f;
  rw0003_driver->info.diagnostic_reset_reason = -1;
  rw0003_driver->info.speed_limit1_rad_s = 900.0f;  // デフォルト値を初期値とする TODO_L 設定・読み出し関数を作る
  rw0003_driver->info.speed_limit2_rad_s = 1000.0f; // デフォルト値を初期値とする TODO_L 設定・読み出し関数を作る
  rw0003_driver->info.rotation_direction_b[0] = 1.0f;
  rw0003_driver->info.rotation_direction_b[1] = 0.0f;
  rw0003_driver->info.rotation_direction_b[2] = 0.0f;
  rw0003_driver->info.crc_state = RW0003_REC_CRC_STATE_OK;

  rw0003_driver->driver.i2c_config.ch = ch;
  rw0003_driver->driver.i2c_config.device_address = i2c_address;
  rw0003_driver->driver.i2c_config.frequency_khz = 400;
  rw0003_driver->driver.i2c_config.stop_flag = 0;
  rw0003_driver->driver.i2c_config.timeout_threshold = 1000;

  DS_ERR_CODE ret;
  ret = DS_init(&(rw0003_driver->driver.super),
                &(rw0003_driver->driver.i2c_config),
                rx_buffer,
                RW0003_load_driver_super_init_settings_);
  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;
  return DS_INIT_OK;
}

DS_CMD_ERR_CODE RW0003_start_app(RW0003_Driver* rw0003_driver)
{
  // Command Definition
  DS_CMD_ERR_CODE ret;
  const uint8_t kStartAppPayload[] = { 0x00, 0x10, 0x00, 0x00 };
  const size_t payload_length = sizeof(kStartAppPayload) / sizeof(uint8_t);
  uint8_t mcf;
  ret = RW0003_convert_to_mcf_(RW0003_POLL_WITHOUT_REPLY, RW0003_kCmdIdInit_, &mcf);
  if (ret != DS_CMD_OK) return ret;

  // NSP message conversion
  const size_t kNspDataLength = RW0003_NSP_HEADER_SIZE + payload_length + RW0003_NSP_CRC_SIZE + RW0003_FOOTER_SIZE;
  uint8_t nsp_data[kNspDataLength];
  RW0003_convert_to_nsp_with_data_(rw0003_driver->driver.i2c_config.device_address,
                                   mcf, kStartAppPayload, payload_length, nsp_data);
  nsp_data[kNspDataLength - 1] = RW0003_kSlipFend_;

  ret = RW0003_send_cmd_(rw0003_driver, nsp_data, kNspDataLength);

  return ret;
}

DS_CMD_ERR_CODE RW0003_set_idle(RW0003_Driver* rw0003_driver)
{
  const uint8_t kIdleWriteData[RW0003_WRITE_MODE_FILE_DATA_LENGTH] = { 0, 0, 0, 0 };

  DS_CMD_ERR_CODE ret;
  ret = RW0003_write_mode_file_(rw0003_driver, RW0003_kWriteAddressIdle_, kIdleWriteData);

  return ret;
}

DS_CMD_ERR_CODE RW0003_drive_speed(RW0003_Driver* rw0003_driver, const float speed_rad_s)
{
  float clipped_speed_rad_s;
  clipped_speed_rad_s = C2A_MATH_clip_value_abs(speed_rad_s, rw0003_driver->info.speed_limit1_rad_s);

  uint8_t write_data[RW0003_WRITE_MODE_FILE_DATA_LENGTH];
  memcpy(write_data, &clipped_speed_rad_s, RW0003_WRITE_MODE_FILE_DATA_LENGTH);

  DS_CMD_ERR_CODE ret;
  ret = RW0003_write_mode_file_(rw0003_driver, RW0003_kWriteAddressSpeed_, write_data);

  return ret;
}

DS_CMD_ERR_CODE RW0003_drive_torque(RW0003_Driver* rw0003_driver, const float torque_Nm)
{
  float clipped_torque_Nm;
  clipped_torque_Nm = C2A_MATH_clip_value_abs(torque_Nm, RW0003_kMaxTorqueNm);

  uint8_t write_data[RW0003_WRITE_MODE_FILE_DATA_LENGTH];
  memcpy(write_data, &clipped_torque_Nm, RW0003_WRITE_MODE_FILE_DATA_LENGTH);

  DS_CMD_ERR_CODE ret;
  ret = RW0003_write_mode_file_(rw0003_driver, RW0003_kWriteAddressTorque_, write_data);

  if (ret == DS_CMD_OK) rw0003_driver->info.torque_Nm = clipped_torque_Nm;

  return ret;
}

DS_CMD_ERR_CODE RW0003_observe_speed(RW0003_Driver* rw0003_driver)
{
  return RW0003_read_edac_(rw0003_driver, RW0003_kReadAddressSpeed_);
}

DS_CMD_ERR_CODE RW0003_observe_temperature(RW0003_Driver* rw0003_driver)
{
  return RW0003_read_edac_(rw0003_driver, RW0003_kReadAddressTemperature_);
}

C2A_MATH_ERROR RW0003_set_rotation_direction_b(RW0003_Driver* rm3100_driver,
                                               const float rotation_direction_b[PHYSICAL_CONST_THREE_DIM])
{
  C2A_MATH_ERROR is_normalized = VECTOR3_is_normalized(rotation_direction_b);
  if (is_normalized != C2A_MATH_ERROR_OK) return is_normalized;
  VECTOR3_copy(rm3100_driver->info.rotation_direction_b,
              rotation_direction_b);
  return C2A_MATH_ERROR_OK;
}

DS_CMD_ERR_CODE RW0003_read_vdd(RW0003_Driver* rw0003_driver)
{
  return RW0003_read_edac_(rw0003_driver, RW0003_kReadAddressVDD_);
}

DS_CMD_ERR_CODE RW0003_read_seu_count(RW0003_Driver* rw0003_driver)
{
  return RW0003_read_edac_(rw0003_driver, RW0003_kReadAddressSEUCount_);
}

DS_CMD_ERR_CODE RW0003_read_fault_state(RW0003_Driver* rw0003_driver)
{
  return RW0003_read_edac_(rw0003_driver, RW0003_kReadAddressFaultState_);
}

DS_CMD_ERR_CODE RW0003_diagnostic(RW0003_Driver* rw0003_driver)
{
  DS_CMD_ERR_CODE ret;

  // Command Definition
  uint8_t mcf;
  ret = RW0003_convert_to_mcf_(RW0003_POLL_WITH_REPLY, RW0003_kCmdIdDiagnostic_, &mcf);
  if (ret != DS_CMD_OK) return ret;

  // NSP message conversion
  const size_t kNspDataLength = RW0003_NSP_HEADER_SIZE + sizeof(uint8_t) + RW0003_NSP_CRC_SIZE;
  uint8_t nsp_data[kNspDataLength];
  RW0003_convert_to_nsp_with_data_(rw0003_driver->driver.i2c_config.device_address,
                                   mcf, &RW0003_kDiagnosticsChannel_, sizeof(RW0003_kDiagnosticsChannel_), nsp_data);

  const size_t kSlipDataMaxLength = kNspDataLength + sizeof(RW0003_kSlipFend_);
  uint8_t slip_data[kSlipDataMaxLength];
  size_t slip_data_length = kNspDataLength;
  RW0003_encode_slip_(nsp_data, kNspDataLength, slip_data, &slip_data_length);
  slip_data[slip_data_length] = RW0003_kSlipFend_;
  slip_data_length++;

  const size_t kReadDataLength = RW0003_RX_MAX_FRAME_SIZE;
  ret = RW0003_send_read_(rw0003_driver, slip_data, slip_data_length, kReadDataLength);
  if (ret != DS_CMD_OK) return ret;

  return DS_CMD_OK;
}


static DS_ERR_CODE RW0003_load_driver_super_init_settings_(DriverSuper* super)
{
  DS_StreamConfig* stream_config;

  super->interface = I2C;

  stream_config = &(super->stream_config[RW0003_STREAM_TLM_CMD]);
  DSSC_enable(stream_config);

  DSSC_set_rx_frame_size(stream_config, RW0003_RX_MAX_FRAME_SIZE);  // 可変だがサイズ情報なし, 都度設定する
  DSSC_set_data_analyzer(stream_config, RW0003_analyze_rec_data_);

  return DS_ERR_CODE_OK;
}


static DS_CMD_ERR_CODE RW0003_convert_to_mcf_(const RW0003_POLL poll, const uint8_t cmd_id, uint8_t* mcf)
{
  if (cmd_id >= RW0003_kCmdIdMax_) return DS_CMD_ILLEGAL_PARAMETER;

  if (poll == RW0003_POLL_WITHOUT_REPLY)
  {
    *mcf = cmd_id | 0x00;
  }
  else
  {
    *mcf = cmd_id | 0x80;
  }

  return DS_CMD_OK;
}

static void RW0003_convert_to_nsp_with_data_(const uint8_t i2c_address,
                                             const uint8_t mcf,
                                             const uint8_t* payload,
                                             const size_t payload_length,
                                             uint8_t* nsp_data)
{
  // CRC calculation
  uint16_t crc16 = RW0003_kCrcInitial_;
  crc16 = CRC_calc_crc_16_ccitt_right(crc16, &i2c_address, sizeof(i2c_address), RW0003_kCrcRevFlag_);
  crc16 = CRC_calc_crc_16_ccitt_right(crc16, &RW0003_kSrcAddress_, sizeof(RW0003_kSrcAddress_), RW0003_kCrcRevFlag_);
  crc16 = CRC_calc_crc_16_ccitt_right(crc16, &mcf, sizeof(mcf), RW0003_kCrcRevFlag_);
  crc16 = CRC_calc_crc_16_ccitt_right(crc16, payload, payload_length, RW0003_kCrcRevFlag_);

  // NSP message generation
  nsp_data[0] = RW0003_kSrcAddress_;
  nsp_data[1] = mcf;
  memcpy(nsp_data + RW0003_NSP_HEADER_SIZE, payload, payload_length);
  memcpy(nsp_data + RW0003_NSP_HEADER_SIZE + payload_length, &crc16, sizeof(crc16));
}

static DS_CMD_ERR_CODE RW0003_write_mode_file_(RW0003_Driver* rw0003_driver,
                                                   const uint8_t write_address,
                                                   const uint8_t write_data[RW0003_WRITE_MODE_FILE_DATA_LENGTH])
{
  DS_CMD_ERR_CODE ret;

  // Write payload
  const size_t kAddressLength = 2;
  const size_t kPayloadLength = RW0003_WRITE_MODE_FILE_DATA_LENGTH + kAddressLength;
  uint8_t payload[kPayloadLength];
  payload[0] = 0x00;  // 第一Byteは必ず0になる。これまで定数で表現するのはやりすぎな気もするのでこのままにしておく
  payload[1] = write_address;
  memcpy(payload + kAddressLength, write_data, (size_t)RW0003_WRITE_MODE_FILE_DATA_LENGTH);

  // Command Definition
  uint8_t mcf;
  ret = RW0003_convert_to_mcf_(RW0003_POLL_WITHOUT_REPLY, RW0003_kCmdIdWriteFile_, &mcf);
  if (ret != DS_CMD_OK) return ret;

  // NSP message conversion
  const size_t kNspDataLength = RW0003_NSP_HEADER_SIZE + kPayloadLength + RW0003_NSP_CRC_SIZE;
  uint8_t nsp_data[kNspDataLength];
  RW0003_convert_to_nsp_with_data_(rw0003_driver->driver.i2c_config.device_address,
                                   mcf, payload, kPayloadLength, nsp_data);

  // SLIP
  const size_t kSlipDataMaxLength = kNspDataLength + RW0003_WRITE_MODE_FILE_DATA_LENGTH + RW0003_NSP_CRC_SIZE;
  uint8_t slip_data[kSlipDataMaxLength];
  size_t slip_data_length = kSlipDataMaxLength;
  RW0003_encode_slip_(nsp_data, kNspDataLength, slip_data, &slip_data_length);
  slip_data[slip_data_length] = RW0003_kSlipFend_;
  slip_data_length++;

  // Send data
  ret = RW0003_send_cmd_(rw0003_driver, slip_data, slip_data_length);

  return ret;
}

static DS_CMD_ERR_CODE RW0003_read_edac_(RW0003_Driver* rw0003_driver, const uint8_t read_address)
{
  if (read_address == 0x00) return DS_CMD_ILLEGAL_PARAMETER;  // 0 is mode file

  DS_CMD_ERR_CODE ret;

  // Command Definition
  uint8_t mcf;
  ret = RW0003_convert_to_mcf_(RW0003_POLL_WITH_REPLY, RW0003_kCmdIdReadFile_, &mcf);
  if (ret != DS_CMD_OK) return ret;

  // NSP message conversion
  const  size_t kNspDataLength = RW0003_NSP_HEADER_SIZE + sizeof(uint8_t) + RW0003_NSP_CRC_SIZE;
  uint8_t nsp_data[kNspDataLength];
  RW0003_convert_to_nsp_with_data_(rw0003_driver->driver.i2c_config.device_address,
                                   mcf, &read_address, sizeof(read_address), nsp_data);

  // SLIP: read_edacはCRCのみSLIPする可能性がある
  const size_t kSlipDataMaxLength = kNspDataLength + RW0003_NSP_CRC_SIZE;
  uint8_t slip_data[kSlipDataMaxLength];
  size_t slip_data_length = kSlipDataMaxLength;
  RW0003_encode_slip_(nsp_data, kNspDataLength, slip_data, &slip_data_length);
  slip_data[slip_data_length] = RW0003_kSlipFend_;
  slip_data_length++;

  const size_t kReadDataLength = RW0003_RX_MAX_FRAME_SIZE;
  ret = RW0003_send_read_(rw0003_driver, slip_data, slip_data_length, kReadDataLength);
  if (ret != DS_CMD_OK) return ret;

  return DS_CMD_OK;
}

static DS_CMD_ERR_CODE RW0003_send_read_(RW0003_Driver* rw0003_driver,
                                             uint8_t* write_data,
                                             const size_t write_data_length,
                                             const size_t read_data_length)
{
  DS_StreamConfig* stream_config;
  stream_config = &(rw0003_driver->driver.super.stream_config[RW0003_STREAM_TLM_CMD]);

  // read setting
  DSSC_set_rx_frame_size(stream_config, read_data_length);
  I2C_set_rx_length(&(rw0003_driver->driver.i2c_config), (uint32_t)read_data_length);
  DS_clear_rx_buffer(&(rw0003_driver->driver.super));

  // send
  I2C_set_stop_flag(&(rw0003_driver->driver.i2c_config), 0);
  DSSC_set_tx_frame(stream_config, write_data);
  DSSC_set_tx_frame_size(stream_config, write_data_length);
  DS_ERR_CODE ret;
  ret = DS_send_req_tlm_cmd(&(rw0003_driver->driver.super), RW0003_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
  // read
#ifndef SILS_FW
  delayMicroseconds(2000);  // RWの仕様でここでdelayをはさまないと通信失敗する。諸々調査したがこれしか解がない
#endif
  I2C_set_stop_flag(&(rw0003_driver->driver.i2c_config), 1);
  ret = DS_receive(&(rw0003_driver->driver.super));

  if (ret != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

#ifdef DRIVER_RW0003_DEBUG_SHOW_REC_DATA
  if (rw0003_driver->driver.super.config.info.rec_status_.ret_from_if_rx > 0)
  {
    Printf("rw rx_frame_: %d Bytes \n", rw0003_driver->driver.super.config.info.rec_status_.ret_from_if_rx);
    for (int i = 0; i < rw0003_driver->driver.super.config.info.rec_status_.ret_from_if_rx; i++)
    {
      Printf("%02x ", rw0003_driver->driver.super.stream_config[0].settings.rx_buffer_->buffer[i]);
      if (i % 4 == 3) Printf("   ");
    }
    Printf("\n");
  }
#endif

  ret = DS_analyze_rec_data(&(rw0003_driver->driver.super), RW0003_STREAM_TLM_CMD, rw0003_driver);
  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

static DS_CMD_ERR_CODE RW0003_send_cmd_(RW0003_Driver* rw0003_driver,
                                            uint8_t* write_data,
                                            const size_t write_data_length)
{
  DS_StreamConfig* stream_config;
  stream_config = &(rw0003_driver->driver.super.stream_config[RW0003_STREAM_TLM_CMD]);

  I2C_set_stop_flag(&(rw0003_driver->driver.i2c_config), 1);
  DSSC_set_tx_frame(stream_config, write_data);
  DSSC_set_tx_frame_size(stream_config, write_data_length);

  DS_ERR_CODE ret;
  ret = DS_send_general_cmd(&(rw0003_driver->driver.super), RW0003_STREAM_TLM_CMD);

  if (ret != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  return DS_CMD_OK;
}

static DS_ERR_CODE RW0003_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver)
{
  const uint8_t* rx_data = DSSC_get_rx_frame(stream_config);
  size_t rx_data_length = DSSC_get_rx_frame_size(stream_config);
  RW0003_Driver* rw0003_driver = (RW0003_Driver*)driver;

  // SLIP decode
  uint8_t decoded_rx_data[RW0003_RX_MAX_FRAME_SIZE];
  size_t decoded_rx_data_length = RW0003_RX_MAX_FRAME_SIZE;
  RW0003_decode_slip_(rx_data, rx_data_length, decoded_rx_data, &decoded_rx_data_length);

  // CRC: SLIP解除後に行う
  uint16_t crc16 = RW0003_kCrcInitial_;
  crc16 = CRC_calc_crc_16_ccitt_right(crc16, &RW0003_kSrcAddress_, sizeof(RW0003_kSrcAddress_), RW0003_kCrcRevFlag_);
  crc16 = CRC_calc_crc_16_ccitt_right(crc16,
                             &(rw0003_driver->driver.i2c_config.device_address),
                             sizeof(rw0003_driver->driver.i2c_config.device_address), RW0003_kCrcRevFlag_);
  crc16 = CRC_calc_crc_16_ccitt_right(crc16,
                             decoded_rx_data,
                             decoded_rx_data_length - RW0003_FOOTER_SIZE, RW0003_kCrcRevFlag_);

  if (crc16 == 0x0000)
  {
    rw0003_driver->info.crc_state = RW0003_REC_CRC_STATE_OK;
  }
  else
  {
    rw0003_driver->info.crc_state = RW0003_REC_CRC_STATE_NG;
  }
  // analyze
  size_t pos = 0;
  uint8_t RW0003_rx_mcf = decoded_rx_data[pos];
  pos++;
  if (RW0003_rx_mcf == RW0003_kMcfReadEdac_)
  {
    uint8_t read_address = decoded_rx_data[pos];
    pos++;
    if (read_address == RW0003_kReadAddressTemperature_)
    {
      memcpy(&rw0003_driver->info.temperature_degC, decoded_rx_data + pos, sizeof(float));
    }
    else if (read_address == RW0003_kReadAddressSpeed_)
    {
      memcpy(&rw0003_driver->info.speed_rad_s, decoded_rx_data + pos, sizeof(float));
    }
    else if (read_address == RW0003_kReadAddressVDD_)
    {
      memcpy(&rw0003_driver->info.vdd_V, decoded_rx_data + pos, sizeof(float));
    }
    else if (read_address == RW0003_kReadAddressSEUCount_)
    {
      memcpy(&rw0003_driver->info.seu_count, decoded_rx_data + pos, sizeof(float));
    }
    else if (read_address == RW0003_kReadAddressFaultState_)
    {
      memcpy(&rw0003_driver->info.fault_state, decoded_rx_data + pos, sizeof(float));
    }
    else
    {
      return DS_ERR_CODE_ERR;
    }
  }
  else if (RW0003_rx_mcf == RW0003_kMcfDiagnostics_)
  {
    uint8_t read_address = decoded_rx_data[pos];
    pos++;
    if (read_address == RW0003_kDiagnosticsChannel_)
    {
      memcpy(&rw0003_driver->info.diagnostic_reset_reason, decoded_rx_data + pos, sizeof(int32_t));
    }
  }
  return DS_ERR_CODE_OK;
}

static void RW0003_decode_slip_(const uint8_t* in, const size_t in_length, uint8_t* out, size_t* out_length)
{
  size_t in_frame_length = 0;
  // footer search
  for (size_t i = 0; i < in_length; i++)
  {
    if (in[i] == RW0003_kSlipFend_)
    {
      in_frame_length = i + 1;
      break;
    }
  }
  if (in_frame_length == 0) return;

  // db search
  size_t out_cnt = 0;
  for (size_t in_cnt = 0; in_cnt < in_frame_length; in_cnt++)
  {
    if (in[in_cnt] == RW0003_kSlipFesc_)
    {
      if (in[in_cnt + 1] == RW0003_kSlipTfend_)
      {
        out[out_cnt] = RW0003_kSlipFend_;
        out_cnt++;
      }
      else if (in[in_cnt + 1] == RW0003_kSlipTfesc_)
      {
        out[out_cnt] = RW0003_kSlipFesc_;
        out_cnt++;
      }
      else
      {
        // TODO_L: error処理
        return;
      }
      in_cnt++;
    }
    else
    {
      out[out_cnt] = in[in_cnt];
      out_cnt++;
    }
  }
  *out_length = out_cnt;
}

static void RW0003_encode_slip_(const uint8_t* in, const size_t in_length, uint8_t* out, size_t* out_length)
{
  size_t out_cnt = 0;
  for (size_t in_cnt = 0; in_cnt < in_length; in_cnt++)
  {
    if (in[in_cnt] == RW0003_kSlipFend_)
    {
      out[out_cnt] = RW0003_kSlipFesc_;
      out_cnt++;
      out[out_cnt] = RW0003_kSlipTfend_;
    }
    else if (in[in_cnt] == RW0003_kSlipFesc_)
    {
      out[out_cnt] = RW0003_kSlipFesc_;
      out_cnt++;
      out[out_cnt] = RW0003_kSlipTfesc_;
    }
    else
    {
      out[out_cnt] = in[in_cnt];
    }
    out_cnt++;
  }
  *out_length = out_cnt;
}

#pragma section
