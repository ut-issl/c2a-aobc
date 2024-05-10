/**
 * @file  stim210_parameters.h
 * @brief STIM210に関する衛星固有パラメータを管理する
 */

#ifndef STIM210_PARAMETERS_H_
#define STIM210_PARAMETERS_H_

#include <stdint.h>
#include <src_user/Library/physical_constants.h>

// Constant value
#define STIM210_PARAMETERS_kNumCoeffTempCalib (2)  //!< Number of coefficient for calibration. This value cannot be changed since it relates Tlm/Cmd definition.

// Gyro Bias and scale factor temperature calibration
extern const float STIM210_PARAMETERS_temperature_range_high_degC;                                       //!< Temperature range high [degC]
extern const float STIM210_PARAMETERS_temperature_range_low_degC;                                        //!< Temperature range low [degC]
extern const float STIM210_PARAMETERS_bias_coeff_compo_x[STIM210_PARAMETERS_kNumCoeffTempCalib];         //!< Bias coefficients @ component frame X axis
extern const float STIM210_PARAMETERS_scale_factor_coeff_compo_x[STIM210_PARAMETERS_kNumCoeffTempCalib]; //!< Scale Factor coefficients @ component frame X axis
extern const float STIM210_PARAMETERS_bias_coeff_compo_y[STIM210_PARAMETERS_kNumCoeffTempCalib];         //!< Bias coefficients @ component frame Y axis
extern const float STIM210_PARAMETERS_scale_factor_coeff_compo_y[STIM210_PARAMETERS_kNumCoeffTempCalib]; //!< Scale Factor coefficients @ component frame Y axis
extern const float STIM210_PARAMETERS_bias_coeff_compo_z[STIM210_PARAMETERS_kNumCoeffTempCalib];         //!< Bias coefficients @ component frame Z axis
extern const float STIM210_PARAMETERS_scale_factor_coeff_compo_z[STIM210_PARAMETERS_kNumCoeffTempCalib]; //!< Scale Factor coefficients @ component frame Z axis

// Gyro filter
// 1st order Low Pass Filter
extern const float STIM210_PARAMETERS_gyro_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM]; //!< Cut off frequency for gyro filter [Hz]
// Spike filter
extern const uint8_t STIM210_PARAMETERS_gyro_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                   //!< Count limit to accept
extern const uint8_t STIM210_PARAMETERS_gyro_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM]; //!< Count limit to reject continued warning
extern const float STIM210_PARAMETERS_gyro_spike_reject_threshold_rad_s[PHYSICAL_CONST_THREE_DIM];                    //!< Reject threshold [rad/s]
extern const float STIM210_PARAMETERS_gyro_spike_amplitude_limit_to_accept_as_step_rad_s[PHYSICAL_CONST_THREE_DIM];   //!< Amplitude limit to accept as step input [rad/s]

// Magnetic sensor bias calibration
extern const float STIM210_PARAMETERS_mag_bias_compo[PHYSICAL_CONST_THREE_DIM]; //!< Bias @ component frame X axis

#endif // STIM210_PARAMETERS_H_
