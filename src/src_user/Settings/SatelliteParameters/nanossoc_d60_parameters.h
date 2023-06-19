/**
 * @file  nanossoc_d60_parameters.h
 * @brief nanossoc-D60に関する衛星固有パラメータを管理する
 */

#ifndef NANOSSOC_D60_PARAMETERS_H_
#define NANOSSOC_D60_PARAMETERS_H_

#include <src_user/Library/quaternion.h>

// Frame conversion
extern const Quaternion NANOSSOC_D60_PARAMETERS_py_quaternion_c2b;  //!< Frame conversion quaternion for PY sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_my_quaternion_c2b;  //!< Frame conversion quaternion for MY sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_pz_quaternion_c2b;  //!< Frame conversion quaternion for PZ sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_mz_quaternion_c2b;  //!< Frame conversion quaternion for MZ sun sensor

// Spike Filter
extern uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_accept;                   //!< Count limit to accept
extern uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning; //!< Count limit to reject continued warning
extern float NANOSSOC_D60_PARAMETERS_spike_filter_config_reject_threshold_rad;                      //!< Reject threshold [rad]
extern float NANOSSOC_D60_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad;     //!< Amplitude limit to accept as step input [rad]

#endif // NANOSSOC_D60_PARAMETERS_H_
