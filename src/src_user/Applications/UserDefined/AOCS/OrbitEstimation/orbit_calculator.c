#pragma section REPRO
/**
* @file   orbit_calculator.c
* @brief  軌道計算(伝搬・推定)をまとめるアプリ
*/

#include "orbit_calculator.h"

#include <string.h> // for memcpy

#include "kepler_orbit_propagator.h"
#include "sgp4_orbit_propagator.h"
#include "gpsr_orbit_propagator.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../../app_registry.h"
#include <src_core/Library/print.h>
#include "../aocs_manager.h"
#include "../../../../Library/vector3.h"
#include "../../../../Library/matrix33.h"
#include "../../../../Library/time_space.h"

// SatelliteParameters
#include "../../../../Settings/SatelliteParameters/orbit_parameters.h"

static OrbitCalculator        orbit_calculator_;
const  OrbitCalculator* const orbit_calculator = &orbit_calculator_;

static void APP_ORBIT_CALC_init_(void);
static void APP_ORBIT_CALC_exec_(void);

AppInfo APP_ORBIT_CALC_create_app(void)
{
  return AI_create_app_info("OrbitCalculator", APP_ORBIT_CALC_init_, APP_ORBIT_CALC_exec_);
}

static void APP_ORBIT_CALC_init_(void)
{
  orbit_calculator_.method = ORBIT_PARAMETERS_orbit_calculation_method;
  orbit_calculator_.gpsr_update = APP_ORBIT_CALC_GPSR_UPDATE_DISABLE;
  memset(&(orbit_calculator_.position_gpsr_kepler_eci_km), 0x00, sizeof(orbit_calculator_.position_gpsr_kepler_eci_km));
  memset(&(orbit_calculator_.velocity_gpsr_kepler_eci_km_s), 0x00, sizeof(orbit_calculator_.velocity_gpsr_kepler_eci_km_s));
  return;
}

static void APP_ORBIT_CALC_exec_(void)
{
  double current_time_jday = aocs_manager->reference_jday;  // 基準時刻更新は別アプリで一括で行うことを想定

  float position_eci_km[PHYSICAL_CONST_THREE_DIM];
  float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM];
  C2A_MATH_ERROR ret = C2A_MATH_ERROR_OK;
  C2A_MATH_ERROR ret_gpsr = C2A_MATH_ERROR_OK;

  // 各手法をコール
  switch (orbit_calculator_.method)
  {
  case APP_ORBIT_CALC_METHOD_KEPLER:
    ret = KEPLER_ORBIT_calc_position(position_eci_km,
                                     velocity_eci_km_s,
                                     kepler_orbit_propagator->orbit_constants,
                                     current_time_jday);
    break;
  case APP_ORBIT_CALC_METHOD_SGP4:
    ret = SGP4_calc_orbit(position_eci_km,
                          velocity_eci_km_s,
                          &(sgp4_orbit_propagator->constants),
                          current_time_jday);
    break;
  case APP_ORBIT_CALC_METHOD_KEPLER_GPSR:
    // Keplerで伝播。GPSRテレメが軌道要素更新に使えない時のため。 TODO_L 検証により不必要となれば削除する。
    ret = KEPLER_ORBIT_calc_position(position_eci_km,
                                     velocity_eci_km_s,
                                     kepler_orbit_propagator->orbit_constants,
                                     current_time_jday);

    // GPSRを用いたKeplerで伝播
    ret_gpsr = GPSR_ORBIT_PROPAGATOR_calc_position(orbit_calculator_.position_gpsr_kepler_eci_km,
                                                   orbit_calculator_.velocity_gpsr_kepler_eci_km_s,
                                                   current_time_jday);

    if (orbit_calculator->gpsr_update == APP_ORBIT_CALC_GPSR_UPDATE_ENABLE && ret_gpsr == C2A_MATH_ERROR_OK)
    {
      VECTOR3_copy(position_eci_km, orbit_calculator_.position_gpsr_kepler_eci_km);
      VECTOR3_copy(velocity_eci_km_s, orbit_calculator_.velocity_gpsr_kepler_eci_km_s);
      ret = ret_gpsr;
    }
    break;
  case APP_ORBIT_CALC_METHOD_GPSR_EKF:
    // TODO_L 実装する
    break;
  default:
    // Not Reached
    break;
  }

  // retのエラー処理
  if (ret != C2A_MATH_ERROR_OK)
  {
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_ORBIT_CALCULATOR, EL_ERROR_LEVEL_LOW, (uint32_t)ret);
    // エラー登録して、値更新はしない
    return;
  }

  // 共通手順
  // 単位変換
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
  TIME_SPACE_convert_posvel_eci_to_ecef(position_ecef_m, velocity_ecef_m_s, aocs_manager->dcm_eci_to_ecef,
                                        position_eci_m, velocity_eci_m_s);
  AOCS_MANAGER_set_sat_pos_vel_est_ecef_m(position_ecef_m, velocity_ecef_m_s);

  // LLA座標変換
  double lla_rad_m[PHYSICAL_CONST_THREE_DIM];
  TIME_SPACE_convert_ecef_to_lla(lla_rad_m, position_ecef_m);
  AOCS_MANAGER_set_sat_pos_est_lla_rad_m(lla_rad_m);

  return;
}

CCP_CmdRet Cmd_APP_ORBIT_CALC_SET_METHOD(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  APP_ORBIT_CALC_METHOD method;

  method = (APP_ORBIT_CALC_METHOD)param[0];
  if (method >= APP_ORBIT_CALC_METHOD_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  orbit_calculator_.method = method;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_ORBIT_CALC_SET_GPSR_UPDATE(const CommonCmdPacket* packet)
{
  APP_ORBIT_CALC_GPSR_UPDATE gpsr_update = (APP_ORBIT_CALC_GPSR_UPDATE)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (gpsr_update >= APP_ORBIT_CALC_GPSR_UPDATE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  orbit_calculator_.gpsr_update = gpsr_update;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
