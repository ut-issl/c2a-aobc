#pragma section REPRO
/**
 * @file
 * @brief  自動モード遷移を実行するアプリ
 */

#include "aocs_mode_manager.h"

#include <math.h>

#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/EventManager/event_logger.h>
#include "aocs_manager.h"
#include <src_user/Library/vector3.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/math_constants.h>

// SatelliteParameters
#include <src_user/Settings/SatelliteParameters/fdir_parameters.h>

// #define APP_AOCS_MM_TEST_WITH_S2E //!< S2Eでテストするときにコメントを外すと実行後Bdotモードに遷移する
#ifdef SILS_FW
#ifdef APP_AOCS_MM_TEST_WITH_S2E
static uint32_t APP_AOCS_MM_kDelayTimeInit_ms_ = 15000;       //!< 初期化モードでS2E使用時にモード遷移するまでの待機時間 [ms]
static uint8_t APP_AOCS_MM_initial_mode_transition_flag_ = 0;  //!< S2E使用時にモード遷移で初期モードからBdotに移動したかのフラグ
#endif
#endif

static AocsModeManager        aocs_mode_manager_;
const  AocsModeManager* const aocs_mode_manager = &aocs_mode_manager_;

static float APP_AOCS_MM_kLimitDiffTime_s_ = 1.0f;                 //!< 時間差分の最大リミット値 [s]
static float APP_AOCS_MM_kBdotAnomalyLimitDiffTime_s_   = 300.0f;  //!< Bdot異常判定のための時間差分の最大リミット値 [s]
static float APP_AOCS_MM_ang_vel_conv_judge_counter_s_  = 0.0f;    //!< 角速度収束判定カウンタ [s]
static float APP_AOCS_MM_ang_div_judge_counter_s_       = 0.0f;    //!< 角度発散判定カウンタ [s]
static float APP_AOCS_MM_sensor_error_judge_counter_s_  = 0.0f;    //!< センサエラー判定カウンタ [s]
static float APP_AOCS_MM_ang_vel_prev_norm_rad_s_       = 0.0f;    //!< 前回の角速度ノルム [rad/s]
static ObcTime APP_AOCS_MM_ang_vel_update_previous_obc_time_ = OBCT_create(0, 0, 0);  //!< 前回角速度ノルムを更新した時のOBC時刻

static void APP_AOCS_MM_init_(void);
static void APP_AOCS_MM_initial_exec_(void);
static void APP_AOCS_MM_bdot_exec_(void);
static void APP_AOCS_MM_sun_pointing_exec_(void);
static void APP_AOCS_MM_rough_three_axis_exec_(void);
static void APP_AOCS_MM_fine_three_axis_exec_(void);

/**
 * @brief  モード遷移開始関数
 * @param  target_mode : 遷移先のモード
 */
static void APP_AOCS_MM_change_mode_(MD_MODEID target_mode);

/**
 * @brief  連続時間を考慮したモード遷移条件判定関数
 * @param[in]      condition_flag : 単発の条件フラグ
 * @param[in]      time_threshold_s : 時間閾値
 * @param[in]      prev_obc_time : 前回の判定時間
 * @param[in,out]  counter_s : 時間計測カウンタ
 * @return         判定結果: 1ならモード遷移条件を満たす
 */
static uint8_t APP_AOCS_MM_judge_condition_(const uint8_t condition_flag, const float time_threshold_s,
                                            const ObcTime prev_obc_time, float* counter_s);


AppInfo APP_AOCS_MM_initial_create_app(void)
{
  return AI_create_app_info("AocsModeManagerInitial", NULL, APP_AOCS_MM_initial_exec_);
}

AppInfo APP_AOCS_MM_bdot_create_app(void)
{
  return AI_create_app_info("AocsModeManagerBdot", APP_AOCS_MM_init_, APP_AOCS_MM_bdot_exec_);
}

AppInfo APP_AOCS_MM_sun_pointing_create_app(void)
{
  return AI_create_app_info("AocsModeManagerSunPointing", APP_AOCS_MM_init_, APP_AOCS_MM_sun_pointing_exec_);
}

AppInfo APP_AOCS_MM_rough_three_axis_create_app(void)
{
  return AI_create_app_info("AocsModeManagerRoughThreeAxis", APP_AOCS_MM_init_, APP_AOCS_MM_rough_three_axis_exec_);
}

AppInfo APP_AOCS_MM_fine_three_axis_create_app(void)
{
  return AI_create_app_info("AocsModeManagerFineThreeAxis", APP_AOCS_MM_init_, APP_AOCS_MM_fine_three_axis_exec_);
}

static void APP_AOCS_MM_init_(void)
{
  aocs_mode_manager_.is_enabled_auto_mode_transition = 1;
  aocs_mode_manager_.previous_obc_time = TMGR_get_master_clock();
  aocs_mode_manager_.bdot_retry_count = 0;
  aocs_mode_manager_.is_control_error_small = 0;

  aocs_mode_manager_.ang_vel_conv_limit_rad_s    = FDIR_PARAMETERS_ang_vel_conv_limit_rad_s;
  aocs_mode_manager_.ang_vel_conv_time_limit_s   = FDIR_PARAMETERS_ang_vel_conv_time_limit_s;

  aocs_mode_manager_.ang_vel_norm_increase_limit_rad_s = FDIR_PARAMETERS_ang_vel_norm_increase_limit_rad_s;
  aocs_mode_manager_.ang_vel_anomaly_detection_period_s = FDIR_PARAMETERS_ang_vel_anomaly_detection_period_s;

  aocs_mode_manager_.sun_angle_div_limit_rad     = FDIR_PARAMETERS_sun_angle_div_limit_rad;
  aocs_mode_manager_.sun_angle_div_time_limit_s  = FDIR_PARAMETERS_sun_angle_div_time_limit_s;

  // Rough MTQを想定して初期値は大きめに設定、各モード遷移時に適切に設定することを想定
  aocs_mode_manager_.three_axis_div_limit_rad    = FDIR_PARAMETERS_rough_three_axis_mtq_div_limit_rad;
  aocs_mode_manager_.three_axis_div_time_limit_s = FDIR_PARAMETERS_rough_three_axis_mtq_div_time_limit_s;

  aocs_mode_manager_.sun_invisible_time_limit_s = FDIR_PARAMETERS_sun_invisible_time_limit_s;
  aocs_mode_manager_.stt_invisible_time_limit_s = FDIR_PARAMETERS_stt_invisible_time_limit_s;

  return;
}


static void APP_AOCS_MM_initial_exec_(void)
{
#ifdef SILS_FW
#ifdef APP_AOCS_MM_TEST_WITH_S2E
  uint32_t current_ti_ms = (uint32_t)TMGR_get_master_total_cycle_in_msec(); // S2Eでしか使わないのでオーバーフローは気にしない
  if (current_ti_ms >= APP_AOCS_MM_kDelayTimeInit_ms_ && APP_AOCS_MM_initial_mode_transition_flag_ == 0)
  {
    APP_AOCS_MM_initial_mode_transition_flag_ = 1;
    APP_AOCS_MM_change_mode_(MD_MODEID_BDOT);
  }
#endif
#endif

  return;
}


static void APP_AOCS_MM_bdot_exec_(void)
{
  if (aocs_mode_manager_.is_enabled_auto_mode_transition == 0) return;
  MD_MODEID current_mode = mode_manager->current_id;
  if (current_mode != MD_MODEID_BDOT) return;

  // Bdot収束判定
  float ang_vel_norm_rad_s;
  ang_vel_norm_rad_s = VECTOR3_norm(aocs_manager->ang_vel_est_body_rad_s);

  uint8_t convergence_condition = 0;
  if (ang_vel_norm_rad_s < aocs_mode_manager_.ang_vel_conv_limit_rad_s) convergence_condition = 1;

  uint8_t is_conv = APP_AOCS_MM_judge_condition_(convergence_condition,
                                                 aocs_mode_manager_.ang_vel_conv_time_limit_s,
                                                 aocs_mode_manager_.previous_obc_time,
                                                 &APP_AOCS_MM_ang_vel_conv_judge_counter_s_);

  if (is_conv == 1)
  {
    APP_AOCS_MM_change_mode_(MD_MODEID_ROUGH_SUN_POINTING);
    return;
  }

  // Bdot 異常検知 (センサ、MTQ符号ミス)
  // エラー増えていることを検知してInitialに戻る
  ObcTime current_obc_time = TMGR_get_master_clock();
  float diff_time_s = (float)OBCT_diff_in_sec(&APP_AOCS_MM_ang_vel_update_previous_obc_time_, &current_obc_time);
  if (diff_time_s >= aocs_mode_manager_.ang_vel_anomaly_detection_period_s)
  {
    float ang_vel_norm_diff_rad_s = ang_vel_norm_rad_s - APP_AOCS_MM_ang_vel_prev_norm_rad_s_;
    if (APP_AOCS_MM_ang_vel_prev_norm_rad_s_ < 1e-4f)
    {
      // 初回なので何もしない
    }
    else if (ang_vel_norm_diff_rad_s > aocs_mode_manager_.ang_vel_norm_increase_limit_rad_s)
    {
      if (diff_time_s < APP_AOCS_MM_kBdotAnomalyLimitDiffTime_s_)
      {
        // Initialに戻って制御をやめる
        APP_AOCS_MM_change_mode_(MD_MODEID_INITIAL);
      }
    }
    else
    {
      // 特に何もしない
    }
    APP_AOCS_MM_ang_vel_update_previous_obc_time_ = current_obc_time;
    APP_AOCS_MM_ang_vel_prev_norm_rad_s_ = ang_vel_norm_rad_s;
  }

  aocs_mode_manager_.previous_obc_time = TMGR_get_master_clock();
  return;
}


static void APP_AOCS_MM_sun_pointing_exec_(void)
{
  // モード遷移後、制御収束まではdisableにしておく想定
  if (aocs_mode_manager_.is_enabled_auto_mode_transition == 0) return;
  MD_MODEID current_mode = mode_manager->current_id;
  if (current_mode != MD_MODEID_ROUGH_SUN_POINTING) return;

  // エラーが大きすぎることを検知してBdotに戻る
  float sun_error_angle_rad = fabsf(aocs_manager->sun_vec_error_rad);
  uint8_t divergence_condition = 0;
  if (sun_error_angle_rad > aocs_mode_manager_.sun_angle_div_limit_rad) divergence_condition = 1;
  uint8_t is_div = APP_AOCS_MM_judge_condition_(divergence_condition,
                                                aocs_mode_manager_.sun_angle_div_time_limit_s,
                                                aocs_mode_manager_.previous_obc_time,
                                                &APP_AOCS_MM_ang_div_judge_counter_s_);

  // 太陽センサ非可視時間が長すぎることを検知してBdotに戻る
  uint8_t sensor_error_condition = 0;
  if (aocs_manager->sun_visibility != AOCS_MANAGER_SUN_VISIBLE) sensor_error_condition = 1;
  uint8_t is_sensor_error = APP_AOCS_MM_judge_condition_(sensor_error_condition,
                                                         aocs_mode_manager_.sun_invisible_time_limit_s,
                                                         aocs_mode_manager_.previous_obc_time,
                                                         &APP_AOCS_MM_sensor_error_judge_counter_s_);

  aocs_mode_manager_.previous_obc_time = TMGR_get_master_clock();

  // モード遷移するかの判定
  if (is_div == 1 || is_sensor_error == 1)
  {
    APP_AOCS_MM_change_mode_(MD_MODEID_BDOT);
  }
  // エラーが小さいかの判定、ただしセンサが見えていないときは適合しない TODO_L: 本来はもう少し丁寧に判定すべき
  if (sensor_error_condition == 0 && divergence_condition == 0)
  {
    aocs_mode_manager_.is_control_error_small = 1;
  }
  else
  {
    aocs_mode_manager_.is_control_error_small = 0;
  }
}

// 現時点ではRWとMTQとで条件が同じなので一つにまとめている
// 閾値はモード遷移時に適切に変更する
static void APP_AOCS_MM_rough_three_axis_exec_(void)
{
  // モード遷移後、制御収束まではdisableにしておく想定
  if (aocs_mode_manager_.is_enabled_auto_mode_transition == 0) return;
  MD_MODEID current_mode = mode_manager->current_id;
  if ((current_mode != MD_MODEID_ROUGH_THREE_AXIS) && (current_mode != MD_MODEID_ROUGH_THREE_AXIS_RW)) return;

  // エラーが大きすぎることを検知してBdotに戻る
  float error_angle_rad = fabsf(2.0f * acosf(aocs_manager->quaternion_error_b2t.scalar_part));
  uint8_t divergence_condition = 0;
  if (error_angle_rad > aocs_mode_manager_.three_axis_div_limit_rad) divergence_condition = 1;
  uint8_t is_div = APP_AOCS_MM_judge_condition_(divergence_condition,
                                                aocs_mode_manager_.three_axis_div_time_limit_s,
                                                aocs_mode_manager_.previous_obc_time,
                                                &APP_AOCS_MM_ang_div_judge_counter_s_);

  // 太陽センサ非可視時間が長すぎることを検知してBdotに戻る
  uint8_t sensor_error_condition = 0;
  if (aocs_manager->sun_visibility != AOCS_MANAGER_SUN_VISIBLE) sensor_error_condition = 1;
  uint8_t is_sensor_error = APP_AOCS_MM_judge_condition_(sensor_error_condition,
                                                         aocs_mode_manager_.sun_invisible_time_limit_s,
                                                         aocs_mode_manager_.previous_obc_time,
                                                         &APP_AOCS_MM_sensor_error_judge_counter_s_);

  aocs_mode_manager_.previous_obc_time = TMGR_get_master_clock();

  // モード遷移するかの判定
  if (is_div == 1 || is_sensor_error == 1)
  {
    APP_AOCS_MM_change_mode_(MD_MODEID_BDOT);
  }
  // エラーが小さいかの判定、ただしセンサが見えていないときは適合しない TODO_L: 本来はもう少し丁寧に判定すべき
  if (sensor_error_condition == 0 && divergence_condition == 0)
  {
    aocs_mode_manager_.is_control_error_small = 1;
  }
  else
  {
    aocs_mode_manager_.is_control_error_small = 0;
  }
}

static void APP_AOCS_MM_fine_three_axis_exec_(void)
{
  // モード遷移後、制御収束まではdisableにしておく想定
  if (aocs_mode_manager_.is_enabled_auto_mode_transition == 0) return;
  MD_MODEID current_mode = mode_manager->current_id;
  if ((current_mode != MD_MODEID_FINE_THREE_AXIS) && (current_mode != MD_MODEID_GPSR_RANGE_OBSERVE)) return;

  // エラーが大きすぎることを検知してBdotに戻る
  float error_angle_rad = fabsf(2.0f * acosf(aocs_manager->quaternion_error_b2t.scalar_part));
  uint8_t divergence_condition = 0;
  if (error_angle_rad > aocs_mode_manager_.three_axis_div_limit_rad) divergence_condition = 1;
  uint8_t is_div = APP_AOCS_MM_judge_condition_(divergence_condition,
                                                aocs_mode_manager_.three_axis_div_time_limit_s,
                                                aocs_mode_manager_.previous_obc_time,
                                                &APP_AOCS_MM_ang_div_judge_counter_s_);

  // STT非可視時間が長すぎることを検知してBdotに戻る
  uint8_t sensor_error_condition = 0;
  if (aocs_manager->stt_availability != AOCS_MANAGER_STT_AVAILABLE) sensor_error_condition = 1;
  uint8_t is_sensor_error = APP_AOCS_MM_judge_condition_(sensor_error_condition,
                                                         aocs_mode_manager_.stt_invisible_time_limit_s,
                                                         aocs_mode_manager_.previous_obc_time,
                                                         &APP_AOCS_MM_sensor_error_judge_counter_s_);

  aocs_mode_manager_.previous_obc_time = TMGR_get_master_clock();

  // モード遷移するかの判定
  if (is_div == 1 || is_sensor_error == 1)
  {
    APP_AOCS_MM_change_mode_(MD_MODEID_BDOT);
  }
  // エラーが小さいかの判定、ただしセンサが見えていないときは適合しない TODO_L: 本来はもう少し丁寧に判定すべき
  if (sensor_error_condition == 0 && divergence_condition == 0)
  {
    aocs_mode_manager_.is_control_error_small = 1;
  }
  else
  {
    aocs_mode_manager_.is_control_error_small = 0;
  }
}

// Static functions
static void APP_AOCS_MM_change_mode_(MD_MODEID target_mode)
{
  if (mode_manager->stat == MM_STATUS_IN_PROGRESS) return;
  if (target_mode == MD_MODEID_BDOT)
  {
    aocs_mode_manager_.bdot_retry_count++;
    if (aocs_mode_manager_.bdot_retry_count >= 0xff)
    {
      aocs_mode_manager_.bdot_retry_count = 0;
    }
  }

  CommonCmdPacket packet;
  uint8_t param = (uint8_t)target_mode;

  EL_record_event(EL_GROUP_AUTO_MODE_TRANSITION, (uint32_t)mode_manager->current_id, EL_ERROR_LEVEL_HIGH, (uint32_t)target_mode);
  CCP_form_rtc(&packet, Cmd_CODE_MM_START_TRANSITION, &param, 1);
  PH_analyze_cmd_packet(&packet);
  APP_AOCS_MM_ang_vel_conv_judge_counter_s_ = 0.0f;
  APP_AOCS_MM_ang_div_judge_counter_s_ = 0.0f;
  APP_AOCS_MM_sensor_error_judge_counter_s_  = 0.0f;

  return;
}

static uint8_t APP_AOCS_MM_judge_condition_(const uint8_t condition_flag, const float time_threshold_s,
                                            const ObcTime prev_obc_time, float* counter_s)
{
  uint8_t ret = 0;
  ObcTime current_obc_time = TMGR_get_master_clock();

  if (condition_flag)
  {
    float diff_time_s = (float)OBCT_diff_in_sec(&prev_obc_time, &current_obc_time);
    // (初回など)時間差が大きすぎたら異常なので足さない
    // TODO: 時間アサーションが正しいかどうか検討する
    if (diff_time_s < APP_AOCS_MM_kLimitDiffTime_s_)
    {
      *counter_s += diff_time_s;
    }
  }
  else
  {
    *counter_s = 0.0f;
  }

  if (*counter_s > time_threshold_s)
  {
    ret = 1;
  }

  return ret;
}


// Commands
CCP_CmdRet Cmd_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION(const CommonCmdPacket* packet)
{
  uint8_t is_enabled = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (is_enabled > 1)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  aocs_mode_manager_.is_enabled_auto_mode_transition = is_enabled;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MM_SET_ANG_VEL_THRESHOLD(const CommonCmdPacket* packet)
{
  uint8_t arg_idx = 0;
  float angular_rate_limit_rad_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (angular_rate_limit_rad_s < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  float time_limit_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (time_limit_s < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  float angular_rate_norm_increase_limit_rad_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (angular_rate_norm_increase_limit_rad_s < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  float ang_vel_anomaly_detection_period_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (ang_vel_anomaly_detection_period_s < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  aocs_mode_manager_.ang_vel_conv_limit_rad_s = angular_rate_limit_rad_s;
  aocs_mode_manager_.ang_vel_conv_time_limit_s = time_limit_s;
  aocs_mode_manager_.ang_vel_norm_increase_limit_rad_s = angular_rate_norm_increase_limit_rad_s;
  aocs_mode_manager_.ang_vel_anomaly_detection_period_s = ang_vel_anomaly_detection_period_s;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MM_SET_SUN_ANGLE_THRESHOLD(const CommonCmdPacket* packet)
{
  uint8_t arg_idx = 0;
  float sun_angle_div_limit_rad = CCP_get_param_from_packet(packet, arg_idx, float);
  C2A_MATH_ERROR check_zero_pi = C2A_MATH_check_range_violation(sun_angle_div_limit_rad, MATH_CONST_PI, 0.0f);
  if (check_zero_pi != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  float sun_angle_div_time_limit_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (sun_angle_div_time_limit_s <= 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  aocs_mode_manager_.sun_angle_div_limit_rad = sun_angle_div_limit_rad;
  aocs_mode_manager_.sun_angle_div_time_limit_s = sun_angle_div_time_limit_s;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MM_SET_THREE_AXIS_THRESHOLD(const CommonCmdPacket* packet)
{
  uint8_t arg_idx = 0;
  float three_axis_div_limit_rad = CCP_get_param_from_packet(packet, arg_idx, float);
  C2A_MATH_ERROR check_zero_pi = C2A_MATH_check_range_violation(three_axis_div_limit_rad, MATH_CONST_PI, 0.0f);
  if (check_zero_pi != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  float three_axis_div_time_limit_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (three_axis_div_time_limit_s <= 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  aocs_mode_manager_.three_axis_div_limit_rad = three_axis_div_limit_rad;
  aocs_mode_manager_.three_axis_div_time_limit_s = three_axis_div_time_limit_s;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MM_SET_INVISIBLE_THRESHOLD(const CommonCmdPacket* packet)
{
  uint8_t arg_idx = 0;
  float sun_invisible_time_limit_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (sun_invisible_time_limit_s <= 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  float stt_invisible_time_limit_s = CCP_get_param_from_packet(packet, arg_idx, float);
  if (stt_invisible_time_limit_s <= 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  arg_idx++;

  aocs_mode_manager_.sun_invisible_time_limit_s = sun_invisible_time_limit_s;
  aocs_mode_manager_.stt_invisible_time_limit_s = stt_invisible_time_limit_s;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
