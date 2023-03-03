#pragma section REPRO
/**
* @file   di_mpu9250.c
* @brief  MPU9250のDriver Instance
*/

#include "di_mpu9250.h"

#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../Settings/port_config.h"
#include "../UserDefined/Power/power_switch_control.h"
#include "../../Library/matrix33.h"

static MPU9250_Driver mpu9250_driver_[MPU9250_IDX_MAX];
const  MPU9250_Driver* const mpu9250_driver[MPU9250_IDX_MAX] = {&mpu9250_driver_[MPU9250_IDX_ON_AOBC]};

static DiMpu9250        di_mpu9250_[MPU9250_IDX_MAX];
const  DiMpu9250* const di_mpu9250[MPU9250_IDX_MAX] = {&di_mpu9250_[MPU9250_IDX_ON_AOBC]};

static const uint8_t DI_MPU9250_kNumCoeffTempCalib_ = 2;

static void DI_MPU9250_init_(void);
static void DI_MPU9250_update_(void);
static void DI_MPU9250_temperature_caliblation_(void);

static uint8_t DI_MPU9250_is_initialized_[MPU9250_IDX_MAX] = { 0 };  //!< 0 = not initialized, 1 = initialized

AppInfo DI_MPU9250_update(void)
{
  return AI_create_app_info("update_DI_MPU9250", DI_MPU9250_init_, DI_MPU9250_update_);
}


static void DI_MPU9250_init_(void)
{
  int ret = MPU9250_init(&mpu9250_driver_[MPU9250_IDX_ON_AOBC], PORT_CH_I2C_SENSORS, I2C_DEVICE_ADDR_AOBC_MPU, I2C_DEVICE_ADDR_AOBC_MPU_AK);
  if (ret != 0)
  {
    Printf("MPU9250 init Failed ! %d \n", ret);
  }

  C2A_MATH_ERROR ret_math;
  Quaternion q_aobc_c2b;
  QUATERNION_make_from_euler_angles(&q_aobc_c2b,
                                    PHYSICAL_CONST_degree_to_radian(0.0f),
                                    PHYSICAL_CONST_degree_to_radian(90.0f),
                                    PHYSICAL_CONST_degree_to_radian(180.0f),
                                    EULER_ANGLE_ROTATION_ORDER_123);
  ret_math = MPU9250_set_frame_transform_c2b(&mpu9250_driver_[MPU9250_IDX_ON_AOBC], q_aobc_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("MPU9250: q_aobc_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM] = { -0.01678f, -0.00752f, 0.02206f}; // TODO_L: 温度補正されるので、削除しても良い
  ret_math = MPU9250_set_ang_vel_bias_compo_rad_s(&mpu9250_driver_[MPU9250_IDX_ON_AOBC], ang_vel_bias_compo_rad_s);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("MPU9250: ang_vel_bias set error.\n");
  }

  float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM] = { -18417.53f, -30423.31f, 20969.18f }; // 実機計測値
  ret_math = MPU9250_set_mag_bias_compo_nT(&mpu9250_driver_[MPU9250_IDX_ON_AOBC], mag_bias_compo_nT);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("MPU9250: mag_bias set error.\n");
  }

  // 温度補正
  const float kRangeLow = -50.0f; // degC
  const float kRangeHigh = 50.0f; // degC
  float bias_coeff[DI_MPU9250_kNumCoeffTempCalib_];
  float scale_factor_coeff[DI_MPU9250_kNumCoeffTempCalib_];
  // 切片は実機計測値、切片はOPT-1, RWASATでの測定値を利用
  // 特にSFは小さいのでなしとする
  // SF,バイアスは y = SF*x - BIASという式を想定
  // X軸
  bias_coeff[0] = -0.024f;
  bias_coeff[1] = 0.0002f;
  ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_bias_compo_rad_s[0]),
                                     DI_MPU9250_kNumCoeffTempCalib_, bias_coeff, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("MPU9250 Gyro-X Bias Temperature Caliblation init Failed ! \n");
  scale_factor_coeff[0] = 1.0f;
  scale_factor_coeff[1] = 0.0f;
  ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_scale_factor_compo[0]),
                                     DI_MPU9250_kNumCoeffTempCalib_, scale_factor_coeff, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("MPU9250 Gyro-X SF Temperature Caliblation init Failed ! \n");
  // Y軸
  bias_coeff[0] = 0.0058f;
  bias_coeff[1] = -0.0005f;
  ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_bias_compo_rad_s[1]),
                                     DI_MPU9250_kNumCoeffTempCalib_, bias_coeff, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("MPU9250 Gyro-Y Bias Temperature Caliblation init Failed ! \n");
  scale_factor_coeff[0] = 1.0f;
  scale_factor_coeff[1] = 0.0f;
  ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_scale_factor_compo[1]),
                                     DI_MPU9250_kNumCoeffTempCalib_, scale_factor_coeff, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("MPU9250 Gyro-Y SF Temperature Caliblation init Failed ! \n");
  // Z軸
  bias_coeff[0] = 0.0179f;
  bias_coeff[1] = 0.0001f;
  ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_bias_compo_rad_s[2]),
                                     DI_MPU9250_kNumCoeffTempCalib_, bias_coeff, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("MPU9250 Gyro-Z Bias Temperature Caliblation init Failed ! \n");
  scale_factor_coeff[0] = 1.0f;
  scale_factor_coeff[1] = 0.0f;
  ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_scale_factor_compo[2]),
                                     DI_MPU9250_kNumCoeffTempCalib_, scale_factor_coeff, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("MPU9250 Gyro-Z SF Temperature Caliblation init Failed ! \n");

  return;
}


static void DI_MPU9250_update_(void)
{
  if (power_switch_control->switch_state_5v[APP_PSC_5V_IDX_MPU9250] == APP_PSC_STATE_OFF)
  {
    DI_MPU9250_is_initialized_[MPU9250_IDX_ON_AOBC] = 0;
    return;
  }

  int ret;
  if (DI_MPU9250_is_initialized_[MPU9250_IDX_ON_AOBC])
  {
    ret = MPU9250_rec(&mpu9250_driver_[MPU9250_IDX_ON_AOBC]);
    if (ret != 0)
    {
      EL_record_event(EL_GROUP_TLM_ERROR_MPU9250, (uint32_t)MPU9250_IDX_ON_AOBC, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
    }

    DI_MPU9250_temperature_caliblation_();
  }
  return;
}

static void DI_MPU9250_temperature_caliblation_(void)
{
  for (int mpu_idx = 0; mpu_idx < MPU9250_IDX_MAX; mpu_idx++)
  {
    float gyro_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
    float gyro_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
    MATRIX33_make_unit(gyro_scale_factor_compo);

    float temperature_degC = mpu9250_driver_[mpu_idx].info.temperature_degC;
    DiMpu9250 temp_cal = di_mpu9250_[mpu_idx];

    for (int axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
    {
      int ret;
      ret = POLYNOMIAL_APPROX_calc_output(&gyro_bias_compo_rad_s[axis_id],
                                          temp_cal.gyro_bias_compo_rad_s[axis_id],
                                          temperature_degC);
      if (ret < 0) gyro_bias_compo_rad_s[axis_id] = 0.0f; // 異常時はバイアス補正はしない
      ret = POLYNOMIAL_APPROX_calc_output(&gyro_scale_factor_compo[axis_id][axis_id],
                                          temp_cal.gyro_scale_factor_compo[axis_id],
                                          temperature_degC);
      if (ret < 0) gyro_scale_factor_compo[axis_id][axis_id] = 1.0f; // 異常時はSF補正はしない
    }
    MPU9250_set_ang_vel_bias_compo_rad_s(&(mpu9250_driver_[mpu_idx]), gyro_bias_compo_rad_s);
    MPU9250_set_ang_vel_scale_factor_compo(&(mpu9250_driver_[mpu_idx]), gyro_scale_factor_compo);
  }
}

CCP_EXEC_STS Cmd_DI_MPU9250_INIT(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;

  // 実行時間の問題がないか? -> 実機試験で問題ない事を確認しているのでこれで進める
  // TODO_L: このままで進める場合はどこでエラーが出たか分かるようにする
  // ジャイロ出力ON
  ret = MPU9250_enable_gyro(&mpu9250_driver_[MPU9250_IDX_ON_AOBC]);
  if (ret != DS_CMD_OK)  return DS_conv_cmd_err_to_ccp_exec_sts(ret);

  // LPF設定ON
  ret = MPU9250_set_lpf(&mpu9250_driver_[MPU9250_IDX_ON_AOBC]);
  if (ret != DS_CMD_OK)  return DS_conv_cmd_err_to_ccp_exec_sts(ret);

  // 角速度範囲指定
  ret = MPU9250_set_ang_vel_range(&mpu9250_driver_[MPU9250_IDX_ON_AOBC], MPU9250_ANG_VEL_RANGE_250_DEG_S);
  if (ret != DS_CMD_OK)  return DS_conv_cmd_err_to_ccp_exec_sts(ret);

  // 加速度範囲指定
  ret = MPU9250_set_accel_range(&mpu9250_driver_[MPU9250_IDX_ON_AOBC], MPU9250_ACCEL_RANGE_2_G);
  if (ret != DS_CMD_OK)  return DS_conv_cmd_err_to_ccp_exec_sts(ret);

  // 磁気センサ出力ON
  ret = MPU9250_enable_mag(&mpu9250_driver_[MPU9250_IDX_ON_AOBC]);
  if (ret != DS_CMD_OK)  return DS_conv_cmd_err_to_ccp_exec_sts(ret);

  DI_MPU9250_is_initialized_[MPU9250_IDX_ON_AOBC] = 1;

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_MPU9250_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  MPU9250_IDX idx;
  idx = (MPU9250_IDX)param[0];
  if (idx >= MPU9250_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float q_array_c2b[PHYSICAL_CONST_QUATERNION_DIM];
  for (int idx = 0; idx < PHYSICAL_CONST_QUATERNION_DIM; idx++)
  {
    endian_memcpy(&q_array_c2b[idx], param + 1 + idx * sizeof(float), sizeof(float));
  }
  Quaternion quaternion_c2b;
  C2A_MATH_ERROR ret;
  ret = QUATERNION_make_from_array(&quaternion_c2b, q_array_c2b, QUATERNION_SCALAR_POSITION_LAST);
  if (ret != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  MPU9250_set_frame_transform_c2b(&mpu9250_driver_[idx], quaternion_c2b);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_MPU9250_SET_ANG_VEL_BIAS_COMPO_RAD_S(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  MPU9250_IDX idx;
  idx = (MPU9250_IDX)param[0];
  if (idx >= MPU9250_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    endian_memcpy(&ang_vel_bias_compo_rad_s[axis], param + 1 + axis * sizeof(float), sizeof(float));
  }

  C2A_MATH_ERROR ret;
  ret = MPU9250_set_ang_vel_bias_compo_rad_s(&mpu9250_driver_[idx], ang_vel_bias_compo_rad_s);
  if (ret != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_MPU9250_SET_MAG_BIAS_COMPO_NT(const CommonCmdPacket* packet)
{
  uint8_t arg_num = 0;
  MPU9250_IDX idx = (MPU9250_IDX)CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (idx >= MPU9250_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    mag_bias_compo_nT[axis] = CCP_get_param_from_packet(packet, arg_num, float);
    arg_num++;
  }

  uint8_t add_flag = CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (idx >= 2) return CCP_EXEC_ILLEGAL_PARAMETER;

  C2A_MATH_ERROR ret;
  if (add_flag == 0)
  {
    ret = MPU9250_set_mag_bias_compo_nT(&mpu9250_driver_[idx], mag_bias_compo_nT);
  }
  else
  {
    ret = MPU9250_add_mag_bias_compo_nT(&mpu9250_driver_[idx], mag_bias_compo_nT);
  }
  if (ret != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_MPU9250_SET_ANG_VEL_BIAS_TEMP_CALIB(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t offset = 0;

  // TODO_L: コマンド引数サイズ制限的にMPU9250_IDXは入れ込めない。必要になったら対応する
  uint8_t axis;
  axis = param[0];
  offset += 1;
  if (axis >= PHYSICAL_CONST_THREE_DIM) return CCP_EXEC_ILLEGAL_PARAMETER;

  float range_low_degC, range_high_degC;
  endian_memcpy(&range_low_degC, param + offset, sizeof(float));
  offset += sizeof(float);
  endian_memcpy(&range_high_degC, param + offset, sizeof(float));
  offset += sizeof(float);

  float coeff[DI_MPU9250_kNumCoeffTempCalib_];
  endian_memcpy(&coeff[0], param + offset, sizeof(float));
  offset += sizeof(float);
  endian_memcpy(&coeff[1], param + offset, sizeof(float));
  offset += sizeof(float);

  int ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_bias_compo_rad_s[axis]),
                                         DI_MPU9250_kNumCoeffTempCalib_, coeff, range_low_degC, range_high_degC);
  if (ret < 0)  return CCP_EXEC_ILLEGAL_CONTEXT;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_MPU9250_SET_ANG_VEL_SF_TEMP_CALIB(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t offset = 0;

  // TODO_L: コマンド引数サイズ制限的にMPU9250_IDXは入れ込めない。必要になったら対応する
  uint8_t axis;
  axis = param[0];
  offset += 1;
  if (axis >= PHYSICAL_CONST_THREE_DIM) return CCP_EXEC_ILLEGAL_PARAMETER;

  float range_low_degC, range_high_degC;
  endian_memcpy(&range_low_degC, param + offset, sizeof(float));
  offset += sizeof(float);
  endian_memcpy(&range_high_degC, param + offset, sizeof(float));
  offset += sizeof(float);

  float coeff[DI_MPU9250_kNumCoeffTempCalib_];
  endian_memcpy(&coeff[0], param + offset, sizeof(float));
  offset += sizeof(float);
  endian_memcpy(&coeff[1], param + offset, sizeof(float));
  offset += sizeof(float);

  int ret = POLYNOMIAL_APPROX_initialize(&(di_mpu9250_[MPU9250_IDX_ON_AOBC].gyro_scale_factor_compo[axis]),
                                         DI_MPU9250_kNumCoeffTempCalib_, coeff, range_low_degC, range_high_degC);
  if (ret < 0)  return CCP_EXEC_ILLEGAL_CONTEXT;

  return CCP_EXEC_SUCCESS;
}

#pragma section
