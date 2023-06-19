/**
 * @file  rm3100_parameters.h
 * @brief RM3100に関する衛星固有パラメータを管理する
 */

#ifndef RM3100_PARAMETERS_H_
#define RM3100_PARAMETERS_H_

#include <stdint.h>
#include <src_user/Applications/DriverInstances/di_mpu9250.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/quaternion.h>

// AOBC RM3100
// Magnetometer bias
extern const float RM3100_PARAMETERS_mag_aobc_bias_compo_nT[PHYSICAL_CONST_THREE_DIM];  //!< Magnetic field observation bias for AOBC RM3100 @ component frame [nT]
// Magnetometer filter
// 1st order Low Pass Filter
extern const float RM3100_PARAMETERS_mag_aobc_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM]; //!< Cut off frequency for magnetometer filter [Hz]
// Spike filter
extern const uint8_t RM3100_PARAMETERS_mag_aobc_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                    //!< Count limit to accept
extern const uint8_t RM3100_PARAMETERS_mag_aobc_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM];  //!< Count limit to reject continued warning
extern const float RM3100_PARAMETERS_mag_aobc_spike_reject_threshold_nT[PHYSICAL_CONST_THREE_DIM];                         //!< Reject threshold [nT]
extern const float RM3100_PARAMETERS_mag_aobc_spike_amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM];        //!< Amplitude limit to accept as step input [nT]


// External RM3100
// Frame conversion
extern const Quaternion RM3100_PARAMETERS_mag_ext_quaternion_c2b;  //!< Frame conversion Quaternion from component to body frame
// Magnetometer bias
extern const float RM3100_PARAMETERS_mag_ext_bias_compo_nT[PHYSICAL_CONST_THREE_DIM];   //!< Magnetic field observation bias for external RM3100 @ component frame [nT]
// Magnetometer filter
// 1st order Low Pass Filter
extern const float RM3100_PARAMETERS_mag_ext_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM]; //!< Cut off frequency for magnetometer filter [Hz]
// Spike filter
extern const uint8_t RM3100_PARAMETERS_mag_ext_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                    //!< Count limit to accept
extern const uint8_t RM3100_PARAMETERS_mag_ext_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM];  //!< Count limit to reject continued warning
extern const float RM3100_PARAMETERS_mag_ext_spike_reject_threshold_nT[PHYSICAL_CONST_THREE_DIM];                         //!< Reject threshold [nT]
extern const float RM3100_PARAMETERS_mag_ext_spike_amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM];        //!< Amplitude limit to accept as step input [nT]

#endif // RM3100_PARAMETERS_H_
