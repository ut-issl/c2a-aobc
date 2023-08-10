/**
 * @file  nanossoc_d60_parameters.h
 * @brief nanossoc-D60に関する衛星固有パラメータを管理する
 */

#ifndef NANOSSOC_D60_PARAMETERS_H_
#define NANOSSOC_D60_PARAMETERS_H_

#include <src_user/Library/quaternion.h>

// Number of Sun sensor
#define NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR (6)  //!< Number of mounted sun sensor ( <= 6)

// Use user defined sensor number
#ifdef USER_DEFINED_SENSOR_NUMBER
#include <Settings/user_defined_sensor_number.h>
#endif

// Frame conversion
extern const Quaternion NANOSSOC_D60_PARAMETERS_py_quaternion_c2b;  //!< Frame conversion quaternion for PY sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_my_quaternion_c2b;  //!< Frame conversion quaternion for MY sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_pz_quaternion_c2b;  //!< Frame conversion quaternion for PZ sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_mz_quaternion_c2b;  //!< Frame conversion quaternion for MZ sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_px_quaternion_c2b;  //!< Frame conversion quaternion for PX sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_mx_quaternion_c2b;  //!< Frame conversion quaternion for MX sun sensor

// Spike Filter
extern uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_accept;                   //!< Count limit to accept
extern uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning; //!< Count limit to reject continued warning
extern float NANOSSOC_D60_PARAMETERS_spike_filter_config_reject_threshold_rad;                      //!< Reject threshold [rad]
extern float NANOSSOC_D60_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad;     //!< Amplitude limit to accept as step input [rad]

// Sun intensity threshold
extern float NANOSSOC_D60_PARAMETERS_sun_intensity_lower_threshold_percent; //!< Sun intensity lower threshold [%]
extern float NANOSSOC_D60_PARAMETERS_sun_intensity_upper_threshold_percent; //!< Sun intensity upper threshold [%]

#endif // NANOSSOC_D60_PARAMETERS_H_
