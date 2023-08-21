#pragma section REPRO
/**
* @file   gpsr_orbit_propagator.c
* @brief  GPSRテレメを用いてケプラー軌道の伝搬を行うアプリ
*/

#include "gpsr_orbit_propagator.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/Library/print.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "kepler_orbit_propagator.h"
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Library/math_constants.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/time_space.h>
#include <string.h>

// SatelliteParameters
#include <src_user/Settings/SatelliteParameters/orbit_parameters.h>

static GpsrOrbitPropagator        gpsr_orbit_propagator_;
const  GpsrOrbitPropagator* const gpsr_orbit_propagator = &gpsr_orbit_propagator_;

static void APP_GPSROP_init_(void);
static void APP_GPSROP_exec_(void);

/**
 * @brief GPSRテレメを用いて計算した軌道要素定数がそれまでの定数に対して一定の範囲に収まっていることを確認する
 * @param[in] orbital_elements_in  : GPSRテレメを用いて計算したケプラー運動軌道計算における定数値
 * @param[in] orbital_elements_in  : 変更前のケプラー運動軌道計算における定数値
 * @return    C2A_MATH_ERRORに準ずる
 */
static C2A_MATH_ERROR GPSR_ORBIT_PROPAGATOR_verify_orbital_elements_(const KeplerOrbitalElements* after, const KeplerOrbitalElements* before);

AppInfo APP_GPSROP_create_app(void)
{
  return AI_create_app_info("GpsrOrbitPropagator", APP_GPSROP_init_, APP_GPSROP_exec_);
}

static void APP_GPSROP_init_(void)
{
  memset(&gpsr_orbit_propagator_, 0x00, sizeof(GpsrOrbitPropagator));

  gpsr_orbit_propagator_.orbital_elements.semi_major_axis_km = ORBIT_PARAMETERS_gpsr_semi_major_axis_km;
  gpsr_orbit_propagator_.orbital_elements.eccentricity = ORBIT_PARAMETERS_gpsr_eccentricity;
  gpsr_orbit_propagator_.orbital_elements.inclination_rad = ORBIT_PARAMETERS_gpsr_inclination_rad;
  gpsr_orbit_propagator_.orbital_elements.raan_rad = ORBIT_PARAMETERS_gpsr_raan_rad;
  gpsr_orbit_propagator_.orbital_elements.arg_perigee_rad = ORBIT_PARAMETERS_gpsr_arg_perigee_rad;
  gpsr_orbit_propagator_.orbital_elements.epoch_jday = ORBIT_PARAMETERS_gpsr_epoch_jday;

  gpsr_orbit_propagator_.orbital_elements_threshold.semi_major_axis_km = ORBIT_PARAMETERS_gpsr_threshold_semi_major_axis_km;
  gpsr_orbit_propagator_.orbital_elements_threshold.eccentricity = ORBIT_PARAMETERS_gpsr_threshold_eccentricity;
  gpsr_orbit_propagator_.orbital_elements_threshold.inclination_rad = ORBIT_PARAMETERS_gpsr_threshold_inclination_rad;
  gpsr_orbit_propagator_.orbital_elements_threshold.raan_rad = ORBIT_PARAMETERS_gpsr_threshold_raan_rad;
  gpsr_orbit_propagator_.orbital_elements_threshold.arg_perigee_rad =ORBIT_PARAMETERS_gpsr_threshold_arg_perigee_rad;
  gpsr_orbit_propagator_.orbital_elements_threshold.epoch_jday = ORBIT_PARAMETERS_gpsr_threshold_epoch_jday;

  KEPLER_ORBIT_init_constants(&gpsr_orbit_propagator_.orbit_constants,
                               gpsr_orbit_propagator_.orbital_elements);

  return;
}


static void APP_GPSROP_exec_(void)
{
  double current_time_jday = aocs_manager->reference_jday;  // 基準時刻更新は別アプリで一括で行うことを想定

  float position_eci_km[PHYSICAL_CONST_THREE_DIM];
  float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM];

  GPSR_ORBIT_PROPAGATOR_calc_position(position_eci_km,
                                      velocity_eci_km_s,
                                      current_time_jday);

  double position_eci_m[PHYSICAL_CONST_THREE_DIM];
  double velocity_eci_m_s[PHYSICAL_CONST_THREE_DIM];
  for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
  {
    position_eci_m[idx]   = (double)position_eci_km[idx] * 1000.0f;
    velocity_eci_m_s[idx] = (double)velocity_eci_km_s[idx] * 1000.0f;
  }

  AOCS_MANAGER_set_sat_pos_vel_est_eci_m(position_eci_m, velocity_eci_m_s);

  // ECEF座標変換
  double position_ecef_m[PHYSICAL_CONST_THREE_DIM];
  double velocity_ecef_m_s[PHYSICAL_CONST_THREE_DIM];
  MATRIX33_multiply_matrix_vector_double(position_ecef_m,
                                       aocs_manager->dcm_eci_to_ecef,
                                       position_eci_m);
  MATRIX33_multiply_matrix_vector_double(velocity_ecef_m_s,
                                       aocs_manager->dcm_eci_to_ecef,
                                       velocity_eci_m_s);

  AOCS_MANAGER_set_sat_pos_vel_est_ecef_m(position_ecef_m, velocity_ecef_m_s);

  // LLA座標変換
  double lla_rad_m[PHYSICAL_CONST_THREE_DIM];
  TIME_SPACE_convert_ecef_to_lla(lla_rad_m, position_ecef_m);

  AOCS_MANAGER_set_sat_pos_est_lla_rad_m(lla_rad_m);

  return;
}

C2A_MATH_ERROR GPSR_ORBIT_PROPAGATOR_calc_position(float position_eci_km[PHYSICAL_CONST_THREE_DIM],
                                                   float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                                                   const double current_time_jday)
{
  C2A_MATH_ERROR ret = C2A_MATH_ERROR_OTHERS;

  if (aocs_manager->gps_visibility == AOCS_MANAGER_GPS_VISIBLE)
  {
    // gpsrテレメをもとに接触軌道要素を計算
    float position_eci_km_tmp[PHYSICAL_CONST_THREE_DIM];
    float velocity_eci_km_s_tmp[PHYSICAL_CONST_THREE_DIM];
    double reference_jday = TIME_SPACE_convert_gpstime_to_julian_day(aocs_manager->current_gps_time_obs);
    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      position_eci_km_tmp[idx]   = (float)aocs_manager->sat_pos_obs_eci_m[idx]   / 1000.0f;
      velocity_eci_km_s_tmp[idx] = (float)aocs_manager->sat_vel_obs_eci_m_s[idx] / 1000.0f;
    }

    KEPLER_ORBIT_calc_oe_from_pos_vel(&(gpsr_orbit_propagator_.orbital_elements),
                                      position_eci_km_tmp,
                                      velocity_eci_km_s_tmp,
                                      reference_jday);

    // ケプラー軌道伝播における接触軌道要素と比較し、一定の範囲内であればgpsrテレメをもとに軌道要素を更新
    KeplerOrbitalElements orbital_elements_tmp;
    KEPLER_ORBIT_calc_position(position_eci_km_tmp,
                               velocity_eci_km_s_tmp,
                               kepler_orbit_propagator->orbit_constants,
                               reference_jday);
    KEPLER_ORBIT_calc_oe_from_pos_vel(&orbital_elements_tmp,
                                      position_eci_km_tmp,
                                      velocity_eci_km_s_tmp,
                                      reference_jday);
    ret = GPSR_ORBIT_PROPAGATOR_verify_orbital_elements_(&(gpsr_orbit_propagator->orbital_elements), &orbital_elements_tmp);

    if (ret == C2A_MATH_ERROR_OK)
    {
      KEPLER_ORBIT_init_constants(&gpsr_orbit_propagator_.orbit_constants, gpsr_orbit_propagator->orbital_elements);
    }
  }

  KEPLER_ORBIT_calc_position(position_eci_km,
                             velocity_eci_km_s,
                             gpsr_orbit_propagator_.orbit_constants,
                             current_time_jday);

  return ret;
}

static C2A_MATH_ERROR GPSR_ORBIT_PROPAGATOR_verify_orbital_elements_(const KeplerOrbitalElements* after, const KeplerOrbitalElements* before)
{
  C2A_MATH_ERROR ret;
  ret = C2A_MATH_check_abs_range_violation(after->semi_major_axis_km - before->semi_major_axis_km,
                                           gpsr_orbit_propagator_.orbital_elements_threshold.semi_major_axis_km);
  if (ret != C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_RANGE_OVER;
  ret = C2A_MATH_check_abs_range_violation(after->eccentricity - before->eccentricity,
                                           gpsr_orbit_propagator_.orbital_elements_threshold.eccentricity);
  if (ret != C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_RANGE_OVER;
  ret = C2A_MATH_check_abs_range_violation(after->inclination_rad - before->inclination_rad,
                                           gpsr_orbit_propagator_.orbital_elements_threshold.inclination_rad);
  if (ret != C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_RANGE_OVER;
  ret = C2A_MATH_check_abs_range_violation(after->raan_rad - before->raan_rad,
                                           gpsr_orbit_propagator_.orbital_elements_threshold.raan_rad);
  if (ret != C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_RANGE_OVER;
  ret = C2A_MATH_check_abs_range_violation(after->arg_perigee_rad - before->arg_perigee_rad,
                                           gpsr_orbit_propagator_.orbital_elements_threshold.arg_perigee_rad);
  if (ret != C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_RANGE_OVER;
  // TODO_L epoch_jdayの比較

  return C2A_MATH_ERROR_OK;
}

CCP_CmdRet Cmd_APP_GPSROP_SET_ORBIT_ELEMENTS_THRESHOLD(const CommonCmdPacket* packet)
{
  uint8_t oe_id = CCP_get_param_from_packet(packet, 0, uint8_t);
  double  value = CCP_get_param_from_packet(packet, 1, double);

  switch (oe_id)
  {
  case 0:
    if (value < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    gpsr_orbit_propagator_.orbital_elements_threshold.semi_major_axis_km = (float)value;
    break;
  case 1:
    if (value < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if (value > 1.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    gpsr_orbit_propagator_.orbital_elements_threshold.eccentricity = (float)value;
    break;
  case 2:
    if (value < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if (value > MATH_CONST_PI_DOUBLE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    gpsr_orbit_propagator_.orbital_elements_threshold.inclination_rad = (float)value;
    break;
  case 3:
    if (value < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if (value > MATH_CONST_2PI_DOUBLE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    gpsr_orbit_propagator_.orbital_elements_threshold.raan_rad = (float)value;
    break;
  case 4:
    if (value < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    if (value > MATH_CONST_2PI_DOUBLE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    gpsr_orbit_propagator_.orbital_elements_threshold.arg_perigee_rad = (float)value;
    break;
  case 5:
    if (value < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    gpsr_orbit_propagator_.orbital_elements_threshold.epoch_jday = value;
    break;
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
