#pragma section REPRO
/**
* @file   stt_gyro_ekf.c
* @brief  STTとジャイロでEKFを組み，高精度姿勢決定を行うアプリ
*/

#include <math.h>

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/EventManager/event_logger.h>

#include "stt_gyro_ekf.h"
#include "../aocs_manager.h"
#include "../../../app_registry.h"
#include "../../../../Library/vector3.h"
#include "../../../DriverInstances/di_stim210.h"
#include "../../../DriverInstances/di_sagitta.h"
#include "../HardwareDependent/SensorFilters/sagitta_filter.h"
#include "../../../../Library/math_constants.h"

MATRIX_DEFINE_MATRIX_SRTUCT(6, 3, float);
MATRIX_DEFINE_MATRIX_SRTUCT(6, 1, float);
MATRIX_DEFINE_MATRIX_SRTUCT(3, 6, float);
MATRIX_DEFINE_MATRIX_SRTUCT(3, 3, float);
MATRIX_DEFINE_MATRIX_SRTUCT(3, 1, float);

static SttGyroEkf        stt_gyro_ekf_;
const  SttGyroEkf* const stt_gyro_ekf = &stt_gyro_ekf_;

static void APP_STT_GYRO_EKF_init_(void);
static void APP_STT_GYRO_EKF_exec_(void);

/**
 * @struct CalculationTime
 * @brief  EKF伝播処理に使う計算時間刻み
 */
typedef struct
{
  ObcTime previous; //!< 1ステップ前のOBC Time
  ObcTime current;  //!< 現在のOBC Time
} CalculationTime;

/**
 * @struct PropagatedStateVariable
 * @brief  伝搬された状態変数
 */
typedef struct
{
  Quaternion quaternion_i2b;  //!< 伝搬された後の姿勢quaternion
  float rate_bias_body_rad_s[PHYSICAL_CONST_THREE_DIM];  //!< 伝搬された後のレートバイアス
} PropagatedStateVariable;

/**
 * @struct UpdatedStateVariable
 * @brief  観測更新された状態変数
 */
typedef struct
{
  Quaternion quaternion_i2b;  //!< 伝搬された後の姿勢quaternion
  float rate_bias_body_rad_s[PHYSICAL_CONST_THREE_DIM];  //!< 伝搬された後のレートバイアス
} UpdatedStateVariable;

/**
 * @struct DeltaStateVariable
 * @brief  リファレンス近傍の状態変数
 */
typedef struct
{
  Quaternion quaternion_i2b;  //!< リファレンス近傍の姿勢quaternion
  float rate_bias_body_rad_s[PHYSICAL_CONST_THREE_DIM];  //!< リファレンス近傍のレートバイアス
} DeltaStateVariableRefToTrue;

/**
 * @struct SystemMatrix
 * @brief  状態方程式におけるシステム行列
 */
typedef struct
{
  /**
  * 状態方程式の行列A
  * ( γ(ω)  0.5I  )
  * ( O    (-1/τ)I)
  */
  MATRIX_T(6, 6, float) a;
  /**
   * 状態方程式の行列B
   * ( -0.5I  O )
   * (   O    I )
   */
  MATRIX_T(6, 6, float) b;
} SystemMatrix;

/**
 * @enum   APP_STT_GYRO_EKF_STT_STATUS
 * @brief  STTからQuaternionが更新されたかどうか
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_STT_GYRO_EKF_STT_STATUS_OK = 0, //!< STTからQuaternionが更新された
  APP_STT_GYRO_EKF_STT_STATUS_NG      //!< STTからQuaternionが更新されていない
} APP_STT_GYRO_EKF_STT_STATUS;

static const float APP_STT_GYRO_EKF_kAccceptableMaxTimeStepSec_ = 1.0f; //!< 時間刻みがこれより大きい場合フィルタ計算をスキップ
static const float APP_STT_GYRO_EKF_kComputationCycle_ = 0.1f; //!< EKFアプリの計算周期．タスクリストでの実行頻度と合わせること
static float APP_STT_GYRO_EKF_rate_bias_propagation_coefficient_; //!< レートバイアス伝搬の漸化式の係数．計算速度のために固定値．
static CalculationTime APP_STT_GYRO_EKF_calculation_time_;
static uint32_t APP_STT_GYRO_EKF_before_stt_unix_time_ms_; //!< 前回のSTT unixtime
static SystemMatrix    APP_STT_GYRO_EKF_system_matrix_;
static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_process_noise_covariance_;     //!< プロセスノイズ共分散行列Q
static MATRIX_T(3, 3, float) APP_STT_GYRO_EKF_observation_noise_covariance_; //!< 観測ノイズ共分散行列R
static MATRIX_T(3, 6, float) APP_STT_GYRO_EKF_observation_matrix_; //!< 観測行列H

/**
* @brief  EKFにおける状態量伝搬を実行する
* @param  time_step_s: 時間刻み
*/
static PropagatedStateVariable APP_STT_GYRO_EKF_propagate_state_variable_(float time_step_s);

/**
* @brief  EKFにおける共分散行列伝搬を実行する
* @param  time_step_s: 時間刻み
*/
static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_propagate_covariance_(float time_step_s);

/**
* @brief  共分散行列の更新：P(t+1)=FP(t)F^t+GQG^tにおける，FP(t)F^tの計算
* @param  time_step_s: 計算周期
* @return 行列FP(t)F^t
*/
static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_calculate_fpft_(float time_step_s);

/**
* @brief 共分散行列の更新：P(t+1)=FP(t)F^t+GQG^tにおける，GQG^tの計算
* @param  time_step_s: 計算周期
* @return 行列GQG^t
*/
static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_calculate_gqgt_(float time_step_s);

/**
* @brief      カルマンゲインの計算
* @param[in]  伝播された共分散行列P~(t+1)
* @param[out] カルマンゲイン
* @return     APP_STT_GYRO_EKF_CALCULATIONに準拠
*/
static APP_STT_GYRO_EKF_CALCULATION APP_STT_GYRO_EKF_calculate_kalman_gain_(MATRIX_T(6, 3, float) *kalman_gain,
                                                                            MATRIX_T(6, 6, float) propagated_covariance_matrix);

/**
* @brief  状態量の観測更新
* @param  kalman_gain: カルマンゲインK
*         propagated_state_variable: 伝播された状態量x~(t+1)
* @return 観測更新された状態量x^(t+1)
*/
static UpdatedStateVariable APP_STT_GYRO_EKF_update_state_variable_(MATRIX_T(6, 3, float) kalman_gain,
                                                                    PropagatedStateVariable propagated_state_variable);

/**
* @brief  共分散行列の観測更新
* @param  kalman_gain: カルマンゲインK
*         伝播された共分散行列P~(t+1)
* @return 観測更新された共分散行列P^(t+1)
*/
static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_update_covariance_matrix_(MATRIX_T(6, 3, float) kalman_gain,
                                                                        MATRIX_T(6, 6, float) propagated_covariance_matrix);

/**
* @brief  STTが更新されたかどうかを確認する
* @param
* @return APP_STT_GYRO_EKF_STT_STATUSに準拠
*/
static APP_STT_GYRO_EKF_STT_STATUS APP_STT_GYRO_EKF_check_stt_status_();

/**
* @brief  共分散行列Pの初期化
*/
static void APP_STT_GYRO_EKF_initialize_covariance_matrix_(void);

/**
* @brief  システム行列Aの更新
*/
static void APP_STT_GYRO_EKF_update_system_matrix_(void);

/**
* @brief  姿勢推定をリセットして推定をやり直す
* @note   デフォルトでは，姿勢quaternionはAOCS managerから取ってきて初期化する．
*/
static void APP_STT_GYRO_EKF_reset_estimation_(void);

AppInfo APP_STT_GYRO_EKF_create_app(void)
{
  return AI_create_app_info("SttGyroEkf", APP_STT_GYRO_EKF_init_, APP_STT_GYRO_EKF_exec_);
}

static void APP_STT_GYRO_EKF_init_(void)
{
  APP_STT_GYRO_EKF_process_noise_covariance_ = MATRIX_MAKE_INIT_MATRIX(6, 6, float);
  // 姿勢変化は制御周期0.1秒間での軌道位相変化と同程度とし，分散はその2乗になる．
  const float kOrbitPeriodSec = 5700.0f; //!< 軌道周期
  stt_gyro_ekf_.process_noise_covariance.attitude_element =
    powf((MATH_CONST_2PI / kOrbitPeriodSec * APP_STT_GYRO_EKF_kComputationCycle_), 2.0f);
  // 姿勢レート変化はどの程度の値にするか未定．
  stt_gyro_ekf_.process_noise_covariance.attitude_rate_element =
    powf((MATH_CONST_2PI / kOrbitPeriodSec * APP_STT_GYRO_EKF_kComputationCycle_), 2.0f);
  APP_STT_GYRO_EKF_process_noise_covariance_ = MATRIX_MAKE_INIT_MATRIX(6, 6, float);
  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    APP_STT_GYRO_EKF_process_noise_covariance_.data[i][i] = stt_gyro_ekf_.process_noise_covariance.attitude_element;
    APP_STT_GYRO_EKF_process_noise_covariance_.data[i + 3][i + 3] = stt_gyro_ekf_.process_noise_covariance.attitude_rate_element;
  }

  APP_STT_GYRO_EKF_observation_matrix_ = MATRIX_MAKE_INIT_MATRIX(3, 6, float);
  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    APP_STT_GYRO_EKF_observation_matrix_.data[i][i] = 1.0f;
  }

  APP_STT_GYRO_EKF_calculation_time_.previous = TMGR_get_master_clock();
  APP_STT_GYRO_EKF_calculation_time_.current = TMGR_get_master_clock();
  APP_STT_GYRO_EKF_before_stt_unix_time_ms_ = 0;

  stt_gyro_ekf_.estimated_result.quaternion_i2b = QUATERNION_make_unit();
  VECTOR3_initialize(stt_gyro_ekf_.estimated_result.angular_velocity_body_rad_s, 0.0f);
  VECTOR3_initialize(stt_gyro_ekf_.estimated_result.rate_bias_body_rad_s, 0.0f);

  stt_gyro_ekf_.gyro_random_noise.standard_deviation_compo_rad_s[0] = 4.363323e-5f; // STIM210ランダムノイズN：15deg/sq(h)
  stt_gyro_ekf_.gyro_random_noise.standard_deviation_compo_rad_s[1] = 4.363323e-5f; // STIM210ランダムノイズN：15deg/sq(h)
  stt_gyro_ekf_.gyro_random_noise.standard_deviation_compo_rad_s[2] = 4.363323e-5f; // STIM210ランダムノイズN：15deg/sq(h)
  QUATERNION_trans_coordinate(stt_gyro_ekf_.gyro_random_noise.standard_deviation_body_rad_s,
    stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b,
    stt_gyro_ekf_.gyro_random_noise.standard_deviation_compo_rad_s);

  // ランダムウォークノイズ：STIM210 bias stability B = 0.3deg/h
  stt_gyro_ekf_.gyro_random_walk.standard_deviation_compo_rad_s2[0] = 7.272205e-8f; // K=3/2*B^2/Nとして計算
  stt_gyro_ekf_.gyro_random_walk.standard_deviation_compo_rad_s2[1] = 7.272205e-8f; // K=3/2*B^2/Nとして計算
  stt_gyro_ekf_.gyro_random_walk.standard_deviation_compo_rad_s2[2] = 7.272205e-8f; // K=3/2*B^2/Nとして計算
  QUATERNION_trans_coordinate(stt_gyro_ekf_.gyro_random_walk.standard_deviation_body_rad_s2,
    stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b,
    stt_gyro_ekf_.gyro_random_walk.standard_deviation_compo_rad_s2);

  // STIM210のノイズは，ECRVよりは単純なランダムウォークノイズとしてモデル化するのが適切．
  // ECRV時定数->無限大の極限がランダムウォークノイズと一致するので，今回は大きな値として10^9で設定する．
  stt_gyro_ekf_.gyro_random_walk.time_constant_s = 1.0E9f;

  APP_STT_GYRO_EKF_system_matrix_.a = MATRIX_MAKE_INIT_MATRIX(6, 6, float);
  APP_STT_GYRO_EKF_system_matrix_.a.data[0][3] = -0.5f;
  APP_STT_GYRO_EKF_system_matrix_.a.data[1][4] = -0.5f;
  APP_STT_GYRO_EKF_system_matrix_.a.data[2][5] = -0.5f;
  APP_STT_GYRO_EKF_system_matrix_.a.data[3][3] = -1.0f / stt_gyro_ekf_.gyro_random_walk.time_constant_s;
  APP_STT_GYRO_EKF_system_matrix_.a.data[4][4] = -1.0f / stt_gyro_ekf_.gyro_random_walk.time_constant_s;
  APP_STT_GYRO_EKF_system_matrix_.a.data[5][5] = -1.0f / stt_gyro_ekf_.gyro_random_walk.time_constant_s;

  APP_STT_GYRO_EKF_system_matrix_.b = MATRIX_MAKE_INIT_MATRIX(6, 6, float);
  APP_STT_GYRO_EKF_system_matrix_.b.data[0][0] = -0.5f;
  APP_STT_GYRO_EKF_system_matrix_.b.data[1][1] = -0.5f;
  APP_STT_GYRO_EKF_system_matrix_.b.data[2][2] = -0.5f;
  APP_STT_GYRO_EKF_system_matrix_.b.data[3][3] = 1.0f;
  APP_STT_GYRO_EKF_system_matrix_.b.data[4][4] = 1.0f;
  APP_STT_GYRO_EKF_system_matrix_.b.data[5][5] = 1.0f;

  stt_gyro_ekf_.stt_error.standard_deviation_compo_rad[0] = 4.8481e-5f; // STT roll方向精度：10秒角
  stt_gyro_ekf_.stt_error.standard_deviation_compo_rad[1] = 9.6963e-6f; // STT cross方向精度：2秒角
  stt_gyro_ekf_.stt_error.standard_deviation_compo_rad[2] = 9.6963e-6f; // STT cross方向精度：2秒角
  QUATERNION_trans_coordinate(stt_gyro_ekf_.stt_error.standard_deviation_body_rad,
    sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.frame_transform_c2b,
    stt_gyro_ekf_.stt_error.standard_deviation_compo_rad);

  stt_gyro_ekf_.initial_covariance.diagonal_component_stt[0] =
    powf(stt_gyro_ekf_.stt_error.standard_deviation_body_rad[0], 2.0f);
  stt_gyro_ekf_.initial_covariance.diagonal_component_stt[1] =
    powf(stt_gyro_ekf_.stt_error.standard_deviation_body_rad[1], 2.0f);
  stt_gyro_ekf_.initial_covariance.diagonal_component_stt[2] =
    powf(stt_gyro_ekf_.stt_error.standard_deviation_body_rad[2], 2.0f);
  stt_gyro_ekf_.initial_covariance.diagonal_component_gyro   =
    powf(stt_gyro_ekf_.gyro_random_noise.standard_deviation_body_rad_s[0], 2.0f);
  APP_STT_GYRO_EKF_initialize_covariance_matrix_();

  stt_gyro_ekf_.use_ekf_estimated_attitude = APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE_DISABLE;
  stt_gyro_ekf_.calculation_state = APP_STT_GYRO_EKF_CALCULATION_DIVERGED;

  APP_STT_GYRO_EKF_observation_noise_covariance_ = MATRIX_MAKE_INIT_MATRIX(3, 3, float);
  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    APP_STT_GYRO_EKF_observation_noise_covariance_.data[i][i] =
      powf(stt_gyro_ekf_.stt_error.standard_deviation_body_rad[i], 2.0f);
  }

  APP_STT_GYRO_EKF_rate_bias_propagation_coefficient_ =
    expf(-1.0f / stt_gyro_ekf_.gyro_random_walk.time_constant_s * APP_STT_GYRO_EKF_kComputationCycle_);
}

static void APP_STT_GYRO_EKF_exec_(void)
{
  // 時間刻みを計算する
  APP_STT_GYRO_EKF_calculation_time_.current = TMGR_get_master_clock();
  float time_step_s = (float)OBCT_diff_in_sec(&(APP_STT_GYRO_EKF_calculation_time_.previous),
    &(APP_STT_GYRO_EKF_calculation_time_.current));
  APP_STT_GYRO_EKF_calculation_time_.previous = APP_STT_GYRO_EKF_calculation_time_.current;

  APP_STT_GYRO_EKF_execute_estimation(time_step_s);
}

void APP_STT_GYRO_EKF_execute_estimation(float time_step_s)
{
  if (time_step_s > APP_STT_GYRO_EKF_kAccceptableMaxTimeStepSec_)
  {
    // フィルタ計算を回し始めた初回や，別のモードに遷移したあと戻ってきた初回などは，time_step_sが大きな値になっている．
    // これらのケースでは，状態変数や推定結果に，ある程度正しい値を入れて初期化しておく．
    // そのようにすることで，次回のEKFアプリ実行時に真値に近いところから計算を始められるようになる．
    APP_STT_GYRO_EKF_reset_estimation_();
    return;
  }

  if (time_step_s <= 0.0f)
  {
    // time_step_sが0以下の場合，フィルタ計算がおかしくなるため，このときも一度初期化する．
    // これは想定しない挙動なので，計算時EL登録も行う．
    APP_STT_GYRO_EKF_reset_estimation_();
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_STT_GYRO_EKF,
      EL_ERROR_LEVEL_LOW, (uint32_t)C2A_MATH_ERROR_OTHERS);
    return;
  }

  //!< 伝播された状態量x~(t+1)
  PropagatedStateVariable propagated_state_variable = APP_STT_GYRO_EKF_propagate_state_variable_(time_step_s);

  //!< 伝播された共分散行列P~(t+1)
  MATRIX_T(6, 6, float) propagated_covariance_matrix = APP_STT_GYRO_EKF_propagate_covariance_(time_step_s);

  //!< 観測更新された状態量x^(t+1)
  UpdatedStateVariable updated_state_variable;

  //!< 観測更新された共分散行列P^(t+1)
  MATRIX_T(6, 6, float) updated_covariance_matrix;

  // STTが更新されたときに，カルマンフィルタでも観測更新をする．
  APP_STT_GYRO_EKF_STT_STATUS stt_status = APP_STT_GYRO_EKF_check_stt_status_();
  if (stt_status == APP_STT_GYRO_EKF_STT_STATUS_OK)
  {
    //!< カルマンゲインK
    MATRIX_T(6, 3, float) kalman_gain;
    stt_gyro_ekf_.calculation_state = APP_STT_GYRO_EKF_calculate_kalman_gain_(&kalman_gain, propagated_covariance_matrix);

    // カルマンゲインを計算する過程で発散した場合，以降の計算は行わず，推定をリセットする
    if (stt_gyro_ekf_.calculation_state == APP_STT_GYRO_EKF_CALCULATION_DIVERGED)
    {
      APP_STT_GYRO_EKF_reset_estimation_();
      return;
    }

    updated_state_variable = APP_STT_GYRO_EKF_update_state_variable_(kalman_gain, propagated_state_variable);
    updated_covariance_matrix = APP_STT_GYRO_EKF_update_covariance_matrix_(kalman_gain, propagated_covariance_matrix);

    // 更新した結果を，推定結果に代入する
    stt_gyro_ekf_.estimated_result.quaternion_i2b = updated_state_variable.quaternion_i2b;
    VECTOR3_copy(stt_gyro_ekf_.estimated_result.rate_bias_body_rad_s, updated_state_variable.rate_bias_body_rad_s);
    VECTOR3_subtract(stt_gyro_ekf_.estimated_result.angular_velocity_body_rad_s,
      aocs_manager->ang_vel_est_body_rad_s,
      updated_state_variable.rate_bias_body_rad_s);
    stt_gyro_ekf_.estimated_result.covariance_matrix = updated_covariance_matrix;
  }
  else
  {
    // STTが更新されなかったときは，伝播された量をそのまま推定結果とする．
    stt_gyro_ekf_.estimated_result.quaternion_i2b = propagated_state_variable.quaternion_i2b;
    VECTOR3_copy(stt_gyro_ekf_.estimated_result.rate_bias_body_rad_s, propagated_state_variable.rate_bias_body_rad_s);
    VECTOR3_subtract(stt_gyro_ekf_.estimated_result.angular_velocity_body_rad_s,
      aocs_manager->ang_vel_est_body_rad_s,
      propagated_state_variable.rate_bias_body_rad_s);
    stt_gyro_ekf_.estimated_result.covariance_matrix = propagated_covariance_matrix;
  }
}

static PropagatedStateVariable APP_STT_GYRO_EKF_propagate_state_variable_(float time_step_s)
{
  PropagatedStateVariable propagated_state_variable;
  propagated_state_variable.quaternion_i2b =
    QUATERNION_euler_propagate(stt_gyro_ekf_.estimated_result.quaternion_i2b,
    stt_gyro_ekf_.estimated_result.angular_velocity_body_rad_s, time_step_s);
  VECTOR3_scalar_product(propagated_state_variable.rate_bias_body_rad_s,
                         APP_STT_GYRO_EKF_rate_bias_propagation_coefficient_,
                         stt_gyro_ekf_.estimated_result.rate_bias_body_rad_s);

  return propagated_state_variable;
}

static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_propagate_covariance_(float time_step_s)
{
  MATRIX_T(6, 6, float) propagated_covariance = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< P(t+1)

  MATRIX_T(6, 6, float) fpft = APP_STT_GYRO_EKF_calculate_fpft_(time_step_s); //!< FPF^T
  MATRIX_T(6, 6, float) gqgt = APP_STT_GYRO_EKF_calculate_gqgt_(time_step_s); //!< GQG^T
  MATRIX_add(&propagated_covariance, &fpft, &gqgt);

  return propagated_covariance;
}

static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_calculate_fpft_(float time_step_s)
{
  APP_STT_GYRO_EKF_update_system_matrix_();
  MATRIX_T(6, 6, float) f = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< 行列F
  MATRIX_T(6, 6, float) unit_matrix = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< 単位行列I
  for (size_t i = 0; i < 6; i++)
  {
    unit_matrix.data[i][i] = 1.0f;
  }
  MATRIX_T(6, 6, float) dta  = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< ΔtA
  MATRIX_T(6, 6, float) ft   = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< F^T
  MATRIX_T(6, 6, float) pft  = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< PF^T
  MATRIX_T(6, 6, float) fpft = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< FPF^T

  MATRIX_multiply_scalar(&dta, time_step_s, &APP_STT_GYRO_EKF_system_matrix_.a);
  MATRIX_add(&f, &unit_matrix, &dta); // TODO_L: この近似でよいのかは確認する必要がある．
  MATRIX_transpose(&ft, &f);
  MATRIX_multiply(&pft, &stt_gyro_ekf_.estimated_result.covariance_matrix, &ft);
  MATRIX_multiply(&fpft, &f, &pft);

  return fpft;
}

static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_calculate_gqgt_(float time_step_s)
{
  MATRIX_T(6, 6, float) g    = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< 行列G
  MATRIX_T(6, 6, float) gt   = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< G^T
  MATRIX_T(6, 6, float) qgt  = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< QG^T
  MATRIX_T(6, 6, float) gqgt = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< GQG^T

  MATRIX_multiply_scalar(&g, time_step_s, &APP_STT_GYRO_EKF_system_matrix_.b);

  MATRIX_transpose(&gt, &g);
  MATRIX_multiply(&qgt, &APP_STT_GYRO_EKF_process_noise_covariance_, &gt);
  MATRIX_multiply(&gqgt, &g, &qgt);

  return gqgt;
}

static APP_STT_GYRO_EKF_CALCULATION APP_STT_GYRO_EKF_calculate_kalman_gain_(MATRIX_T(6, 3, float)  *kalman_gain,
                                                                            MATRIX_T(6, 6, float)  propagated_covariance_matrix)
{
  *kalman_gain = MATRIX_MAKE_INIT_MATRIX(6, 3, float); //!< カルマンゲインK

  // 中間変数
  MATRIX_T(6, 3, float) ht           = MATRIX_MAKE_INIT_MATRIX(6, 3, float); //!< H^T
  MATRIX_T(6, 3, float) pht          = MATRIX_MAKE_INIT_MATRIX(6, 3, float); //!< PH^T
  MATRIX_T(3, 3, float) hpht         = MATRIX_MAKE_INIT_MATRIX(3, 3, float); //!< HPH^T
  MATRIX_T(3, 3, float) hpht_r       = MATRIX_MAKE_INIT_MATRIX(3, 3, float); //!< HPH^T+R
  MATRIX_T(3, 3, float) s_hpht_r     = MATRIX_MAKE_INIT_MATRIX(3, 3, float); //!< s(HPH^T+R) (s: スカラー)
  MATRIX_T(3, 3, float) s_hpht_r_inv = MATRIX_MAKE_INIT_MATRIX(3, 3, float); //!< (1/s)(HPH^T+R)^-1 (s: スカラー)
  MATRIX_T(3, 3, float) hpht_r_inv   = MATRIX_MAKE_INIT_MATRIX(3, 3, float); //!< (HPH^T+R)^-1

  // カルマンゲインの計算
  MATRIX_transpose(&ht, &APP_STT_GYRO_EKF_observation_matrix_);
  MATRIX_multiply(&pht, &propagated_covariance_matrix, &ht);
  MATRIX_multiply(&hpht, &APP_STT_GYRO_EKF_observation_matrix_, &pht);
  MATRIX_add(&hpht_r, &hpht, &APP_STT_GYRO_EKF_observation_noise_covariance_);

  // hpht_rは，各成分が1E-10程度のオーダーになることが予想され，逆行列計算時に値が小さすぎるというエラーが出ることがある．
  // これに対応するため，いったんhpht_rをスカラー倍して1程度のオーダーに直したあとに逆行列計算する．
  float scale_factor = hpht_r.data[0][0]; //!< s
  MATRIX_multiply_scalar(&s_hpht_r, (1.0f / scale_factor), &hpht_r);

  MATRIX_ERROR ret;
  ret = MATRIX_inverse(&s_hpht_r_inv, &s_hpht_r);
  if (ret != MATRIX_ERROR_OK)
  {
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_STT_GYRO_EKF,
                    EL_ERROR_LEVEL_LOW, (uint32_t)C2A_MATH_ERROR_SINGULAR);
    return APP_STT_GYRO_EKF_CALCULATION_DIVERGED;
  }

  MATRIX_multiply_scalar(&hpht_r_inv, (1.0f / scale_factor), &s_hpht_r_inv);
  MATRIX_multiply(&(*kalman_gain), &pht, &hpht_r_inv);
  return APP_STT_GYRO_EKF_CALCULATION_CONVERGED;
}

static UpdatedStateVariable APP_STT_GYRO_EKF_update_state_variable_(MATRIX_T(6, 3, float) kalman_gain,
                                                                    PropagatedStateVariable propagated_state_variable)
{
  // 観測量の計算
  // 観測量は微小クオータニオンのベクトル部
  // q_refには，前回EKFアプリが推定した姿勢quaternionを入れる．
  // aocs managerのq_estには，フィルタをかけた後のSTT出力が入っている

  //!< (q_ref)^-1 * q_sttで表される観測量の，ベクトル部
  MATRIX_T(3, 1, float) observed_variable = MATRIX_MAKE_INIT_MATRIX(3, 1, float);
  Quaternion quaternion_ref_inv = QUATERNION_inverse(stt_gyro_ekf_.estimated_result.quaternion_i2b); //!< (q_ref)^-1
  Quaternion observed_delta_quaternion = QUATERNION_product(quaternion_ref_inv, aocs_manager->quaternion_est_i2b);
  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    observed_variable.data[i][0] = observed_delta_quaternion.vector_part[i];
  }

  MATRIX_T(6, 1, float) updated_state_variable_matrix_form = MATRIX_MAKE_INIT_MATRIX(6, 1, float); //!< 推定結果（行列型）
  MATRIX_multiply(&updated_state_variable_matrix_form, &kalman_gain, &observed_variable);

  // updated_state_variable_matrix_formから，計算に使えるように値を取り出す
  Quaternion delta_quaternion_i2b_ref_to_true = QUATERNION_make_unit(); //!< リファレンス値近傍の微小quaternion q'
  float delta_rate_bias_ref_to_true_rad_s[PHYSICAL_CONST_THREE_DIM]; //!< リファレンス値近傍の微小レートバイアスb'
  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    delta_quaternion_i2b_ref_to_true.vector_part[i] = updated_state_variable_matrix_form.data[i][0];
    delta_rate_bias_ref_to_true_rad_s[i] = updated_state_variable_matrix_form.data[i + 3][0];
  }
  delta_quaternion_i2b_ref_to_true.scalar_part = sqrtf(1.0f - VECTOR3_norm(delta_quaternion_i2b_ref_to_true.vector_part));

  UpdatedStateVariable updated_state_variable;
  updated_state_variable.quaternion_i2b =
    QUATERNION_product(propagated_state_variable.quaternion_i2b, delta_quaternion_i2b_ref_to_true);
  VECTOR3_add(updated_state_variable.rate_bias_body_rad_s,
    propagated_state_variable.rate_bias_body_rad_s, delta_rate_bias_ref_to_true_rad_s);

  return updated_state_variable;
}

static MATRIX_T(6, 6, float) APP_STT_GYRO_EKF_update_covariance_matrix_(MATRIX_T(6, 3, float) kalman_gain,
                                                                        MATRIX_T(6, 6, float) propagated_covariance_matrix)
{
  MATRIX_T(6, 6, float) updated_covariance_matrix = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< 観測更新された共分散行列P^(t+1)

  // 中間変数
  MATRIX_T(3, 6, float) hp  = MATRIX_MAKE_INIT_MATRIX(3, 6, float); //!< KH
  MATRIX_T(6, 6, float) khp = MATRIX_MAKE_INIT_MATRIX(6, 6, float); //!< KHP

  MATRIX_multiply(&hp, &APP_STT_GYRO_EKF_observation_matrix_, &propagated_covariance_matrix);
  MATRIX_multiply(&khp, &kalman_gain, &hp);
  MATRIX_subtract(&updated_covariance_matrix, &propagated_covariance_matrix, &khp);

  return updated_covariance_matrix;
}

static APP_STT_GYRO_EKF_STT_STATUS APP_STT_GYRO_EKF_check_stt_status_()
{
  uint32_t current_stt_unixtime = sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.unix_time_ms;
  uint32_t unixtime_step_ms = current_stt_unixtime - APP_STT_GYRO_EKF_before_stt_unix_time_ms_;
  APP_STT_GYRO_EKF_before_stt_unix_time_ms_ = current_stt_unixtime;

  // unixtimeが更新されていないなら，STTから新しいQuaternionは来ていないため更新はできていないと判断する
  if (unixtime_step_ms <= 0)
  {
    return APP_STT_GYRO_EKF_STT_STATUS_NG;
  }

  // STTのQuaternionの状態がNGなら，Quaternionは正しく更新されていないとみて更新できなかったと判断する
  if (aocs_manager->stt_availability == AOCS_MANAGER_STT_UNAVAILABLE)
  {
    return APP_STT_GYRO_EKF_STT_STATUS_NG;
  }

  // STTのスパイクフィルタが異常を検知した場合，正しくQuaternionが更新できなかったと判断する
  if (sagitta_filter->q_i2b_filter_error != C2A_MATH_ERROR_OK)
  {
    return APP_STT_GYRO_EKF_STT_STATUS_NG;
  }

  return APP_STT_GYRO_EKF_STT_STATUS_OK;
}

static void APP_STT_GYRO_EKF_initialize_covariance_matrix_(void)
{
  // 一旦すべての成分を0で初期化する
  stt_gyro_ekf_.estimated_result.covariance_matrix = MATRIX_MAKE_INIT_MATRIX(6, 6, float);

  // 上側対角成分を初期化
  for (size_t i = 0; i < 3; i++)
  {
    stt_gyro_ekf_.estimated_result.covariance_matrix.data[i][i] =
      stt_gyro_ekf_.initial_covariance.diagonal_component_stt[i];
  }
  // 下側対角成分を初期化
  for (size_t i = 3; i < 6; i++)
  {
    stt_gyro_ekf_.estimated_result.covariance_matrix.data[i][i] =
      stt_gyro_ekf_.initial_covariance.diagonal_component_gyro;
  }
}

static void APP_STT_GYRO_EKF_reset_estimation_(void)
{
  // デフォルトでは，AOCS managerからquaternionを取ってきて推定をやり直す
  stt_gyro_ekf_.estimated_result.quaternion_i2b = aocs_manager->quaternion_est_i2b;
  VECTOR3_initialize(stt_gyro_ekf_.estimated_result.rate_bias_body_rad_s, 0.0f);

  APP_STT_GYRO_EKF_initialize_covariance_matrix_();
}

static void APP_STT_GYRO_EKF_update_system_matrix_(void)
{
  APP_STT_GYRO_EKF_system_matrix_.a.data[0][1] =  aocs_manager->ang_vel_est_body_rad_s[2];
  APP_STT_GYRO_EKF_system_matrix_.a.data[0][2] = -aocs_manager->ang_vel_est_body_rad_s[1];
  APP_STT_GYRO_EKF_system_matrix_.a.data[1][0] = -aocs_manager->ang_vel_est_body_rad_s[2];
  APP_STT_GYRO_EKF_system_matrix_.a.data[1][2] =  aocs_manager->ang_vel_est_body_rad_s[0];
  APP_STT_GYRO_EKF_system_matrix_.a.data[2][0] =  aocs_manager->ang_vel_est_body_rad_s[1];
  APP_STT_GYRO_EKF_system_matrix_.a.data[2][1] = -aocs_manager->ang_vel_est_body_rad_s[0];
}

CCP_CmdRet Cmd_STT_GYRO_EKF_SET_GYRO_RANDOM_NOISE_STANDARD_DEVIATION(const CommonCmdPacket* packet)
{
  float gyro_random_noise_standard_deviation_rad_s[PHYSICAL_CONST_THREE_DIM];
  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    gyro_random_noise_standard_deviation_rad_s[i] = CCP_get_param_from_packet(packet, i, float);
    if (gyro_random_noise_standard_deviation_rad_s[i] < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  VECTOR3_copy(stt_gyro_ekf_.gyro_random_noise.standard_deviation_compo_rad_s, gyro_random_noise_standard_deviation_rad_s);

  QUATERNION_trans_coordinate(stt_gyro_ekf_.gyro_random_noise.standard_deviation_body_rad_s,
                              stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b,
                              stt_gyro_ekf_.gyro_random_noise.standard_deviation_compo_rad_s);
  // パラメータが更新されたので，姿勢推定をリセットして再度新しいパラメータで推定をやり直す
  APP_STT_GYRO_EKF_reset_estimation_();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_STT_GYRO_EKF_SET_GYRO_RANDOM_WALK_STANDARD_DEVIATION(const CommonCmdPacket* packet)
{
  size_t arg_num;
  float gyro_random_walk_standard_deviation_rad_s2[PHYSICAL_CONST_THREE_DIM];

  for (arg_num = 0; arg_num < PHYSICAL_CONST_THREE_DIM; arg_num++)
  {
    gyro_random_walk_standard_deviation_rad_s2[arg_num] = CCP_get_param_from_packet(packet, arg_num, float);
    if (gyro_random_walk_standard_deviation_rad_s2[arg_num] < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  float gyro_random_walk_time_constant_s = CCP_get_param_from_packet(packet, arg_num, float);
  if (gyro_random_walk_time_constant_s < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  VECTOR3_copy(stt_gyro_ekf_.gyro_random_walk.standard_deviation_compo_rad_s2, gyro_random_walk_standard_deviation_rad_s2);
  QUATERNION_trans_coordinate(stt_gyro_ekf_.gyro_random_walk.standard_deviation_body_rad_s2,
                              stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b,
                              stt_gyro_ekf_.gyro_random_walk.standard_deviation_compo_rad_s2);

  stt_gyro_ekf_.gyro_random_walk.time_constant_s = gyro_random_walk_time_constant_s;

  // パラメータが更新されたので，姿勢推定をリセットして再度新しいパラメータで推定をやり直す
  APP_STT_GYRO_EKF_reset_estimation_();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_STT_GYRO_EKF_SET_STT_ERROR_STANDARD_DEVIATION(const CommonCmdPacket* packet)
{
  float stt_error_standard_deviation_rad[PHYSICAL_CONST_THREE_DIM];

  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    stt_error_standard_deviation_rad[i] = CCP_get_param_from_packet(packet, i, float);
    if (stt_error_standard_deviation_rad[i] < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  VECTOR3_copy(stt_gyro_ekf_.stt_error.standard_deviation_compo_rad, stt_error_standard_deviation_rad);

  QUATERNION_trans_coordinate(stt_gyro_ekf_.stt_error.standard_deviation_body_rad,
                              sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.frame_transform_c2b,
                              stt_gyro_ekf_.stt_error.standard_deviation_compo_rad);

  for (size_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    APP_STT_GYRO_EKF_observation_noise_covariance_.data[i][i] =
      powf(stt_gyro_ekf_.stt_error.standard_deviation_body_rad[i], 2.0f);
  }

  // パラメータが更新されたので，姿勢推定をリセットして再度新しいパラメータで推定をやり直す
  APP_STT_GYRO_EKF_reset_estimation_();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_STT_GYRO_EKF_RESET_ESTIMATION(const CommonCmdPacket* packet)
{
  uint8_t reset_mode = CCP_get_param_from_packet(packet, 0, uint8_t);

  switch (reset_mode)
  {
  case 0:
    // 姿勢quaternionのみ別途単位quaternionで初期化する
    stt_gyro_ekf_.estimated_result.quaternion_i2b = QUATERNION_make_unit();
    break;
  case 1:
    stt_gyro_ekf_.estimated_result.quaternion_i2b = aocs_manager->quaternion_est_i2b;
    break;
  case 2:
    {
      float init_quaternion_i2b_array[PHYSICAL_CONST_QUATERNION_DIM];
      for (size_t i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
      {
        init_quaternion_i2b_array[i] = CCP_get_param_from_packet(packet, i + 1, float);
      }
      Quaternion init_quaternion_i2b;
      // 一旦，入力が正規化されているかどうか確認し，正規化されていないならreturnする
      QUATERNION_make_from_array_unnormalizing(&init_quaternion_i2b, init_quaternion_i2b_array, QUATERNION_SCALAR_POSITION_LAST);
      if (QUATERNION_is_normalized(init_quaternion_i2b) != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

      // returnされなければ，入力が正規化されていることが保証されているため，改めて正規化はしない
      stt_gyro_ekf_.estimated_result.quaternion_i2b = init_quaternion_i2b;
    }
    break;
  default:
    // NOT REACHED
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  // Quaternion以外の要素（角速度，共分散行列）の初期化
  VECTOR3_initialize(stt_gyro_ekf_.estimated_result.rate_bias_body_rad_s, 0.0f);
  APP_STT_GYRO_EKF_initialize_covariance_matrix_();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_STT_GYRO_EKF_ENABLE_ESTIMATED_ATTITUDE(const CommonCmdPacket* packet)
{
  uint8_t use_ekf_estimated_quaternion = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (use_ekf_estimated_quaternion > APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE_MAX)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
  stt_gyro_ekf_.use_ekf_estimated_attitude = (APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE)use_ekf_estimated_quaternion;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_STT_GYRO_EKF_SET_INITIAL_COVARIANCE_MATRIX(const CommonCmdPacket* packet)
{
  size_t arg_num;
  for (arg_num = 0; arg_num < PHYSICAL_CONST_THREE_DIM; arg_num++)
  {
    float initial_covarinace_upper_diagonal_component = CCP_get_param_from_packet(packet, arg_num, float);
    if (initial_covarinace_upper_diagonal_component < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    stt_gyro_ekf_.initial_covariance.diagonal_component_stt[arg_num] = initial_covarinace_upper_diagonal_component;
  }

  float initial_covarinace_lower_diagonal_component = CCP_get_param_from_packet(packet, arg_num, float);
  if (initial_covarinace_lower_diagonal_component < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  stt_gyro_ekf_.initial_covariance.diagonal_component_gyro = initial_covarinace_lower_diagonal_component;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_STT_GYRO_EKF_SET_PROCESS_NOISE_MATRIX(const CommonCmdPacket* packet)
{
  float process_noise_covariance_attitude_element = CCP_get_param_from_packet(packet, 0, float);
  if (process_noise_covariance_attitude_element < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  stt_gyro_ekf_.process_noise_covariance.attitude_element = process_noise_covariance_attitude_element;

  float process_noise_covariance_attitude_rate_element = CCP_get_param_from_packet(packet, 1, float);
  if (process_noise_covariance_attitude_rate_element < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  stt_gyro_ekf_.process_noise_covariance.attitude_rate_element = process_noise_covariance_attitude_rate_element;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
