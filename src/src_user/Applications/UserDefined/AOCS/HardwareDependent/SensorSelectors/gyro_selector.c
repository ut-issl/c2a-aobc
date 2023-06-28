#pragma section REPRO
/**
* @file   gyro_selector.c
* @brief  複数あるジャイロセンサ出力を組み合わせて一つの観測角速度ベクトルを計算するアプリ
*/

#include "gyro_selector.h"

#include <src_core/Library/print.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

#include "../../../../DriverInstances/di_mpu9250.h"
#include "../../../../DriverInstances/di_stim210.h"
#include "../SensorFilters/mpu9250_filter.h"
#include "../SensorFilters/stim210_filter.h"
#include "../../aocs_manager.h"

// Satellite Parameters
#include "../../../../../Settings/SatelliteParameters/component_selector_parameters.h"

static GyroSelector        gyro_selector_;
const  GyroSelector* const gyro_selector = &gyro_selector_;

static void APP_GYRO_SELECTOR_init_(void);
static void APP_GYRO_SELECTOR_exec_(void);

static void APP_GYRO_SELECTOR_auto_select_(void);
static void APP_GYRO_SELECTOR_fusion_(void);

AppInfo APP_GYRO_SELECTOR_create_app(void)
{
  return AI_create_app_info("GyroSelector", APP_GYRO_SELECTOR_init_, APP_GYRO_SELECTOR_exec_);
}

static void APP_GYRO_SELECTOR_init_(void)
{
  gyro_selector_.state = COMPONENT_SELECTOR_PARAMETERS_initial_selected_gyro;
  gyro_selector_.auto_flag = 0;
  return;
}

static void APP_GYRO_SELECTOR_exec_(void)
{
  if (gyro_selector_.auto_flag != 0)
  {
    APP_GYRO_SELECTOR_auto_select_();
  }

  switch (gyro_selector_.state)
  {
  case APP_GYRO_SELECTOR_STATE_MPU:
    AOCS_MANAGER_set_ang_vel_obs_body_rad_s(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_body_rad_s);
    AOCS_MANAGER_set_ang_vel_est_body_rad_s(mpu9250_filter->ang_vel_est_body_rad_s);
    break;
  case APP_GYRO_SELECTOR_STATE_STIM:
    AOCS_MANAGER_set_ang_vel_obs_body_rad_s(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_body_rad_sec);
    AOCS_MANAGER_set_ang_vel_est_body_rad_s(stim210_filter->ang_vel_est_body_rad_s);
    break;
  case APP_GYRO_SELECTOR_STATE_FUSION:
    APP_GYRO_SELECTOR_fusion_();
    break;
  default:
    // Not Reached
    break;
  }
  return;
}

static void APP_GYRO_SELECTOR_auto_select_(void)
{
  // TODO_L 実装する
}

static void APP_GYRO_SELECTOR_fusion_(void)
{
  // TODO_L 実装する
}

CCP_CmdRet Cmd_APP_GYRO_SELECTOR_SET_STATE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  APP_GYRO_SELECTOR_STATE state;

  state = (APP_GYRO_SELECTOR_STATE)param[0];
  if (state >= APP_GYRO_SELECTOR_STATE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  gyro_selector_.state = state;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
