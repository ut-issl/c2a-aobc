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
// TLE for SGP4
// 1st line
extern const double ORBIT_PARAMETERS_tle_epoch_year;        //!< Epoch year in TLE 1st line for SGP4 Propagator
extern const double ORBIT_PARAMETERS_tle_epoch_day;         //!< Epoch day in TLE 1st line for SGP4 Propagator
extern const double ORBIT_PARAMETERS_tle_b_star;            //!< B-star in TLE 1st line for SGP4 Propagator
// 2nd line
extern const double ORBIT_PARAMETERS_tle_inclination_deg;   //!< Inclination in TLE 2nd line for SGP4 Propagator [deg]
extern const double ORBIT_PARAMETERS_tle_raan_deg;          //!< RAAN (Right Ascension of Ascending Node) in TLE 2nd line for SGP4 Propagator [deg]
extern const double ORBIT_PARAMETERS_tle_eccentricity;      //!< Eccentricity in TLE 2nd line for SGP4 Propagator [-]
extern const double ORBIT_PARAMETERS_tle_arg_perigee_deg;   //!< Argment of perigee in TLE 2nd line for SGP4 Propagator [deg]
extern const double ORBIT_PARAMETERS_tle_mean_anomaly_deg;  //!< Mean anomaly in TLE 2nd line for SGP4 Propagator [deg]
extern const double ORBIT_PARAMETERS_tle_mean_motion_rpd;   //!< Mean motion in TLE 2nd line for SGP4 Propagator [RPD]
// GPS-R Orbit Propagator
// Initial orbital elements
extern const double ORBIT_PARAMETERS_gpsr_semi_major_axis_km; //!< Semi-major axis for GPS-R Orbit Propagator [km]
extern const double ORBIT_PARAMETERS_gpsr_eccentricity;       //!< Eccentricity for GPS-R Orbit Propagator
extern const double ORBIT_PARAMETERS_gpsr_inclination_rad;    //!< Inclination for GPS-R Orbit Propagator [rad]
extern const double ORBIT_PARAMETERS_gpsr_raan_rad;           //!< RAAN (Right Ascension of Ascending Node) for GPS-R Orbit Propagator [rad]
extern const double ORBIT_PARAMETERS_gpsr_arg_perigee_rad;    //!< Argment of perigee for GPS-R Orbit Propagator [rad]
extern const double ORBIT_PARAMETERS_gpsr_epoch_jday;         //!< Epoch for GPS-R Orbit Propagator [Julian day]
// Threshold
extern const double ORBIT_PARAMETERS_gpsr_threshold_semi_major_axis_km; //!< Semi-major axis threshold for GPS-R Orbit Propagator [km]
extern const double ORBIT_PARAMETERS_gpsr_threshold_eccentricity;       //!< Eccentricity threshold for GPS-R Orbit Propagator
extern const double ORBIT_PARAMETERS_gpsr_threshold_inclination_rad;    //!< Inclination threshold for GPS-R Orbit Propagator [rad]
extern const double ORBIT_PARAMETERS_gpsr_threshold_raan_rad;           //!< RAAN (Right Ascension of Ascending Node) threshold for GPS-R Orbit Propagator [rad]
extern const double ORBIT_PARAMETERS_gpsr_threshold_arg_perigee_rad;    //!< Argment of perigee threshold for GPS-R Orbit Propagator [rad]
extern const double ORBIT_PARAMETERS_gpsr_threshold_epoch_jday;         //!< Epoch threshold for GPS-R Orbit Propagator [Julian day]


#endif // ORBIT_PARAMETERS_H_
