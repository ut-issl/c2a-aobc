/**
 * @file  oem7600_parameters.h
 * @brief OEM7600に関する衛星固有パラメータを管理する
 */

#ifndef OEM7600_PARAMETERS_H_
#define OEM7600_PARAMETERS_H_

#include <src_user/Library/quaternion.h>

// Spike Filter
extern uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_accept;                   //!< Count limit to accept about position
extern uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_reject_continued_warning; //!< Count limit to reject continued warning about position
extern double OEM7600_PARAMETERS_position_spike_filter_config_reject_threshold_m;                       //!< Reject threshold about position [m]
extern double OEM7600_PARAMETERS_position_spike_filter_config_amplitude_limit_to_accept_as_step_m;      //!< Amplitude limit to accept as step input about position [m]

extern uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_accept;                   //!< Count limit to accept about velocity
extern uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_reject_continued_warning; //!< Count limit to reject continued warning about velocity
extern double OEM7600_PARAMETERS_velocity_spike_filter_config_reject_threshold_m_s;                     //!< Reject threshold about velocity [m/s]
extern double OEM7600_PARAMETERS_velocity_spike_filter_config_amplitude_limit_to_accept_as_step_m_s;    //!< Amplitude limit to accept as step input about velocity [m/s]

#endif // OEM7600_PARAMETERS_H_
