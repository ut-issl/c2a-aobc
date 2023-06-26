/**
 * @file  oem7600_parameters.c
 * @brief OEM7600に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/oem7600_parameters.h>

// Frame conversion: GPS-Rについては無し

// Spike Filter
uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_accept = 3;
uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_reject_continued_warning = 60;
double OEM7600_PARAMETERS_position_spike_filter_config_reject_threshold_m = 10000.0;
double OEM7600_PARAMETERS_position_spike_filter_config_amplitude_limit_to_accept_as_step_m = 8000.0;

uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_accept = 3;
uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_reject_continued_warning = 60;
double OEM7600_PARAMETERS_velocity_spike_filter_config_reject_threshold_m_s = 1000.0;
double OEM7600_PARAMETERS_velocity_spike_filter_config_amplitude_limit_to_accept_as_step_m_s = 100.0;

// Sun intensity threshold
