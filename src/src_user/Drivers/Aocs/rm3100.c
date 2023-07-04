#pragma section REPRO
/**
* @file   rm3100.c
* @brief  RM3100のDriver
*/

#include "rm3100.h"
#include <src_user/Library/matrix33.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/c2a_math.h>

#define RM3100_STREAM_TLM_CMD  (0)
#define RM3100_RX_FRAME_SIZE   (9)  //!< RM3100の場合この値で固定

const static uint8_t RM3100_kCmdIdModeSet_ = 0x01;
const static uint8_t RM3100_kCmdIdObserve_ = 0x24;

const uint8_t RM3100_kCmmMode = 0x7d;

static const float RM3100_kConvertRawToNt_ = 13.0f;  //!< 出力値をnT単位に変えるための定数

static const float RM3100_kMaxMagBias_nT_ = 100000.0f; //!< 通信エラー検知のための磁場バイアスノルム最大値 nT 値は磁気試験結果をもとに余裕をもって設定

static DS_ERR_CODE RM3100_load_driver_super_init_settings_(DriverSuper* super);
static DS_ERR_CODE RM3100_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver);
static void RM3100_convert_bytes_to_float_(const uint8_t mag_c_bytes[RM3100_RX_FRAME_SIZE], float mag_c_nT[3]);
static void RM3100_calc_mag_calibration_(RM3100_Info* info);


DS_INIT_ERR_CODE RM3100_init(RM3100_Driver* rm3100_driver, uint8_t ch, uint8_t i2c_address, DS_StreamRecBuffer* rx_buffer)
{
  for (int axis = 0; axis < 3; axis++)
  {
    rm3100_driver->info.mag_raw_compo_nT[axis] = 0.0f;
    rm3100_driver->info.mag_bias_compo_nT[axis] = 0.0f;
  }
  rm3100_driver->info.frame_transform_c2b = QUATERNION_make_unit();
  RM3100_calc_mag_calibration_(&(rm3100_driver->info));

  DS_ERR_CODE ret;

  rm3100_driver->driver.i2c_config.ch = ch;
  rm3100_driver->driver.i2c_config.device_address = i2c_address;
  rm3100_driver->driver.i2c_config.frequency_khz = 100;
  rm3100_driver->driver.i2c_config.stop_flag = 1;
  rm3100_driver->driver.i2c_config.timeout_threshold = 1000;

  ret = DS_init(&(rm3100_driver->driver.super),
                &(rm3100_driver->driver.i2c_config),
                rx_buffer,
                RM3100_load_driver_super_init_settings_);

  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;
  return DS_INIT_OK;
}

DS_CMD_ERR_CODE RM3100_set_mode(RM3100_Driver* rm3100_driver, const uint8_t mode)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* stream_config;
  const uint8_t cmd_size = 2;
  uint8_t cmd[cmd_size];

  stream_config = &(rm3100_driver->driver.super.stream_config[RM3100_STREAM_TLM_CMD]);

  cmd[0] = RM3100_kCmdIdModeSet_;
  cmd[1] = mode;
  DSSC_set_tx_frame(stream_config, cmd);
  DSSC_set_tx_frame_size(stream_config, cmd_size);

  ret = DS_send_general_cmd(&(rm3100_driver->driver.super), RM3100_STREAM_TLM_CMD);
  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

DS_CMD_ERR_CODE RM3100_observe_mag(RM3100_Driver* rm3100_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* stream_config;

  const uint8_t cmd_size = 1;
  uint8_t cmd[cmd_size];

  stream_config = &(rm3100_driver->driver.super.stream_config[RM3100_STREAM_TLM_CMD]);

  // Rx Settings
  DSSC_set_rx_frame_size(stream_config, RM3100_RX_FRAME_SIZE);
  I2C_set_rx_length(&(rm3100_driver->driver.i2c_config), (uint32_t)RM3100_RX_FRAME_SIZE);
  DS_clear_rx_buffer(&(rm3100_driver->driver.super));

  // Send command
  cmd[0] = RM3100_kCmdIdObserve_;
  DSSC_set_tx_frame(stream_config, cmd);
  DSSC_set_tx_frame_size(stream_config, cmd_size);
  ret = DS_send_req_tlm_cmd(&(rm3100_driver->driver.super), RM3100_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK)  return DS_CMD_DRIVER_SUPER_ERR;

  // Receive data
  ret = DS_receive(&(rm3100_driver->driver.super));
  if (ret != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  ret = DS_analyze_rec_data(&(rm3100_driver->driver.super), RM3100_STREAM_TLM_CMD, rm3100_driver);
  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

C2A_MATH_ERROR RM3100_set_frame_transform_c2b(RM3100_Driver* rm3100_driver, const Quaternion q_c2b)
{
  C2A_MATH_ERROR ret;
  ret = QUATERNION_is_normalized(q_c2b);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  rm3100_driver->info.frame_transform_c2b = q_c2b;
  return ret;
}

C2A_MATH_ERROR RM3100_set_mag_bias_compo_nT(RM3100_Driver* rm3100_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM])
{
  float norm_nT = VECTOR3_norm(mag_bias_compo_nT);
  if (norm_nT > RM3100_kMaxMagBias_nT_) return C2A_MATH_ERROR_RANGE_OVER;

  VECTOR3_copy(rm3100_driver->info.mag_bias_compo_nT, mag_bias_compo_nT);
  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR RM3100_add_mag_bias_compo_nT(RM3100_Driver* rm3100_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM])
{
  float added_mag_bias_nT[PHYSICAL_CONST_THREE_DIM];

  VECTOR3_add(added_mag_bias_nT, rm3100_driver->info.mag_bias_compo_nT, mag_bias_compo_nT);

  return RM3100_set_mag_bias_compo_nT(rm3100_driver, added_mag_bias_nT);
}


static DS_ERR_CODE RM3100_load_driver_super_init_settings_(DriverSuper* super)
{
  DS_StreamConfig* stream_config;

  super->interface = I2C;

  stream_config = &(super->stream_config[RM3100_STREAM_TLM_CMD]);
  DSSC_enable(stream_config);

  DSSC_set_rx_frame_size(stream_config, RM3100_RX_FRAME_SIZE);
  DSSC_set_data_analyzer(stream_config, RM3100_analyze_rec_data_);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE RM3100_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver)
{
  const uint8_t* rm3100_rx_data = DSSC_get_rx_frame(stream_config);

  RM3100_Driver* rm3100_driver = (RM3100_Driver*)driver;

  RM3100_convert_bytes_to_float_(rm3100_rx_data, rm3100_driver->info.mag_raw_compo_nT);

  RM3100_calc_mag_calibration_(&(rm3100_driver->info));

  return DS_ERR_CODE_OK;
}

static void RM3100_convert_bytes_to_float_(const uint8_t mag_c_bytes[RM3100_RX_FRAME_SIZE], float mag_c_nT[3])
{
  // float conversion
  uint32_t mag_tmp_uint;
  int32_t mag_tmp_int;
  for (uint8_t i = 0; i < 3; i++)
  {
    // 24bit -> uint32
    mag_tmp_uint = mag_c_bytes[i * 3];
    mag_tmp_uint = (mag_tmp_uint << 8) | mag_c_bytes[i * 3 + 1];
    mag_tmp_uint = (mag_tmp_uint << 8) | mag_c_bytes[i * 3 + 2];

    // uint32 -> int32
    if (mag_tmp_uint > 0x00800000)
    {
      mag_tmp_int = (int32_t)(mag_tmp_uint) - (int32_t)(0x01000000);
    }
    else
    {
      mag_tmp_int = (int32_t)(mag_tmp_uint);
    }

    // int32 -> float
    mag_c_nT[i] = (float)mag_tmp_int * RM3100_kConvertRawToNt_;
  }
}

static void RM3100_calc_mag_calibration_(RM3100_Info* info)
{
  // Bias calibration
  VECTOR3_subtract(info->mag_compo_nT,
                  info->mag_raw_compo_nT,
                  info->mag_bias_compo_nT);

  // Coordinate transformation
  QUATERNION_trans_coordinate(info->mag_body_nT,
                              info->frame_transform_c2b,
                              info->mag_compo_nT);
  return;
}

#pragma section
