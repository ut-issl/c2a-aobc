/**
 * @file  mpu9250_parameters.h
 * @brief MPU9250に関する衛星固有パラメータを管理する
 */

#ifndef MPU9250_PARAMETERS_H_
#define MPU9250_PARAMETERS_H_

#include <stdint.h>
#include <src_user/Applications/DriverInstances/di_mpu9250.h>
#include <src_user/Library/physical_constants.h>

// Constant value
const uint8_t MPU9250_PARAMETERS_kNumCoeffTempCalib = 2; //!< Number of coefficient for calibration. This value cannot be changed since it relates Tlm/Cmd definition.

// Communication port

// Magnetometer bias
extern const float MPU9250_PARAMETERS_mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM]; //!< Magnetic field observation bias @ component frame [nT]


// Gyro Bias and scale factor temperature calibration
extern const float MPU9250_PARAMETERS_temperature_range_high_degC;  //!< Temperature range high [degC]
extern const float MPU9250_PARAMETERS_temperature_range_low_degC;   //!< Temperature range low [degC]

extern const float MPU9250_PARAMETERS_bias_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib];          //!< Bias coefficients @ component frame X axis
extern const float MPU9250_PARAMETERS_scale_factor_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib];  //!< Scale Factor coefficients @ component frame X axis
extern const float MPU9250_PARAMETERS_bias_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib];          //!< Bias coefficients @ component frame Y axis
extern const float MPU9250_PARAMETERS_scale_factor_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib];  //!< Scale Factor coefficients @ component frame Y axis
extern const float MPU9250_PARAMETERS_bias_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib];          //!< Bias coefficients @ component frame Z axis
extern const float MPU9250_PARAMETERS_scale_factor_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib];  //!< Scale Factor coefficients @ component frame Z axis

#endif // MPU9250_PARAMETERS_H_
