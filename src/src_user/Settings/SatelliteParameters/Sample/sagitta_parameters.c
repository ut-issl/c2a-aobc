/**
 * @file  sagitta_parameters.c
 * @brief Sagittaに関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/sagitta_parameters.h>
#include <src_user/Settings/SatelliteParameters/parameter_setting_macro.h>

// Spike Filter
uint8_t SAGITTA_PARAMETERS_q_i2b_spike_filter_config_count_limit_to_accept = 20;
uint8_t SAGITTA_PARAMETERS_q_i2b_spike_filter_config_count_limit_to_reject_continued_warning = 200;
double SAGITTA_PARAMETERS_q_i2b_spike_filter_config_reject_threshold_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(1.0f);
double SAGITTA_PARAMETERS_q_i2b_spike_filter_config_amplitude_limit_to_accept_as_step_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(0.6f);

// Magnetic sensor bias calibration
const float SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[PHYSICAL_CONST_THREE_DIM] = {71.50f, 70.20f, -14.3f}; //!< Magnetic sensor bias calibration

// Sun intensity threshold
