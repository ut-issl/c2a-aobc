#pragma section REPRO
/**
* @file   mpu9250.c
* @brief  MPU9250のDriver
*/

#include "./mpu9250.h"
#include <string.h>
#include "../../Library/physical_constants.h"
#include "../../Library/vector3.h"
#include "../../Library/matrix33.h"

#define MPU9250_STREAM_TLM_CMD         (0)  //!< テレコマで使うストリーム
#define MPU9250_TX_FOR_REC_FRAME_SIZE  (1)  //!< 観測時の1回の送信データ長（バイト）
#define MPU9250_GYRO_RX_FRAME_SIZE     (14) //!< 1回の受信で取得するデータ長（バイト）
#define MPU9250_MAG_RX_FRAME_SIZE      (8)  //!< 1回の受信で取得するデータ長（バイト）．ST1からST2までの8byteを読み込む．

static const uint8_t MPU9250_kRegObsMpu9250_  = 0x3b; //!< MPU9250の観測
static const uint8_t MPU9250_kRegObsAk8963_   = 0x02; //!< AK8963の観測
static const uint8_t MPU9250_kRegGyroEnable_  = 0x6b; //!< ジャイロセンサON
static const uint8_t MPU9250_kRegGyroLPF_     = 0x1a; //!< ローパスフィルタ設定
static const uint8_t MPU9250_kRegGyroRange_   = 0x1b; //!< 角速度レンジ設定
static const uint8_t MPU9250_kRegAccelRange_  = 0x1c; //!< 加速度レンジ設定
static const uint8_t MPU9250_kRegAccelLPF_    = 0x1d; //!< 加速度LPF設定
static const uint8_t MPU9250_kRegMagEnable_   = 0x37; //!< 磁気センサON
static const uint8_t MPU9250_kRegMagConfig_   = 0x0a; //!< 磁気センサ出力変更

static const uint8_t MPU9250_kParamGyroEnable_          = 0x00;
static const uint8_t MPU9250_kParamGyroLPF_             = 0x03;
static const uint8_t MPU9250_kParamAccelLPF_            = 0x09;
static const uint8_t MPU9250_kParamGyroRange_250_DegS_  = 0x00;
static const uint8_t MPU9250_kParamGyroRange_500_DegS_  = 0x08;
static const uint8_t MPU9250_kParamGyroRange_1000_DegS_ = 0x10;
static const uint8_t MPU9250_kParamGyroRange_2000_DegS_ = 0x18;
static const uint8_t MPU9250_kParamAccelRange_2_G_  = 0x00;
static const uint8_t MPU9250_kParamAccelRange_4_G_  = 0x08;
static const uint8_t MPU9250_kParamAccelRange_8_G_  = 0x10;
static const uint8_t MPU9250_kParamAccelRange_16_G_ = 0x18;
static const uint8_t MPU9250_kParamMagEnable_ = 0x02;
static const uint8_t MPU9250_kParamMagConfig_ = 0x16;

static float MPU9250_ang_vel_divider_;                     //!< 角速度スケール
static float MPU9250_accel_divider_;                       //!< 加速度スケール
static const float MPU9250_kTemperatureDivider_ = 333.87f; //!< 温度スケール
static const float MPU9250_kMagDivider_ = 6.83f;           //!< 磁場スケール 4800uT/(2^15)
static const float MPU9250_kTemperatureOffset_ = 21.0f;

static const float MPU9250_kMaxAngVelBias_rad_s_ = 0.3f; //!< 角速度バイアスノルム最大値 rad/s 値は過去衛星をベースに余裕をもって設定
static const float MPU9250_kMaxMagBias_nT_ = 100000.0f;   //!< 通信エラー検知のための磁場バイアスノルム最大値 nT 値は過去衛星をベースに余裕をもって設定

static DS_ERR_CODE MPU9250_gyro_load_driver_super_init_settings_(DriverSuper* super);
static DS_ERR_CODE MPU9250_mag_load_driver_super_init_settings_(DriverSuper* super);
static DS_ERR_CODE MPU9250_gyro_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver);
static DS_ERR_CODE MPU9250_mag_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver);
static int MPU9250_rec_i2c_(MPU9250_Driver* mpu9250_driver, DriverSuper* super, I2C_Config* i2c_config, uint8_t* cmd_code, uint32_t rx_frame_size);
static DS_CMD_ERR_CODE MPU9250_send_cmd_(DriverSuper* super, uint8_t* cmd_code, const uint16_t cmd_len);

static void MPU9250_convert_bytes_to_info_ang_vel_(const uint8_t gyro_bytes[MPU9250_GYRO_RX_FRAME_SIZE], MPU9250_Info* info);
static void MPU9250_convert_bytes_to_info_accel_(const uint8_t gyro_bytes[MPU9250_GYRO_RX_FRAME_SIZE], MPU9250_Info* info);
static void MPU9250_convert_bytes_to_info_temperature_(const uint8_t gyro_bytes[MPU9250_GYRO_RX_FRAME_SIZE], MPU9250_Info* info);
static void MPU9250_convert_bytes_to_info_mag_(const uint8_t mag_bytes[MPU9250_MAG_RX_FRAME_SIZE], MPU9250_Info* info);
static uint16_t MPU9250_convert_uint8_to_uint16_(const uint8_t value_high, const uint8_t value_low);
static float MPU9250_convert_uint16_to_float_(const uint16_t value);

DS_INIT_ERR_CODE MPU9250_init(MPU9250_Driver* mpu9250_driver,
                         uint8_t ch,
                         uint8_t i2c_address_mpu9250,
                         uint8_t i2c_address_ak8963,
                         DS_StreamRecBuffer* rx_buffer_mpu9250,
                         DS_StreamRecBuffer* rx_buffer_ak8963)
{
  DS_ERR_CODE ret;
  uint8_t axis;

  // MPU9250のI2C初期化
  mpu9250_driver->driver_mpu9250.i2c_config.ch = ch;
  mpu9250_driver->driver_mpu9250.i2c_config.device_address = i2c_address_mpu9250;
  mpu9250_driver->driver_mpu9250.i2c_config.frequency_khz = 100;
  mpu9250_driver->driver_mpu9250.i2c_config.stop_flag = 1;
  mpu9250_driver->driver_mpu9250.i2c_config.timeout_threshold = 1000;

  ret = DS_init(&(mpu9250_driver->driver_mpu9250.super),
                &(mpu9250_driver->driver_mpu9250.i2c_config),
                rx_buffer_mpu9250,
                MPU9250_gyro_load_driver_super_init_settings_);

  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;

  // MPU9250内の磁気センサのI2C初期化
  // 磁気センサの利用にはMPU9250の初期化が必要なのでここで初期化を行う
  mpu9250_driver->driver_ak8963.i2c_config.ch = ch;
  mpu9250_driver->driver_ak8963.i2c_config.device_address = i2c_address_ak8963;
  mpu9250_driver->driver_ak8963.i2c_config.frequency_khz = 100;
  mpu9250_driver->driver_ak8963.i2c_config.stop_flag = 1;
  mpu9250_driver->driver_ak8963.i2c_config.timeout_threshold = 500;

  ret = DS_init(&(mpu9250_driver->driver_ak8963.super),
                &(mpu9250_driver->driver_ak8963.i2c_config),
                rx_buffer_ak8963,
                MPU9250_mag_load_driver_super_init_settings_);

  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;

  for (axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    // raw value
    mpu9250_driver->info.ang_vel_raw_compo_rad_s[axis] = 0.0f;
    mpu9250_driver->info.accel_compo_m_s2[axis] = 0.0f;
    mpu9250_driver->info.mag_raw_compo_nT[axis] = 0.0f;
    // bias
    mpu9250_driver->info.ang_vel_bias_compo_rad_s[axis] = 0.0f;
    mpu9250_driver->info.mag_bias_compo_nT[axis] = 0.0f;
  }
  // Scale factor
  MATRIX33_make_unit(mpu9250_driver->info.ang_vel_scale_factor_compo);
  MATRIX33_make_unit(mpu9250_driver->info.mag_scale_factor_compo);
  // Bias calibration
  VECTOR3_subtract(mpu9250_driver->info.ang_vel_compo_rad_s,
                  mpu9250_driver->info.ang_vel_raw_compo_rad_s,
                  mpu9250_driver->info.ang_vel_bias_compo_rad_s);
  VECTOR3_subtract(mpu9250_driver->info.mag_compo_nT,
                  mpu9250_driver->info.mag_raw_compo_nT,
                  mpu9250_driver->info.mag_bias_compo_nT);
  // Coordinate transformation
  mpu9250_driver->info.frame_transform_c2b = QUATERNION_make_unit();
  QUATERNION_trans_coordinate(mpu9250_driver->info.ang_vel_body_rad_s,
                              mpu9250_driver->info.frame_transform_c2b,
                              mpu9250_driver->info.ang_vel_compo_rad_s);
  QUATERNION_trans_coordinate(mpu9250_driver->info.accel_body_m_s2,
                              mpu9250_driver->info.frame_transform_c2b,
                              mpu9250_driver->info.accel_compo_m_s2);
  QUATERNION_trans_coordinate(mpu9250_driver->info.mag_body_nT,
                              mpu9250_driver->info.frame_transform_c2b,
                              mpu9250_driver->info.mag_compo_nT);

  mpu9250_driver->info.temperature_degC = 0.0f;
  mpu9250_driver->info.mag_status_overrun  = 0;
  mpu9250_driver->info.mag_status_overflow = 0;
  mpu9250_driver->info.is_mag_enabled = 0;
  MPU9250_ang_vel_divider_ = 131.0f;
  MPU9250_accel_divider_ = 16384.0f;

  return DS_INIT_OK;
}

static DS_ERR_CODE MPU9250_gyro_load_driver_super_init_settings_(DriverSuper* super)
{
  DS_StreamConfig* stream_config;

  super->interface = I2C;

  stream_config = &(super->stream_config[MPU9250_STREAM_TLM_CMD]);

  DSSC_enable(stream_config);
  DSSC_set_rx_frame_size(stream_config, MPU9250_GYRO_RX_FRAME_SIZE);
  DSSC_set_data_analyzer(stream_config, MPU9250_gyro_analyze_rec_data_);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE MPU9250_mag_load_driver_super_init_settings_(DriverSuper* super)
{
  DS_StreamConfig* stream_config;

  super->interface = I2C;

  stream_config = &(super->stream_config[MPU9250_STREAM_TLM_CMD]);

  DSSC_enable(stream_config);
  DSSC_set_rx_frame_size(stream_config, MPU9250_MAG_RX_FRAME_SIZE);
  DSSC_set_data_analyzer(stream_config, MPU9250_mag_analyze_rec_data_);

  return DS_ERR_CODE_OK;
}

// TODO_L: cmd_code に const をつける
//        そもそも DS の設計では，高速化のため， tx_frame は初期化時に設定して，使い回すことを想定してる．
//        そのうちリファクタする．
static DS_CMD_ERR_CODE MPU9250_send_cmd_(DriverSuper* super, uint8_t* cmd_code, const uint16_t cmd_len)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* stream_config;

  stream_config = &(super->stream_config[MPU9250_STREAM_TLM_CMD]);
  DSSC_set_tx_frame_size(stream_config, cmd_len);
  DSSC_set_tx_frame(stream_config, cmd_code);

  ret = DS_send_general_cmd(super, MPU9250_STREAM_TLM_CMD);

  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

DS_CMD_ERR_CODE MPU9250_enable_mag(MPU9250_Driver* mpu9250_driver)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t cmd_len = 2;
  uint8_t cmd_code[cmd_len];

  // 磁気センサ出力ON
  cmd_code[0] = MPU9250_kRegMagEnable_;
  cmd_code[1] = MPU9250_kParamMagEnable_;

  ret = MPU9250_send_cmd_(&(mpu9250_driver->driver_mpu9250.super), cmd_code, cmd_len);
  if (ret != DS_CMD_OK) return ret;

  // 磁気センサ100Hz連続出力モード
  cmd_code[0] = MPU9250_kRegMagConfig_;
  cmd_code[1] = MPU9250_kParamMagConfig_;

  MPU9250_send_cmd_(&(mpu9250_driver->driver_ak8963.super), cmd_code, cmd_len);
  if (ret != DS_CMD_OK) return ret;

  mpu9250_driver->info.is_mag_enabled = 1;

  return ret;
}

DS_CMD_ERR_CODE MPU9250_enable_gyro(MPU9250_Driver* mpu9250_driver)
{
  const uint8_t cmd_len = 2;
  uint8_t cmd_code[cmd_len];
  cmd_code[0] = MPU9250_kRegGyroEnable_;
  cmd_code[1] = MPU9250_kParamGyroEnable_;

  return MPU9250_send_cmd_(&(mpu9250_driver->driver_mpu9250.super), cmd_code, cmd_len);
}

DS_CMD_ERR_CODE MPU9250_set_lpf(MPU9250_Driver* mpu9250_driver)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t cmd_len = 2;
  uint8_t cmd_code[cmd_len];
  cmd_code[0] = MPU9250_kRegGyroLPF_;
  cmd_code[1] = MPU9250_kParamGyroLPF_;

  ret = MPU9250_send_cmd_(&(mpu9250_driver->driver_mpu9250.super), cmd_code, cmd_len);
  if (ret != DS_CMD_OK)  return ret;

  cmd_code[0] = MPU9250_kRegAccelLPF_;
  cmd_code[1] = MPU9250_kParamAccelLPF_;

  return MPU9250_send_cmd_(&(mpu9250_driver->driver_mpu9250.super), cmd_code, cmd_len);
}

DS_CMD_ERR_CODE MPU9250_set_ang_vel_range(MPU9250_Driver* mpu9250_driver, MPU9250_ANG_VEL_RANGE range)
{
  const uint8_t cmd_len = 2;
  uint8_t cmd_code[cmd_len];
  cmd_code[0] = MPU9250_kRegGyroRange_;
  cmd_code[1] = 0x00;

  // divider = range / (2^15)
  switch (range)
  {
  case MPU9250_ANG_VEL_RANGE_250_DEG_S:
    MPU9250_ang_vel_divider_ = 131.0f;
    cmd_code[1] = MPU9250_kParamGyroRange_250_DegS_;
    break;
  case MPU9250_ANG_VEL_RANGE_500_DEG_S:
    MPU9250_ang_vel_divider_ = 65.5f;
    cmd_code[1] = MPU9250_kParamGyroRange_500_DegS_;
    break;
  case MPU9250_ANG_VEL_RANGE_1000_DEG_S:
    MPU9250_ang_vel_divider_ = 32.8f;
    cmd_code[1] = MPU9250_kParamGyroRange_1000_DegS_;
    break;
  case MPU9250_ANG_VEL_RANGE_2000_DEG_S:
    MPU9250_ang_vel_divider_ = 16.4f;
    cmd_code[1] = MPU9250_kParamGyroRange_2000_DegS_;
    break;
  default:
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  return MPU9250_send_cmd_(&(mpu9250_driver->driver_mpu9250.super), cmd_code, cmd_len);
}

DS_CMD_ERR_CODE MPU9250_set_accel_range(MPU9250_Driver* mpu9250_driver, MPU9250_ACCEL_RANGE range)
{
  const uint8_t cmd_len = 2;
  uint8_t cmd_code[2];
  cmd_code[0] = MPU9250_kRegAccelRange_;
  cmd_code[1] = 0x00;

  // divider = range / (2^15)
  switch (range)
  {
  case MPU9250_ACCEL_RANGE_2_G:
    MPU9250_accel_divider_ = 16384.0f;
    cmd_code[1] = MPU9250_kParamAccelRange_2_G_;
    break;
  case MPU9250_ACCEL_RANGE_4_G:
    MPU9250_accel_divider_ = 8192.0f;
    cmd_code[1] = MPU9250_kParamAccelRange_4_G_;
    break;
  case MPU9250_ACCEL_RANGE_8_G:
    MPU9250_accel_divider_ = 4096.0f;
    cmd_code[1] = MPU9250_kParamAccelRange_8_G_;
    break;
  case MPU9250_ACCEL_RANGE_16_G:
    MPU9250_accel_divider_ = 2048.0f;
    cmd_code[1] = MPU9250_kParamAccelRange_16_G_;
    break;
  default:
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  return MPU9250_send_cmd_(&(mpu9250_driver->driver_mpu9250.super), cmd_code, cmd_len);
}

C2A_MATH_ERROR MPU9250_set_frame_transform_c2b(MPU9250_Driver* mpu9250_driver, const Quaternion q_c2b)
{
  C2A_MATH_ERROR ret;
  ret = QUATERNION_is_normalized(q_c2b);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  mpu9250_driver->info.frame_transform_c2b = q_c2b;
  return ret;
}

C2A_MATH_ERROR MPU9250_set_ang_vel_bias_compo_rad_s(MPU9250_Driver* mpu9250_driver, const float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM])
{
  float norm_rad_s = VECTOR3_norm(ang_vel_bias_compo_rad_s);
  if (norm_rad_s > MPU9250_kMaxAngVelBias_rad_s_) return C2A_MATH_ERROR_RANGE_OVER;

  VECTOR3_copy(mpu9250_driver->info.ang_vel_bias_compo_rad_s, ang_vel_bias_compo_rad_s);
  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR MPU9250_set_mag_bias_compo_nT(MPU9250_Driver* mpu9250_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM])
{
  float norm_nT = VECTOR3_norm(mag_bias_compo_nT);
  if (norm_nT > MPU9250_kMaxMagBias_nT_) return C2A_MATH_ERROR_RANGE_OVER;

  VECTOR3_copy(mpu9250_driver->info.mag_bias_compo_nT, mag_bias_compo_nT);
  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR MPU9250_add_mag_bias_compo_nT(MPU9250_Driver* mpu9250_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM])
{
  float added_mag_bias_nT[PHYSICAL_CONST_THREE_DIM];

  VECTOR3_add(added_mag_bias_nT, mpu9250_driver->info.mag_bias_compo_nT, mag_bias_compo_nT);

  return MPU9250_set_mag_bias_compo_nT(mpu9250_driver, added_mag_bias_nT);
}

C2A_MATH_ERROR MPU9250_set_ang_vel_scale_factor_compo(MPU9250_Driver* mpu9250_driver,
                                                      const float ang_vel_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  MATRIX33_copy(mpu9250_driver->info.ang_vel_scale_factor_compo, ang_vel_scale_factor_compo);
  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR MPU9250_set_mag_scale_factor_compo(MPU9250_Driver* mpu9250_driver,
                                                  const float mag_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  MATRIX33_copy(mpu9250_driver->info.mag_scale_factor_compo, mag_scale_factor_compo);
  return C2A_MATH_ERROR_OK;
}

int MPU9250_rec(MPU9250_Driver* mpu9250_driver)
{
  int ret;
  const uint8_t cmd_len = 1;
  uint8_t cmd_code[cmd_len];

  // Gyro observation
  cmd_code[0] = MPU9250_kRegObsMpu9250_;
  ret = MPU9250_rec_i2c_(mpu9250_driver,
                         &(mpu9250_driver->driver_mpu9250.super),
                         &(mpu9250_driver->driver_mpu9250.i2c_config),
                         cmd_code, MPU9250_GYRO_RX_FRAME_SIZE);
  if (ret != 0) return ret;

  // Mag observation
  cmd_code[0] = MPU9250_kRegObsAk8963_;
  ret = MPU9250_rec_i2c_(mpu9250_driver,
                         &(mpu9250_driver->driver_ak8963.super),
                         &(mpu9250_driver->driver_ak8963.i2c_config),
                         cmd_code, MPU9250_MAG_RX_FRAME_SIZE);
  if (ret != 0) return ret;

  return ret;
}

/**
 * @private
 * @brief  1つのI2Cアドレスに対して定常観測を行う
 * @param  *mpu9250_driver  : MPU9250_Driver構造体へのポインタ
 * @param  *super           : 観測を行うDriverSuper構造体へのポインタ
 * @param  *i2c_config      : 観測を行うi2c_config構造体へのポインタ
 * @return 0                : 正常終了
 * @return 1                : コマンド送信失敗
 * @return 2                : テレメトリ受信失敗
 * @return 3                : 適切なフレーム長を受信失敗
 * @return 4                : テレメトリ解析失敗
 */
// TODO_L: cmd_code に const をつける
//        そもそも DS の設計では，高速化のため， tx_frame は初期化時に設定して，使い回すことを想定してる．
//        そのうちリファクタする．
static int MPU9250_rec_i2c_(MPU9250_Driver* mpu9250_driver, DriverSuper* super, I2C_Config* i2c_config, uint8_t* cmd_code, uint32_t rx_frame_size)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* stream_config;

  stream_config = &(super->stream_config[MPU9250_STREAM_TLM_CMD]);

  DSSC_set_tx_frame_size(stream_config, MPU9250_TX_FOR_REC_FRAME_SIZE);
  DSSC_set_tx_frame(stream_config, cmd_code);
  DSSC_set_rx_frame_size(stream_config, rx_frame_size);
  I2C_set_rx_length(i2c_config, rx_frame_size);

  ret = DS_send_req_tlm_cmd(super, MPU9250_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK)  return 1;

  ret = DS_receive(super);
  if (ret != DS_ERR_CODE_OK) return 2;
  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME) return 3;

  ret = DS_analyze_rec_data(super, MPU9250_STREAM_TLM_CMD, mpu9250_driver);
  if (ret != DS_ERR_CODE_OK) return 4;

  return 0;
}

static DS_ERR_CODE MPU9250_gyro_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver)
{
  const uint8_t* mpu9250_rx_data = DSSC_get_rx_frame(stream_config);
  MPU9250_Driver* mpu9250_driver = (MPU9250_Driver*)p_driver;

  MPU9250_convert_bytes_to_info_ang_vel_(mpu9250_rx_data, &(mpu9250_driver->info));
  MPU9250_convert_bytes_to_info_accel_(mpu9250_rx_data, &(mpu9250_driver->info));
  MPU9250_convert_bytes_to_info_temperature_(mpu9250_rx_data, &(mpu9250_driver->info));

  // Scale Factor and Bias caliblation: y = SF*x - BIAS
  MATRIX33_multiply_matrix_vector(mpu9250_driver->info.ang_vel_compo_rad_s,
                                mpu9250_driver->info.ang_vel_scale_factor_compo,
                                mpu9250_driver->info.ang_vel_raw_compo_rad_s);
  VECTOR3_subtract(mpu9250_driver->info.ang_vel_compo_rad_s,
                  mpu9250_driver->info.ang_vel_compo_rad_s,
                  mpu9250_driver->info.ang_vel_bias_compo_rad_s);

  // Coordinate transformation
  QUATERNION_trans_coordinate(mpu9250_driver->info.ang_vel_body_rad_s,
                              mpu9250_driver->info.frame_transform_c2b,
                              mpu9250_driver->info.ang_vel_compo_rad_s);
  QUATERNION_trans_coordinate(mpu9250_driver->info.accel_body_m_s2,
                              mpu9250_driver->info.frame_transform_c2b,
                              mpu9250_driver->info.accel_compo_m_s2);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE MPU9250_mag_analyze_rec_data_(DS_StreamConfig* stream_config, void* p_driver)
{
  const uint8_t* mpu9250_rx_data = DSSC_get_rx_frame(stream_config);
  MPU9250_Driver* mpu9250_driver = (MPU9250_Driver*)p_driver;

  MPU9250_convert_bytes_to_info_mag_(mpu9250_rx_data, &(mpu9250_driver->info));

  // Bias caliblation
  VECTOR3_subtract(mpu9250_driver->info.mag_compo_nT,
                   mpu9250_driver->info.mag_raw_compo_nT,
                   mpu9250_driver->info.mag_bias_compo_nT);

  // Scale Factor caliblation
  MATRIX33_multiply_matrix_vector(mpu9250_driver->info.mag_compo_nT,
                                  mpu9250_driver->info.mag_scale_factor_compo,
                                  mpu9250_driver->info.mag_compo_nT);

  // Coordinate transformation
  QUATERNION_trans_coordinate(mpu9250_driver->info.mag_body_nT,
                              mpu9250_driver->info.frame_transform_c2b,
                              mpu9250_driver->info.mag_compo_nT);

  return DS_ERR_CODE_OK;
}

static void MPU9250_convert_bytes_to_info_accel_(const uint8_t gyro_bytes[MPU9250_GYRO_RX_FRAME_SIZE], MPU9250_Info* info)
{
  uint16_t value_int[PHYSICAL_CONST_THREE_DIM];
  float value_float_G[PHYSICAL_CONST_THREE_DIM];

  for (uint8_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    // 加速度
    value_int[i]   = MPU9250_convert_uint8_to_uint16_(gyro_bytes[i * 2], gyro_bytes[i * 2 + 1]);
    value_float_G[i] = MPU9250_convert_uint16_to_float_(value_int[i]) / MPU9250_accel_divider_;
    info->accel_compo_m_s2[i] = PHYSICAL_CONST_G_to_m_s2(value_float_G[i]);
  }

  return;
}

static void MPU9250_convert_bytes_to_info_ang_vel_(const uint8_t gyro_bytes[MPU9250_GYRO_RX_FRAME_SIZE], MPU9250_Info* info)
{
  uint16_t value_int[PHYSICAL_CONST_THREE_DIM];
  float value_float_deg_s[PHYSICAL_CONST_THREE_DIM];

  for (uint8_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    // 角速度
    value_int[i] = MPU9250_convert_uint8_to_uint16_(gyro_bytes[i * 2 + 8], gyro_bytes[i * 2 + 9]);
    value_float_deg_s[i] = MPU9250_convert_uint16_to_float_(value_int[i]) / MPU9250_ang_vel_divider_;
    info->ang_vel_raw_compo_rad_s[i] = PHYSICAL_CONST_degree_to_radian(value_float_deg_s[i]);
  }

  return;
}

static void MPU9250_convert_bytes_to_info_temperature_(const uint8_t gyro_bytes[MPU9250_GYRO_RX_FRAME_SIZE], MPU9250_Info* info)
{
  uint16_t value_int;
  float value_float_degC;

  // 温度
  value_int   = MPU9250_convert_uint8_to_uint16_(gyro_bytes[6], gyro_bytes[7]);
  value_float_degC = MPU9250_convert_uint16_to_float_(value_int) / MPU9250_kTemperatureDivider_ + MPU9250_kTemperatureOffset_;
  info->temperature_degC = value_float_degC;

  return;
}

static void MPU9250_convert_bytes_to_info_mag_(const uint8_t mag_bytes[MPU9250_MAG_RX_FRAME_SIZE], MPU9250_Info* info)
{
  uint16_t value_int[PHYSICAL_CONST_THREE_DIM];
  float value_float_uT[PHYSICAL_CONST_THREE_DIM];
  uint8_t i;

  // 磁気センサの出力状態
  info->mag_status_overrun = mag_bytes[0];

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    // 磁場
    value_int[i]   = MPU9250_convert_uint8_to_uint16_(mag_bytes[i * 2 + 2], mag_bytes[i * 2 + 1]);
    value_float_uT[i] = MPU9250_convert_uint16_to_float_(value_int[i]) / MPU9250_kMagDivider_;
  }

  // mag->compo座標変換, 単位変換
  info->mag_raw_compo_nT[0] = value_float_uT[1] * 1000.0f;
  info->mag_raw_compo_nT[1] = value_float_uT[0] * 1000.0f;
  info->mag_raw_compo_nT[2] = -1.0f * value_float_uT[2] * 1000.0f;

  // 磁気センサの出力状態
  info->mag_status_overflow = mag_bytes[7];

  return;
}

static uint16_t MPU9250_convert_uint8_to_uint16_(const uint8_t value_high, const uint8_t value_low)
{
  uint16_t value_uint16;
  value_uint16 = ((uint16_t)value_high << 8) | value_low;

  return value_uint16;
}

static float MPU9250_convert_uint16_to_float_(const uint16_t value)
{
  if (value > 0x8000)
  {
    return (float)(value - 0x10000);
  }
  else
  {
    return (float)value;
  }
}

#pragma section
