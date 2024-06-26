#pragma section REPRO
/**
* @file   target_attitude_calculator.c
* @brief  目標姿勢計算をまとめるアプリ
*/

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/EventManager/event_logger.h>

#include "target_attitude_calculator.h"
#include "quaternion_interpolator.h"
#include "target_attitude_from_orbit.h"
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Applications/app_registry.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/SignalProcess/z_filter.h>

// SatelliteParameters
#include <src_user/Settings/SatelliteParameters/attitude_target_parameters.h>

static TargetAttitudeCalculator        target_attitude_calculator_;
const  TargetAttitudeCalculator* const target_attitude_calculator = &target_attitude_calculator_;
static ZFilter APP_TARGET_ATT_CALC_lpf_1st_ang_vel_[PHYSICAL_CONST_THREE_DIM];     //!< 角速度LPF

static void APP_TARGET_ATT_CALC_init_(void);
static void APP_TARGET_ATT_CALC_exec_(void);

/**
 * @brief  目標Quaternion計算関数
 *         指定の方法で目標Quaternionを計算する
 * @return void
 */
static void APP_TARGET_ATT_CALC_calc_target_quaternion_(void);

/**
 * @brief  目標角速度計算関数
 * @return C2A_MATH_ERRORに準拠
 * @note   Quaternionを手動で指定（MANUALモード）する場合は目標角速度を0とする
 */
static C2A_MATH_ERROR APP_TARGET_ATT_CALC_calc_target_angular_velocity_(void);


AppInfo APP_TARGET_ATT_CALC_create_app(void)
{
  return AI_create_app_info("TargetAttitudeCalculator", APP_TARGET_ATT_CALC_init_, APP_TARGET_ATT_CALC_exec_);
}


static void APP_TARGET_ATT_CALC_init_(void)
{
  // デフォルトはMANUALモード
  target_attitude_calculator_.mode = ATTITUDE_TARGET_PARAMETERS_mode;
  target_attitude_calculator_.quaternion_target_i2t = ATTITUDE_TARGET_PARAMETERS_quaternion_target_i2t;
  target_attitude_calculator_.is_enabled = 0;

  // 角速度計算LPF
  target_attitude_calculator_.sampling_freq_Hz = 10.0f;
  target_attitude_calculator_.cut_off_freq_lpf_1st_Hz = 0.05f;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    Z_FILTER_init_as_lpf_1st(&APP_TARGET_ATT_CALC_lpf_1st_ang_vel_[axis_id],
                             target_attitude_calculator_.sampling_freq_Hz,
                             target_attitude_calculator_.cut_off_freq_lpf_1st_Hz);
  }
}

static void APP_TARGET_ATT_CALC_exec_(void)
{
  // 目標Quaternion計算
  APP_TARGET_ATT_CALC_calc_target_quaternion_();
  if (target_attitude_calculator_.is_enabled)
  {
    AOCS_MANAGER_set_quaternion_target_i2t(target_attitude_calculator_.quaternion_target_i2t);
  }

  // 目標角速度計算
  if (target_attitude_calculator_.mode == APP_TARGET_ATT_CALC_MODE_MANUAL)
  {
    // MANUAL時は目標角速度を{0.0, 0.0, 0.0}とする．
    float ang_vel_target_body_rad_s[PHYSICAL_CONST_THREE_DIM] = {0.0f, 0.0f, 0.0f};
    VECTOR3_copy(target_attitude_calculator_.ang_vel_target_body_rad_s, ang_vel_target_body_rad_s);
  }
  else
  {
    C2A_MATH_ERROR ret = APP_TARGET_ATT_CALC_calc_target_angular_velocity_();
    if (ret != C2A_MATH_ERROR_OK) return;
  }
  AOCS_MANAGER_set_ang_vel_target_body_rad_s(target_attitude_calculator_.ang_vel_target_body_rad_s);
}

static void APP_TARGET_ATT_CALC_calc_target_quaternion_(void)
{
  CommonCmdPacket packet;

  switch (target_attitude_calculator_.mode)
  {
  case APP_TARGET_ATT_CALC_MODE_MANUAL:
    // なにもしない
    break;
  case APP_TARGET_ATT_CALC_MODE_CALCULATOR_FROM_ORBIT:
    CCP_form_app_cmd(&packet, 0, AR_APP_TARGET_ATTITUDE_FROM_ORBIT);
    PH_dispatch_command(&packet);
    target_attitude_calculator_.quaternion_target_i2t = target_attitude_from_orbit->quaternion_target_i2t;
    break;
  case APP_TARGET_ATT_CALC_MODE_QUATERNION_INTERPOLATOR:
    CCP_form_app_cmd(&packet, 0, AR_APP_QUATERNION_INTERPOLATOR);
    PH_dispatch_command(&packet);
    target_attitude_calculator_.quaternion_target_i2t = quaternion_interpolator->quaternion_target_i2t;
    break;
  default:
    // NOT REACHED
    break;
  }
}

static C2A_MATH_ERROR APP_TARGET_ATT_CALC_calc_target_angular_velocity_(void)
{
  Quaternion q_target_previous = target_attitude_calculator_.quaternion_target_i2t_previous;
  Quaternion q_target_current = target_attitude_calculator_.quaternion_target_i2t;
  ObcTime obctime_current = TMGR_get_master_clock();

  // Quaternion差より角速度計算
  float time_diff_sec = (float)OBCT_diff_in_sec(&target_attitude_calculator_.obctime, &obctime_current);
  // TODO: 時間アサーションを入れるかどうか検討する
  float ang_vel_target_body_rad_s[PHYSICAL_CONST_THREE_DIM];
  C2A_MATH_ERROR ret = QUATERNION_calc_angular_velocity(ang_vel_target_body_rad_s,
                                                        q_target_previous,
                                                        q_target_current,
                                                        time_diff_sec);
  if (ret != C2A_MATH_ERROR_OK)
  {
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_TARGET_ATTITUDE_CALCULATOR, EL_ERROR_LEVEL_LOW, (uint32_t)ret);
    return ret;
  }
  // LPF
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    target_attitude_calculator_.ang_vel_target_body_rad_s[axis_id] =
      Z_FILTER_calc_output(&APP_TARGET_ATT_CALC_lpf_1st_ang_vel_[axis_id], ang_vel_target_body_rad_s[axis_id]);
  }
  // 次回計算のために現在の値を代入
  target_attitude_calculator_.quaternion_target_i2t_previous = target_attitude_calculator_.quaternion_target_i2t;
  target_attitude_calculator_.obctime = obctime_current;

  return C2A_MATH_ERROR_OK;
}



CCP_CmdRet Cmd_APP_TARGET_ATT_CALC_SET_MODE(const CommonCmdPacket* packet)
{
  APP_TARGET_ATT_CALC_MODE mode = (APP_TARGET_ATT_CALC_MODE)CCP_get_param_from_packet(packet, 0, uint8_t);
  if (mode >= APP_TARGET_ATT_CALC_MODE_MAX)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  target_attitude_calculator_.mode = mode;
  // MANUAL時はaocs_managerの更新を無効化
  if (target_attitude_calculator_.mode == APP_TARGET_ATT_CALC_MODE_MANUAL)
  {
    target_attitude_calculator_.is_enabled = 0;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TARGET_ATT_CALC_ENABLE(const CommonCmdPacket* packet)
{
  uint8_t is_enabled = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (is_enabled > 1)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  target_attitude_calculator_.is_enabled = is_enabled;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TARGET_ATT_CALC_SET_LPF_ANGULAR_VELOCITY(const CommonCmdPacket* packet)
{
  float sampling_Hz = CCP_get_param_from_packet(packet, 0, float);
  if (sampling_Hz <= 0.0f)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
  float cutoff_Hz = CCP_get_param_from_packet(packet, 1, float);
  if (cutoff_Hz <= 0.0f)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  target_attitude_calculator_.sampling_freq_Hz = sampling_Hz;
  target_attitude_calculator_.cut_off_freq_lpf_1st_Hz = cutoff_Hz;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
