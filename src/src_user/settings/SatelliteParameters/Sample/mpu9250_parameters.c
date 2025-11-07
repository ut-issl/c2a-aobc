/**
 * @file  mpu9250_parameters.c
 * @brief MPU9250に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/mpu9250_parameters.h>

// Communication port

// Magnetometer bias
// The following parameter should be tuned with magnetic experiment
const float MPU9250_PARAMETERS_mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM] = { -18417.53f, -30423.31f, 20969.18f };

// Gyro Bias and scale factor temperature calibration
const float MPU9250_PARAMETERS_temperature_range_high_degC = 60.0f;
const float MPU9250_PARAMETERS_temperature_range_low_degC = -50.0f;
// The following parameters should be tuned with temperature experiment results
const float MPU9250_PARAMETERS_bias_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { -0.024f, 0.0002f };
const float MPU9250_PARAMETERS_scale_factor_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};
const float MPU9250_PARAMETERS_bias_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 0.0058f, -0.0005f };
const float MPU9250_PARAMETERS_scale_factor_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};
const float MPU9250_PARAMETERS_bias_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 0.0179f, 0.0001f };
const float MPU9250_PARAMETERS_scale_factor_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};

// Magnetometer filter
// 1st order Low Pass Filter
const float MPU9250_PARAMETERS_mag_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM] = { 0.5f, 0.5f, 0.5f };
// Spike filter
const uint8_t MPU9250_PARAMETERS_mag_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = { 3, 3, 3 };
const uint8_t MPU9250_PARAMETERS_mag_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = { 60, 60, 60 };
const float MPU9250_PARAMETERS_mag_spike_reject_threshold_nT[PHYSICAL_CONST_THREE_DIM] = { 5000.0f, 5000.0f, 5000.0f };
const float MPU9250_PARAMETERS_mag_spike_amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM] = { 1500.0f, 1500.0f, 1500.0f };

// Gyro filter
// 1st order Low Pass Filter
const float MPU9250_PARAMETERS_gyro_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM] = { 0.05f, 0.05f, 0.05f };
// Spike filter
const uint8_t MPU9250_PARAMETERS_gyro_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = { 3, 3, 3 };
const uint8_t MPU9250_PARAMETERS_gyro_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = { 60, 60, 60 };
const float MPU9250_PARAMETERS_gyro_spike_reject_threshold_rad_s[PHYSICAL_CONST_THREE_DIM] = { 0.01f, 0.01f, 0.01f };
const float MPU9250_PARAMETERS_gyro_spike_amplitude_limit_to_accept_as_step_rad_s[PHYSICAL_CONST_THREE_DIM] = { 0.005f, 0.005f, 0.005f };
