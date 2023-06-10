/**
 * @file  attitude_target_parameters.h
 * @brief 姿勢目標の衛星固有パラメータを管理する
 */

#ifndef ATTITUDE_TARGET_PARAMETERS_H_
#define ATTITUDE_TARGET_PARAMETERS_H_

#include <src_user/Applications/UserDefined/AOCS/TargetAttitude/target_attitude_calculator.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

// Target Calculation Mode
extern const APP_TARGET_ATT_CALC_MODE ATTITUDE_TARGET_PARAMETERS_mode; //!< Attitude target mode

// Manual mode target
extern const Quaternion ATTITUDE_TARGET_PARAMETERS_quaternion_target_i2t;  //!< Target Quaternion

#endif // ATTITUDE_TARGET_PARAMETERS_H_
