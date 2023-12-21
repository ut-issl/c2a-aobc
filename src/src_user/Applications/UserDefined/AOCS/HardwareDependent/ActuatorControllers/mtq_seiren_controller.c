#pragma section REPRO
/**
* @file   mtq_seiren_controller.c
* @brief  aocs_managerの目標磁気情報から，MTQ_SEIRENのPWMデューティー比を計算・設定するアプリ
*/

#include "mtq_seiren_controller.h"

#include <math.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/c2a_math.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/math_constants.h>
#include <src_user/Library/ControlUtility/cross_product_control.h>
#include <src_user/Applications/DriverInstances/di_mtq_seiren.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Applications/UserDefined/AOCS/ExclusiveControl/magnetic_exclusive_control_timer.h>

static MtqSeirenController        mtq_seiren_controller_;
const  MtqSeirenController* const mtq_seiren_controller = &mtq_seiren_controller_;

static void APP_MTQ_SEIREN_CONTROLLER_init_(void);
static void APP_MTQ_SEIREN_CONTROLLER_exec_(void);

/**
 * @brief  MTQ出力時間計算関数
 * @param  void
 * @return void
 * @note   AOCSマネージャーから読み込んだMTQ出力目標磁気モーメントからMTQに電流を流す時間を計算する
*/
static void APP_MTQ_SEIREN_CONTROLLER_convert_mag_moment_to_output_duration_(void);

/**
 * @brief  指令トルクの積分バッファのゼロクリア
 * @note   積分区間 dt [sec]が長すぎるが長すぎる場合，および指令角運動量を出力した際にバッファをゼロクリアする
 */
static void APP_MTQ_SEIREN_CONTROLLER_reset_integrated_trq_(void);

/**
 * @brief  指令トルクの積分による角運動量指令値の計算
 * @note   駆動 + 消磁動作中の指令トルクを積分して角運動量化することで，MTQへの指令可能タイミング以外のタイミングの計算結果も出力に反映させる
 * @note   PWM周期が遅い場合，実際のMTQ出力の単位は実質 [Am^2sec]となり，角運動量の単位となる
 */
static void APP_MTQ_SEIREN_CONTROLLER_integrate_trq_(void);


AppInfo APP_MTQ_SEIREN_CONTROLLER_create_app(void)
{
  return AI_create_app_info("MtqSeirenController", APP_MTQ_SEIREN_CONTROLLER_init_, APP_MTQ_SEIREN_CONTROLLER_exec_);
}

void APP_MTQ_SEIREN_CONTROLLER_init_(void)
{
  VECTOR3_initialize(mtq_seiren_controller_.mtq_output_duration_ms, 0.0f);
  for (size_t idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
  {
    mtq_seiren_controller_.mtq_output_polarity[idx] = MTQ_SEIREN_NO_OUTPUT;
  }

  APP_MTQ_SEIREN_CONTROLLER_reset_integrated_trq_();
  mtq_seiren_controller_.previous_trq_integration_obc_time = TMGR_get_master_clock();
  mtq_seiren_controller_.cross_product_error = CROSS_PRODUCT_CONTROL_ERROR_OK;
  mtq_seiren_controller_.cross_product_error_ratio = 0.0f;
}

void APP_MTQ_SEIREN_CONTROLLER_exec_(void)
{
  APP_MTQ_SEIREN_CONTROLLER_integrate_trq_();

  switch (magnetic_exclusive_control_timer->current_state)
  {
  case APP_MECT_STATE_OBSERVE:
    // 次にMTQを出力するとき、何秒間電流を流すかを決める
    APP_MTQ_SEIREN_CONTROLLER_convert_mag_moment_to_output_duration_();
    // 次回の出力を決めたら、トルク積分値をリセットする
    APP_MTQ_SEIREN_CONTROLLER_reset_integrated_trq_();
    break;
  case APP_MECT_STATE_CONTROL:
    // APP_MECT_STATE_OBSERVEのケースで計算していた出力時間の分だけMTQを出力する
    for (size_t idx = 0; idx < MTQ_SEIREN_IDX_MAX; idx++)
    {
      if (magnetic_exclusive_control_timer->state_timer_ms <= mtq_seiren_controller_.mtq_output_duration_ms[idx])
      {
        MTQ_SEIREN_output(mtq_seiren_driver[idx], mtq_seiren_controller_.mtq_output_polarity[idx]);
      }
      else
      {
        MTQ_SEIREN_output(mtq_seiren_driver[idx], MTQ_SEIREN_NO_OUTPUT);
      }
    }
    break;
  case APP_MECT_STATE_STANDBY:
    // 制御期間が終了したら、すべてのMTQの出力をOFFにする
    for (size_t idx = 0; idx < MTQ_SEIREN_IDX_MAX; idx++)
    {
      MTQ_SEIREN_output(mtq_seiren_driver[idx], MTQ_SEIREN_NO_OUTPUT);
    }
    break;
  default:
    // NOT REACHED
    return;
  }

  return;
}

static void APP_MTQ_SEIREN_CONTROLLER_convert_mag_moment_to_output_duration_(void)
{
  float mag_moment_target_body_Am2[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_copy(mag_moment_target_body_Am2, aocs_manager->mag_moment_target_body_Am2);

  // 座標変換
  float mag_moment_mtq_Am2[PHYSICAL_CONST_THREE_DIM];
  MATRIX33_multiply_matrix_vector(mag_moment_mtq_Am2,
                                  aocs_manager->mtq_distribution_matrix,
                                  mag_moment_target_body_Am2);

  for (size_t idx = 0; idx < MTQ_SEIREN_IDX_MAX; idx++)
  {
    if (mag_moment_mtq_Am2[idx] > mtq_seiren_driver[idx]->driver.max_mag_moment)
    {
      // 出力最大で制御する必要がある場合、排他制御でMTQ出力のために確保している時間すべてを使う
      mtq_seiren_controller_.mtq_output_duration_ms[idx] = magnetic_exclusive_control_timer->control_duration_ms;
      mtq_seiren_controller_.mtq_output_polarity[idx] = MTQ_SEIREN_POLARITY_POSITIVE;
    }
    else if (mag_moment_mtq_Am2[idx] < -1.0f * mtq_seiren_driver[idx]->driver.max_mag_moment)
    {
      // 出力最大で制御する必要がある場合、排他制御でMTQ出力のために確保している時間すべてを使う
      mtq_seiren_controller_.mtq_output_duration_ms[idx] = magnetic_exclusive_control_timer->control_duration_ms;
      mtq_seiren_controller_.mtq_output_polarity[idx] = MTQ_SEIREN_POLARITY_NEGATIVE;
    }
    else
    {
      mtq_seiren_controller_.mtq_output_duration_ms[idx] = fabsf((mag_moment_mtq_Am2[idx] / mtq_seiren_driver[idx]->driver.max_mag_moment) *
                                                                  magnetic_exclusive_control_timer->control_duration_ms);
      
      // [TODO_H] 消磁のために出力時間が薄まる効果を補正する
      mtq_seiren_controller_.mtq_output_polarity[idx] = (mag_moment_mtq_Am2[idx] > 0.0f) ? MTQ_SEIREN_POLARITY_POSITIVE : MTQ_SEIREN_POLARITY_NEGATIVE;
    }
  }
}

void APP_MTQ_SEIREN_CONTROLLER_set_cross_product_output_(const CrossProductControl cross_product_cntrl)
{
  float mag_moment_target_Am2[PHYSICAL_CONST_THREE_DIM];
  CROSS_PRODUCT_CONTROL_calc_mag_moment_from_ext_torque(cross_product_cntrl,
                                                        mag_moment_target_Am2,
                                                        mtq_seiren_controller_.integrated_trq_Nms,
                                                        aocs_manager->mag_vec_est_body_nT,
                                                        &mtq_seiren_controller_.cross_product_error_ratio);

  // 現状に合わせて，指令磁気モーメント連続出力値[Am^2sec]最大値を補正し，その結果を元に指令磁気モーメント連続出力値をクリップする
  float clipped_mag_moment_target_Am2s[PHYSICAL_CONST_THREE_DIM];
  APP_MTQ_SEIREN_CONTROLLER_clip_mtq_out_Am2s_(clipped_mag_moment_target_Am2s, mag_moment_target_Am2);

  AOCS_MANAGER_set_mag_moment_target_body_Am2(clipped_mag_moment_target_Am2s);
  return;
}

static void APP_MTQ_SEIREN_CONTROLLER_clip_mtq_out_Am2s_(float clipped_mag_moment_cmd_Am2s[PHYSICAL_CONST_THREE_DIM],
                                                         const float mag_moment_cmd_Am2[PHYSICAL_CONST_THREE_DIM])
{
  float drive_cycle_time_length_ms = magnetic_exclusive_control_timer->observe_duration_ms +
                                     magnetic_exclusive_control_timer->control_duration_ms +
                                     magnetic_exclusive_control_timer->standby_duration_ms;

  float ratio_of_output_time_to_cycle = magnetic_exclusive_control_timer->control_duration_ms / drive_cycle_time_length_ms;

  float max_mag_flux_Am2s[PHYSICAL_CONST_THREE_DIM]; //!< max mag moment integrated during one-drive-cycle [Am^2sec]
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    max_mag_flux_Am2s[i] = mtq_seiren_driver[i]->driver.max_mag_moment * ratio_of_output_time_to_cycle;
  }

  float adjust_ratio_inverse = 1.0f;
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    // 最大駆動可能量に対する逸脱度合いが最も高い軸に合わせて正規化調整係数を決める
    float adjust_ratio_inverse_candidate = fabsf(mag_moment_cmd_Am2[i] / max_mag_flux_Am2s[i]);
    if (adjust_ratio_inverse_candidate > adjust_ratio_inverse)
    {
      adjust_ratio_inverse = adjust_ratio_inverse_candidate;
    }
  }

  // 磁気モーメント方向を保存したまま，最大駆動可能量逸脱軸成分が最大駆動可能量に収まる様にベクトルを正規化する
  VECTOR3_scalar_product(clipped_mag_moment_cmd_Am2s, (1.0f / adjust_ratio_inverse), mag_moment_cmd_Am2);

  return;
}

static void APP_MTQ_SEIREN_CONTROLLER_reset_integrated_trq_(void)
{
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    mtq_seiren_controller_.integrated_trq_Nms[i] = 0.0f;
  }

  return;
}

static void APP_MTQ_SEIREN_CONTROLLER_integrate_trq_(void)
{
  static const float kDtLimitSec = 10.0f; //!< モード遷移等が入って間が空いた際に積分が跳ばないようにするためのdt制限値 [sec]

  ObcTime current_obc_time = TMGR_get_master_clock();
  float dt = (float)OBCT_diff_in_sec(&(mtq_seiren_controller_.previous_trq_integration_obc_time), &current_obc_time);
  mtq_seiren_controller_.previous_trq_integration_obc_time = current_obc_time;

  if (dt > kDtLimitSec || dt <= MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    // この場合，一旦リセット
    APP_MTQ_SEIREN_CONTROLLER_reset_integrated_trq_();
    dt = 0.0f;
  }

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    mtq_seiren_controller_.integrated_trq_Nms[i] += aocs_manager->external_torque_target_body_Nm[i] * dt;
  }

  return;
}

#pragma section
