/**
 * @file  nanossoc_d60_parameters.c
 * @brief nanossoc-D60に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/nanossoc_d60_parameters.h>
#include <src_user/Settings/SatelliteParameters/parameter_setting_macro.h>

// Frame conversion
const Quaternion NANOSSOC_D60_PARAMETERS_quaternion_c2b[NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR] = { 0.70710665f, 0.0f, 0.0f, 0.70710665f,     // IDX 0
                                                                                                              0.0f, -0.70710665f, 0.70710665f, 0.0f,    // IDX 1
                                                                                                              0.0f, 0.0f, 0.70710665f, 0.70710665f,     // IDX 2
                                                                                                              0.707106471f, -0.707106471f, 0.0f, 0.0f}; // IDX 3

// Spike Filter
uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_accept = 10;
uint8_t NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning = 60;
float NANOSSOC_D60_PARAMETERS_spike_filter_config_reject_threshold_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(3);
float NANOSSOC_D60_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(180);

// Sun intensity threshold
float NANOSSOC_D60_PARAMETERS_sun_intensity_lower_threshold_percent = 80.0f;
float NANOSSOC_D60_PARAMETERS_sun_intensity_upper_threshold_percent = 120.0f;
