/**
 * @file
 * @brief  PID制御を行うライブラリ
 */
#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_

#include "physical_constants.h"
#include "c2a_math.h"

#include <src_core/System/TimeManager/time_manager.h>


/**
 * @struct PidGain
 * @brief  PID制御ゲインの構造体
 * @note   特に無し
 */
typedef struct
{
  float p_gain; //!< PID制御の偏差にかけるゲイン
  float i_gain; //!< PID制御の偏差の積分値にかけるゲイン
  float d_gain; //!< PID制御の偏差の微分値にかけるゲイン
} PidGains;

/**
 * @struct PidControl
 * @brief  PID制御パラメータの構造体
 * @note   各軸で一つこの構造体を定義することを想定
 */
typedef struct
{
  PidGains gains;              //!< ゲイン
  float error;                //!< PID制御の偏差
  float i_error;              //!< PID制御の偏差の積分値
  float d_error;              //!< PID制御の偏差の微分値
  float pre_error;            //!< 前回の観測におけるPID制御の偏差
  float control_output;       //!< PID制御の制御出力値
  ObcTime previous_obc_time;  //!< 前回の観測における時間を格納する構造体
  float dt_s;                 //!< 前回の観測からの時間差 [s]
  float dt_min_s;             //!< 前回の観測からの時間差の最小値 [s]
  float dt_max_s;             //!< 前回の観測からの時間差の最大値 [s]

  // 下記の値はAnti-Windup用変数だが，これらの設定値がゼロ以下の場合，Anti-WindupをDisableする
  float i_component_limit;                //!< 積分偏差の上限値 (これ以上の積分偏差はFB出力に加算されない)
  float acceptable_error_to_run_integral; //!< 状態偏差絶対値がこの範囲内に収まった時のみ積分器の使用を許可する

} PidControl;


/**
 * @brief  PID制御則各種パラメータ初期化関数
 * @param[in,out]  pid_control : PidControl構造体へのポインタ
 * @return void
 */
void PID_CONTROL_init(PidControl* pid_control);

/**
 * @brief  PID制御則の出力を計算する関数
 * @param[in,out] pid_control : PidControl構造体へのポインタ
 * @param[in] error : PIDの目標値との偏差
 * @return void
 */
void PID_CONTROL_calc_output(PidControl* pid_control, const float error);

/**
 * @brief  PID制御則ゲイン設定関数
 * @param[out] pid_control : PidControl構造体へのポインタ
 * @param[in]  gains : ゲイン
 * @return C2A_MATH_ERRORに準拠
 */
C2A_MATH_ERROR PID_CONTROL_set_gain(PidControl* pid_control, const PidGains gains);

/**
 * @brief  積分誤差上限値設定関数
 * @param[out] pid_control                      : PidControl構造体へのポインタ
 * @param[in]  i_component_limit                : 積分誤差絶対値がこの値にならないようにクリップする
 * @param[in]  acceptable_error_to_run_integral : 積分前の偏差絶対値がこの値を超えたら積分をリセットする
 * @return C2A_MATH_ERRORに準拠
 */
C2A_MATH_ERROR PID_CONTROL_set_integral_limit(PidControl* pid_control,
                                              const float i_component_limit,
                                              const float acceptable_error_to_run_integral);

/**
 * @brief  積分誤差リセット関数
 * @param[out] pid_control : PidControl構造体へのポインタ
 * @return void
 */
void PID_CONTROL_reset_integral_error(PidControl* pid_control);

#endif
