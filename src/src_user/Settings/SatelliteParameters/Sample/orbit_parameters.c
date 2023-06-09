/**
 * @file  orbit_parameters.c
 * @brief 軌道・時刻関連の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/orbit_parameters.h>
#include <src_user/Library/physical_constants.h>

// Time
const double ORBIT_PARAMETERS_reference_jday = 2459932.000;  // 2022/12/18 12:00:00

// Orbit
// Orbit Calculator
const APP_ORBIT_CALC_METHOD ORBIT_PARAMETERS_orbit_calculation_method = APP_ORBIT_CALC_METHOD_KEPLER;
// Kepler Orbit
const double ORBIT_PARAMETERS_kepler_semi_major_axis_km = 6899.3234f;
const double ORBIT_PARAMETERS_kepler_eccentricity = 4.86396e-4f;
const double ORBIT_PARAMETERS_kepler_inclination_rad = PHYSICAL_CONST_degree_to_radian(97.50372f);
const double ORBIT_PARAMETERS_kepler_raan_rad = PHYSICAL_CONST_degree_to_radian(340.20189f);
const double ORBIT_PARAMETERS_kepler_arg_perigee_rad = PHYSICAL_CONST_degree_to_radian(221.60537f);
const double ORBIT_PARAMETERS_kepler_epoch_jday = 2.459931936719433e6;
