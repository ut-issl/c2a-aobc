/**
 * @file  oem7600_parameters.h
 * @brief OEM7600に関する衛星固有パラメータを管理する
*/

#ifndef OEM7600_PARAMETERS_H_
#define OEM7600_PARAMETERS_H_

#include <src_user/Library/quaternion.h>

// Frame conversion: GPS-Rについては無し

// Spike Filter
extern uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_accept;
extern uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_reject_continued_warning;
extern double OEM7600_PARAMETERS_position_spike_filter_config_reject_threshold_m;
extern double OEM7600_PARAMETERS_position_spike_filter_config_amplitude_limit_to_accept_as_step_m;

extern uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_accept;
extern uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_reject_continued_warning;
extern double OEM7600_PARAMETERS_velocity_spike_filter_config_reject_threshold_m_s;
extern double OEM7600_PARAMETERS_velocity_spike_filter_config_amplitude_limit_to_accept_as_step_m_s;


#endif // OEM7600_PARAMETERS_H_
