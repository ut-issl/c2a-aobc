/**
 * @file  attitude_determination_parameters.c
 * @brief 姿勢決定系に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/attitude_determination_parameters.h>
#include <math.h>
#include <src_user/Library/math_constants.h>

// Rough Three Axis Determination
const APP_RTAD_METHOD ATTITUDE_DETERMINATION_PARAMETERS_rtad_method = APP_RTAD_METHOD_TRIAD;
const APP_RTAD_SUN_INVISIBLE_PROPAGATION ATTITUDE_DETERMINATION_PARAMETERS_rtad_sun_invisible_propagation = APP_RTAD_SUN_INVISIBLE_PROPAGATION_QUATERNION;
const float ATTITUDE_DETERMINATION_PARAMETERS_q_method_sun_vec_weight = 0.5f;

// Fine Three Axis Determination
const APP_FTAD_METHOD ATTITUDE_DETERMINATION_PARAMETERS_ftad_method = APP_FTAD_METHOD_STT;

// STT-Gyro Extended Kalman Filter
// STIM210 random noise N = 15deg/sq(h)
const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_gyro_random_noise_standard_deviation_compo_rad_s[PHYSICAL_CONST_THREE_DIM] = { 4.363323e-5f,
                                                                                                                                 4.363323e-5f,
                                                                                                                                 4.363323e-5f };
// STIM210 bias stability B = 0.3deg/h, K=3/2*B^2/N
const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_gyro_random_walk_standard_deviation_compo_rad_s2[PHYSICAL_CONST_THREE_DIM] = { 7.272205e-8f,
                                                                                                                                 7.272205e-8f,
                                                                                                                                 7.272205e-8f };
// As result of sensor feature measurement, STIM210 noise looks a simple random walk model.
// When ECRV time constant is infinite, the model is same as a simple random walk, so we set very large value here.
const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_gyro_random_walk_time_constant_s = 1e9f;

// STT noise roll direction 10 arcsec, cross direction 2 arcsec
const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_stt_standard_deviation_compo_rad[PHYSICAL_CONST_THREE_DIM] = { 4.8481e-5f,
                                                                                                                 9.6963e-6f,
                                                                                                                 9.6963e-6f };
// Process noise model
// We assume the attitude target is operated to synchronize with orbit.
#define ATTITUDE_PARAMETER_ORBIT_PERIOD_SEC (5700.0f)
#define ATTITUDE_PARAMETER_COMPUTATION_CYCLE_SEC (0.1f)
const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_process_noise_covariance_attitude = ((MATH_CONST_2PI / ATTITUDE_PARAMETER_ORBIT_PERIOD_SEC * ATTITUDE_PARAMETER_COMPUTATION_CYCLE_SEC))
                                                                                    * ((MATH_CONST_2PI / ATTITUDE_PARAMETER_ORBIT_PERIOD_SEC * ATTITUDE_PARAMETER_COMPUTATION_CYCLE_SEC));
// The attitude rate process noise is not well tuned at this moment
const float ATTITUDE_DETERMINATION_PARAMETERS_ekf_process_noise_covariance_attitude_rate = ((MATH_CONST_2PI / ATTITUDE_PARAMETER_ORBIT_PERIOD_SEC * ATTITUDE_PARAMETER_COMPUTATION_CYCLE_SEC))
                                                                                    * ((MATH_CONST_2PI / ATTITUDE_PARAMETER_ORBIT_PERIOD_SEC * ATTITUDE_PARAMETER_COMPUTATION_CYCLE_SEC));
