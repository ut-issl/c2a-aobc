/**
 * @file  attitude_determination_parameters.h
 * @brief 姿勢決定系に関する衛星固有パラメータを管理する
 */

#ifndef ATTITUDE_DETERMINATION_PARAMETERS_H_
#define ATTITUDE_DETERMINATION_PARAMETERS_H_

#include <src_user/Applications/UserDefined/AOCS/AttitudeDetermination/rough_three_axis_determination.h>
#include <src_user/Applications/UserDefined/AOCS/AttitudeDetermination/fine_three_axis_determination.h>
#include <src_user/Applications/UserDefined/AOCS/AttitudeDetermination/stt_gyro_ekf.h>

// Rough Three Axis Determination
extern const APP_RTAD_METHOD ATTITUDE_DETERMINATION_PARAMETERS_rtad_method;                                        //!< Rough three axis determination method
extern const APP_RTAD_SUN_INVISIBLE_PROPAGATION ATTITUDE_DETERMINATION_PARAMETERS_rtad_sun_invisible_propagation;  //!< Rough three axis determination sun invisible propagation method
extern const float ATTITUDE_DETERMINATION_PARAMETERS_q_method_sun_vec_weight;                                      //!< Q-method sun vector weight (0.0f ~ 1.0f)

// Fine Three Axis Determination
extern const APP_FTAD_METHOD ATTITUDE_DETERMINATION_PARAMETERS_ftad_method; //!< Fine three axis determination method

// STT-Gyro Extended Kalman Filter
// Observation noise model of Gyro sensor
extern const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_gyro_random_noise_standard_deviation_compo_rad_s[PHYSICAL_CONST_THREE_DIM];  //!< EKF gyro sensor random noise standard deviation @ component frame [rad/s]
extern const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_gyro_random_walk_standard_deviation_compo_rad_s2[PHYSICAL_CONST_THREE_DIM];  //!< EKF gyro sensor random walk standard deviation @ component frame [rad/s2]
extern const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_gyro_random_walk_time_constant_s;                                            //!< EKF gyro sensor random walk time constant [s]
// Observation noise model of Star sensor
extern const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_stt_standard_deviation_compo_rad[PHYSICAL_CONST_THREE_DIM];  //!< EKF star sensor random noise standard deviation @ component frame [rad]
// Process noise model
extern const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_process_noise_covariance_attitude;       //!< Process noise covariance for attitude
extern const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_process_noise_covariance_attitude_rate;  //!< Process noise covariance for attitude rate

#endif // ATTITUDE_DETERMINATION_PARAMETERS_H_
