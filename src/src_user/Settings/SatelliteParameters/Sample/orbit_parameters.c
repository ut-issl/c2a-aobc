/**
 * @file  orbit_parameters.c
 * @brief 軌道・時刻関連の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/orbit_parameters.h>

// Time
const double ORBIT_PARAMETERS_reference_jday = 2459932.000;  // 2022/12/18 12:00:00

// Orbit
const APP_ORBIT_CALC_METHOD ORBIT_PARAMETERS_orbit_calculation_method = APP_ORBIT_CALC_METHOD_KEPLER;
