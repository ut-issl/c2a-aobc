/**
 * @file  orbit_parameters.c
 * @brief 軌道・時刻関連の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/orbit_parameters.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Settings/SatelliteParameters/parameter_setting_macro.h>

// Time
const double ORBIT_PARAMETERS_reference_jday = 2459932.000;  // 2022/12/18 12:00:00

// Orbit Calculator
const APP_ORBIT_CALC_METHOD ORBIT_PARAMETERS_orbit_calculation_method = APP_ORBIT_CALC_METHOD_SGP4;

// Kepler Orbit
const float ORBIT_PARAMETERS_kepler_semi_major_axis_km = 6899.3234f;
const float ORBIT_PARAMETERS_kepler_eccentricity = 4.86396e-4f;
const float ORBIT_PARAMETERS_kepler_inclination_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(97.50372f);
const float ORBIT_PARAMETERS_kepler_raan_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(340.20189f);
const float ORBIT_PARAMETERS_kepler_arg_perigee_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(221.60537f);
const double ORBIT_PARAMETERS_kepler_epoch_jday = 2.459931936719433e6;

// TLE for SGP4
// 1st line
const uint8_t ORBIT_PARAMETERS_tle_epoch_year = 20;
const double ORBIT_PARAMETERS_tle_epoch_day = 76.51604214;
const float ORBIT_PARAMETERS_tle_b_star = 0.0;
// 2nd line
const float ORBIT_PARAMETERS_tle_inclination_deg = 51.6412f;
const float ORBIT_PARAMETERS_tle_raan_deg = 86.9962f;
const float ORBIT_PARAMETERS_tle_eccentricity = 0.0006063f;
const float ORBIT_PARAMETERS_tle_arg_perigee_deg = 30.9353f;
const float ORBIT_PARAMETERS_tle_mean_anomaly_deg = 329.2153f;
const float ORBIT_PARAMETERS_tle_mean_motion_rpd = 15.49228202f;

// GPS-R Orbit Propagator
// Initial orbital elements
const float ORBIT_PARAMETERS_gpsr_semi_major_axis_km = 6899.3234f;
const float ORBIT_PARAMETERS_gpsr_eccentricity = 4.86396e-4f;
const float ORBIT_PARAMETERS_gpsr_inclination_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(97.50372f);
const float ORBIT_PARAMETERS_gpsr_raan_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(340.20189f);
const float ORBIT_PARAMETERS_gpsr_arg_perigee_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(0.0f);
const double ORBIT_PARAMETERS_gpsr_epoch_jday = 2.459931936719433e6;
// Threshold
const float ORBIT_PARAMETERS_gpsr_threshold_semi_major_axis_km = 50.0f;
const float ORBIT_PARAMETERS_gpsr_threshold_eccentricity = 0.1f;
const float ORBIT_PARAMETERS_gpsr_threshold_inclination_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(20.0f);
const float ORBIT_PARAMETERS_gpsr_threshold_raan_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(20.0f);
const float ORBIT_PARAMETERS_gpsr_threshold_arg_perigee_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(50.0f);
const double ORBIT_PARAMETERS_gpsr_threshold_epoch_jday = 0.1;
