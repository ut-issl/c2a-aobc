/**
 * @file  oem7600_parameters.c
 * @brief OEM7600に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/oem7600_parameters.h>

// Spike Filter
uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = {3, 3, 3};
uint8_t OEM7600_PARAMETERS_position_spike_filter_config_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = {60, 60, 60};
double OEM7600_PARAMETERS_position_spike_filter_config_reject_threshold_m[PHYSICAL_CONST_THREE_DIM] = {10000.0, 10000.0, 10000.0};
double OEM7600_PARAMETERS_position_spike_filter_config_amplitude_limit_to_accept_as_step_m[PHYSICAL_CONST_THREE_DIM] = {8000.0, 8000.0, 8000.0};

uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = {3, 3, 3};
uint8_t OEM7600_PARAMETERS_velocity_spike_filter_config_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = {60, 60, 60};
double OEM7600_PARAMETERS_velocity_spike_filter_config_reject_threshold_m_s[PHYSICAL_CONST_THREE_DIM] = {1000.0, 1000.0, 1000.0};
double OEM7600_PARAMETERS_velocity_spike_filter_config_amplitude_limit_to_accept_as_step_m_s[PHYSICAL_CONST_THREE_DIM] = {100.0, 100.0, 100.0};

uint8_t OEM7600_PARAMETERS_total_gps_time_spike_filter_config_count_limit_to_accept = 5;
uint8_t OEM7600_PARAMETERS_total_gps_time_spike_filter_config_count_limit_to_reject_continued_warning = 60;
double OEM7600_PARAMETERS_total_gps_time_spike_filter_config_reject_threshold_s = 3000.0;
double OEM7600_PARAMETERS_total_gps_time_spike_filter_config_amplitude_limit_to_accept_as_step_s = 3000.0;
