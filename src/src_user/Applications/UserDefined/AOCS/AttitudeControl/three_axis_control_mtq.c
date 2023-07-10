#pragma section REPRO
/**
* @file   three_axis_control_mtq.c
* @brief  MTQを使った三軸姿勢制御アプリケーション
* @note   三軸姿勢制御のためのPID制御を行い、目標磁気モーメントを計算する。
*/

#include "three_axis_control_mtq.h"

#include <math.h>
#include <string.h>

#include <src_user/Library/vector3.h>
#include <src_user/Library/quaternion.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/math_constants.h>
#include <src_user/Library/c2a_math.h>
#include <src_user/Library/ControlUtility/gyroscopic_effect.h>
#include <src_user/Applications/UserDefined/AOCS/HardwareDependent/ActuatorControllers/mtq_seiren_controller.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

#include <src_core/Library/print.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/endian.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/attitude_control_parameters.h>

static ThreeAxisControlMtq        three_axis_control_mtq_;
const  ThreeAxisControlMtq* const three_axis_control_mtq = &three_axis_control_mtq_;

static void APP_TAC_MTQ_init_(void);
static void APP_TAC_MTQ_exec_(void);

/**
 * @brief  三軸姿勢制御における目標トルク計算関数
 * @param[in]    angle_limit_rad   : 偏差が大きい場合にも設計通りのFB特性を保証するための角度偏差リミッタ [rad]
 * @return       void
 */
static void APP_TAC_MTQ_calc_target_torque_(const float angle_limit_rad);

/**
 * @brief        機体固定座標系姿勢回転ベクトルの偏差の計算
 * @param[out]   rot_vec_body_rad  : 機体固定座標系姿勢回転ベクトル [rad]
 * @param[in]    angle_limit_rad   : 偏差が大きい場合にも設計通りのFB特性を保証するための角度偏差リミッタ [rad]
 * @return       angle_error_rad   : クリップ前の3軸角度偏差 [rad]
 */
static float APP_TAC_MTQ_calc_rot_vec_body_rad_(float rot_vec_body_rad[PHYSICAL_CONST_THREE_DIM], const float angle_limit_rad);

/**
 * @brief        3軸誤差角RSSに応じたCrossProductのパラメータ調整
 * @param        angle_error_rad : 3軸誤差角RSS [rad]
 * @return       void
 * @note         CROSS_PRODUCT_CONTROL_set_anti_intoxication_parametersに条件分岐機能を付けた拡張
 */
static void APP_TAC_MTQ_adjust_cross_product_(const float angle_error_rad);


AppInfo APP_TAC_MTQ_create_app(void)
{
  return AI_create_app_info("ThreeAxisControlMtq", APP_TAC_MTQ_init_, APP_TAC_MTQ_exec_);
}

static void APP_TAC_MTQ_init_(void)
{
  memset(&three_axis_control_mtq_, 0x00, sizeof(ThreeAxisControlMtq));

  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_init(&three_axis_control_mtq_.pid_omega[axis]);
    PID_CONTROL_init(&three_axis_control_mtq_.pid_att[axis]);
  }

  CROSS_PRODUCT_CONTROL_init(&three_axis_control_mtq_.cross_product_cntrl);

  PidGains gains_att[PHYSICAL_CONST_THREE_DIM];
  gains_att[0] = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_x;
  gains_att[1] = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_y;
  gains_att[2] = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_gains_body_z;

  PidGains gains_omega[PHYSICAL_CONST_THREE_DIM];
  gains_omega[0] = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_x;
  gains_omega[1] = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_y;
  gains_omega[2] = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_attitude_rate_gains_body_z;

  three_axis_control_mtq_.max_direct_feedback_angle_mtq_rad     = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_direct_feedback_angle_rad;
  three_axis_control_mtq_.max_direct_feedback_rate_mtq_rad_s    = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_direct_feedback_rate_rad_s;
  three_axis_control_mtq_.max_integral_angle_mtq_rad            = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_integral_angle_rad;
  three_axis_control_mtq_.max_angle_to_run_integral_control_rad = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_max_angle_to_run_integral_control_rad;

  three_axis_control_mtq_.mtq_lpf_sample_freq_Hz = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_sample_freq_Hz;
  three_axis_control_mtq_.mtq_lpf_cutoff_freq_Hz = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_trq_cutoff_freq_Hz;
  three_axis_control_mtq_.mtq_lpf_damping_factor = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_lpf_trq_damping_factor;

  three_axis_control_mtq_.allowable_error_ratio_transient      = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_mtq_allowable_error_ratio_transient;
  three_axis_control_mtq_.correction_gain_transient            = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_correction_gain_transient;
  three_axis_control_mtq_.allowable_error_ratio_stable         = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_mtq_allowable_error_ratio_stable;
  three_axis_control_mtq_.correction_gain_stable               = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_correction_gain_stable ;
  three_axis_control_mtq_.acceptable_angle_error_as_stable_rad = ATTITUDE_CONTROL_PARAMETERS_tac_mtq_acceptable_angle_error_as_stable_rad;

  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_set_gain(&three_axis_control_mtq_.pid_omega[axis], gains_omega[axis]);
    PID_CONTROL_set_gain(&three_axis_control_mtq_.pid_att[axis], gains_att[axis]);
    PID_CONTROL_set_integral_limit(&three_axis_control_mtq_.pid_att[axis],
                                   three_axis_control_mtq_.max_integral_angle_mtq_rad,
                                   three_axis_control_mtq_.max_angle_to_run_integral_control_rad);
    Z_FILTER_init_as_lpf_2nd(&three_axis_control_mtq_.lpf_mtq_cntrl[axis],
                              three_axis_control_mtq_.mtq_lpf_sample_freq_Hz,
                              three_axis_control_mtq_.mtq_lpf_cutoff_freq_Hz,
                              three_axis_control_mtq_.mtq_lpf_damping_factor);
  }

  CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(&three_axis_control_mtq_.cross_product_cntrl,
                                                         three_axis_control_mtq_.allowable_error_ratio_transient,
                                                         three_axis_control_mtq_.correction_gain_transient);

  return;
}

void APP_TAC_MTQ_exec_(void)
{
  APP_TAC_MTQ_calc_target_torque_(three_axis_control_mtq_.max_direct_feedback_angle_mtq_rad);
  AOCS_MANAGER_set_external_torque_target_body_Nm(three_axis_control_mtq_.target_torque_body_Nm);
  APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_output(three_axis_control_mtq_.cross_product_cntrl);

  return;
}

void APP_TAC_MTQ_calc_target_torque_(const float angle_limit_rad)
{
  // 機体固定座標系姿勢角速度の偏差の計算
  float ang_vel_error_body_rad_s[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };
  VECTOR3_subtract(ang_vel_error_body_rad_s, aocs_manager->ang_vel_target_body_rad_s, aocs_manager->ang_vel_est_body_rad_s);
  // Anti-WindUp
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ang_vel_error_body_rad_s[i] = C2A_MATH_clip_value_abs(ang_vel_error_body_rad_s[i], three_axis_control_mtq_.max_direct_feedback_rate_mtq_rad_s);
  }
  AOCS_MANAGER_set_ang_vel_error_body_rad_s(ang_vel_error_body_rad_s);

  // 機体固定座標系姿勢の偏差の計算
  //!< Euler軸ベクトルと (clip後の) 3軸誤差角RSSの積 [rad]
  float rot_vec_body_rad[PHYSICAL_CONST_THREE_DIM] = {1.0f, 0.0f, 0.0f};
  Quaternion quaternion_error_b2t = QUATERNION_divide(aocs_manager->quaternion_est_i2b, aocs_manager->quaternion_target_i2t);
  if (quaternion_error_b2t.scalar_part < 0.0f)
  {
    quaternion_error_b2t = QUATERNION_scalar_product(-1.0f, quaternion_error_b2t);
  }
  AOCS_MANAGER_set_quaternion_error_b2t(quaternion_error_b2t);


  //!< 収束判定等に用いる (clip前の) 3軸誤差角RSS
  float angle_error_three_axis_rad = APP_TAC_MTQ_calc_rot_vec_body_rad_(rot_vec_body_rad, angle_limit_rad);

  // 3軸誤差角RSSに基づいたパラメータ変更
  APP_TAC_MTQ_adjust_cross_product_(angle_error_three_axis_rad);

  // PIDによる目標トルクの計算
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    float cntrl_trq_before_lpf = 0.0f; //!< buffer for store trq calc result before passing LPF
    PID_CONTROL_calc_output(&three_axis_control_mtq_.pid_omega[i], aocs_manager->ang_vel_error_body_rad_s[i]);
    PID_CONTROL_calc_output(&three_axis_control_mtq_.pid_att[i], rot_vec_body_rad[i]);
    cntrl_trq_before_lpf = three_axis_control_mtq_.pid_omega[i].control_output + three_axis_control_mtq_.pid_att[i].control_output;
    three_axis_control_mtq_.target_torque_body_Nm[i]  = Z_FILTER_calc_output(&three_axis_control_mtq_.lpf_mtq_cntrl[i], cntrl_trq_before_lpf);
  }

  // ジャイロ効果トルクの追加
  AOCS_MANAGER_add_gyroscopic_torque_body_nm(three_axis_control_mtq_.target_torque_body_Nm,
                                             three_axis_control_mtq_.target_torque_body_Nm);
  // 固定トルクの追加
  AOCS_MANAGER_add_constant_torque_body_nm(three_axis_control_mtq_.target_torque_body_Nm,
                                           three_axis_control_mtq_.target_torque_body_Nm);

  return;
}

float APP_TAC_MTQ_calc_rot_vec_body_rad_(float rot_vec_body_rad[PHYSICAL_CONST_THREE_DIM], const float angle_limit_rad)
{
  float rot_vec_body[PHYSICAL_CONST_THREE_DIM] = { 1.0f, 0.0f, 0.0f };
  float rot_scalar_rad = 0.0f;

  QUATERNION_to_euler_axis_angle(rot_vec_body, &rot_scalar_rad, aocs_manager->quaternion_error_b2t);
  float rot_scalar_clipped_rad = C2A_MATH_clip_value_abs(rot_scalar_rad, angle_limit_rad);
  VECTOR3_scalar_product(rot_vec_body_rad, rot_scalar_clipped_rad, rot_vec_body);

  return rot_scalar_rad;
}

static void APP_TAC_MTQ_adjust_cross_product_(const float angle_error_rad)
{
  float allowable_error_ratio = three_axis_control_mtq_.allowable_error_ratio_transient;
  float gain_for_error        = three_axis_control_mtq_.correction_gain_transient;

  if (fabsf(angle_error_rad) < three_axis_control_mtq_.acceptable_angle_error_as_stable_rad)
  {
    allowable_error_ratio = three_axis_control_mtq_.allowable_error_ratio_stable;
    gain_for_error        = three_axis_control_mtq_.correction_gain_stable;
  }

  CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(&three_axis_control_mtq_.cross_product_cntrl,
                                                          allowable_error_ratio, gain_for_error);

  return;
}

CCP_CmdRet Cmd_APP_TAC_MTQ_SET_GAIN(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;
  uint8_t att_omega_flag = param[param_id];
  param_id += (int)sizeof(uint8_t);
  if (att_omega_flag >= 2) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint8_t axis = param[param_id];
  param_id += (int)sizeof(uint8_t);
  if (axis >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  PidGains gains;
  ENDIAN_memcpy(&gains.p_gain, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  ENDIAN_memcpy(&gains.i_gain, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  ENDIAN_memcpy(&gains.d_gain, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);

  C2A_MATH_ERROR ret;
  if (att_omega_flag == 0)
  {
    three_axis_control_mtq_.gains_omega_cmd_tmp[axis] = gains;
  }
  else
  {
    three_axis_control_mtq_.gains_att_cmd_tmp[axis] = gains;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAC_MTQ_SAVE_GAIN(const CommonCmdPacket* packet)
{
  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_set_gain(&three_axis_control_mtq_.pid_omega[axis],
                         three_axis_control_mtq_.gains_omega_cmd_tmp[axis]);
    PID_CONTROL_set_gain(&three_axis_control_mtq_.pid_att[axis],
                         three_axis_control_mtq_.gains_att_cmd_tmp[axis]);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAC_MTQ_SET_FEEDBACK_LIMIT_PARAMS(const CommonCmdPacket* packet)
{
  float limit_angle_rad = CCP_get_param_from_packet(packet, 0, float);
  C2A_MATH_ERROR range_check_result = C2A_MATH_check_range_violation(limit_angle_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.max_direct_feedback_angle_mtq_rad = limit_angle_rad;

  float limit_rate_rad_s = CCP_get_param_from_packet(packet, 1, float);
  range_check_result = C2A_MATH_check_range_violation(limit_rate_rad_s, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.max_direct_feedback_rate_mtq_rad_s = limit_rate_rad_s;

  float max_integral_angle_rad = CCP_get_param_from_packet(packet, 2, float);
  range_check_result = C2A_MATH_check_range_violation(max_integral_angle_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float max_angle_to_run_integral_rad = CCP_get_param_from_packet(packet, 3, float);
  range_check_result = C2A_MATH_check_range_violation(max_angle_to_run_integral_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.max_integral_angle_mtq_rad = max_integral_angle_rad;
  three_axis_control_mtq_.max_angle_to_run_integral_control_rad = max_angle_to_run_integral_rad;

  // 積分器へのリミッタは反映用の関数を呼び出さないと実施されない
  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_set_integral_limit(&three_axis_control_mtq_.pid_att[axis],
                                   three_axis_control_mtq_.max_integral_angle_mtq_rad,
                                   three_axis_control_mtq_.max_angle_to_run_integral_control_rad);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAC_MTQ_SET_LPF_PARAMS(const CommonCmdPacket* packet)
{
  static const float kMaxSampleRateHz  = 1000.0f; //!< アプリ実行周期最大値
  static const float kMaxNyquistRateHz = kMaxSampleRateHz / 2.0f;  //!< LPFに設定可能な理論上最大周波数

  float mtq_lpf_sample_freq_Hz = CCP_get_param_from_packet(packet, 0, float);
  C2A_MATH_ERROR range_check_result = C2A_MATH_check_range_violation(mtq_lpf_sample_freq_Hz, kMaxSampleRateHz, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float mtq_lpf_cutoff_freq_Hz = CCP_get_param_from_packet(packet, 1, float);
  range_check_result = C2A_MATH_check_range_violation(mtq_lpf_cutoff_freq_Hz, kMaxNyquistRateHz, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float mtq_lpf_damping_factor = CCP_get_param_from_packet(packet, 2, float);
  range_check_result = C2A_MATH_check_range_violation(mtq_lpf_damping_factor, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.mtq_lpf_cutoff_freq_Hz = mtq_lpf_cutoff_freq_Hz;
  three_axis_control_mtq_.mtq_lpf_sample_freq_Hz = mtq_lpf_sample_freq_Hz;
  three_axis_control_mtq_.mtq_lpf_damping_factor = mtq_lpf_damping_factor;

  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    Z_FILTER_init_as_lpf_2nd(&three_axis_control_mtq_.lpf_mtq_cntrl[axis],
                              three_axis_control_mtq_.mtq_lpf_sample_freq_Hz,
                              three_axis_control_mtq_.mtq_lpf_cutoff_freq_Hz,
                              three_axis_control_mtq_.mtq_lpf_damping_factor);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAC_MTQ_SET_ANTI_INTOXICATION_PARAMS(const CommonCmdPacket* packet)
{
  float allowable_error_ratio_transient = CCP_get_param_from_packet(packet, 0, float);
  // CrossProduct誤差許容率の値域は 0:全く許容しない ~ 1:全て許容
  C2A_MATH_ERROR range_check_result = C2A_MATH_check_range_violation(allowable_error_ratio_transient, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.allowable_error_ratio_transient = allowable_error_ratio_transient;

  float correction_gain_transient = CCP_get_param_from_packet(packet, 1, float);
  // CrossProduct誤差抑制ゲインの値域は 0:許容値より大きい誤差がある時は出力しない ~ 1:許容値より大きい誤差がある時もFull出力
  range_check_result = C2A_MATH_check_range_violation(correction_gain_transient, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.correction_gain_transient = correction_gain_transient;

  float allowable_error_ratio_stable = CCP_get_param_from_packet(packet, 2, float);
  // CrossProduct誤差許容率の値域は 0:全く許容しない ~ 1:全て許容
  range_check_result = C2A_MATH_check_range_violation(allowable_error_ratio_stable, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.allowable_error_ratio_stable = allowable_error_ratio_stable;

  float correction_gain_stable = CCP_get_param_from_packet(packet, 3, float);
  // CrossProduct誤差抑制ゲインの値域は 0:許容値より大きい誤差がある時は出力しない ~ 1:許容値より大きい誤差がある時もFull出力
  range_check_result = C2A_MATH_check_range_violation(correction_gain_stable, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.correction_gain_stable = correction_gain_stable;

  float acceptable_angle_error_as_stable_rad = CCP_get_param_from_packet(packet, 4, float);
  range_check_result = C2A_MATH_check_range_violation(acceptable_angle_error_as_stable_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  three_axis_control_mtq_.acceptable_angle_error_as_stable_rad = acceptable_angle_error_as_stable_rad;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
