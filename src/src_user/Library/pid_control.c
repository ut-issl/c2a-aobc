#pragma section REPRO
/**
 * @file
 * @brief  PID制御を行うライブラリ
 */

#include "pid_control.h"

#include <math.h>

#include <src_core/System/TimeManager/time_manager.h>

#include "math_constants.h"


void PID_CONTROL_init(PidControl* pid_control)
{
  const float kMaxDtSecDefault = 10.0f;    //!< 前回の観測からの時間差の上限デフォルト値 [s]
  const float kMinDtSecDefault = 0.01f;    //!< 前回の観測からの時間差の下限デフォルト値 [s]

  pid_control->gains.p_gain = 0.0f;
  pid_control->gains.i_gain = 0.0f;
  pid_control->gains.d_gain = 0.0f;
  pid_control->error  = 0.0f;
  pid_control->i_error = 0.0f;
  pid_control->d_error = 0.0f;
  pid_control->pre_error = 0.0f;
  pid_control->control_output = 0.0f;
  pid_control->previous_obc_time = TMGR_get_master_clock();
  pid_control->dt_s = 1.0f; // 割り算をするので0にしない
  pid_control->dt_max_s = kMaxDtSecDefault;
  pid_control->dt_min_s = kMinDtSecDefault;
  pid_control->i_component_limit                = 1.0e-9f; // デフォルトでは実質的に積分が無効になるような値にする
  pid_control->acceptable_error_to_run_integral = 1.0e-9f; // デフォルトでは実質的に積分が無効になるような値にする

  return;
}

void PID_CONTROL_calc_output(PidControl* pid_control, const float error)
{
  ObcTime current_obc_time = TMGR_get_master_clock();
  pid_control->dt_s = (float)OBCT_diff_in_sec(&(pid_control->previous_obc_time), &current_obc_time);
  pid_control->previous_obc_time = current_obc_time;
  pid_control->error = error;

  // 刻み幅範囲を超えたらP制御のみとする
  if (pid_control->dt_s > pid_control->dt_max_s ||
      pid_control->dt_s < pid_control->dt_min_s)
  {
    PID_CONTROL_reset_integral_error(pid_control);

    pid_control->control_output  = pid_control->gains.p_gain * pid_control->error;
    pid_control->pre_error = pid_control->error;
    return;
  }

  // 積分偏差の処理
  // Anti-Windup処理1: 積分器使用可否判定
  if (fabsf(pid_control->error) > pid_control->acceptable_error_to_run_integral)
  {
    pid_control->i_error = 0.0f;
  }
  else
  {
    pid_control->i_error += pid_control->error * pid_control->dt_s;
    // Anti-Windup機能2: 積分偏差上限判定
    pid_control->i_error = C2A_MATH_clip_value_abs(pid_control->i_error, pid_control->i_component_limit);
  }

  // 微分偏差の処理
  pid_control->d_error = (pid_control->error - pid_control->pre_error) / pid_control->dt_s;

  // 制御量の計算
  pid_control->control_output  = pid_control->gains.p_gain * pid_control->error +
                                 pid_control->gains.i_gain * pid_control->i_error +
                                 pid_control->gains.d_gain * pid_control->d_error;
  pid_control->pre_error = pid_control->error;

  // NAN確認
  C2A_MATH_ERROR ret = C2A_MATH_check_nan_inf(pid_control->control_output);
  if (ret != C2A_MATH_ERROR_OK)
  {
    pid_control->control_output = 0.0f;
    PID_CONTROL_reset_integral_error(pid_control);
    pid_control->pre_error = 0.0f;
    // TODO: Add Event Logger
  }

  return;
}

C2A_MATH_ERROR PID_CONTROL_set_gain(PidControl* pid_control, const PidGains gains)
{
  // TODO_L 負の値を除外するアサーションを入れるか要検討
  pid_control->gains.p_gain = gains.p_gain;
  pid_control->gains.i_gain = gains.i_gain;
  pid_control->gains.d_gain = gains.d_gain;

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR PID_CONTROL_set_integral_limit(PidControl* pid_control,
                                              const float i_component_limit,
                                              const float acceptable_error_to_run_integral)
{
  // TODO_L 負の値を除外するアサーションを入れるか要検討
  pid_control->i_component_limit = i_component_limit;
  pid_control->acceptable_error_to_run_integral = acceptable_error_to_run_integral;

  return C2A_MATH_ERROR_OK;
}

void PID_CONTROL_reset_integral_error(PidControl* pid_control)
{
  pid_control->i_error = 0.0f;
  return;
}

#pragma section
