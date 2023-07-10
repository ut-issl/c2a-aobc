/**
 * @file  rw0003_parameters.c
 * @brief RW.0003に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/rw0003_parameters.h>

// Spike Filter
uint8_t RW0003_PARAMETERS_spike_filter_config_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = {3, 3, 3};
uint8_t RW0003_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = {10, 10, 10};
double RW0003_PARAMETERS_spike_filter_config_reject_threshold_rad_s[PHYSICAL_CONST_THREE_DIM] = {50.0, 50.0, 50.0};
double RW0003_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad_s[PHYSICAL_CONST_THREE_DIM] = {25.0, 25.0, 25.0};
