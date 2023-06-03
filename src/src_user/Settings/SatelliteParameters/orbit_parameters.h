/**
 * @file  orbit_parameters.h
 * @brief 軌道・時刻関連の衛星固有パラメータを管理する
 */

#include <src_user/Applications/UserDefined/AOCS/OrbitEstimation/orbit_calculator.h>

// Time
extern const double ORBIT_PARAMETERS_reference_jday; //!< Reference time for orbit calculation [Julian day]

// Orbit
extern const APP_ORBIT_CALC_METHOD ORBIT_PARAMETERS_orbit_calculation_method; //!< Orbit calculation method
