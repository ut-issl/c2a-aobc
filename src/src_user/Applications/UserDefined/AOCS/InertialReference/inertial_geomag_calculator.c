#pragma section REPRO
/**
* @file   inertial_geomag_calculator.c
* @brief  各種慣性系リファレンス計算のうち，IGRF計算を行う
*/

#include "inertial_geomag_calculator.h"
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/time_space.h>
#include <src_user/Library/geomagnetism.h>

#include <src_core/System/EventManager/event_logger.h>
#include <src_user/Applications/app_registry.h>


static InertialGeomagCalculator        inertial_geomag_calulator_;
const  InertialGeomagCalculator* const inertial_geomag_calulator = &inertial_geomag_calulator_;

static void APP_GEOMAG_ECI_CALC_init_(void);
static void APP_GEOMAG_ECI_CALC_exec_(void);


AppInfo APP_GEOMAG_ECI_CALC_create_app(void)
{
  return AI_create_app_info("GeoMagCalc", APP_GEOMAG_ECI_CALC_init_, APP_GEOMAG_ECI_CALC_exec_);
}


static void APP_GEOMAG_ECI_CALC_init_(void)
{
  inertial_geomag_calulator_.calc_order = 13;
  return;
}


static void APP_GEOMAG_ECI_CALC_exec_(void)
{
  double dcm_ecef2eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_transpose_double(dcm_ecef2eci, aocs_manager->dcm_eci_to_ecef);

  float mag_i_nT_temp[PHYSICAL_CONST_THREE_DIM];
  C2A_MATH_ERROR error = GEOMAGNETISM_calc_igrf(mag_i_nT_temp,
                                                inertial_geomag_calulator_.calc_order,
                                                aocs_manager->reference_jday,
                                                dcm_ecef2eci,
                                                aocs_manager->sat_pos_est_lla_rad_m);

  if (error == C2A_MATH_ERROR_OK)
  {
    AOCS_MANAGER_set_mag_vec_est_eci_nT(mag_i_nT_temp);
  }
  else
  {
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_GEOMAG_ECI_CALCULATOR, EL_ERROR_LEVEL_LOW, (uint32_t)error);
  }

  return;
}

#pragma section
