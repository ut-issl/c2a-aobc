/**
 * @file  nanossoc_d60_parameters.c
 * @brief nanossoc-D60に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/nanossoc_d60_parameters.h>

// Frame conversion
const Quaternion NANOSSOC_D60_PARAMETERS_py_quaternion_c2b = { 0.70710665f, 0.0f, 0.0f, 0.70710665f };
const Quaternion NANOSSOC_D60_PARAMETERS_my_quaternion_c2b = { 0.0f, -0.70710665f, 0.70710665f, 0.0f };
const Quaternion NANOSSOC_D60_PARAMETERS_pz_quaternion_c2b = { 0.0f, 0.0f, 0.70710665f, 0.70710665f };
const Quaternion NANOSSOC_D60_PARAMETERS_mz_quaternion_c2b = { -0.707106471f, 0.707106471f, 0.0f, 0.0f };

// Spike Filter
uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_accept = 10;
uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning = 60;
float NANOSSOC_D60_PARAMETERS_spike_filter_config_reject_threshold_rad = PHYSICAL_CONST_degree_to_radian(3);
float NANOSSOC_D60_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad = PHYSICAL_CONST_degree_to_radian(180);

// Sun intensity threshold
float NANOSSOC_D60_PARAMETERS_sun_intensity_lower_threshold_percent = 80.0f;
float NANOSSOC_D60_PARAMETERS_sun_intensity_upper_threshold_percent = 120.0f;