/**
 * @file  sagitta_parameters.h
 * @brief SAGITTAに関する衛星固有パラメータを管理する
 */

#ifndef SAGITTA_PARAMETERS_H_
#define SAGITTA_PARAMETERS_H_

#include <src_user/Library/physical_constants.h>
#include <stdint.h>

// Spike Filter
extern uint8_t SAGITTA_PARAMETERS_q_i2b_spike_filter_config_count_limit_to_accept;                   //!< Count limit to accept
extern uint8_t SAGITTA_PARAMETERS_q_i2b_spike_filter_config_count_limit_to_reject_continued_warning; //!< Count limit to reject continued warning
extern double SAGITTA_PARAMETERS_q_i2b_spike_filter_config_reject_threshold_rad;                     //!< Reject threshold [rad]
extern double SAGITTA_PARAMETERS_q_i2b_spike_filter_config_amplitude_limit_to_accept_as_step_rad;    //!< Amplitude limit to accept as step input [rad]

// Magnetic sensor bias calibration
extern const float SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[PHYSICAL_CONST_THREE_DIM]; //!< Magnetic sensor bias calibration

#endif // SAGITTA_PARAMETERS_H_
