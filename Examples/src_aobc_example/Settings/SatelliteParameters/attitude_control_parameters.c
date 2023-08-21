/**
 * @file  attitude_control_parameters.c
 * @brief 姿勢制御関連の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/attitude_control_parameters.h>
#include <src_user/Settings/SatelliteParameters/parameter_setting_macro.h>

// Bdot
const float ATTITUDE_CONTROL_PARAMETERS_bdot_control_gain[PHYSICAL_CONST_THREE_DIM] = { -0.1f, -0.1f, -0.1f };
const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_minimum_time_derivative_step_ms = 100;
const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_mtq_output_time_length_ms = 1000;

// Sun Pointing
const SUN_POINTING_AXIS_ID ATTITUDE_CONTROL_PARAMETERS_sun_pointing_axis_id = SUN_POINTING_AXIS_ID_Z;
// Gain
const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_x = { 5.0e-5f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_y = { 10.0e-5f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_z = { 0.0f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_x = { 1.0e-2f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_y = { 2.0e-2f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_z = { 7.0e-3f, 0.0f, 2.0e-2f };
// MTQ control settings
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_direct_feedback_angle_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(20.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_direct_feedback_rate_rad_s = 1.6e-3f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_mtq_allowable_error_ratio_transient = 0.6f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_correction_gain_transient = 0.0f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_mtq_allowable_error_ratio_stable = 0.8f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_correction_gain_stable = 0.25f;
// Integral control setting
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_integral_angle_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(20.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_angle_to_run_integral_control_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(30.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_integral_control_permission_angle_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(40.0f);
// Spin control setting
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_acceptable_angle_error_to_spin_up_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(30.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_spin_rate_around_sun_rad_s = 1.4e-2f;
// Low Pass Filter setting
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_sample_freq_Hz = 10.0f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_Hz = 0.10f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_damping_factor = 1.0f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_spin_axis_Hz = 0.03f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_spin_rate_cutoff_freq_Hz = 5e-4f;

// Three Axis Control with MTQ
// Gain
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_x = { 1.2e-4f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_y = { 2.0f * 1.2e-4f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_z = { 1.5f * 1.2e-4f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_x = { 2.5e-2f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_y = { 2.0f * 2.5e-2f, 0.0f, 0.0f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_z = { 1.5f * 2.5e-2f, 0.0f, 0.0f };
// MTQ control settings
// 下記の値はISS軌道を想定した値, 変更時は，HW側の出力最大値にかからない様に留意しながら，ゲインと組み合わせて調整すること．
const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_direct_feedback_angle_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(18.0f);
const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_direct_feedback_rate_rad_s = 5.0e-3f;
const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_mtq_allowable_error_ratio_transient = 0.6f;
const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_correction_gain_transient = 0.0f;
const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_mtq_allowable_error_ratio_stable = 0.6f;
const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_correction_gain_stable = 0.1f;
const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_acceptable_angle_error_as_stable_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(20.0f);
// Integral control setting
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_integral_angle_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(40.0f);
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_angle_to_run_integral_control_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(30.0f);
// Output torque Low Pass Filter(LPF) setting
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_sample_freq_Hz = 10.0f;
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_trq_cutoff_freq_Hz = 0.3f;
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_trq_damping_factor = 1.0f;

// Three Axis Control with RW
// Gain
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_gains_body_x = { 1.0e-3f, 0.0f, 1.0e-3f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_gains_body_y = { 2.0f * 1.0e-3f, 0.0f, 2.0f * 1.0e-3f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_gains_body_z = { 2.0f * 1.0e-3f, 0.0f, 2.0f * 1.0e-3f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_rate_gains_body_x = { 7.0e-2f, 0.0f, 2.0e-3f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_rate_gains_body_y = { 2.0f * 7.0e-2f, 0.0f, 2.0f * 2.0e-3f };
const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_rate_gains_body_z = { 2.0f * 7.0e-2f, 0.0f, 2.0f * 2.0e-3f };

// Unloading
const float ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_upper_threshold_rad_s[AOCS_MANAGER_NUM_OF_RW] =
    {PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(7000.0f),
     PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(7000.0f),
     PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(7000.0f)};
const float ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_lower_threshold_rad_s[AOCS_MANAGER_NUM_OF_RW] =
    {PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(-7000.0f),
     PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(-7000.0f),
     PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(-7000.0f)};
const float ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_target_rad_s[AOCS_MANAGER_NUM_OF_RW] =
    {PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(0.0f),
     PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(0.0f),
     PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(0.0f)};
const float ATTITUDE_CONTROL_PARAMETERS_unloading_control_gain = -1.0e-7f;
const APP_UNLOADING_EXEC ATTITUDE_CONTROL_PARAMETERS_unloading_exec_is_enable = APP_UNLOADING_EXEC_DISABLE;
const float ATTITUDE_CONTROL_PARAMETERS_unloading_minimum_torque_Nm = 1.0e-7f;

// Control Torques
const AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION ATTITUDE_CONTROL_PARAMETERS_constant_torque_permission = AOCS_MANAGER_CONSTANT_TORQUE_DISABLE;
const float ATTITUDE_CONTROL_PARAMETERS_constant_torque_body_Nm[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };
const float ATTITUDE_CONTROL_PARAMETERS_internal_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM] = { 5.0e-3f, 5.0e-3f, 5.0e-3f };
const float ATTITUDE_CONTROL_PARAMETERS_external_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM] = { 3.0e-4f, 3.0e-4f, 3.0e-4f };

// Target Setting parameters
const float ATTITUDE_CONTROL_PARAMETERS_limit_angular_velocity_rad_s = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(0.8f);
const float ATTITUDE_CONTROL_PARAMETERS_limit_maneuver_angle_rad = PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(30.0f);
