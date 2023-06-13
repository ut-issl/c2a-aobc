/**
 * @file  mpu9250_parameters.c
 * @brief MPU9250に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/mpu9250_parameters.h>

// Communication port

// Magnetometer bias
const float MPU9250_PARAMETERS_mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM] = { -18417.53f, -30423.31f, 20969.18f };

// Gyro Bias and scale factor temperature calibration
const float MPU9250_PARAMETERS_temperature_range_high_degC = 60.0f;
const float MPU9250_PARAMETERS_temperature_range_low_degC = -50.0f;

const float MPU9250_PARAMETERS_bias_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { -0.024f, 0.0002f };
const float MPU9250_PARAMETERS_scale_factor_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};
const float MPU9250_PARAMETERS_bias_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 0.0058f, -0.0005f };
const float MPU9250_PARAMETERS_scale_factor_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};
const float MPU9250_PARAMETERS_bias_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 0.0179f, 0.0001f };
const float MPU9250_PARAMETERS_scale_factor_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib] = { 1.0f, 0.0f};
