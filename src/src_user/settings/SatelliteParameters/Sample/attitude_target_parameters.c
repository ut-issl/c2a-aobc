/**
 * @file  attitude_target_parameters.c
 * @brief 姿勢目標の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/attitude_target_parameters.h>
#include <src_user/Settings/SatelliteParameters/parameter_setting_macro.h>

// Target Calculation Mode
const APP_TARGET_ATT_CALC_MODE ATTITUDE_TARGET_PARAMETERS_mode = APP_TARGET_ATT_CALC_MODE_MANUAL;

// Manual mode target
const Quaternion ATTITUDE_TARGET_PARAMETERS_quaternion_target_i2t = { 0.5f, 0.5f, 0.5f, 0.5f };

// Target calculation from orbit
// Main target
extern const APP_TAFO_TARGET_DIRECITON ATTITUDE_TARGET_PARAMETERS_main_target_direction = APP_TAFO_TARGET_DIRECITON_SUN;
extern const float ATTITUDE_TARGET_PARAMETERS_vec_to_main_target_body[PHYSICAL_CONST_THREE_DIM] = { 1.0f, 0.0f, 0.0f };
// Sub target
extern const APP_TAFO_TARGET_DIRECITON ATTITUDE_TARGET_PARAMETERS_sub_target_direction = APP_TAFO_TARGET_DIRECITON_EARTH_CENTER;
extern const float ATTITUDE_TARGET_PARAMETERS_vec_to_sub_target_body[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 1.0f, 0.0f };
// Offset angle
extern const MATRIX33_ROTATION_AXIS ATTITUDE_TARGET_PARAMETERS_offset_angle_axis = MATRIX33_ROTATION_AXIS_X;
extern const float ATTITUDE_TARGET_PARAMETERS_offset_angle_rad = 0.0f;
// Target on earth surface
extern const float ATTITUDE_TARGET_PARAMETERS_target_lla_rad_m[PHYSICAL_CONST_THREE_DIM] = { PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(35.7130f),
                                                                                             PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(139.7596f),
                                                                                             23.0f };
