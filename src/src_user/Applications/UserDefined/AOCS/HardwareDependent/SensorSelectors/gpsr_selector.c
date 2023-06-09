#pragma section REPRO
/**
* @file   gpsr_selector.c
* @brief  複数あるGPSR出力を組み合わせて位置・時刻を求めるアプリ
* @note   現時点では一つしか搭載されていないので、その一つを使う
*/

#include "gpsr_selector.h"

#include <src_core/Library/print.h>
#include "../../../../DriverInstances/di_oem7600.h"
#include "../../aocs_manager.h"

static GpsrSelector        gpsr_selector_;
const  GpsrSelector* const gpsr_selector = &gpsr_selector_;

static void APP_GPSR_SELECTOR_init_(void);
static void APP_GPSR_SELECTOR_exec_(void);

AppInfo APP_GPSR_SELECTOR_create_app(void)
{
  return AI_create_app_info("GPSR Selector", APP_GPSR_SELECTOR_init_, APP_GPSR_SELECTOR_exec_);
}

static void APP_GPSR_SELECTOR_init_(void)
{
  gpsr_selector_.state = APP_GPSR_SELECTOR_STATE_OEM_IN_UNIT;

  return;
}

static void APP_GPSR_SELECTOR_exec_(void)
{
  //!< GPSRの可視衛星数が十分と判断する際の閾値とする可視衛星数
  static const uint8_t kEnoughNumberOfVisibleGps = 4;

  switch (gpsr_selector_.state)
  {
  case APP_GPSR_SELECTOR_STATE_OEM_IN_UNIT:
    if (oem7600_driver[OEM7600_IDX_IN_UNIT]->info.num_of_visible_sats >= kEnoughNumberOfVisibleGps)
    {
      GPS_TIME_OF_WEEK obs_gps_time = GPS_TIME_OF_WEEK_create_gps_time(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.times.gps_time_week,
                                                                       oem7600_driver[OEM7600_IDX_IN_UNIT]->info.times.gps_time_ms);
      AOCS_MANAGER_set_current_gps_time_obs(obs_gps_time, oem7600_driver[OEM7600_IDX_IN_UNIT]->info.times.obct_gps_time_obs);

      AOCS_MANAGER_set_sat_pos_vel_obs_ecef_m(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.pos_antenna_ecef_m,
                                              oem7600_driver[OEM7600_IDX_IN_UNIT]->info.vel_antenna_ecef_m_s);

      AOCS_MANAGER_set_gps_visibility(AOCS_MANAGER_GPS_VISIBLE);
    }
    else
    {
      AOCS_MANAGER_set_gps_visibility(AOCS_MANAGER_GPS_INVISIBILE);
    }
    break;
  default:
    // Not Reached
    break;
  }
  return;
}

#pragma section
