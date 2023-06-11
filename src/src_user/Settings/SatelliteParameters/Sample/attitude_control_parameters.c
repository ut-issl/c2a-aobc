/**
 * @file  attitude_control_parameters.c
 * @brief 姿勢制御関連の衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/attitude_control_parameters.h>

// Bdor
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
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_direct_feedback_angle_rad = PHYSICAL_CONST_degree_to_radian(20.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_direct_feedback_rate_rad_s = 1.6e-3f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_mtq_allowable_error_ratio_transient = 0.6f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_correction_gain_transient = 0.0f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_mtq_allowable_error_ratio_stable = 0.8f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_correction_gain_stable = 0.25f;
// Integral control setting
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_integral_angle_rad = PHYSICAL_CONST_degree_to_radian(20.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_angle_to_run_integral_control_rad = PHYSICAL_CONST_degree_to_radian(30.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_integral_control_permission_angle_rad = PHYSICAL_CONST_degree_to_radian(40.0f);
// Spin control setting
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_acceptable_angle_error_to_spin_up_rad = PHYSICAL_CONST_degree_to_radian(30.0f);
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_spin_rate_around_sun_rad_s = 1.4e-2f;
// Low Pass Filter setting
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_sample_freq_Hz = 10.0f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_Hz = 0.10f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_damping_factor = 1.0f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_spin_axis_Hz = 0.03f;
const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_spin_rate_cutoff_freq_Hz = 5e-4f;