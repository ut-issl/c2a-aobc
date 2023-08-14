#pragma section REPRO
/**
* @file   stim210.c
* @brief  STIM210のDriver
*/

#include "stim210.h"
#include <src_core/Library/endian.h>
#include <src_core/Library/print.h>
#include <string.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/crc8.h>


#define STIM210_STREAM_TLM_CMD (0)       //!< テレコマで使うストリーム
#define STIM210_TX_MAX_DATA_SIZE (15)    //!< TXの最大サイズ（フッターを含む）
#define STIM210_TX_FOOTER_SIZE (1)       //!< TXのフッターサイズ
#define STIM210_RX_HEADER_SIZE (1)       //!< RXのヘッダーサイズ
#define STIM210_RX_FOOTER_SIZE (2)       //!< RXのフッターサイズ（CRLFあり）
#define STIM210_RX_FOOTER_NONE_SIZE (0)  //!< RXのフッターサイズ（CRLFなし）

#define STIM210_RX_GYRO_OUTPUT_SIZE (3)  //!< テレメのジャイロ出力バイト数
#define STIM210_RX_TEMPERATURE_SIZE (2)  //!< テレメの温度計出力バイト数
#define STIM210_RX_LATENCY_SIZE (2)      //!< テレメの遅延時間バイト数

static const float STIM210_kMaxAngVelBias_rad_s_ = 0.3f; //!< 角速度バイアスノルム最大値 rad/s 値は過去衛星をベースに余裕をもって設定

// #define STIM210_DEBUG_SHOW_REC_DATA (0)  //!< STIM210テレメのデバッグ表示をありにする場合は定義する

//!< ヘッダーフッター
static uint8_t STIM210_rx_header_[STIM210_RX_HEADER_SIZE] = { 0x90 };                 //!< RXのヘッダー。ノーマルモードのフォーマットによって可変。
static const uint8_t STIM210_rx_footer_crlf_[STIM210_RX_FOOTER_SIZE] = { 0x0d, 0x0a}; //!< RXのフッター。終端子がCRLFの時のみ使用。
static const uint8_t STIM210_tx_footer_[STIM210_TX_FOOTER_SIZE] = { 0x0d };           //!< TXのフッター。常に用いる。

//!< ノーマルモードRX関連パラメータ
static const int16_t STIM210_normal_mode_rx_frame_size_crlf_[9] = { 14, 17, 20, 15, 16, 17, 21, 22, 23 }; //!< ノーマルモードのフォーマットごとのバイト数 (CRLFあり)
static const int16_t STIM210_normal_mode_rx_frame_size_none_[9] = { 12, 15, 18, 13, 14, 15, 19, 20, 21 }; //!< ノーマルモードのフォーマットごとのバイト数 (CRLFなし)
static const uint8_t STIM210_normal_mode_format_idx_[9]    = { 0x90, 0x92, 0xa0, 0xa2, 0xa4, 0xa5, 0x99, 0xa6, 0xa8}; //!< ノーマルモードでのテレメの1byte目に含まれるidx

//!< 工学値変換
static const float STIM210_kScaleRaw2AngVel_[STIM210_RX_GYRO_OUTPUT_SIZE] = { 4.0f, 0.015625f, 6.103515e-5f };
static const float STIM210_kScaleRaw2Ang_[STIM210_RX_GYRO_OUTPUT_SIZE] = { 0.03125f, 1.2207031e-4f, 4.7683716e-7 };
static const float STIM210_kScaleRaw2Temperature_[STIM210_RX_TEMPERATURE_SIZE] = { 1.0f, 3.90625e-3f };
static const float STIM210_kScaleRaw2Latency_[STIM210_RX_LATENCY_SIZE] = { 2.56e-4f, 1.0e-6f };
static const float STIM210_kMaxAngVelRadSec_ = 512.0f;
static const float STIM210_kMaxAngRad_ = 2.0f;
static const float STIM210_kMaxTemperatureDegC_ = 128.0f;

//!< CRC計算
static const bool     STIM210_kCrcRevFlag_ = false; //!< 反転フラグ
static const uint16_t STIM210_kCrcInitial_ = 0xff;  //!< 初期値

static DS_CMD_ERR_CODE STIM210_send_cmd_(STIM210_Driver* stim210_driver, uint8_t param_size, const char* cmd_param);
static DS_ERR_CODE STIM210_load_driver_super_init_settings_(DriverSuper* p_super);
static DS_ERR_CODE STIM210_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver);
static DS_CMD_ERR_CODE STIM210_set_rec_frame_size_(STIM210_Driver* stim210_driver);
static int STIM210_analyze_normal_mode_format_idx_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_analyze_gyro_output_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_analyze_gyro_status_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_analyze_buffer_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_analyze_temperature_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_analyze_counter_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_analyze_latency_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_analyze_crc_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx);
static int STIM210_convert_gyro_output_(STIM210_Driver* stim210_driver, const uint8_t* gyro_output_bytes);
static int STIM210_convert_temperature_(STIM210_Driver* stim210_driver, const uint8_t* temperature_bytes);
static int STIM210_convert_latency_(STIM210_Driver* stim210_driver, const uint8_t* latency_bytes);

static void STIM210_calc_ang_vel_calibration_(STIM210_Info* info);

int STIM210_init(STIM210_Driver* stim210_driver,
                 uint8_t ch,
                 uint8_t ch_gpio_trig,
                 uint8_t ch_gpio_reset,
                 DS_StreamRecBuffer* rx_buffer)
{
  DS_ERR_CODE ret;
  GPIO_ERR_CODE ret_gpio_trig_setting, ret_gpio_reset_setting;

  stim210_driver->driver.uart_config.ch = ch;
  stim210_driver->driver.uart_config.baudrate = 460800;
  stim210_driver->driver.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  stim210_driver->driver.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  stim210_driver->driver.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  ret = DS_init(&(stim210_driver->driver.super),
                &(stim210_driver->driver.uart_config),
                rx_buffer,
                STIM210_load_driver_super_init_settings_);
  if (ret != DS_ERR_CODE_OK) return 1;

  stim210_driver->driver.ch_gpio_trig = ch_gpio_trig;
  stim210_driver->driver.ch_gpio_reset = ch_gpio_reset;

  ret_gpio_trig_setting  = (GPIO_ERR_CODE)GPIO_set_direction(stim210_driver->driver.ch_gpio_trig, GPIO_OUTPUT);
  ret_gpio_reset_setting = (GPIO_ERR_CODE)GPIO_set_direction(stim210_driver->driver.ch_gpio_reset, GPIO_OUTPUT);

  if ((ret_gpio_trig_setting != GPIO_OK) || (ret_gpio_reset_setting != GPIO_OK)) return 2;

  ret_gpio_trig_setting  = (GPIO_ERR_CODE)GPIO_set_output(stim210_driver->driver.ch_gpio_trig, GPIO_HIGH); // HIGHで初期化する
  ret_gpio_reset_setting = (GPIO_ERR_CODE)GPIO_set_output(stim210_driver->driver.ch_gpio_reset, GPIO_LOW); // LOWで初期化する

  if ((ret_gpio_trig_setting != GPIO_OK) || (ret_gpio_reset_setting != GPIO_OK)) return 3;

  stim210_driver->info.frame_transform_c2b = QUATERNION_make_unit();
  MATRIX33_make_unit(stim210_driver->info.ang_vel_scale_factor_compo);
  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    stim210_driver->info.ang_vel_bias_compo_rad_sec[axis] = 0.0f;
  }

  return STIM210_reset_param(stim210_driver);
}

int STIM210_reset_param(STIM210_Driver* stim210_driver)
{
  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    stim210_driver->info.ang_vel_raw_compo_rad_sec[axis] = 0.0f;
    stim210_driver->info.ang_compo_rad[axis] = 0.0f;
    stim210_driver->info.temperature_compo_degC[axis] = 0.0f;
  }

  STIM210_calc_ang_vel_calibration_(&(stim210_driver->info));

  stim210_driver->info.status = 1; // 0がVALID、1がINVALIDなので観測のしていない初期値は1
  stim210_driver->info.counter = 0;
  stim210_driver->info.latency_sec = 0;
  stim210_driver->info.operation_mode = STIM210_OPERATION_INIT_MODE;
  stim210_driver->info.normal_mode_format = STIM210_NORMAL_MODE_STANDARD;
  stim210_driver->info.gyro_output_mode = STIM210_GYRO_OUTPUT_ANGULAR_RATE;
  stim210_driver->info.termination_mode = STIM210_TERMINATION_OFF;
  stim210_driver->info.low_pass_filter_frequency = STIM210_LPF_262HZ;
  stim210_driver->info.sample_rate = STIM210_SAMPLE_RATE_2000HZ;
  stim210_driver->info.crc_state = STIM210_CRC_STATE_OK;

  return 0;
}

// ---------- UART Telemetry ----------
DS_REC_ERR_CODE STIM210_rec(STIM210_Driver* stim210_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* stream_config;

  // NORMAL Modeだけデータを受け取る
  if (stim210_driver->info.operation_mode != STIM210_OPERATION_NORMAL_MODE) return DS_REC_OTHER_ERR;

  STIM210_set_rec_frame_size_(stim210_driver);
  ret = DS_receive(&(stim210_driver->driver.super));
  stream_config = &(stim210_driver->driver.super.stream_config[STIM210_STREAM_TLM_CMD]);

  if (ret != DS_ERR_CODE_OK)
  {
    return DS_REC_DS_RECEIVE_ERR;
  }

  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    return DS_REC_OTHER_ERR;
  }

  ret = DS_analyze_rec_data(&(stim210_driver->driver.super), STIM210_STREAM_TLM_CMD, stim210_driver);

  if (ret != DS_ERR_CODE_OK)
  {
    return DS_REC_ANALYZE_ERR;
  }

  return DS_REC_OK;
}

// ---------- GPIO ----------
int STIM210_send_tlm_by_gpio(STIM210_Driver* stim210_driver)
{
  int ret;
  ret = GPIO_set_output(stim210_driver->driver.ch_gpio_trig, GPIO_LOW);
  if (ret != 0) return ret;
  // GPIO LOWを250ns以上キープする
  ret = GPIO_set_output(stim210_driver->driver.ch_gpio_trig, GPIO_HIGH);
  return ret;
}

int STIM210_reset_by_gpio(STIM210_Driver* stim210_driver)
{
  int ret;
  ret = GPIO_set_output(stim210_driver->driver.ch_gpio_reset, GPIO_HIGH);
  if (ret != 0) return ret;
  // GPIO HIGHを1us以上キープする
  ret = GPIO_set_output(stim210_driver->driver.ch_gpio_reset, GPIO_LOW);
  if (ret != 0) return ret;
  return STIM210_reset_param(stim210_driver);
}

// ---------- UART Command ----------
DS_CMD_ERR_CODE STIM210_set_service_mode(STIM210_Driver* stim210_driver)
{
  DS_CMD_ERR_CODE ret;
  char code[STIM210_TX_MAX_DATA_SIZE];
  uint8_t len = 0;

  strcpy(code, "SERVICEMODE");
  len = strlen(code);

  if (stim210_driver->info.operation_mode == STIM210_OPERATION_SERVICE_MODE) return DS_CMD_ILLEGAL_CONTEXT;

  ret = STIM210_send_cmd_(stim210_driver, len, code);
  if (ret != DS_CMD_OK) return ret;

  stim210_driver->info.operation_mode = STIM210_OPERATION_SERVICE_MODE;

  return ret;
}

DS_CMD_ERR_CODE STIM210_set_normal_mode_format(STIM210_Driver* stim210_driver, STIM210_NORMAL_MODE_FORMAT normal_mode_format)
{
  DS_CMD_ERR_CODE ret;
  char code[STIM210_TX_MAX_DATA_SIZE];
  uint8_t len = 0;

  if (stim210_driver->info.operation_mode != STIM210_OPERATION_SERVICE_MODE) return DS_CMD_ILLEGAL_CONTEXT;

  switch (normal_mode_format)
  {
  case STIM210_NORMAL_MODE_STANDARD:
    strcpy(code, "d s");
    break;
  case STIM210_NORMAL_MODE_EXTENDED:
    strcpy(code, "d e");
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE:
    strcpy(code, "d j");
    break;
  case STIM210_NORMAL_MODE_RATE_COUNT:
    strcpy(code, "d k");
    break;
  case STIM210_NORMAL_MODE_RATE_LATENCY:
    strcpy(code, "d l");
    break;
  case STIM210_NORMAL_MODE_RATE_COUNT_LATENCY:
    strcpy(code, "d m");
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE_COUNT:
    strcpy(code, "d n");
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE_LATENCY:
    strcpy(code, "d o");
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE_COUNT_LATENCY:
    strcpy(code, "d p");
    break;
  default:
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  len = strlen(code);
  ret = STIM210_send_cmd_(stim210_driver, len, code);
  if (ret != DS_CMD_OK) return ret;

  stim210_driver->info.normal_mode_format = normal_mode_format;

  return ret;
}

DS_CMD_ERR_CODE STIM210_set_sample_rate(STIM210_Driver* stim210_driver, STIM210_SAMPLE_RATE sample_rate)
{
  DS_CMD_ERR_CODE ret;
  char code[STIM210_TX_MAX_DATA_SIZE];
  uint8_t len = 0;

  if (stim210_driver->info.operation_mode != STIM210_OPERATION_SERVICE_MODE) return DS_CMD_ILLEGAL_CONTEXT;

  switch (sample_rate)
  {
  case STIM210_SAMPLE_RATE_125HZ:
    strcpy(code, "m 125");
    break;
  case STIM210_SAMPLE_RATE_250HZ:
    strcpy(code, "m 250");
    break;
  case STIM210_SAMPLE_RATE_500HZ:
    strcpy(code, "m 500");
    break;
  case STIM210_SAMPLE_RATE_1000HZ:
    strcpy(code, "m 1000");
    break;
  case STIM210_SAMPLE_RATE_2000HZ:
    strcpy(code, "m 2000");
    break;
  case STIM210_SAMPLE_RATE_EXTERNAL_TRIGGER:
    strcpy(code, "m 0");
    break;
  default:
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  len = strlen(code);
  ret = STIM210_send_cmd_(stim210_driver, len, code);
  if (ret != DS_CMD_OK) return ret;

  stim210_driver->info.sample_rate = sample_rate;

  return ret;
}

DS_CMD_ERR_CODE STIM210_set_gyro_output(STIM210_Driver* stim210_driver, STIM210_GYRO_OUTPUT_MODE gyro_output_mode)
{
  DS_CMD_ERR_CODE ret;
  char code[STIM210_TX_MAX_DATA_SIZE];
  uint8_t len = 0;

  if (stim210_driver->info.operation_mode != STIM210_OPERATION_SERVICE_MODE) return DS_CMD_ILLEGAL_CONTEXT;

  switch (gyro_output_mode)
  {
  case STIM210_GYRO_OUTPUT_ANGULAR_RATE:
    strcpy(code, "u a");
    break;
  case STIM210_GYRO_OUTPUT_INCREMENTAL_ANGLE:
    strcpy(code, "u i");
    break;
  case STIM210_GYRO_OUTPUT_AVERAGE_ANGULAR_RATE:
    strcpy(code, "u m");
    break;
  case STIM210_GYRO_OUTPUT_INTEGRATED_ANGLE:
    strcpy(code, "u s");
    break;
  default:
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  len = strlen(code);
  ret = STIM210_send_cmd_(stim210_driver, len, code);
  if (ret != DS_CMD_OK) return ret;

  stim210_driver->info.gyro_output_mode = gyro_output_mode;

  return ret;
}

DS_CMD_ERR_CODE STIM210_set_termination_mode(STIM210_Driver* stim210_driver, STIM210_TERMINATION_MODE termination_mode)
{
  DS_CMD_ERR_CODE ret;
  char code[STIM210_TX_MAX_DATA_SIZE];
  uint8_t len = 0;

  if (stim210_driver->info.operation_mode != STIM210_OPERATION_SERVICE_MODE) return DS_CMD_ILLEGAL_CONTEXT;

  switch (termination_mode)
  {
  case STIM210_TERMINATION_OFF:
    strcpy(code, "r 2");
    break;
  case STIM210_TERMINATION_CRLF:
    strcpy(code, "r 3");
    break;
  default:
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  len = strlen(code);
  ret = STIM210_send_cmd_(stim210_driver, len, code);
  if (ret != DS_CMD_OK) return ret;

  stim210_driver->info.termination_mode = termination_mode;

  return ret;
}

DS_CMD_ERR_CODE STIM210_set_low_pass_filter(STIM210_Driver* stim210_driver, STIM210_LPF low_pass_filter_frequency)
{
  DS_CMD_ERR_CODE ret;
  char code[STIM210_TX_MAX_DATA_SIZE];
  uint8_t len = 0;
  if (stim210_driver->info.operation_mode != STIM210_OPERATION_SERVICE_MODE) return DS_CMD_ILLEGAL_CONTEXT;

  switch (low_pass_filter_frequency)
  {
  case STIM210_LPF_16HZ:
    strcpy(code, "f 16");
    break;
  case STIM210_LPF_33HZ:
    strcpy(code, "f 33");
    break;
  case STIM210_LPF_66HZ:
    strcpy(code, "f 66");
    break;
  case STIM210_LPF_131HZ:
    strcpy(code, "f 131");
    break;
  case STIM210_LPF_262HZ:
    strcpy(code, "f 262");
    break;
  default:
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  len = strlen(code);
  ret = STIM210_send_cmd_(stim210_driver, len, code);
  if (ret != DS_CMD_OK) return ret;

  stim210_driver->info.low_pass_filter_frequency = low_pass_filter_frequency;

  return (ret);
}

DS_CMD_ERR_CODE STIM210_set_normal_mode(STIM210_Driver* stim210_driver)
{
  DS_CMD_ERR_CODE ret;
  char code[STIM210_TX_MAX_DATA_SIZE];
  uint8_t len = 0;

  strcpy(code, "x N");
  len = strlen(code);

  if (stim210_driver->info.operation_mode != STIM210_OPERATION_SERVICE_MODE) return DS_CMD_ILLEGAL_CONTEXT;

  ret = STIM210_send_cmd_(stim210_driver, len, code);
  if (ret != DS_CMD_OK) return ret;

  stim210_driver->info.operation_mode = STIM210_OPERATION_NORMAL_MODE;

  return (ret);
}

C2A_MATH_ERROR STIM210_set_frame_transform_c2b(STIM210_Driver* stim210_driver, const Quaternion q_c2b)
{
  C2A_MATH_ERROR ret;
  ret = QUATERNION_is_normalized(q_c2b);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  stim210_driver->info.frame_transform_c2b = q_c2b;
  return ret;
}

C2A_MATH_ERROR STIM210_set_ang_vel_bias_compo_rad_s(STIM210_Driver* stim210_driver, const float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM])
{
  float norm_rad_s = VECTOR3_norm(ang_vel_bias_compo_rad_s);
  if (norm_rad_s > STIM210_kMaxAngVelBias_rad_s_) return C2A_MATH_ERROR_RANGE_OVER;

  VECTOR3_copy(stim210_driver->info.ang_vel_bias_compo_rad_sec, ang_vel_bias_compo_rad_s);

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR STIM210_set_ang_vel_scale_factor_compo(STIM210_Driver* stim210_driver,
                                                      const float ang_vel_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  MATRIX33_copy(stim210_driver->info.ang_vel_scale_factor_compo, ang_vel_scale_factor_compo);
  return C2A_MATH_ERROR_OK;
}

// *****************************************************************************
//  内部モジュール
// *****************************************************************************

/*
 * @private
 * @brief ドライバ初期化設定
 * @param p_super: ドライバーへのポインタ
 * @return DS_ERR_CODEを参照
 */
static DS_ERR_CODE STIM210_load_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* stream_config;

  p_super->interface = UART;

  // streamは0のみ
  stream_config = &(p_super->stream_config[STIM210_STREAM_TLM_CMD]);

  DSC_set_rx_buffer_size_in_if_rx(p_super, DS_IF_RX_BUFFER_SIZE_STIM210);

  DSSC_enable(stream_config);

  // 定期的な受信はするがフォーマットによって異なるので、STIM210_set_rec_frame_size_で設定する

  DSSC_set_rx_header(stream_config, STIM210_rx_header_, STIM210_RX_HEADER_SIZE);
  DSSC_set_data_analyzer(stream_config, STIM210_analyze_rec_data_);

  return DS_ERR_CODE_OK;
}

/*
 * @private
 * @brief テレメトリのフレームサイズとヘッダー・フッターを指定する関数。
 * @param stim210: ドライバーへのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
static DS_CMD_ERR_CODE STIM210_set_rec_frame_size_(STIM210_Driver* stim210_driver)
{
  DS_StreamConfig* stream_config;
  stream_config = &(stim210_driver->driver.super.stream_config[STIM210_STREAM_TLM_CMD]);
  // Headerはノーマルモードのフォーマットによって可変
  STIM210_rx_header_[0] = STIM210_normal_mode_format_idx_[(int8_t)stim210_driver->info.normal_mode_format];

  if (stim210_driver->info.termination_mode == STIM210_TERMINATION_CRLF)
  {
    DSSC_set_rx_footer(stream_config, STIM210_rx_footer_crlf_, STIM210_RX_FOOTER_SIZE); // CRLFの時のみ
    DSSC_set_rx_frame_size(stream_config, STIM210_normal_mode_rx_frame_size_crlf_[stim210_driver->info.normal_mode_format]);
  }
  else
  {
    DSSC_set_rx_footer(stream_config, STIM210_rx_footer_crlf_, STIM210_RX_FOOTER_NONE_SIZE); // CRLF OFFではフッターなし
    DSSC_set_rx_frame_size(stream_config, STIM210_normal_mode_rx_frame_size_none_[stim210_driver->info.normal_mode_format]);
  }

  return DS_CMD_OK;
}

/*
 * @private
 * @brief 汎用コマンド作成関数
 * @param stim210: ドライバーへのポインタ
 * @param param_size: cmd paramで使われているバイト長 (フッターを含まない)
 * @param cmd_param: 送信するコマンドパラメータ
 * @return DS_CMD_ERR_CODEを参照
 */
static DS_CMD_ERR_CODE STIM210_send_cmd_(STIM210_Driver* stim210_driver, uint8_t param_size, const char* cmd_param)
{
  DS_StreamConfig* stream_config;
  DS_ERR_CODE ret;
  uint8_t cmd[STIM210_TX_MAX_DATA_SIZE];
  uint8_t i = 0;
  if (param_size + STIM210_TX_FOOTER_SIZE > STIM210_TX_MAX_DATA_SIZE) return DS_CMD_ILLEGAL_LENGTH;

  for (i = 0; i < param_size; i++)
  {
    cmd[i] = cmd_param[i];
  }

  cmd[param_size] = STIM210_tx_footer_[0]; // TXのフッターを追加

  stream_config = &(stim210_driver->driver.super.stream_config[STIM210_STREAM_TLM_CMD]);
  DSSC_set_tx_frame_size(stream_config, param_size + STIM210_TX_FOOTER_SIZE);
  DSSC_set_tx_frame(stream_config, cmd);

  ret = DS_send_general_cmd(&(stim210_driver->driver.super), STIM210_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK) return DS_CMD_DRIVER_SUPER_ERR;

  return DS_CMD_OK;
}

/*
 * @private
 * @brief 受け取ったデータを変数に格納していく
 * @param stream_config: ドライバーの設定
 * @param p_driver: ドライバーへのポインタ
 * @return DS_ERR_CODEを参照
 */
static DS_ERR_CODE STIM210_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver)
{
  uint8_t tlm_body_idx = 0;
  STIM210_Driver* stim210_driver = (STIM210_Driver*)p_driver;
  const uint8_t* stim210_rx_data = DSSC_get_rx_frame(stream_config);

#ifdef STIM210_DEBUG_SHOW_REC_DATA
  Printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",
    stim210_rx_data[0], stim210_rx_data[1], stim210_rx_data[2], stim210_rx_data[3], stim210_rx_data[4], stim210_rx_data[5],
    stim210_rx_data[6], stim210_rx_data[7], stim210_rx_data[8], stim210_rx_data[9], stim210_rx_data[10], stim210_rx_data[11],
    stim210_rx_data[12], stim210_rx_data[13], stim210_rx_data[14], stim210_rx_data[15], stim210_rx_data[16], stim210_rx_data[17],
    stim210_rx_data[18], stim210_rx_data[19], stim210_rx_data[20], stim210_rx_data[21], stim210_rx_data[22]);
#endif
  if (stim210_driver->info.operation_mode != STIM210_OPERATION_NORMAL_MODE) return DS_ERR_CODE_OK;

  tlm_body_idx = STIM210_analyze_normal_mode_format_idx_(stim210_driver, stim210_rx_data, tlm_body_idx);
  tlm_body_idx = STIM210_analyze_gyro_output_(stim210_driver, stim210_rx_data, tlm_body_idx);
  tlm_body_idx = STIM210_analyze_gyro_status_(stim210_driver, stim210_rx_data, tlm_body_idx);

  switch (stim210_driver->info.normal_mode_format)
  {
  case STIM210_NORMAL_MODE_STANDARD:
    break;
  case STIM210_NORMAL_MODE_EXTENDED:
    tlm_body_idx = STIM210_analyze_buffer_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE:
    tlm_body_idx = STIM210_analyze_temperature_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  case STIM210_NORMAL_MODE_RATE_COUNT:
    tlm_body_idx = STIM210_analyze_counter_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  case STIM210_NORMAL_MODE_RATE_LATENCY:
    tlm_body_idx = STIM210_analyze_latency_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  case STIM210_NORMAL_MODE_RATE_COUNT_LATENCY:
    tlm_body_idx = STIM210_analyze_counter_(stim210_driver, stim210_rx_data, tlm_body_idx);
    tlm_body_idx = STIM210_analyze_latency_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE_COUNT:
    tlm_body_idx = STIM210_analyze_temperature_(stim210_driver, stim210_rx_data, tlm_body_idx);
    tlm_body_idx = STIM210_analyze_counter_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE_LATENCY:
    tlm_body_idx = STIM210_analyze_temperature_(stim210_driver, stim210_rx_data, tlm_body_idx);
    tlm_body_idx = STIM210_analyze_latency_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  case STIM210_NORMAL_MODE_RATE_TEMPERATURE_COUNT_LATENCY:
    tlm_body_idx = STIM210_analyze_temperature_(stim210_driver, stim210_rx_data, tlm_body_idx);
    tlm_body_idx = STIM210_analyze_counter_(stim210_driver, stim210_rx_data, tlm_body_idx);
    tlm_body_idx = STIM210_analyze_latency_(stim210_driver, stim210_rx_data, tlm_body_idx);
    break;
  default:
    break;
  }

  tlm_body_idx = STIM210_analyze_crc_(stim210_driver, stim210_rx_data, tlm_body_idx);

  return DS_ERR_CODE_OK;
}

static int STIM210_analyze_normal_mode_format_idx_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  uint8_t format_idx = 0;
  size_t size = sizeof(format_idx);
  ENDIAN_memcpy(&(format_idx), &(stim210_rx_data[tlm_body_idx]), size);
  tlm_body_idx += size;

  // ヘッダーとしてノーマルモードフォーマットとの整合性を確認されているのでこの値はinfoに格納しない

  return tlm_body_idx;
}

static int STIM210_analyze_gyro_output_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  size_t size = 0;
  uint8_t idx = 0;
  uint8_t gyro_output[STIM210_RX_GYRO_OUTPUT_SIZE * PHYSICAL_CONST_THREE_DIM];

  // endian_memcpyすると反転するので1byteずつ行っている
  // TODO_L: endianが反転しないmemcpyを実装次第更新
  for (idx = 0; idx < STIM210_RX_GYRO_OUTPUT_SIZE * PHYSICAL_CONST_THREE_DIM; idx++)
  {
    size = sizeof(gyro_output[idx]);
    ENDIAN_memcpy(&(gyro_output[idx]), &(stim210_rx_data[tlm_body_idx]), size);
    tlm_body_idx += size;
  }

  STIM210_convert_gyro_output_(stim210_driver, gyro_output);

  return tlm_body_idx;
}

static int STIM210_analyze_gyro_status_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  size_t size = sizeof(stim210_driver->info.status);
  ENDIAN_memcpy(&(stim210_driver->info.status), &(stim210_rx_data[tlm_body_idx]), size);
  tlm_body_idx += size;

  return tlm_body_idx;
}

static int STIM210_analyze_buffer_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  size_t size = sizeof(stim210_driver->info.buffer);
  ENDIAN_memcpy(&(stim210_driver->info.buffer), &(stim210_rx_data[tlm_body_idx]), size);
  tlm_body_idx += size;

  return tlm_body_idx;
}

static int STIM210_analyze_temperature_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  size_t size = 0;
  uint8_t idx = 0;
  uint8_t temperature_bytes[STIM210_RX_TEMPERATURE_SIZE * PHYSICAL_CONST_THREE_DIM];

  for (idx = 0; idx < STIM210_RX_TEMPERATURE_SIZE * PHYSICAL_CONST_THREE_DIM; idx++)
  {
    size = sizeof(temperature_bytes[idx]);
    ENDIAN_memcpy(&(temperature_bytes[idx]), &(stim210_rx_data[tlm_body_idx]), size);
    tlm_body_idx += size;
  }

  STIM210_convert_temperature_(stim210_driver, temperature_bytes);

  return tlm_body_idx;
}

static int STIM210_analyze_counter_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  size_t size = 0;

  size = sizeof(stim210_driver->info.counter);
  ENDIAN_memcpy(&(stim210_driver->info.counter), &(stim210_rx_data[tlm_body_idx]), size);
  tlm_body_idx += size;

  return tlm_body_idx;
}

static int STIM210_analyze_latency_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  uint8_t latency_bytes[STIM210_RX_LATENCY_SIZE];
  size_t size = sizeof(latency_bytes);
  ENDIAN_memcpy(&(latency_bytes), &(stim210_rx_data[tlm_body_idx]), size);
  tlm_body_idx += size;

  STIM210_convert_latency_(stim210_driver, latency_bytes);

  return tlm_body_idx;
}

static int STIM210_analyze_crc_(STIM210_Driver* stim210_driver, const uint8_t* stim210_rx_data, uint8_t tlm_body_idx)
{
  size_t crc_size = sizeof(stim210_driver->info.crc);
  ENDIAN_memcpy(&(stim210_driver->info.crc), &(stim210_rx_data[tlm_body_idx]), crc_size);
  tlm_body_idx += crc_size;

  uint8_t crc8 = crc_8_atm_left(STIM210_kCrcInitial_, stim210_rx_data, tlm_body_idx - crc_size, STIM210_kCrcRevFlag_);
  crc8 = crc_8_atm_left(crc8, &(stim210_driver->info.crc), crc_size, STIM210_kCrcRevFlag_);

  if (crc8 == 0x00)
  {
    stim210_driver->info.crc_state = STIM210_CRC_STATE_OK;
  }
  else
  {
    stim210_driver->info.crc_state = STIM210_CRC_STATE_NG;
  }
  return tlm_body_idx;
}

static int STIM210_convert_gyro_output_(STIM210_Driver* stim210_driver, const uint8_t* gyro_output_bytes)
{
  uint8_t axis = 0;
  uint8_t idx = 0;
  float ang_vel_compo_deg_sec[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };
  float ang_compo_deg[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };

  if ((stim210_driver->info.gyro_output_mode == STIM210_GYRO_OUTPUT_ANGULAR_RATE) ||
      (stim210_driver->info.gyro_output_mode == STIM210_GYRO_OUTPUT_AVERAGE_ANGULAR_RATE))
  {
    for (axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
    {
      for (idx = 0; idx < STIM210_RX_GYRO_OUTPUT_SIZE; idx++)
      {
        ang_vel_compo_deg_sec[axis] += gyro_output_bytes[axis * STIM210_RX_GYRO_OUTPUT_SIZE + idx] * STIM210_kScaleRaw2AngVel_[idx];
      }
      if (ang_vel_compo_deg_sec[axis] > STIM210_kMaxAngVelRadSec_)
      {
        ang_vel_compo_deg_sec[axis] -= STIM210_kMaxAngVelRadSec_ * 2.0f;
      }
      stim210_driver->info.ang_vel_raw_compo_rad_sec[axis] = PHYSICAL_CONST_degree_to_radian(ang_vel_compo_deg_sec[axis]);
    }
    STIM210_calc_ang_vel_calibration_(&(stim210_driver->info));
  }
  else if ((stim210_driver->info.gyro_output_mode == STIM210_GYRO_OUTPUT_INCREMENTAL_ANGLE) ||
           (stim210_driver->info.gyro_output_mode == STIM210_GYRO_OUTPUT_INTEGRATED_ANGLE))
  {
    for (axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
    {
      for (idx = 0; idx < STIM210_RX_GYRO_OUTPUT_SIZE; idx++)
      {
        ang_compo_deg[axis] += gyro_output_bytes[axis * STIM210_RX_GYRO_OUTPUT_SIZE + idx] * STIM210_kScaleRaw2Ang_[idx];
      }
      if (ang_compo_deg[axis] > STIM210_kMaxAngRad_)
      {
        ang_compo_deg[axis] -= STIM210_kMaxAngRad_ * 2.0f;
      }
      stim210_driver->info.ang_compo_rad[axis] = PHYSICAL_CONST_degree_to_radian(ang_compo_deg[axis]);
    }
  }

  return 0;
}

static int STIM210_convert_temperature_(STIM210_Driver* stim210_driver, const uint8_t* temperature_bytes)
{
  uint8_t axis = 0;
  uint8_t idx = 0;
  float temperature_degC[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };

  for (axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    for (idx = 0; idx < STIM210_RX_TEMPERATURE_SIZE; idx++)
    {
      temperature_degC[axis] += temperature_bytes[axis * STIM210_RX_TEMPERATURE_SIZE + idx] * STIM210_kScaleRaw2Temperature_[idx];
    }
    if (temperature_degC[axis] > STIM210_kMaxTemperatureDegC_)
    {
      temperature_degC[axis] = STIM210_kMaxTemperatureDegC_ * 2.0f;
    }
    stim210_driver->info.temperature_compo_degC[axis] = temperature_degC[axis];
  }

  return 0;
}

static int STIM210_convert_latency_(STIM210_Driver* stim210_driver, const uint8_t* latency_bytes)
{
  uint8_t idx = 0;
  float latency_sec = 0.0f;

  for (idx = 0; idx < STIM210_RX_LATENCY_SIZE; idx++)
  {
    latency_sec += latency_bytes[idx] * STIM210_kScaleRaw2Latency_[idx];
  }
  stim210_driver->info.latency_sec = latency_sec;

  return 0;
}

static void STIM210_calc_ang_vel_calibration_(STIM210_Info* info)
{
  // Scale Factor and Bias calibration: y = SF*x - BIAS
  MATRIX33_multiply_matrix_vector(info->ang_vel_compo_rad_sec,
                                  info->ang_vel_scale_factor_compo,
                                  info->ang_vel_raw_compo_rad_sec);
  VECTOR3_subtract(info->ang_vel_compo_rad_sec,
                  info->ang_vel_compo_rad_sec,
                  info->ang_vel_bias_compo_rad_sec);

  QUATERNION_trans_coordinate(info->ang_vel_body_rad_sec,
                              info->frame_transform_c2b,
                              info->ang_vel_compo_rad_sec);

  return;
}

#pragma section
