/**
 * @file  attitude_control_parameters.h
 * @brief 姿勢制御関連の衛星固有パラメータを管理する
 */

#ifndef ATTITUDE_CONTROL_PARAMETERS_H_
#define ATTITUDE_CONTROL_PARAMETERS_H_

#include <stdint.h>
#include <src_user/Library/physical_constants.h>

// Bdot
extern const float ATTITUDE_CONTROL_PARAMETERS_bdot_control_gain[PHYSICAL_CONST_THREE_DIM]; //!< Control gain for B-dot
extern const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_minimum_time_derivative_step_ms;     //!< Minimum interval for magnetic field derivative [ms]
extern const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_mtq_output_time_length_ms;           //!< Keep time for MTQ ON as an actuation [ms]

#endif // ATTITUDE_CONTROL_PARAMETERS_H_
