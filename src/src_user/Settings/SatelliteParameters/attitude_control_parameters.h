/**
 * @file  attitude_control_parameters.h
 * @brief 姿勢制御関連の衛星固有パラメータを管理する
 */

#ifndef ATTITUDE_CONTROL_PARAMETERS_H_
#define ATTITUDE_CONTROL_PARAMETERS_H_

#include <src_user/Applications/UserDefined/AOCS/AttitudeControl/bdot.h>
#include <src_user/Applications/UserDefined/AOCS/AttitudeControl/sun_pointing.h>

// Bdot
extern const float ATTITUDE_CONTROL_PARAMETERS_bdot_control_gain[PHYSICAL_CONST_THREE_DIM]; //!< Control gain for B-dot
extern const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_minimum_time_derivative_step_ms;     //!< Minimum interval for magnetic field derivative [ms]
extern const uint32_t ATTITUDE_CONTROL_PARAMETERS_bdot_mtq_output_time_length_ms;           //!< Keep time for MTQ ON as an actuation [ms]

// Sun Pointing
extern const SUN_POINTING_AXIS_ID ATTITUDE_CONTROL_PARAMETERS_sun_pointing_axis_id;   //!< Sun pointing target axis ID
// Gain
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_x; //!< Sun pointing attitude gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_y; //!< Sun pointing attitude gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_gains_body_z; //!< Sun pointing attitude gain body Z axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_x; //!< Sun pointing attitude rate gain body X axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_y; //!< Sun pointing attitude rate gain body Y axis
extern const PidGains ATTITUDE_CONTROL_PARAMETERS_sun_pointing_attitude_rate_gains_body_z; //!< Sun pointing attitude rate gain body Z axis
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
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_spin_rate_around_sun_rad_s;            //!< Sun pointing spin rate around sun direction [rad/s]
// Output torque Low Pass Filter(LPF) setting
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_sample_freq_Hz;               //!< Sun pointing sampling frequency for both LPF [Hz]
// Output torque LPF (2nd order filter)
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_Hz;           //!< Sun pointing output LPF cutoff frequency for cross spin axis [Hz]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_cutoff_freq_spin_axis_Hz; //!< Sun pointing output LPF cutoff frequency for spin axis [Hz]
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_trq_damping_factor;           //!< Sun pointing output LPF damping factor
// Spin up LPF (1st order filter)
extern const float ATTITUDE_CONTROL_PARAMETERS_sun_pointing_lpf_spin_rate_cutoff_freq_Hz;     //!< Sun pointing spin up LPF cutoff frequency [Hz]

#endif // ATTITUDE_CONTROL_PARAMETERS_H_
