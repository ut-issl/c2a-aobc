#pragma section REPRO
/**
* @file   kepler_orbit_propagator.c
* @brief  ケプラー軌道の伝搬を行うアプリ
*/

#include "kepler_orbit_propagator.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/Library/print.h>
#include "../aocs_manager.h"
#include "../../../../Library/matrix33.h"
#include "../../../../Library/time_space.h"
#include <string.h>

static KeplerOrbitPropagator        kepler_orbit_propagator_;
const  KeplerOrbitPropagator* const kepler_orbit_propagator = &kepler_orbit_propagator_;

static void APP_KOP_init_(void);
static void APP_KOP_exec_(void);

AppInfo APP_KOP_create_app(void)
{
  return AI_create_app_info("KeplerOrbitPropagator", APP_KOP_init_, APP_KOP_exec_);
}

static void APP_KOP_init_(void)
{
  memset(&kepler_orbit_propagator_, 0x00, sizeof(KeplerOrbitPropagator));

  kepler_orbit_propagator_.orbital_elements.semi_major_axis_km = 6899.3234f;
  kepler_orbit_propagator_.orbital_elements.eccentricity = 4.86396e-4f;
  kepler_orbit_propagator_.orbital_elements.inclination_rad = PHYSICAL_CONST_degree_to_radian(97.50372f);
  kepler_orbit_propagator_.orbital_elements.raan_rad = PHYSICAL_CONST_degree_to_radian(340.20189f);
  kepler_orbit_propagator_.orbital_elements.arg_perigee_rad = PHYSICAL_CONST_degree_to_radian(221.60537f);
  kepler_orbit_propagator_.orbital_elements.epoch_jday = 2.459931936719433e6;

  KEPLER_ORBIT_init_constants(&kepler_orbit_propagator_.orbit_constants,
                               kepler_orbit_propagator_.orbital_elements);

  return;
}


static void APP_KOP_exec_(void)
{
  double current_time_jday = aocs_manager->reference_jday;  // 基準時刻更新は別アプリで一括で行うことを想定

  float position_eci_km[PHYSICAL_CONST_THREE_DIM];
  float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM];
  KEPLER_ORBIT_calc_position(position_eci_km,
                             velocity_eci_km_s,
                             kepler_orbit_propagator_.orbit_constants,
                             current_time_jday);

  double position_eci_m[PHYSICAL_CONST_THREE_DIM];
  double velocity_eci_m_s[PHYSICAL_CONST_THREE_DIM];
  for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
  {
    position_eci_m[idx]   = (double)position_eci_km[idx] * 1000.0;
    velocity_eci_m_s[idx] = (double)velocity_eci_km_s[idx] * 1000.0;
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

CCP_CmdRet Cmd_APP_KOP_SET_ORBIT_ELEMENTS(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;

  float semi_major_axis_km;
  ENDIAN_memcpy(&semi_major_axis_km, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  if (semi_major_axis_km < PHYSICAL_CONST_EARTH_RADIUS_km) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float eccentricity;
  ENDIAN_memcpy(&eccentricity, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  if (eccentricity < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (eccentricity > 1.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float inclination_rad;
  ENDIAN_memcpy(&inclination_rad, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  inclination_rad = C2A_MATH_normalize_zero_2pi(inclination_rad);

  float raan_rad;
  ENDIAN_memcpy(&raan_rad, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  raan_rad = C2A_MATH_normalize_zero_2pi(raan_rad);

  float arg_perigee_rad;
  ENDIAN_memcpy(&arg_perigee_rad, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  arg_perigee_rad = C2A_MATH_normalize_zero_2pi(arg_perigee_rad);

  double epoch_jday;
  ENDIAN_memcpy(&epoch_jday, param + param_id, sizeof(double));
  param_id += (int)sizeof(double);
  if (epoch_jday < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  kepler_orbit_propagator_.orbital_elements_cmd_tmp.semi_major_axis_km = semi_major_axis_km;
  kepler_orbit_propagator_.orbital_elements_cmd_tmp.inclination_rad = inclination_rad;
  kepler_orbit_propagator_.orbital_elements_cmd_tmp.eccentricity = eccentricity;
  kepler_orbit_propagator_.orbital_elements_cmd_tmp.raan_rad = raan_rad;
  kepler_orbit_propagator_.orbital_elements_cmd_tmp.arg_perigee_rad = arg_perigee_rad;
  kepler_orbit_propagator_.orbital_elements_cmd_tmp.epoch_jday = epoch_jday;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_KOP_SAVE_ORBIT_ELEMENTS(const CommonCmdPacket* packet)
{
  if (kepler_orbit_propagator_.orbital_elements_cmd_tmp.semi_major_axis_km < PHYSICAL_CONST_EARTH_RADIUS_km)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }
  if (kepler_orbit_propagator_.orbital_elements_cmd_tmp.eccentricity < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  if (kepler_orbit_propagator_.orbital_elements_cmd_tmp.eccentricity > 1.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  if (kepler_orbit_propagator_.orbital_elements_cmd_tmp.epoch_jday < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  kepler_orbit_propagator_.orbital_elements = kepler_orbit_propagator_.orbital_elements_cmd_tmp;

  KEPLER_ORBIT_init_constants(&kepler_orbit_propagator_.orbit_constants, kepler_orbit_propagator_.orbital_elements);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


#pragma section
