#pragma section REPRO
/**
* @file   sun_pointing.c
* @brief  太陽指向制御アプリケーション
* @note   太陽指向のためのPID制御を行う。
*         粗制御の場合はMTQの目標磁気モーメント、精制御の場合はRWの目標回転トルクを計算する。
*/

#include "sun_pointing.h"

#include <math.h>
#include <string.h>

#include "../../../../Library/vector3.h"
#include "../../../../Library/matrix33.h"
#include "../../../../Library/math_constants.h"
#include "../../../../Library/ControlUtility/gyroscopic_effect.h"
#include "../HardwareDependent/ActuatorControllers/mtq_seiren_controller.h"
#include "../../../../Library/ControlUtility/cross_product_control.h"
#include "../aocs_manager.h"

#include <src_core/Library/print.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/endian.h>

static SunPointing        sun_pointing_;
const  SunPointing* const sun_pointing = &sun_pointing_;

static const float SUN_POINTING_kMaxTargetAngVelRadSec_ = 3.0f; //!< 太陽方向周り目標角速度の最大値 [rad/s]
/**
 * @brief  太陽指向各種パラメータ初期化関数
 */
static void APP_SUN_POINTING_init_(void);

/**
 * @brief  太陽指向定期実行関数
 */
static void APP_SUN_POINTING_exec_(void);

/**
 * @brief  太陽指向における目標トルク計算関数
 * @param  void
 * @return void
 */
static void APP_SUN_POINTING_calc_target_torque_(void);

/**
 * @brief  太陽指向達成度判定
 * @param  angle_error_rad : 現在の姿勢指向誤差角推定値3軸RSS [rad]
 * @return void
 */
static void APP_SUN_POINTING_select_control_state_(const float angle_error_rad);

/**
 * @brief  太陽指向の達成状態に応じたCrossProductのパラメータ調整
 * @param  control_state : 太陽指向の達成状態 (達成状態に応じて調整することを明示するため引数化)
 * @return void
 * @note   CROSS_PRODUCT_CONTROL_set_anti_intoxication_parametersに条件分岐機能を付けた拡張
 */
static void APP_SUN_POINTING_adjust_cross_product_(const SUN_POINTING_CONTROL_STATE control_state);

/**
 * @brief  太陽指向の達成状態に応じた目標レートの調整
 * @param  control_state : 太陽指向の達成状態 (達成状態に応じて調整することを明示するため引数化)
 * @return void
 * @note   AOCS_MANAGER_set_ang_vel_target_body_rad_sに条件分岐と平滑化機能を付けた拡張
 */
static void APP_SUN_POINTING_adjust_target_rate_(const SUN_POINTING_CONTROL_STATE control_state);


AppInfo APP_SUN_POINTING_create_app(void)
{
  return AI_create_app_info("SunPointing", APP_SUN_POINTING_init_, APP_SUN_POINTING_exec_);
}

static void APP_SUN_POINTING_init_(void)
{
  memset(&sun_pointing_, 0x00, sizeof(SunPointing));

  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_init(&sun_pointing_.pid_omega[axis]);
    PID_CONTROL_init(&sun_pointing_.pid_att[axis]);
  }

  CROSS_PRODUCT_CONTROL_init(&sun_pointing_.cross_product_cntrl);

  sun_pointing_.sun_pointing_axis_id = SUN_POINTING_AXIS_ID_Z;
  sun_pointing_.control_state        = SUN_POINTING_CONTROL_STATE_TRANSIENT;

  float sun_vec_target_body[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };
  sun_vec_target_body[sun_pointing_.sun_pointing_axis_id] = 1.0f;
  AOCS_MANAGER_set_sun_vec_target_body(sun_vec_target_body);

  // ノミナルのスピン軸 (Z軸) 周りの姿勢制御はフリーとし，スピン軸周りはレートのみPD制御する
  PidGains gains_att[PHYSICAL_CONST_THREE_DIM];
  gains_att[0].p_gain = 5.0e-5f;
  gains_att[1].p_gain = 10.0e-5f;
  gains_att[2].p_gain = 0.0f;

  gains_att[0].i_gain = 0.0f;
  gains_att[1].i_gain = 0.0f;
  gains_att[2].i_gain = 0.0f;

  gains_att[0].d_gain = 0.0f;
  gains_att[1].d_gain = 0.0f;
  gains_att[2].d_gain = 0.0f;

  PidGains gains_omega[PHYSICAL_CONST_THREE_DIM];
  gains_omega[0].p_gain = 1.0e-2f;
  gains_omega[1].p_gain = 2.0e-2f;
  gains_omega[2].p_gain = 7.0e-3f;

  gains_omega[0].i_gain = 0.0f;
  gains_omega[1].i_gain = 0.0f;
  gains_omega[2].i_gain = 0.0f;

  gains_omega[0].d_gain = 0.0f;
  gains_omega[1].d_gain = 0.0f;
  gains_omega[2].d_gain = 2.0e-2f;

  sun_pointing_.max_direct_feedback_angle_rad         = PHYSICAL_CONST_degree_to_radian(20.0f);
  sun_pointing_.max_direct_feedback_rate_rad_s        = 1.6e-3f;
  sun_pointing_.max_integral_angle_rad                = PHYSICAL_CONST_degree_to_radian(20.0f);
  sun_pointing_.max_angle_to_run_integral_control_rad = PHYSICAL_CONST_degree_to_radian(30.0f);
  sun_pointing_.integral_control_permission_angle_rad = PHYSICAL_CONST_degree_to_radian(40.0f);

  sun_pointing_.acceptable_angle_error_to_spin_up_rad = PHYSICAL_CONST_degree_to_radian(30.0f);
  sun_pointing_.spin_rate_around_sun_rad_s            = 1.4e-2f;

  sun_pointing_.lpf_sample_freq_Hz           = 10.0f;
  sun_pointing_.lpf_trq_cutoff_freq_Hz       = 0.10f;
  sun_pointing_.lpf_trq_damping_factor       = 1.0f;

  sun_pointing_.lpf_trq_cutoff_freq_spin_axis_Hz         = 0.03f;
  sun_pointing_.lpf_spin_rate_cutoff_freq_Hz             = 5e-4f;

  // 下記の値はISS軌道を想定した値 (極軌道に近ければ緩めて良いはず)
  sun_pointing_.mtq_allowable_error_ratio_transient = 0.6f;
  sun_pointing_.correction_gain_transient           = 0.0f;
  sun_pointing_.mtq_allowable_error_ratio_stable    = 0.8f;
  sun_pointing_.correction_gain_stable              = 0.25f;

  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_set_gain(&sun_pointing_.pid_omega[axis], gains_omega[axis]);
    PID_CONTROL_set_gain(&sun_pointing_.pid_att[axis], gains_att[axis]);
    PID_CONTROL_set_integral_limit(&sun_pointing_.pid_att[axis],
                                    sun_pointing_.max_integral_angle_rad,
                                    sun_pointing_.max_angle_to_run_integral_control_rad);
    Z_FILTER_init_as_lpf_2nd(&sun_pointing_.lpf_cntrl_torque[axis],
                              sun_pointing_.lpf_sample_freq_Hz,
                              sun_pointing_.lpf_trq_cutoff_freq_Hz,
                              sun_pointing_.lpf_trq_damping_factor);
  }

  // for spin-axis
  Z_FILTER_init_as_lpf_2nd(&sun_pointing_.lpf_cntrl_torque[sun_pointing_.sun_pointing_axis_id],
                            sun_pointing_.lpf_sample_freq_Hz,
                            sun_pointing_.lpf_trq_cutoff_freq_spin_axis_Hz,
                            sun_pointing_.lpf_trq_damping_factor);

  // for spin rate
  Z_FILTER_init_as_lpf_1st(&sun_pointing_.lpf_spin_rate,
                            sun_pointing_.lpf_sample_freq_Hz,
                            sun_pointing_.lpf_spin_rate_cutoff_freq_Hz);

  CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(&sun_pointing_.cross_product_cntrl,
                                                         sun_pointing_.mtq_allowable_error_ratio_transient,
                                                         sun_pointing_.correction_gain_transient);

  return;
}

void APP_SUN_POINTING_exec_(void)
{
  APP_SUN_POINTING_calc_target_torque_();
  AOCS_MANAGER_set_external_torque_target_body_Nm(sun_pointing_.target_torque_body_Nm);
  APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_output(sun_pointing_.cross_product_cntrl);
  return;
}

void APP_SUN_POINTING_calc_target_torque_(void)
{
  float sun_vec_error_body_vec[PHYSICAL_CONST_THREE_DIM];
  float sun_vec_error_body_vec_norm[PHYSICAL_CONST_THREE_DIM];
  float sun_vec_error_body_rad_vec[PHYSICAL_CONST_THREE_DIM];
  float sun_vec_error_rad_scalar;
  float ang_vel_error_body_rad_s[PHYSICAL_CONST_THREE_DIM];

  // 機体固定座標系姿勢角度の偏差の計算
  VECTOR3_outer_product(sun_vec_error_body_vec, aocs_manager->sun_vec_target_body, aocs_manager->sun_vec_est_body);
  VECTOR3_normalize(sun_vec_error_body_vec_norm, sun_vec_error_body_vec);
  sun_vec_error_rad_scalar = VECTOR3_inner_product(aocs_manager->sun_vec_est_body, aocs_manager->sun_vec_target_body);
  sun_vec_error_rad_scalar = acosf(sun_vec_error_rad_scalar);
  AOCS_MANAGER_set_sun_vec_error_rad(sun_vec_error_rad_scalar);

  // 収束判定と判定結果に応じたパラメータ調整
  APP_SUN_POINTING_select_control_state_(sun_vec_error_rad_scalar);
  APP_SUN_POINTING_adjust_cross_product_(sun_pointing_.control_state);
  APP_SUN_POINTING_adjust_target_rate_(sun_pointing_.control_state);

  // Anti-WindUp
  sun_vec_error_rad_scalar = C2A_MATH_clip_value_abs(sun_vec_error_rad_scalar, sun_pointing_.max_direct_feedback_angle_rad);
  sun_vec_error_rad_scalar = C2A_MATH_normalize_zero_2pi(sun_vec_error_rad_scalar);
  VECTOR3_scalar_product(sun_vec_error_body_rad_vec, sun_vec_error_rad_scalar, sun_vec_error_body_vec_norm);

  // 機体固定座標系姿勢角速度の偏差の計算
  VECTOR3_subtract(ang_vel_error_body_rad_s, aocs_manager->ang_vel_target_body_rad_s, aocs_manager->ang_vel_est_body_rad_s);
  // Anti-WindUp
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ang_vel_error_body_rad_s[i] = C2A_MATH_clip_value_abs(ang_vel_error_body_rad_s[i], sun_pointing_.max_direct_feedback_rate_rad_s);
  }
  AOCS_MANAGER_set_ang_vel_error_body_rad_s(ang_vel_error_body_rad_s);

  // PIDによる目標トルクの計算
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    float cntrl_trq_before_lpf = 0.0f; //!< buffer for store trq calc result before passing LPF
    PID_CONTROL_calc_output(&sun_pointing_.pid_omega[i], aocs_manager->ang_vel_error_body_rad_s[i]);
    PID_CONTROL_calc_output(&sun_pointing_.pid_att[i], sun_vec_error_body_rad_vec[i]);

    cntrl_trq_before_lpf = sun_pointing_.pid_omega[i].control_output + sun_pointing_.pid_att[i].control_output;
    sun_pointing_.target_torque_body_Nm[i] = Z_FILTER_calc_output(&sun_pointing_.lpf_cntrl_torque[i], cntrl_trq_before_lpf);
  }

  // ジャイロ効果トルクの追加
  AOCS_MANAGER_add_gyroscopic_torque_body_nm(sun_pointing_.target_torque_body_Nm,
                                             sun_pointing_.target_torque_body_Nm);
  // 固定トルクの追加
  AOCS_MANAGER_add_constant_torque_body_nm(sun_pointing_.target_torque_body_Nm,
                                           sun_pointing_.target_torque_body_Nm);

  return;
}

// TODO_L: 収束判定の詳細化が必要 (現状はスピンアップ or ダウンの開始判定のみ)
static void APP_SUN_POINTING_select_control_state_(const float angle_error_rad)
{
  float reference_criteria_rad = sun_pointing_.acceptable_angle_error_to_spin_up_rad;

  /* 蝕中のドリフト積算誤差を蝕明けに検知してインパルス的に誤差が跳ねる (~30deg) 際に
     下手にスピンダウンが始まらない様にしたいため，一旦スピンアップに入った後は判定を緩める */
  if ((sun_pointing_.control_state == SUN_POINTING_CONTROL_STATE_SPIN_UP) ||
      (sun_pointing_.control_state == SUN_POINTING_CONTROL_STATE_CONVERGE))
  {
    reference_criteria_rad = 2.5f * sun_pointing_.acceptable_angle_error_to_spin_up_rad;
  }

  if (fabsf(angle_error_rad) < reference_criteria_rad)
  {
    sun_pointing_.control_state = SUN_POINTING_CONTROL_STATE_SPIN_UP;
  }
  else
  {
    sun_pointing_.control_state = SUN_POINTING_CONTROL_STATE_TRANSIENT;
  }

  return;
}

static void APP_SUN_POINTING_adjust_cross_product_(const SUN_POINTING_CONTROL_STATE control_state)
{
  if (control_state == SUN_POINTING_CONTROL_STATE_SPIN_UP)
  {
    CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(&sun_pointing_.cross_product_cntrl,
                                                           sun_pointing_.mtq_allowable_error_ratio_stable,
                                                           sun_pointing_.correction_gain_stable);
  }
  else if (control_state == SUN_POINTING_CONTROL_STATE_CONVERGE)
  {
    // SPIN_UPと条件を変えたくなるかもなので，敢えて分ける
    CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(&sun_pointing_.cross_product_cntrl,
                                                           sun_pointing_.mtq_allowable_error_ratio_stable,
                                                           sun_pointing_.correction_gain_stable);
  }
  else
  {
    CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(&sun_pointing_.cross_product_cntrl,
                                                           sun_pointing_.mtq_allowable_error_ratio_transient,
                                                           sun_pointing_.correction_gain_transient);
  }

  return;
}

static void APP_SUN_POINTING_adjust_target_rate_(const SUN_POINTING_CONTROL_STATE control_state)
{
  float rate_target_body[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };

  if ((control_state == SUN_POINTING_CONTROL_STATE_SPIN_UP) || (control_state == SUN_POINTING_CONTROL_STATE_CONVERGE))
  {
    rate_target_body[sun_pointing_.sun_pointing_axis_id] = sun_pointing_.spin_rate_around_sun_rad_s;
  }

  // 目標レート変化によるMTQ出力指令値の暴発を避けるため，LPFで目標レートをゆっくり変化させる (spin up/downどちらも)
  rate_target_body[sun_pointing_.sun_pointing_axis_id] =
  Z_FILTER_calc_output(&sun_pointing_.lpf_spin_rate, rate_target_body[sun_pointing_.sun_pointing_axis_id]);
  AOCS_MANAGER_set_ang_vel_target_body_rad_s(rate_target_body);

  return;
}

CCP_CmdRet Cmd_APP_SUN_POINTING_SET_GAIN(const CommonCmdPacket* packet)
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
    sun_pointing_.gains_omega_cmd_tmp[axis] = gains;
  }
  else
  {
    sun_pointing_.gains_att_cmd_tmp[axis] = gains;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_SUN_POINTING_SAVE_GAIN(const CommonCmdPacket* packet)
{
  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_set_gain(&sun_pointing_.pid_omega[axis],
                         sun_pointing_.gains_omega_cmd_tmp[axis]);
    PID_CONTROL_set_gain(&sun_pointing_.pid_att[axis],
                         sun_pointing_.gains_att_cmd_tmp[axis]);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_SUN_POINTING_SET_TARGET(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;
  float sun_vec_target_body_cmd[PHYSICAL_CONST_THREE_DIM] = {0.0f, 0.0f, 0.0f};       //!< 機体固定座標系目標太陽方向 [-]
  float ang_vel_target_scalar_rad_s = 0.0f;                                       //!< 太陽方向目標回転角速度 [rad/s]
  float ang_vel_target_body_rad_s[PHYSICAL_CONST_THREE_DIM] = {0.0f, 0.0f, 0.0f}; //!< 機体固定座標系目標角速度 [rad/s]

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ENDIAN_memcpy(&sun_vec_target_body_cmd[i], param + param_id, sizeof(float));
    param_id += (int)sizeof(float);
  }

  if (VECTOR3_is_normalized(sun_vec_target_body_cmd) != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ENDIAN_memcpy(&ang_vel_target_scalar_rad_s, param + param_id, sizeof(float));

  if (fabsf(ang_vel_target_scalar_rad_s) > SUN_POINTING_kMaxTargetAngVelRadSec_) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float sun_vec_target_body[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };
  float cmd_vector_norm = VECTOR3_norm(sun_vec_target_body_cmd);
  VECTOR3_scalar_product(sun_vec_target_body, (1.0f / cmd_vector_norm), sun_vec_target_body_cmd);
  VECTOR3_scalar_product(ang_vel_target_body_rad_s, ang_vel_target_scalar_rad_s, sun_vec_target_body);
  AOCS_MANAGER_set_sun_vec_target_body(sun_vec_target_body);
  AOCS_MANAGER_set_ang_vel_target_body_rad_s(ang_vel_target_body_rad_s);
  sun_pointing_.spin_rate_around_sun_rad_s = ang_vel_target_scalar_rad_s;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_SUN_POINTING_SET_FEEDBACK_LIMIT_PARAMS(const CommonCmdPacket* packet)
{
  float limit_angle_rad = CCP_get_param_from_packet(packet, 0, float);
  C2A_MATH_ERROR range_check_result = C2A_MATH_check_range_violation(limit_angle_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.max_direct_feedback_angle_rad = limit_angle_rad;

  float limit_rate_rad_s = CCP_get_param_from_packet(packet, 1, float);
  range_check_result = C2A_MATH_check_range_violation(limit_rate_rad_s, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.max_direct_feedback_rate_rad_s = limit_rate_rad_s;

  float max_integral_angle_rad = CCP_get_param_from_packet(packet, 2, float);
  range_check_result = C2A_MATH_check_range_violation(max_integral_angle_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float max_angle_to_run_integral_rad = CCP_get_param_from_packet(packet, 3, float);
  range_check_result = C2A_MATH_check_range_violation(max_angle_to_run_integral_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float integral_control_permission_angle_rad = CCP_get_param_from_packet(packet, 4, float);
  range_check_result = C2A_MATH_check_range_violation(integral_control_permission_angle_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.max_integral_angle_rad = max_integral_angle_rad;
  sun_pointing_.max_angle_to_run_integral_control_rad = max_angle_to_run_integral_rad;
  sun_pointing_.integral_control_permission_angle_rad = integral_control_permission_angle_rad;

  // 積分器へのリミッタは反映用の関数を呼び出さないと実施されない
  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    PID_CONTROL_set_integral_limit(&sun_pointing_.pid_att[axis],
                                    sun_pointing_.max_integral_angle_rad,
                                    sun_pointing_.max_angle_to_run_integral_control_rad);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_SUN_POINTING_SET_LPF_PARAMS(const CommonCmdPacket* packet)
{
  static const float kMaxSampleRateHz = 1000.0f; //!< アプリ実行周期最大値
  static const float kMaxNyquistRateHz = kMaxSampleRateHz / 2.0f;  //!< LPFに設定可能な理論上最大周波数

  float lpf_sample_freq_Hz = CCP_get_param_from_packet(packet, 0, float);
  C2A_MATH_ERROR range_check_result = C2A_MATH_check_range_violation(lpf_sample_freq_Hz, kMaxSampleRateHz, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float lpf_trq_cutoff_freq_Hz = CCP_get_param_from_packet(packet, 1, float);
  range_check_result = C2A_MATH_check_range_violation(lpf_trq_cutoff_freq_Hz, kMaxNyquistRateHz, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float lpf_trq_damping_factor = CCP_get_param_from_packet(packet, 2, float);
  range_check_result = C2A_MATH_check_range_violation(lpf_trq_damping_factor, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float lpf_trq_cutoff_freq_spin_axis_Hz = CCP_get_param_from_packet(packet, 3, float);
  range_check_result = C2A_MATH_check_range_violation(lpf_trq_cutoff_freq_spin_axis_Hz, kMaxNyquistRateHz, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float lpf_spin_rate_cutoff_freq_Hz = CCP_get_param_from_packet(packet, 4, float);
  range_check_result = C2A_MATH_check_range_violation(lpf_spin_rate_cutoff_freq_Hz, kMaxNyquistRateHz, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.lpf_sample_freq_Hz = lpf_sample_freq_Hz;
  sun_pointing_.lpf_trq_cutoff_freq_Hz = lpf_trq_cutoff_freq_Hz;
  sun_pointing_.lpf_trq_damping_factor = lpf_trq_damping_factor;
  sun_pointing_.lpf_trq_cutoff_freq_spin_axis_Hz = lpf_trq_cutoff_freq_spin_axis_Hz;
  sun_pointing_.lpf_spin_rate_cutoff_freq_Hz = lpf_spin_rate_cutoff_freq_Hz;

  for (uint8_t axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    Z_FILTER_init_as_lpf_2nd(&sun_pointing_.lpf_cntrl_torque[axis],
                              sun_pointing_.lpf_sample_freq_Hz,
                              sun_pointing_.lpf_trq_cutoff_freq_Hz,
                              sun_pointing_.lpf_trq_damping_factor);
  }

  // for spin-axis
  Z_FILTER_init_as_lpf_2nd(&sun_pointing_.lpf_cntrl_torque[sun_pointing_.sun_pointing_axis_id],
                            sun_pointing_.lpf_sample_freq_Hz,
                            sun_pointing_.lpf_trq_cutoff_freq_spin_axis_Hz,
                            sun_pointing_.lpf_trq_damping_factor);

  // for spin rate
  Z_FILTER_init_as_lpf_1st(&sun_pointing_.lpf_spin_rate, sun_pointing_.lpf_sample_freq_Hz, sun_pointing_.lpf_spin_rate_cutoff_freq_Hz);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_SUN_POINTING_SET_ANTI_INTOXICATION_PARAMS(const CommonCmdPacket* packet)
{
  float allowable_error_ratio_transient = CCP_get_param_from_packet(packet, 0, float);
  // CrossProduct誤差許容率の値域は 0:全く許容しない ~ 1:全て許容
  C2A_MATH_ERROR range_check_result = C2A_MATH_check_range_violation(allowable_error_ratio_transient, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.mtq_allowable_error_ratio_transient = allowable_error_ratio_transient;

  float correction_gain_transient = CCP_get_param_from_packet(packet, 1, float);
  // CrossProduct誤差抑制ゲインの値域は 0:許容値より大きい誤差がある時は出力しない ~ 1:許容値より大きい誤差がある時もFull出力
  range_check_result = C2A_MATH_check_range_violation(correction_gain_transient, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.correction_gain_transient = correction_gain_transient;

  float allowable_error_ratio_stable = CCP_get_param_from_packet(packet, 2, float);
  // CrossProduct誤差許容率の値域は 0:全く許容しない ~ 1:全て許容
  range_check_result = C2A_MATH_check_range_violation(allowable_error_ratio_stable, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.mtq_allowable_error_ratio_stable = allowable_error_ratio_stable;

  float correction_gain_stable = CCP_get_param_from_packet(packet, 3, float);
  // CrossProduct誤差抑制ゲインの値域は 0:許容値より大きい誤差がある時は出力しない ~ 1:許容値より大きい誤差がある時もFull出力
  range_check_result = C2A_MATH_check_range_violation(correction_gain_stable, 1.0f, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.correction_gain_stable = correction_gain_stable;

  float acceptable_angle_error_to_spin_up_rad = CCP_get_param_from_packet(packet, 4, float);
  range_check_result = C2A_MATH_check_range_violation(acceptable_angle_error_to_spin_up_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  sun_pointing_.acceptable_angle_error_to_spin_up_rad = acceptable_angle_error_to_spin_up_rad;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
