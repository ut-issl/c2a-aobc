#pragma section REPRO
/**
* @file    fine_three_axis_determination.c
* @brief   精三軸姿勢決定アプリ
*/

#include "fine_three_axis_determination.h"

#include <math.h>

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_error.h>
#include <src_user/Library/quaternion.h>
#include "stt_gyro_ekf.h"

// Satellite parameters
#include <src_user/Settings/SatelliteParameters/attitude_determination_parameters.h>

static FineThreeAxisDetermination        fine_three_axis_determination_;
const  FineThreeAxisDetermination* const fine_three_axis_determination = &fine_three_axis_determination_;

static void APP_FTAD_init_(void);
static void APP_FTAD_exec_(void);

/**
* @brief  STT直接利用による三軸姿勢決定
* @return ECI->body変換クオータニオン
*/
static Quaternion APP_FTAD_stt_direct_(float time_step_s);

AppInfo APP_FTAD_create_app(void)
{
  return AI_create_app_info("Fine Three Axis Determination", APP_FTAD_init_, APP_FTAD_exec_);
}

static void APP_FTAD_init_(void)
{
  fine_three_axis_determination_.previous_obc_time = TMGR_get_master_clock();
  fine_three_axis_determination_.method = ATTITUDE_DETERMINATION_PARAMETERS_ftad_method;
}

static void APP_FTAD_exec_(void)
{
  // タイマアップデート
  ObcTime current_obc_time = TMGR_get_master_clock();
  float time_step_s = (float)OBCT_diff_in_sec(&(fine_three_axis_determination_.previous_obc_time), &current_obc_time);
  fine_three_axis_determination_.previous_obc_time = current_obc_time;
  if (time_step_s < 0.0f) return;

  Quaternion q_eci_to_body;
  switch (fine_three_axis_determination_.method)
  {
  case APP_FTAD_METHOD_STT:
    q_eci_to_body = APP_FTAD_stt_direct_(time_step_s);
    AOCS_MANAGER_set_quaternion_est_i2b(q_eci_to_body);
    break;
  case APP_FTAD_METHOD_EKF:
    APP_STT_GYRO_EKF_execute_estimation(time_step_s);
    if (stt_gyro_ekf->use_ekf_estimated_attitude == APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE_ENABLE)
    {
      q_eci_to_body = stt_gyro_ekf->estimated_result.quaternion_i2b;
      AOCS_MANAGER_set_quaternion_est_i2b(q_eci_to_body);
      AOCS_MANAGER_set_ang_vel_est_body_rad_s(stt_gyro_ekf->estimated_result.angular_velocity_body_rad_s);
    }
    else
    {
      // EKFアプリの結果を使わない場合は，代わりにSTTの姿勢をAOCS managerに入れる
      q_eci_to_body = APP_FTAD_stt_direct_(time_step_s);
      AOCS_MANAGER_set_quaternion_est_i2b(q_eci_to_body);
    }
    break;
  default:
    // Not Reached
    break;
  }
}

static Quaternion APP_FTAD_stt_direct_(float time_step_s)
{
  Quaternion quaternion_i2b;

  if (aocs_manager->stt_availability == AOCS_MANAGER_STT_AVAILABLE)
  {
    quaternion_i2b = aocs_manager->quaternion_est_i2b;
  }
  else
  {
    quaternion_i2b = QUATERNION_euler_propagate(aocs_manager->quaternion_est_i2b,
                                                aocs_manager->ang_vel_est_body_rad_s,
                                                time_step_s);
  }

  return quaternion_i2b;
}

CCP_CmdRet Cmd_APP_FTAD_SET_METHOD(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  APP_FTAD_METHOD method = (APP_FTAD_METHOD)param[0];

  if (method >= APP_FTAD_METHOD_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  fine_three_axis_determination_.method = method;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
