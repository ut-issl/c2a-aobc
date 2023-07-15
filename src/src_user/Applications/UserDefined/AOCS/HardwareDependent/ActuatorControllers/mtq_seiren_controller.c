#pragma section REPRO
/**
* @file   mtq_seiren_controller.c
* @brief  aocs_managerの目標磁気情報から，MTQ_SEIRENのPWMデューティー比を計算・設定するアプリ
* @note   本来なら，PWMデューティー比 (DI等の下位側でやること) とMTQ駆動・消磁の比率 (ここでやること) は別物となる
* @note   ここでやるべきはMTQ駆動・消磁の比率の計算か，PWMデューティー比の計算のどちらかのはずだが，両者が混ざっている
* @note   とはいえ，PWM周期の遅い現状では，両者の周期を近づけざるを得ず，その状態でDI側のSWを現状維持するために，下記制約を課す
* @note   mtq_output_time_length_msとmtq_demagnetization_required_time_msの和が，遅いPWM周期 (1sec)の整数倍 -100msec と合致する様に定める
* @note   上記制約が満たされない場合，DI側と上位側 (3軸制御やこのアプリ) が思うMTQ駆動動作に齟齬が発生する (100msecはcase処理による余分な分岐で生じる時間)
*/

#include "mtq_seiren_controller.h"

#include <math.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/c2a_math.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/math_constants.h>
#include <src_user/Applications/DriverInstances/di_mtq_seiren.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

static MtqSeirenController        mtq_seiren_controller_;
const  MtqSeirenController* const mtq_seiren_controller = &mtq_seiren_controller_;

//!< 元のcase文仕様に伴って生じる，MTQoff期間の余分な長さ [msec]
static const uint32_t APP_MTQ_SEIREN_CONTROLLER_kDelayForMTQStopExec_ms_ = 100;

static void APP_MTQ_SEIREN_CONTROLLER_init_(void);
static void APP_MTQ_SEIREN_CONTROLLER_exec_(void);

/**
 * @brief  MTQ出力指令関数
 * @param  void
 * @return void
 * @note   AOCSマネージャーから読み込んだMTQ出力目標磁気モーメントを読み出し，PWM制御のデューティー比に変換してMTQに指示する
 */
static void APP_MTQ_SEIREN_CONTROLLER_convert_mag_moment_to_pwm_duty_(void);

/**
 * @brief  MTQ出力状態管理関数
 * @param  void
 * @return void
 * @note   出力目標磁気モーメントから，MTQの出力状態をAOCSマネージャーに伝える関数
 */
static void APP_MTQ_SEIREN_CONTROLLER_set_mtq_output_state_(void);

/**
 * @brief  クロスプロダクト制御時MTQ目標磁気モーメント出力設定関数
 * @param  void
 * @return void
 * @note   MTQの出力がOFFのときに呼び出され，MTQ目標磁気モーメント出力を計算し，AOCSマネージャーに伝える役割を持つ
 */
static void APP_MTQ_SEIREN_CONTROLLER_set_cross_product_output_(const CrossProductControl cross_product_cntrl);

/**
 * @brief  MTQ目標磁気モーメント維持関数
 * @param  void
 * @return void
 * @note   MTQの出力がONのときに呼び出され，所定の時間だけMTQ出力を維持する役割を持つ
 * @note   所定のMTQ出力時間が過ぎたら，MTQ出力を切る
 */
static void APP_MTQ_SEIREN_CONTROLLER_maintain_mtq_output_();

/**
 * @brief  MTQ消磁待機関数
 * @param  void
 * @return void
 * @note   MTQの出力が切られてから，消磁まで待機する役割を持つ
 * @note   消磁までの時間が過ぎたら，MTQ出力を完全にOFFにしたとみなす
 */
static void APP_MTQ_SEIREN_CONTROLLER_wait_for_demagnitization_(void);

/**
 * @brief  MTQ出力磁気モーメント実効値clip関数
 * @param[out]  clipped_mag_moment_cmd_Am2s    : clip後のMTQ駆動/消磁の合計インターバルの間に出力できる磁気モーメント積算値 [Am^2sec]
 * @param[in]   mag_moment_cmd_Am2             : clip前の磁気モーメント出力指令値 [Am^2]
 * @note   MTQ_SEIREN_CONTROLLERアプリ内でMTQ駆動/消磁の比率とPWMduty比の計算が混ざって同じものとして計算されてしまっている
 * @note   このことへの対処として，MTQ駆動/消磁の合計インターバルに対する駆動時間の比率を使って最大MTQ駆動可能量を補正した上でclipする
 */
static void APP_MTQ_SEIREN_CONTROLLER_clip_mtq_out_Am2s_(float clipped_mag_moment_cmd_Am2s[PHYSICAL_CONST_THREE_DIM],
                                                         const float mag_moment_cmd_Am2[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  駆動 + 消磁動作の実行状態制御用カウンタの更新
 * @note   PWM制御が遅い現状では，こういうタイマを用意して，OBC時刻正秒相当のタイミングでMTQ指令値を更新しないとMTQ DriverのPWMと整合が取れない
 */
static void APP_MTQ_SEIREN_CONTROLLER_update_internal_state_timer_(void);

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
  mtq_seiren_controller_.previous_obc_time = TMGR_get_master_clock();
  mtq_seiren_controller_.mtq_output_turned_on_obc_time = TMGR_get_master_clock();

  /* APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_outputのcase文の仕様から，
     実際の消磁時間 (MTQ OFF期間) の長さは下記の値+余分に1回分のカウント (100msec) が入ることに注意 */
  mtq_seiren_controller_.mtq_demagnetization_required_time_ms = 100; // 実機計測などの結果、100msで決めた
  mtq_seiren_controller_.mtq_output_time_length_ms   = 800;

  /* APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_outputのcase文の仕様から，
     駆動 + 消磁の1cycleの長さは，各々の長さ+余分に1回分のカウント (100msec) が入るため，100msecを加算 */
  mtq_seiren_controller_.max_count_internal_timer_ms =
    mtq_seiren_controller_.mtq_output_time_length_ms +
    mtq_seiren_controller_.mtq_demagnetization_required_time_ms +
    APP_MTQ_SEIREN_CONTROLLER_kDelayForMTQStopExec_ms_;

  mtq_seiren_controller_.internal_timer_ms = 0;

  APP_MTQ_SEIREN_CONTROLLER_reset_integrated_trq_();
  mtq_seiren_controller_.previous_trq_integration_obc_time = TMGR_get_master_clock();
  mtq_seiren_controller_.cross_product_error = CROSS_PRODUCT_CONTROL_ERROR_OK;
  mtq_seiren_controller_.cross_product_error_ratio = 0.0f;
}

void APP_MTQ_SEIREN_CONTROLLER_exec_(void)
{
  APP_MTQ_SEIREN_CONTROLLER_update_internal_state_timer_();
  APP_MTQ_SEIREN_CONTROLLER_convert_mag_moment_to_pwm_duty_();
  APP_MTQ_SEIREN_CONTROLLER_set_mtq_output_state_();
}

void APP_MTQ_SEIREN_CONTROLLER_convert_mag_moment_to_pwm_duty_(void)
{
  float mag_moment_target_body_Am2[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_copy(mag_moment_target_body_Am2, aocs_manager->mag_moment_target_body_Am2);

  // 座標変換
  float mag_moment_mtq_Am2[PHYSICAL_CONST_THREE_DIM];
  MATRIX33_multiply_matrix_vector(mag_moment_mtq_Am2,
                                aocs_manager->mtq_distribution_matrix,
                                mag_moment_target_body_Am2);

  for (int idx = 0; idx < MTQ_SEIREN_IDX_MAX; idx++)
  {
    if (mag_moment_mtq_Am2[idx] > mtq_seiren_driver[idx]->driver.max_mag_moment)
    {
      DI_MTQ_SEIREN_set_pwm_duty((MTQ_SEIREN_IDX)idx, 100);
    }
    else if (mag_moment_mtq_Am2[idx] < -1.0f * mtq_seiren_driver[idx]->driver.max_mag_moment)
    {
      DI_MTQ_SEIREN_set_pwm_duty((MTQ_SEIREN_IDX)idx, -100);
    }
    else
    {
      int8_t signed_duty_percent;
      signed_duty_percent = (int8_t)(mag_moment_mtq_Am2[idx] / mtq_seiren_driver[idx]->driver.max_mag_moment * 100.0f);
      DI_MTQ_SEIREN_set_pwm_duty((MTQ_SEIREN_IDX)idx, signed_duty_percent);
    }
  }
}

void APP_MTQ_SEIREN_CONTROLLER_set_mtq_output_state_(void)
{
  if (aocs_manager->mag_exclusive_state == AOCS_MANAGER_MAG_EXCLUSIVE_STATE_IDLE)
  {
    // MTQが非駆動なのか消磁中なのかをこの中だけで区別してフラグ操作するのはツライので，別の上位フラグで操作
    // TODO_L: 全体的に元の排他制御操作が複雑というか何というかなので，正直，時間のある時にゼロから全部直したい…．
    AOCS_MANAGER_set_mtq_output_state(AOCS_MANAGER_MTQ_OUTPUT_STATE_OFF);
    return;
  }

  if (mtq_seiren_controller_.internal_timer_ms < mtq_seiren_controller_.mtq_output_time_length_ms)
  {
    AOCS_MANAGER_set_mtq_output_state(AOCS_MANAGER_MTQ_OUTPUT_STATE_ON);
  }
  else if ((mtq_seiren_controller_.internal_timer_ms >=
           (mtq_seiren_controller_.mtq_output_time_length_ms - APP_MTQ_SEIREN_CONTROLLER_kDelayForMTQStopExec_ms_)) &&
           (mtq_seiren_controller_.internal_timer_ms <
           (mtq_seiren_controller_.max_count_internal_timer_ms - APP_MTQ_SEIREN_CONTROLLER_kDelayForMTQStopExec_ms_)))
  {
    AOCS_MANAGER_set_mtq_output_state(AOCS_MANAGER_MTQ_OUTPUT_STATE_DEMAGNITIZING);
  }
  else
  {
    AOCS_MANAGER_set_mtq_output_state(AOCS_MANAGER_MTQ_OUTPUT_STATE_OFF);
  }

  return;
}

void APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_output(const CrossProductControl cross_product_cntrl)
{

  APP_MTQ_SEIREN_CONTROLLER_integrate_trq_();

  switch (aocs_manager->mtq_output_state)
  {
  case AOCS_MANAGER_MTQ_OUTPUT_STATE_OFF:
    APP_MTQ_SEIREN_CONTROLLER_set_cross_product_output_(cross_product_cntrl);
    APP_MTQ_SEIREN_CONTROLLER_reset_integrated_trq_();
    return;
  case AOCS_MANAGER_MTQ_OUTPUT_STATE_ON:
    return APP_MTQ_SEIREN_CONTROLLER_maintain_mtq_output_();
  case AOCS_MANAGER_MTQ_OUTPUT_STATE_DEMAGNITIZING:
    return APP_MTQ_SEIREN_CONTROLLER_wait_for_demagnitization_();
  default:
    // NOT REACHED
    return;
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
  mtq_seiren_controller_.mtq_output_turned_on_obc_time = TMGR_get_master_clock();
  return;
}

void APP_MTQ_SEIREN_CONTROLLER_maintain_mtq_output_()
{
  ObcTime current_obc_time = TMGR_get_master_clock();
  uint32_t mtq_driving_time_ms = OBCT_diff_in_msec(&(mtq_seiren_controller->mtq_output_turned_on_obc_time), &current_obc_time);
  // 目標の時間長さだけMTQに磁気モーメントを出力させたら，MTQの出力を切り，消磁に入る
  // TODO: 時間アサーションが正しいかどうか検討する
  if (mtq_driving_time_ms >= mtq_seiren_controller->mtq_output_time_length_ms)
  {
    float mag_moment_target_Am2[PHYSICAL_CONST_THREE_DIM];
    VECTOR3_initialize(mag_moment_target_Am2, 0.0f);
    AOCS_MANAGER_set_mag_moment_target_body_Am2(mag_moment_target_Am2);
  }
  return;
}

void APP_MTQ_SEIREN_CONTROLLER_wait_for_demagnitization_(void)
{
  float mag_moment_target_Am2[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_initialize(mag_moment_target_Am2, 0.0f);
  AOCS_MANAGER_set_mag_moment_target_body_Am2(mag_moment_target_Am2);
  return;
}

static void APP_MTQ_SEIREN_CONTROLLER_clip_mtq_out_Am2s_(float clipped_mag_moment_cmd_Am2s[PHYSICAL_CONST_THREE_DIM],
                                                         const float mag_moment_cmd_Am2[PHYSICAL_CONST_THREE_DIM])
{
  float ratio_of_output_time_to_cycle = 1.0f;
  uint32_t drive_cycle_time_length_ms =
  mtq_seiren_controller_.mtq_output_time_length_ms + mtq_seiren_controller_.mtq_demagnetization_required_time_ms +
  APP_MTQ_SEIREN_CONTROLLER_kDelayForMTQStopExec_ms_;

  if (drive_cycle_time_length_ms <= *MTQ_SEIREN_pwm_period_ms)
  {
    ratio_of_output_time_to_cycle = (float)(mtq_seiren_controller_.mtq_output_time_length_ms) / (float)(drive_cycle_time_length_ms);
  }

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

static void APP_MTQ_SEIREN_CONTROLLER_update_internal_state_timer_(void)
{
  ObcTime current_obc_time = TMGR_get_master_clock();
  uint32_t step_time_ms = OBCT_diff_in_msec(&(mtq_seiren_controller_.previous_obc_time), &current_obc_time);
  mtq_seiren_controller_.previous_obc_time = current_obc_time;

  mtq_seiren_controller_.internal_timer_ms += step_time_ms;

  if (mtq_seiren_controller_.internal_timer_ms >= mtq_seiren_controller_.max_count_internal_timer_ms)
  {
    mtq_seiren_controller_.internal_timer_ms = 0;
  }

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


// コマンド関数
CCP_CmdRet Cmd_APP_MTQ_SEIREN_CONTROLLER_SET_DEMAGNITIZATION_TIME(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  uint32_t mtq_demagnetization_required_time_ms;
  ENDIAN_memcpy(&mtq_demagnetization_required_time_ms, param, sizeof(uint32_t));

  if (mtq_demagnetization_required_time_ms < 0)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  mtq_seiren_controller_.mtq_demagnetization_required_time_ms = mtq_demagnetization_required_time_ms;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
