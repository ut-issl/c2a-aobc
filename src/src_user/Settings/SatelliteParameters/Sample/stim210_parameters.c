/**
 * @file  stim210_parameters.c
 * @brief STIM210に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/stim210_parameters.h>

// Gyro constant bias
// The following parameter should be tuned with experiment
const float STIM210_PARAMETERS_ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM] = { -2.27E-04f, 2.80E-04f, -3.37E-04f };

// Gyro Bias and scale factor temperature calibration
const float STIM210_PARAMETERS_temperature_range_high_degC = 50.0f;
const float STIM210_PARAMETERS_temperature_range_low_degC = -50.0f;
// The following parameters should be tuned with temperature experiment results
const float STIM210_PARAMETERS_bias_coeff_compo_x[STIM210_PARAMETERS_kNumCoeffTempCalib] = { -1.698E-04f, 1.309E-06f };
const float STIM210_PARAMETERS_scale_factor_coeff_compo_x[STIM210_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};
const float STIM210_PARAMETERS_bias_coeff_compo_y[STIM210_PARAMETERS_kNumCoeffTempCalib] = { 2.990E-04, -6.060E-07f };
const float STIM210_PARAMETERS_scale_factor_coeff_compo_y[STIM210_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};
const float STIM210_PARAMETERS_bias_coeff_compo_z[STIM210_PARAMETERS_kNumCoeffTempCalib] = { -2.306E-04f, -6.303E-07f };
const float STIM210_PARAMETERS_scale_factor_coeff_compo_z[STIM210_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};

// Gyro filter
// 1st order Low Pass Filter
const float STIM210_PARAMETERS_gyro_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM] = { 1.0f, 1.0f, 1.0f };
// Spike filter
const uint8_t STIM210_PARAMETERS_gyro_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = { 3, 3, 3 };
const uint8_t STIM210_PARAMETERS_gyro_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = { 60, 60, 60 };
const float STIM210_PARAMETERS_gyro_spike_reject_threshold_rad_s[PHYSICAL_CONST_THREE_DIM] = { 0.01f, 0.01f, 0.01f };
const float STIM210_PARAMETERS_gyro_spike_amplitude_limit_to_accept_as_step_rad_s[PHYSICAL_CONST_THREE_DIM] = { 0.002f, 0.002f, 0.002f };
