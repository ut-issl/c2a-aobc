/**
 * @file  oem7600_parameters.h
 * @brief OEM7600に関する衛星固有パラメータを管理する
 */

#ifndef OEM7600_PARAMETERS_H_
#define OEM7600_PARAMETERS_H_

#include <src_user/Library/quaternion.h>

// Spike Filter
extern uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                   //!< Count limit to accept about position
extern uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM]; //!< Count limit to reject continued warning about position
extern double OEM7600_PARAMETERS_position_spike_filter_config_reject_threshold_m[PHYSICAL_CONST_THREE_DIM];                       //!< Reject threshold about position [m]
extern double OEM7600_PARAMETERS_position_spike_filter_config_amplitude_limit_to_accept_as_step_m[PHYSICAL_CONST_THREE_DIM];      //!< Amplitude limit to accept as step input about position [m]

extern uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                   //!< Count limit to accept about velocity
extern uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM]; //!< Count limit to reject continued warning about velocity
extern double OEM7600_PARAMETERS_velocity_spike_filter_config_reject_threshold_m_s[PHYSICAL_CONST_THREE_DIM];                     //!< Reject threshold about velocity [m/s]
extern double OEM7600_PARAMETERS_velocity_spike_filter_config_amplitude_limit_to_accept_as_step_m_s[PHYSICAL_CONST_THREE_DIM];    //!< Amplitude limit to accept as step input about velocity [m/s]

extern uint8_t OEM7600_PARAMETERS_gps_time_total_sec_spike_filter_config_count_limit_to_accept;                   //!< Count limit to accept about gpstime
extern uint8_t OEM7600_PARAMETERS_gps_time_total_sec_spike_filter_config_count_limit_to_reject_continued_warning; //!< Count limit to reject continued warning about gpstime
extern double OEM7600_PARAMETERS_gps_time_total_sec_spike_filter_config_reject_threshold_s;                      //!< Reject threshold about gpstime [ms]
extern double OEM7600_PARAMETERS_gps_time_total_sec_spike_filter_config_amplitude_limit_to_accept_as_step_s;     //!< Amplitude limit to accept as step input about gpstime [ms]

#endif // OEM7600_PARAMETERS_H_
