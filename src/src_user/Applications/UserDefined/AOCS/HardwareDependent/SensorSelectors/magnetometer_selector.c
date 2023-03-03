#pragma section REPRO
/**
* @file   magnetometer_selector.c
* @brief  複数ある磁気センサ出力を組み合わせて一つの観測磁場ベクトルを計算するアプリ
*/

#include "magnetometer_selector.h"

#include <src_core/Library/print.h>
#include "../../../../DriverInstances/di_mpu9250.h"
#include "../../../../DriverInstances/di_rm3100.h"
#include "../SensorFilters/mpu9250_filter.h"
#include "../SensorFilters/rm3100_filter.h"
#include "../../aocs_manager.h"

static MagnetometerSelector        magnetometer_selector_;
const  MagnetometerSelector* const magnetometer_selector = &magnetometer_selector_;

static void APP_MAG_SELECTOR_init_(void);
static void APP_MAG_SELECTOR_exec_(void);

static void APP_MAG_SELECTOR_auto_select_(void);
static void APP_MAG_SELECTOR_fusion_(void);

AppInfo APP_MAG_SELECTOR_create_app(void)
{
  return AI_create_app_info("MagnetometerSelector", APP_MAG_SELECTOR_init_, APP_MAG_SELECTOR_exec_);
}

static void APP_MAG_SELECTOR_init_(void)
{
  magnetometer_selector_.state = APP_MAG_SELECTOR_STATE_RM_EXT;
  magnetometer_selector_.auto_flag = 0;
  return;
}

static void APP_MAG_SELECTOR_exec_(void)
{
  if (magnetometer_selector_.auto_flag != 0)
  {
    APP_MAG_SELECTOR_auto_select_();
  }

  switch (magnetometer_selector_.state)
  {
  case APP_MAG_SELECTOR_STATE_RM_AOBC:
    AOCS_MANAGER_set_mag_vec_obs_body_nT(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_body_nT);
    AOCS_MANAGER_set_mag_vec_est_body_nT(rm3100_filter->mag_est_body_nT[RM3100_IDX_ON_AOBC]);
    break;
  case APP_MAG_SELECTOR_STATE_RM_EXT:
    AOCS_MANAGER_set_mag_vec_obs_body_nT(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_body_nT);
    AOCS_MANAGER_set_mag_vec_est_body_nT(rm3100_filter->mag_est_body_nT[RM3100_IDX_EXTERNAL]);
    break;
  case APP_MAG_SELECTOR_STATE_MPU:
    AOCS_MANAGER_set_mag_vec_obs_body_nT(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_body_nT);
    AOCS_MANAGER_set_mag_vec_est_body_nT(mpu9250_filter->mag_est_body_nT);
    break;
  case APP_MAG_SELECTOR_STATE_FUSION:
    APP_MAG_SELECTOR_fusion_();
    break;
  default:
    // Not Reached
    break;
  }
  return;
}

static void APP_MAG_SELECTOR_auto_select_(void)
{
  // TODO_L 実装する
}

static void APP_MAG_SELECTOR_fusion_(void)
{
  // TODO_L 実装する
}

CCP_EXEC_STS Cmd_APP_MAG_SELECTOR_SET_STATE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  APP_MAG_SELECTOR_STATE state;

  state = (APP_MAG_SELECTOR_STATE)param[0];
  if (state >= APP_MAG_SELECTOR_STATE_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  magnetometer_selector_.state = state;

  return CCP_EXEC_SUCCESS;
}


#pragma section
