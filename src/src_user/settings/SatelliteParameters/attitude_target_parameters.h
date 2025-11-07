/**
 * @file  attitude_target_parameters.h
 * @brief 姿勢目標の衛星固有パラメータを管理する
 */

#ifndef ATTITUDE_TARGET_PARAMETERS_H_
#define ATTITUDE_TARGET_PARAMETERS_H_

#include <src_user/Applications/UserDefined/AOCS/TargetAttitude/target_attitude_calculator.h>
#include <src_user/Applications/UserDefined/AOCS/TargetAttitude/target_attitude_from_orbit.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

// Target Calculation Mode
extern const APP_TARGET_ATT_CALC_MODE ATTITUDE_TARGET_PARAMETERS_mode; //!< Attitude target mode

// Manual mode target
extern const Quaternion ATTITUDE_TARGET_PARAMETERS_quaternion_target_i2t;  //!< Target Quaternion

// Target calculation from orbit
// Main target
extern const APP_TAFO_TARGET_DIRECITON ATTITUDE_TARGET_PARAMETERS_main_target_direction;          //!< Main target
extern const float ATTITUDE_TARGET_PARAMETERS_vec_to_main_target_body[PHYSICAL_CONST_THREE_DIM];  //!< Main direction at body frame
// Sub target
extern const APP_TAFO_TARGET_DIRECITON ATTITUDE_TARGET_PARAMETERS_sub_target_direction;           //!< Sub target
extern const float ATTITUDE_TARGET_PARAMETERS_vec_to_sub_target_body[PHYSICAL_CONST_THREE_DIM];   //!< Sub direction at body frame
// Offset angle
extern const MATRIX33_ROTATION_AXIS ATTITUDE_TARGET_PARAMETERS_offset_angle_axis; //!< Offset angle axis
extern const float ATTITUDE_TARGET_PARAMETERS_offset_angle_rad;                   //!< Offset angle [m]
// Target on earth surface
extern const float ATTITUDE_TARGET_PARAMETERS_target_lla_rad_m[PHYSICAL_CONST_THREE_DIM];  //!< Target as latitude [rad], longitude [rad], altitude [m]

#endif // ATTITUDE_TARGET_PARAMETERS_H_
