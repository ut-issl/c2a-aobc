#pragma section REPRO
/**
* @file   di_stim210.c
* @brief  STIM210のDriver Instance
*/

#include "di_stim210.h"

#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Settings/port_config.h>
#include <src_user/Settings/DriverSuper/driver_buffer_define.h>
#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Library/matrix33.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/stim210_parameters.h>

static void DI_STIM210_init_(void);
static void DI_STIM210_update_(void);
static void DI_STIM210_temperature_caliblation_(void);

static STIM210_Driver stim210_driver_[STIM210_IDX_MAX];
const  STIM210_Driver* const stim210_driver[STIM210_IDX_MAX] = {&stim210_driver_[STIM210_IDX_IN_UNIT]};

static DiStim210        di_stim210_[STIM210_IDX_MAX];
const  DiStim210* const di_stim210 [STIM210_IDX_MAX] = {&di_stim210_[STIM210_IDX_IN_UNIT]};

// バッファ
// 面倒くさいので要素数一つと仮定してバッファを確保する
static DS_StreamRecBuffer DI_STIM210_rx_buffer_;
static uint8_t DI_STIM210_rx_buffer_allocation_[DS_STREAM_REC_BUFFER_SIZE_DEFAULT];

AppInfo DI_STIM210_update(void)
{
  return AI_create_app_info("update_DI_STIM210", DI_STIM210_init_, DI_STIM210_update_);
}

static void DI_STIM210_init_(void)
{
  DS_ERR_CODE ret1;
  int ret;

  ret1 = DS_init_stream_rec_buffer(&DI_STIM210_rx_buffer_,
                                   DI_STIM210_rx_buffer_allocation_,
                                   sizeof(DI_STIM210_rx_buffer_allocation_));
  if (ret1 != DS_ERR_CODE_OK)
  {
    Printf("STIM210 buffer init Failed ! %d \n", ret1);
  }

  // 面倒くさいので要素数一つと仮定して初期化する
  ret = STIM210_init(&stim210_driver_[STIM210_IDX_IN_UNIT],
                      PORT_CH_UART_STIM,
                      PORT_CH_GPIO_OUT_STIM_TRIG,
                      PORT_CH_GPIO_OUT_STIM_RST,
                      &DI_STIM210_rx_buffer_);
  if (ret != 0)
  {
    Printf("STIM210 init Failed ! %d \n", ret);
  }

  C2A_MATH_ERROR ret_math;
  Quaternion q_c2b;
  QUATERNION_make_from_euler_angles(&q_c2b,
                                    PHYSICAL_CONST_degree_to_radian(90.0f),
                                    PHYSICAL_CONST_degree_to_radian(-90.0f),
                                    PHYSICAL_CONST_degree_to_radian(0.0f),
                                    EULER_ANGLE_ROTATION_ORDER_321);
  ret_math = STIM210_set_frame_transform_c2b(&stim210_driver_[STIM210_IDX_IN_UNIT], q_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("STIM210: q_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  // 温度補正
  const float kRangeLow  = STIM210_PARAMETERS_temperature_range_low_degC;
  const float kRangeHigh = STIM210_PARAMETERS_temperature_range_high_degC;
  // 計測値から設定するが温度依存性はかなり小さい
  // 特にSFは取付誤差と見分けづらいのでなしとする
  // SF,バイアスは y = SF*x - BIASという式を想定
  // X軸
  ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].bias_compo_rad_s[0]),
                                     STIM210_PARAMETERS_kNumCoeffTempCalib, STIM210_PARAMETERS_bias_coeff_compo_x, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("STIM210 Gyro-X Bias Temperature Caliblation init Failed ! \n");
  ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].scale_factor_compo[0]),
                                     STIM210_PARAMETERS_kNumCoeffTempCalib, STIM210_PARAMETERS_scale_factor_coeff_compo_x, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("STIM210 Gyro-X SF Temperature Caliblation init Failed ! \n");

  // Y軸
  ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].bias_compo_rad_s[1]),
                                     STIM210_PARAMETERS_kNumCoeffTempCalib, STIM210_PARAMETERS_bias_coeff_compo_y, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("STIM210 Gyro-Y Bias Temperature Caliblation init Failed ! \n");
  ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].scale_factor_compo[1]),
                                     STIM210_PARAMETERS_kNumCoeffTempCalib, STIM210_PARAMETERS_scale_factor_coeff_compo_y, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("STIM210 Gyro-Y SF Temperature Caliblation init Failed ! \n");

  // Z軸
  ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].bias_compo_rad_s[2]),
                                     STIM210_PARAMETERS_kNumCoeffTempCalib, STIM210_PARAMETERS_bias_coeff_compo_z, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("STIM210 Gyro-Z Bias Temperature Caliblation init Failed ! \n");
  ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].scale_factor_compo[2]),
                                     STIM210_PARAMETERS_kNumCoeffTempCalib, STIM210_PARAMETERS_scale_factor_coeff_compo_z, kRangeLow, kRangeHigh);
  if (ret < 0) Printf("STIM210 Gyro-Z SF Temperature Caliblation init Failed ! \n");

  return;
}


static void DI_STIM210_update_(void)
{
  if ((power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_STIM210] == APP_PSC_STATE_ON) &&
      (stim210_driver[STIM210_IDX_IN_UNIT])->info.operation_mode == STIM210_OPERATION_NORMAL_MODE)
  {
    // TODO_L: サービスモード時のアノマリ処理追加
    int ret_gpio = STIM210_send_tlm_by_gpio(&(stim210_driver_[STIM210_IDX_IN_UNIT]));
    if (ret_gpio != 0)
    {
      // Printf("STIM210 gpio failed ! %d \n", ret_gpio);
    }

    // テレメ送信は86us後に始まる
    DS_REC_ERR_CODE ret_rec = STIM210_rec(&(stim210_driver_[STIM210_IDX_IN_UNIT]));

    if (stim210_driver_->driver.super.config.info.rec_status_.ret_from_if_rx == 0) return;

    if (ret_rec != DS_REC_OK)
    {
      EL_record_event(EL_GROUP_TLM_ERROR_STIM210, (uint32_t)STIM210_IDX_IN_UNIT, EL_ERROR_LEVEL_HIGH, (uint32_t)ret_rec);
    }

    STIM210_CRC_STATE state = stim210_driver_->info.crc_state;
    uint8_t status = stim210_driver_->info.status;

    if (state != STIM210_CRC_STATE_OK)
    {
      EL_record_event(EL_GROUP_ERROR_STIM210, (uint32_t)STIM210_IDX_IN_UNIT, EL_ERROR_LEVEL_HIGH, (uint32_t)STIM210_EL_NOTE_CRC_ERROR);
    }
    else if (status != 0)
    {
      // EL_record_event(EL_GROUP_ERROR_STIM210, (uint32_t)STIM210_IDX_IN_UNIT, EL_ERROR_LEVEL_HIGH, (uint32_t)STIM210_EL_NOTE_STATUS_ERROR);
    }
    else
    {
      // NOT REACHED
    }

    DI_STIM210_temperature_caliblation_();
  }
  else if (power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_STIM210] == APP_PSC_STATE_OFF)
  {
    // 電源リセット後はinitモードから始まるようにする
    STIM210_reset_param(&(stim210_driver_[STIM210_IDX_IN_UNIT]));
  }
}

static void DI_STIM210_temperature_caliblation_(void)
{
  for (int stim_idx = 0; stim_idx < STIM210_IDX_MAX; stim_idx++)
  {
    float gyro_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
    float gyro_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
    MATRIX33_make_unit(gyro_scale_factor_compo);

    for (int axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
    {
      float temperature_degC = stim210_driver_[stim_idx].info.temperature_compo_degC[axis_id];
      DiStim210 temp_cal = di_stim210_[stim_idx];
      int ret;
      ret = POLYNOMIAL_APPROX_calc_output(&gyro_bias_compo_rad_s[axis_id],
                                          temp_cal.bias_compo_rad_s[axis_id],
                                          temperature_degC);
      if (ret < 0) gyro_bias_compo_rad_s[axis_id] = 0.0f; // 異常時はバイアス補正はしない
      ret = POLYNOMIAL_APPROX_calc_output(&gyro_scale_factor_compo[axis_id][axis_id],
                                          temp_cal.scale_factor_compo[axis_id],
                                          temperature_degC);
      if (ret < 0) gyro_scale_factor_compo[axis_id][axis_id] = 1.0f; // 異常時はSF補正はしない
    }
    STIM210_set_ang_vel_bias_compo_rad_s(&(stim210_driver_[stim_idx]), gyro_bias_compo_rad_s);
    STIM210_set_ang_vel_scale_factor_compo(&(stim210_driver_[stim_idx]), gyro_scale_factor_compo);
  }
}

CCP_CmdRet Cmd_DI_STIM210_INIT(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;

  ret = STIM210_set_service_mode(&(stim210_driver_[STIM210_IDX_IN_UNIT]));
  ret = STIM210_set_normal_mode(&(stim210_driver_[STIM210_IDX_IN_UNIT]));

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_STIM210_SET_MODE(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_OPERATION_MODE mode = (STIM210_OPERATION_MODE)param[0];
  if (mode >= STIM210_OPERATION_MODE_MAX)  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  if (mode == STIM210_OPERATION_NORMAL_MODE)
  {
    ret = STIM210_set_normal_mode(&(stim210_driver_[STIM210_IDX_IN_UNIT]));
  }
  else if (mode == STIM210_OPERATION_SERVICE_MODE)
  {
    ret = STIM210_set_service_mode(&(stim210_driver_[STIM210_IDX_IN_UNIT]));
  }
  else
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_STIM210_SET_NORMAL_MODE_FORMAT(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_NORMAL_MODE_FORMAT format = (STIM210_NORMAL_MODE_FORMAT)param[0];
  if (format >= STIM210_NORMAL_MODE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = STIM210_set_normal_mode_format(&(stim210_driver_[STIM210_IDX_IN_UNIT]), format);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_STIM210_SET_SAMPLE_RATE(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_SAMPLE_RATE sample_rate = (STIM210_SAMPLE_RATE)param[0];
  if (sample_rate >= STIM210_SAMPLE_RATE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = STIM210_set_sample_rate(&(stim210_driver_[STIM210_IDX_IN_UNIT]), sample_rate);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_STIM210_SET_GYRO_OUTPUT(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_GYRO_OUTPUT_MODE gyro_output_mode = (STIM210_GYRO_OUTPUT_MODE)param[0];
  if (gyro_output_mode >= STIM210_GYRO_OUTPUT_MODE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = STIM210_set_gyro_output(&(stim210_driver_[STIM210_IDX_IN_UNIT]), gyro_output_mode);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_STIM210_SET_TERMINATION_MODE(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_TERMINATION_MODE termination_mode = (STIM210_TERMINATION_MODE)param[0];
  if (termination_mode >= STIM210_TERMINATION_MODE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = STIM210_set_termination_mode(&(stim210_driver_[STIM210_IDX_IN_UNIT]), termination_mode);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_STIM210_SET_LOW_PASS_FILTER(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_LPF low_pass_filter_frequency = (STIM210_LPF)param[0];
  if (low_pass_filter_frequency >= STIM210_LPF_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = STIM210_set_low_pass_filter(&(stim210_driver_[STIM210_IDX_IN_UNIT]), low_pass_filter_frequency);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_STIM210_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_IDX idx;
  idx = (STIM210_IDX)param[0];
  if (idx >= STIM210_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float q_array_c2b[PHYSICAL_CONST_QUATERNION_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_QUATERNION_DIM; axis++)
  {
    ENDIAN_memcpy(&q_array_c2b[axis], param + 1 + axis * sizeof(float), sizeof(float));
  }

  Quaternion quaternion_c2b;
  C2A_MATH_ERROR ret;
  ret = QUATERNION_make_from_array(&quaternion_c2b, q_array_c2b, QUATERNION_SCALAR_POSITION_LAST);
  if (ret != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  STIM210_set_frame_transform_c2b(&stim210_driver_[idx], quaternion_c2b);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_STIM210_SET_ANG_VEL_BIAS_COMPO_RAD_S(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  STIM210_IDX idx;
  idx = (STIM210_IDX)param[0];

  if (idx >= STIM210_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    ENDIAN_memcpy(&ang_vel_bias_compo_rad_s[axis], param + 1 + axis * sizeof(float), sizeof(float));
  }

  C2A_MATH_ERROR ret;
  ret = STIM210_set_ang_vel_bias_compo_rad_s(&stim210_driver_[idx], ang_vel_bias_compo_rad_s);

  if (ret != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_STIM210_SET_ANG_VEL_BIAS_TEMP_CALIB(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t offset = 0;

  // TODO_L: コマンド引数サイズ制限的にSTIM210_IDXは入れ込めない。必要になったら対応する
  uint8_t axis;
  axis = param[0];
  offset += 1;
  if (axis >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float range_low_degC, range_high_degC;
  ENDIAN_memcpy(&range_low_degC, param + offset, sizeof(float));
  offset += sizeof(float);
  ENDIAN_memcpy(&range_high_degC, param + offset, sizeof(float));
  offset += sizeof(float);

  float coeff[STIM210_PARAMETERS_kNumCoeffTempCalib];
  for (uint8_t coeff_idx = 0; coeff_idx < STIM210_PARAMETERS_kNumCoeffTempCalib; coeff_idx++)
  {
    ENDIAN_memcpy(&coeff[coeff_idx], param + offset, sizeof(float));
    offset += sizeof(float);
  }

  int ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].bias_compo_rad_s[axis]),
                                         STIM210_PARAMETERS_kNumCoeffTempCalib, coeff, range_low_degC, range_high_degC);
  if (ret < 0)  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_STIM210_SET_ANG_VEL_SF_TEMP_CALIB(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t offset = 0;

  // TODO_L: コマンド引数サイズ制限的にSTIM210_IDXは入れ込めない。必要になったら対応する
  uint8_t axis;
  axis = param[0];
  offset += 1;
  if (axis >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float range_low_degC, range_high_degC;
  ENDIAN_memcpy(&range_low_degC, param + offset, sizeof(float));
  offset += sizeof(float);
  ENDIAN_memcpy(&range_high_degC, param + offset, sizeof(float));
  offset += sizeof(float);

  float coeff[STIM210_PARAMETERS_kNumCoeffTempCalib];
  for (uint8_t coeff_idx = 0; coeff_idx < STIM210_PARAMETERS_kNumCoeffTempCalib; coeff_idx++)
  {
    ENDIAN_memcpy(&coeff[coeff_idx], param + offset, sizeof(float));
    offset += sizeof(float);
  }

  int ret = POLYNOMIAL_APPROX_initialize(&(di_stim210_[STIM210_IDX_IN_UNIT].scale_factor_compo[axis]),
                                         STIM210_PARAMETERS_kNumCoeffTempCalib, coeff, range_low_degC, range_high_degC);
  if (ret < 0)  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
