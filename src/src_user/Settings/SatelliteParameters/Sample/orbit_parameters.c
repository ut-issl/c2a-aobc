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
// TLE for SGP4
const double ORBIT_PARAMETERS_tle_epoch_year = 22;
const double ORBIT_PARAMETERS_tle_epoch_day = 352.00000000;
const double ORBIT_PARAMETERS_tle_b_star = 0.0;
const double ORBIT_PARAMETERS_tle_inclination_deg = 97.5068f;
const double ORBIT_PARAMETERS_tle_raan_deg = 339.7118f;
const double ORBIT_PARAMETERS_tle_eccentricity = 0.0011775f;
const double ORBIT_PARAMETERS_tle_arg_perigee_deg = 245.9837f;
const double ORBIT_PARAMETERS_tle_mean_anomaly_deg = 114.0163f;
const double ORBIT_PARAMETERS_tle_mean_motion_rpd = 15.15782335f;
