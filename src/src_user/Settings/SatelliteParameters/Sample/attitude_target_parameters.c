/**
 * @file  attitude_target_parameters.c
 * @brief 姿勢目標の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/attitude_target_parameters.h>

// Target Calculation Mode
const APP_TARGET_ATT_CALC_MODE ATTITUDE_TARGET_PARAMETERS_mode = APP_TARGET_ATT_CALC_MODE_MANUAL;

// Manual mode target
const Quaternion ATTITUDE_TARGET_PARAMETERS_quaternion_target_i2t = {0.5f, 0.5f, 0.5f, 0.5f};

