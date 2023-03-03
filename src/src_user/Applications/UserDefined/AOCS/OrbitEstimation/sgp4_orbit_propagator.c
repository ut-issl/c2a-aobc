#pragma section REPRO
/**
* @file   sgp4_orbit_propagator.c
* @brief  SGP4軌道伝搬を行うアプリ
*/

#include "sgp4_orbit_propagator.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../../app_registry.h"

#include "../aocs_manager.h"
#include "../../../../Library/matrix33.h"
#include "../../../../Library/time_space.h"
#include <string.h>

// #define APP_SGP4_DEBUG_OUTPUT //!< デバッグ出力を表示させたいときに定義する

static Sgp4OrbitPropagator        sgp4_orbit_propagator_;
const  Sgp4OrbitPropagator* const sgp4_orbit_propagator = &sgp4_orbit_propagator_;

static void APP_SGP4_init_(void);
static void APP_SGP4_exec_(void);

AppInfo APP_SGP4_create_app(void)
{
  return AI_create_app_info("Sgp4OrbitPropagator", APP_SGP4_init_, APP_SGP4_exec_);
}

static void APP_SGP4_init_(void)
{
  memset(&sgp4_orbit_propagator_, 0x00, sizeof(Sgp4OrbitPropagator));

  // 1行目
  sgp4_orbit_propagator_.tle_input.epoch_year = 22;
  sgp4_orbit_propagator_.tle_input.epoch_day = 352.00000000;
  sgp4_orbit_propagator_.tle_input.b_star = 0.0f;
  // 2行目
  sgp4_orbit_propagator_.tle_input.inclination_deg = 97.5068f;
  sgp4_orbit_propagator_.tle_input.raan_deg = 339.7118f;
  sgp4_orbit_propagator_.tle_input.eccentricity = 0.0011775f;
  sgp4_orbit_propagator_.tle_input.arg_perigee_deg = 245.9837f;
  sgp4_orbit_propagator_.tle_input.mean_anomaly_deg = 114.0163f;
  sgp4_orbit_propagator_.tle_input.mean_motion_rpd = 15.15782335f;

#ifdef APP_SGP4_DEBUG_OUTPUT
  uint32_t start_time_ms = (uint32_t)TMGR_get_master_total_cycle_in_msec();
#endif
  C2A_MATH_ERROR ret = SGP4_initialize(&sgp4_orbit_propagator_.constants,
                                        sgp4_orbit_propagator_.tle_input);
#ifdef APP_SGP4_DEBUG_OUTPUT
  uint32_t end_time_ms = (uint32_t)TMGR_get_master_total_cycle_in_msec();
  uint32_t calc_time_ms = end_time_ms - start_time_ms;
  Printf("SGP4 initialize time ! %d [ms] \n", calc_time_ms);
#endif

  if (ret != C2A_MATH_ERROR_OK)
  {
    Printf("SGP4 initialize error: %d.\n", ret);
  }

  return;
}


static void APP_SGP4_exec_(void)
{
  float position_eci_km[PHYSICAL_CONST_THREE_DIM];
  float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM];

  double current_jday = aocs_manager->reference_jday;  // 基準時刻更新は別アプリで一括で行うことを想定

#ifdef APP_SGP4_DEBUG_OUTPUT
  uint32_t start_time_ms = (uint32_t)TMGR_get_master_total_cycle_in_msec();
#endif
  C2A_MATH_ERROR ret = SGP4_calc_orbit(position_eci_km, velocity_eci_km_s,
                                       &(sgp4_orbit_propagator_.constants), current_jday);
#ifdef APP_SGP4_DEBUG_OUTPUT
  uint32_t end_time_ms = (uint32_t)TMGR_get_master_total_cycle_in_msec();
  uint32_t calc_time_ms = end_time_ms - start_time_ms;
  Printf("SGP4 execution time ! %d [ms] \n", calc_time_ms);
#endif

  if (ret != C2A_MATH_ERROR_OK)
  {
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_SGP4_ORBIT_PROPAGATOR, EL_ERROR_LEVEL_LOW, (uint32_t)ret);
    return;
  }
  double position_eci_m[PHYSICAL_CONST_THREE_DIM];
  double velocity_eci_m_s[PHYSICAL_CONST_THREE_DIM];
  for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
  {
    position_eci_m[idx]   = (double)position_eci_km[idx] * 1000.0;
    velocity_eci_m_s[idx] = (double)velocity_eci_km_s[idx] * 1000.0;
  }

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


CCP_EXEC_STS Cmd_APP_SGP4_SET_TLE_LINE1(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;

  uint8_t epoch_year;
  endian_memcpy(&epoch_year, param + param_id, sizeof(uint8_t));
  param_id += (int)sizeof(uint8_t);

  double epoch_day;
  endian_memcpy(&epoch_day, param + param_id, sizeof(double));
  param_id += (int)sizeof(double);
  if (epoch_day < 1.0) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (epoch_day > 367.0) return CCP_EXEC_ILLEGAL_PARAMETER;

  float b_star;
  endian_memcpy(&b_star, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  if (b_star < 0.0) return CCP_EXEC_ILLEGAL_PARAMETER;

  sgp4_orbit_propagator_.tle_input_cmd_tmp.epoch_day = epoch_day;
  sgp4_orbit_propagator_.tle_input_cmd_tmp.epoch_year = epoch_year;
  sgp4_orbit_propagator_.tle_input_cmd_tmp.b_star = b_star;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_SGP4_SET_TLE_LINE2(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;

  float inclination_deg;
  endian_memcpy(&inclination_deg, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);

  float raan_deg;
  endian_memcpy(&raan_deg, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);

  float eccentricity;
  endian_memcpy(&eccentricity, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  if (eccentricity < 0.0) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (eccentricity > 1.0) return CCP_EXEC_ILLEGAL_PARAMETER;

  float arg_perigee_deg;
  endian_memcpy(&arg_perigee_deg, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);

  float mean_anomaly_deg;
  endian_memcpy(&mean_anomaly_deg, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);

  float mean_motion_rpd;
  endian_memcpy(&mean_motion_rpd, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  if (mean_motion_rpd < 0.0) return CCP_EXEC_ILLEGAL_PARAMETER;

  sgp4_orbit_propagator_.tle_input_cmd_tmp.inclination_deg = inclination_deg;
  sgp4_orbit_propagator_.tle_input_cmd_tmp.raan_deg = raan_deg;
  sgp4_orbit_propagator_.tle_input_cmd_tmp.eccentricity = eccentricity;
  sgp4_orbit_propagator_.tle_input_cmd_tmp.arg_perigee_deg = arg_perigee_deg;
  sgp4_orbit_propagator_.tle_input_cmd_tmp.mean_anomaly_deg = mean_anomaly_deg;
  sgp4_orbit_propagator_.tle_input_cmd_tmp.mean_motion_rpd = mean_motion_rpd;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_SGP4_SAVE_TLE(const CommonCmdPacket* packet)
{
  if (sgp4_orbit_propagator_.tle_input_cmd_tmp.epoch_day < 1.0) return CCP_EXEC_ILLEGAL_CONTEXT;
  if (sgp4_orbit_propagator_.tle_input_cmd_tmp.epoch_day > 367.0) return CCP_EXEC_ILLEGAL_CONTEXT;
  if (sgp4_orbit_propagator_.tle_input_cmd_tmp.b_star < 0.0) return CCP_EXEC_ILLEGAL_CONTEXT;
  if (sgp4_orbit_propagator_.tle_input_cmd_tmp.eccentricity < 0.0) return CCP_EXEC_ILLEGAL_CONTEXT;
  if (sgp4_orbit_propagator_.tle_input_cmd_tmp.eccentricity > 1.0) return CCP_EXEC_ILLEGAL_CONTEXT;
  if (sgp4_orbit_propagator_.tle_input_cmd_tmp.mean_motion_rpd < 0.0) return CCP_EXEC_ILLEGAL_CONTEXT;


  C2A_MATH_ERROR ret = SGP4_initialize(&sgp4_orbit_propagator_.constants,
                                       sgp4_orbit_propagator_.tle_input_cmd_tmp);
  if (ret != C2A_MATH_ERROR_OK)
  {
    SGP4_initialize(&sgp4_orbit_propagator_.constants, sgp4_orbit_propagator_.tle_input);
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }

  sgp4_orbit_propagator_.tle_input = sgp4_orbit_propagator_.tle_input_cmd_tmp;

  return CCP_EXEC_SUCCESS;
}


#pragma section
