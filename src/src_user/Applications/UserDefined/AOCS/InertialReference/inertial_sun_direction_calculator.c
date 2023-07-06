#pragma section REPRO
/**
* @file   inertial_sun_direction_calculator.c
* @brief  各種慣性系リファレンス計算のうち，慣性太陽方向計算を行う
*/

#include "inertial_sun_direction_calculator.h"
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Library/time_space.h>


static void APP_SUN_DIR_ECI_CALC_init_(void);
static void APP_SUN_DIR_ECI_CALC_exec_(void);


AppInfo APP_SUN_DIR_ECI_CALC_create_app(void)
{
  return AI_create_app_info("SunDirCalc", APP_SUN_DIR_ECI_CALC_init_, APP_SUN_DIR_ECI_CALC_exec_);
}


static void APP_SUN_DIR_ECI_CALC_init_(void)
{
  // TODO_L: ホントに何もしなくてOKか要検討

  return;
}


static void APP_SUN_DIR_ECI_CALC_exec_(void)
{
  double julian_century = TIME_SPACE_convert_julian_day_to_century(aocs_manager->reference_jday);

  float sun_direction_eci_temp[PHYSICAL_CONST_THREE_DIM];
  TIME_SPACE_calc_sun_direction_eci(julian_century, sun_direction_eci_temp);
  AOCS_MANAGER_set_sun_vec_est_eci(sun_direction_eci_temp);

  return;
}

#pragma section
