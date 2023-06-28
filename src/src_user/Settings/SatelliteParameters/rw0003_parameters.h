/**
 * @file  rw0003_parameters.h
 * @brief RW0.003に関する衛星固有パラメータを管理する
 */

#ifndef RW0003_PARAMETERS_H_
#define RW0003_PARAMETERS_H_

#include <src_user/Library/physical_constants.h>
#include <stdint.h>

// Spike Filter
extern uint8_t RW0003_PARAMETERS_spike_filter_config_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM];                   //!< Count limit to accept
extern uint8_t RW0003_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM]; //!< Count limit to reject continued warning
extern double RW0003_PARAMETERS_spike_filter_config_reject_threshold_rad_s[PHYSICAL_CONST_THREE_DIM];                       //!< Reject threshold [rad/s]
extern double RW0003_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad_s[PHYSICAL_CONST_THREE_DIM];      //!< Amplitude limit to accept as step input [rad/s]

#endif // RW0003_PARAMETERS_H_
