/**
 * @file  attitude_control_parameters.h
 * @brief 姿勢制御関連の衛星固有パラメータを管理する
 */

#ifndef ATTITUDE_CONTROL_PARAMETERS_H_
#define ATTITUDE_CONTROL_PARAMETERS_H_

#include <src_user/Applications/UserDefined/AOCS/AttitudeControl/bdot.h>
#include <src_user/Applications/UserDefined/AOCS/AttitudeControl/sun_pointing.h>
#include <src_user/Applications/UserDefined/AOCS/AttitudeControl/unloading.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

// Bdot
extern const float ATTITUDE_CONTROL_PARAMETERS_bdot_control_gain[PHYSICAL_CONST_THREE_DIM]; //!< Control gain for B-dot
extern const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_minimum_time_derivative_step_ms;     //!< Minimum interval for magnetic field derivative [ms]
extern const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_mtq_output_time_length_ms;           //!< Keep time for MTQ ON as an actuation [ms]

// Sun Pointing
// Target
extern const SUN_POINTING_AXIS_ID ATTITUDE_CONTROL_PARAMETERS_sun_pointing_axis_id;     //!< Sun pointing target axis ID
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_spin_rate_around_sun_rad_s; //!< Sun pointing target spin rate around sun direction [rad/s]
// Gain
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_x;       //!< Sun pointing attitude gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_y;       //!< Sun pointing attitude gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_z;       //!< Sun pointing attitude gain body Z axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_x;  //!< Sun pointing attitude rate gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_y;  //!< Sun pointing attitude rate gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_z;  //!< Sun pointing attitude rate gain body Z axis
// MTQ control settings
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_direct_feedback_angle_rad;        //!< Sun pointing direct feedback max angle [rad]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_direct_feedback_rate_rad_s;       //!< Sun pointing direct feedback max angular rate [rad_s]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_mtq_allowable_error_ratio_transient;  //!< Sun pointing MTQ allowable error in transient phase
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_correction_gain_transient;            //!< Sun pointing correction gain in transient phase
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_mtq_allowable_error_ratio_stable;     //!< Sun pointing MTQ allowable error in stable phase
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_correction_gain_stable;               //!< Sun pointing correction gain in stable phase
// Integral control setting
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_integral_angle_rad;                 //!< Sun pointing maximum integral angle [rad]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_max_angle_to_run_integral_control_rad;  //!< Sun pointing maximum angle to run integral control [rad]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_integral_control_permission_angle_rad;  //!< Sun pointing integral control permission angle [rad]
// Spin control setting
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_acceptable_angle_error_to_spin_up_rad; //!< Sun pointing acceptable angle error to start spin-up control [rad]
// Output torque Low Pass Filter(LPF) setting
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_sample_freq_Hz;               //!< Sun pointing sampling frequency for both LPF [Hz]
// Output torque LPF (2nd order filter)
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_Hz;           //!< Sun pointing output LPF cutoff frequency for cross spin axis [Hz]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_spin_axis_Hz; //!< Sun pointing output LPF cutoff frequency for spin axis [Hz]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_damping_factor;           //!< Sun pointing output LPF damping factor
// Spin up LPF (1st order filter)
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_spin_rate_cutoff_freq_Hz;     //!< Sun pointing spin up LPF cutoff frequency [Hz]

// Three Axis Control with MTQ
// Gain
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_x;      //!< Three Axis Control MTQ attitude gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_y;      //!< Three Axis Control MTQ attitude gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_z;      //!< Three Axis Control MTQ attitude gain body Z axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_x; //!< Three Axis Control MTQ attitude rate gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_y; //!< Three Axis Control MTQ attitude rate gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_z; //!< Three Axis Control MTQ attitude rate gain body Z axis
// MTQ control settings
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_direct_feedback_angle_rad;        //!< Three Axis Control direct feedback max angle [rad]
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_direct_feedback_rate_rad_s;       //!< Three Axis Control direct feedback max angular rate [rad_s]
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_mtq_allowable_error_ratio_transient;  //!< Three Axis Control MTQ allowable error in transient phase
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_correction_gain_transient;            //!< Three Axis Control correction gain in transient phase
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_mtq_allowable_error_ratio_stable;     //!< Three Axis Control MTQ allowable error in stable phase
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_correction_gain_stable;               //!< Three Axis Control correction gain in stable phase
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_acceptable_angle_error_as_stable_rad; //!< Three Axis Control acceptable angle error as stable [rad]
// Integral control setting
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_integral_angle_rad;                 //!< Three Axis Control maximum integral angle [rad]
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_angle_to_run_integral_control_rad;  //!< Three Axis Control maximum angle to run integral control [rad]
// Output torque Low Pass Filter(LPF) setting
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_sample_freq_Hz;      //!< Three Axis Control sampling frequency for both LPF [Hz]
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_trq_cutoff_freq_Hz;  //!< Three Axis Control output LPF cutoff frequency for cross spin axis [Hz]
extern const float ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_trq_damping_factor;  //!< Three Axis Control output LPF damping factor

// Three Axis Control with RW
// Gain
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_gains_body_x;       //!< Three Axis Control RW attitude gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_gains_body_y;       //!< Three Axis Control RW attitude gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_gains_body_z;       //!< Three Axis Control RW attitude gain body Z axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_rate_gains_body_x;  //!< Three Axis Control RW attitude rate gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_rate_gains_body_y;  //!< Three Axis Control RW attitude rate gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_tac_rw_attitude_rate_gains_body_z;  //!< Three Axis Control RW attitude rate gain body Z axis

// Unloading
extern const float ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_upper_threshold_rad_s;  //!< Unloading upper threshold [rad/s]
extern const float ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_lower_threshold_rad_s;  //!< Unloading lower threshold [rad/s]
extern const float ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_target_rad_s;           //!< Unloading target [rad/s]
extern const float ATTITUDE_CONTROL_PARAMETERS_unloading_control_gain;                            //!< Unloading gain
extern const APP_UNLOADING_EXEC ATTITUDE_CONTROL_PARAMETERS_unloading_exec_is_enable;             //!< Unloading execution enable flag

// Control Torques
extern const AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION ATTITUDE_CONTROL_PARAMETERS_constant_torque_permission;  //!< Constant torque correction permission
extern const float ATTITUDE_CONTROL_PARAMETERS_constant_torque_body_Nm[PHYSICAL_CONST_THREE_DIM];             //!< Constant torque [Nm]
extern const float ATTITUDE_CONTROL_PARAMETERS_internal_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM];         //!< Maximum output torque for internal torque [Nm]
extern const float ATTITUDE_CONTROL_PARAMETERS_external_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM];         //!< Maximum output torque for external torque [Nm]

// Target Setting parameters
extern const float ATTITUDE_CONTROL_PARAMETERS_limit_angular_velocity_rad_s;  //!< Limit of angular velocity target error [rad/s]
extern const float ATTITUDE_CONTROL_PARAMETERS_limit_maneuver_angle_rad;      //!< Limit of angle target error [rad]

#endif // ATTITUDE_CONTROL_PARAMETERS_H_
