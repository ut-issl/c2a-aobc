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
extern const float MPU9250_PARAMETERS_temperature_range_high_degC;                                        //!< Temperature range high [degC]
extern const float MPU9250_PARAMETERS_temperature_range_low_degC;                                         //!< Temperature range low [degC]
extern const float MPU9250_PARAMETERS_bias_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib];          //!< Bias coefficients @ component frame X axis
extern const float MPU9250_PARAMETERS_scale_factor_coeff_compo_x[MPU9250_PARAMETERS_kNumCoeffTempCalib];  //!< Scale Factor coefficients @ component frame X axis
extern const float MPU9250_PARAMETERS_bias_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib];          //!< Bias coefficients @ component frame Y axis
extern const float MPU9250_PARAMETERS_scale_factor_coeff_compo_y[MPU9250_PARAMETERS_kNumCoeffTempCalib];  //!< Scale Factor coefficients @ component frame Y axis
extern const float MPU9250_PARAMETERS_bias_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib];          //!< Bias coefficients @ component frame Z axis
extern const float MPU9250_PARAMETERS_scale_factor_coeff_compo_z[MPU9250_PARAMETERS_kNumCoeffTempCalib];  //!< Scale Factor coefficients @ component frame Z axis

// Magnetometer filter
// 1st order Low Pass Filter
extern const float MPU9250_PARAMETERS_mag_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM]; //!< Cut off frequency for magnetometer filter [Hz]
// Spike filter
extern const uint8_t MPU9250_PARAMETERS_mag_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                    //!< Count limit to accept
extern const uint8_t MPU9250_PARAMETERS_mag_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM];  //!< Count limit to reject continued warning
extern const float MPU9250_PARAMETERS_mag_spike_reject_threshold_nT[PHYSICAL_CONST_THREE_DIM];                         //!< Reject threshold [nT]
extern const float MPU9250_PARAMETERS_mag_spike_amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM];        //!< Amplitude limit to accept as step input [nT]

// Gyro filter
// 1st order Low Pass Filter
extern const float MPU9250_PARAMETERS_gyro_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM]; //!< Cut off frequency for magnetometer filter [Hz]
// Spike filter
extern const uint8_t MPU9250_PARAMETERS_gyro_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                   //!< Count limit to accept
extern const uint8_t MPU9250_PARAMETERS_gyro_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM]; //!< Count limit to reject continued warning
extern const float MPU9250_PARAMETERS_gyro_spike_reject_threshold_rad_s[PHYSICAL_CONST_THREE_DIM];                    //!< Reject threshold [rad/s]
extern const float MPU9250_PARAMETERS_gyro_spike_amplitude_limit_to_accept_as_step_rad_s[PHYSICAL_CONST_THREE_DIM];   //!< Amplitude limit to accept as step input [rad/s]

#endif // MPU9250_PARAMETERS_H_
