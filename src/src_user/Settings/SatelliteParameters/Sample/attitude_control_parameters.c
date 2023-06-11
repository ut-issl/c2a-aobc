/**
 * @file  attitude_control_parameters.c
 * @brief 姿勢制御関連の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/attitude_control_parameters.h>

// Bdor
const float ATTITUDE_CONTROL_PARAMETERS_bdot_control_gain[PHYSICAL_CONST_THREE_DIM] = { -0.1f, -0.1f, -0.1f };
const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_minimum_time_derivative_step_ms = 100;
const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_mtq_output_time_length_ms = 1000;
