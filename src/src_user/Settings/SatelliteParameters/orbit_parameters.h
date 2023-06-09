/**
 * @file  orbit_parameters.h
 * @brief 軌道・時刻関連の衛星固有パラメータを管理する
 */

#ifndef ORBIT_PARAMETERS_H_
#define ORBIT_PARAMETERS_H_

#include <src_user/Applications/UserDefined/AOCS/OrbitEstimation/orbit_calculator.h>

// Time
extern const double ORBIT_PARAMETERS_reference_jday; //!< Reference time for orbit calculation [Julian day]

// Orbit
// Orbit Calculator
extern const APP_ORBIT_CALC_METHOD ORBIT_PARAMETERS_orbit_calculation_method; //!< Orbit calculation method
// Kepler Orbit
extern const double ORBIT_PARAMETERS_kepler_semi_major_axis_km; //!< Semi-major axis for Kepler Propagator [km]
extern const double ORBIT_PARAMETERS_kepler_eccentricity;       //!< Eccentricity for Kepler Propagator
extern const double ORBIT_PARAMETERS_kepler_inclination_rad;    //!< Inclination for Kepler Propagator [rad]
extern const double ORBIT_PARAMETERS_kepler_raan_rad;           //!< RAAN (Right Ascension of Ascending Node) for Kepler Propagator [rad]
extern const double ORBIT_PARAMETERS_kepler_arg_perigee_rad;    //!< Argment of perigee for Kepler Propagator [rad]
extern const double ORBIT_PARAMETERS_kepler_epoch_jday;         //!< Epoch for Kepler Propagator [Julian day]

#endif // ORBIT_PARAMETERS_H_
