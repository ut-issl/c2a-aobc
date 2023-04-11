#pragma section REPRO
/**
* @file   sagitta_filter.c
* @brief  SAGITTA (STT) 出力フィルタ
*/

#include "sagitta_filter.h"

#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../../../DriverInstances/di_sagitta.h"
#include "../../aocs_manager.h"
#include "../../../../../Library/physical_constants.h"

static SagittaFilter        sagitta_filter_;
const  SagittaFilter* const sagitta_filter = &sagitta_filter_;

static SpikeFilter APP_SAGITTA_FILTER_q_i2b_spike_;       //!< スパイク除去フィルタ

static void APP_SAGITTA_FILTER_init_(void);
static void APP_SAGITTA_FILTER_exec_(void);

static int APP_SAGITTA_FILTER_init_spike_filter_(void);

//!< quaternionに対応する様にラッピングしたSPIKE_FILTER_calc_output
static C2A_MATH_ERROR APP_SAGITTA_FILTER_calc_spike_filter_out_quaternion_(Quaternion q_i2b_in);

AppInfo APP_SAGITTA_FILTER_create_app(void)
{
  return AI_create_app_info("SagittaFilter", APP_SAGITTA_FILTER_init_, APP_SAGITTA_FILTER_exec_);
}

static void APP_SAGITTA_FILTER_init_(void)
{
  sagitta_filter_.q_i2b_filtered     = QUATERNION_make_unit();
  sagitta_filter_.q_i2b_filter_error = C2A_MATH_ERROR_OK;

  int spike_filter_init_result = APP_SAGITTA_FILTER_init_spike_filter_();
  if (spike_filter_init_result != 0)
  {
    Printf("SAGITTA SpikeFilter init Failed ! \n");
  }

  return;
}

static void APP_SAGITTA_FILTER_exec_(void)
{
  if (aocs_manager->stt_availability == AOCS_MANAGER_STT_UNAVAILABLE)
  {
    sagitta_filter_.q_i2b_filtered = aocs_manager->quaternion_est_i2b;
    return;
  }
  // TODO_L: STTを複数搭載する場合には，SAGITTA_IDX_IN_UNITをfor-loopに変更
  sagitta_filter_.q_i2b_filter_error =
    APP_SAGITTA_FILTER_calc_spike_filter_out_quaternion_(aocs_manager->quaternion_obs_i2b);

  // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
  if (sagitta_filter_.q_i2b_filter_error == C2A_MATH_ERROR_OTHERS)
  {
    EL_record_event(EL_GROUP_ERROR_SAGITTA, (uint32_t)SAGITTA_IDX_IN_UNIT, EL_ERROR_LEVEL_HIGH, (uint32_t)SAGITTA_EL_NOTE_NOISE_REMOVED);
  }
  else if (sagitta_filter_.q_i2b_filter_error == C2A_MATH_ERROR_RANGE_OVER)
  {
    // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
    // EL_record_event(EL_GROUP_ERROR_SAGITTA, (uint32_t)SAGITTA_IDX_IN_UNIT, EL_ERROR_LEVEL_LOW, (uint32_t)SAGITTA_EL_NOTE_NOISE_REMOVED);
  }
  else
  {
    // 何もしない
  }

  AOCS_MANAGER_set_quaternion_est_i2b(sagitta_filter_.q_i2b_filtered);

  return;
}

static int APP_SAGITTA_FILTER_init_spike_filter_(void)
{
  // 値は調整してよいが一旦これで進める
  sagitta_filter_.q_i2b_spike_filter_config.count_limit_to_accept = 20;
  sagitta_filter_.q_i2b_spike_filter_config.count_limit_to_reject_continued_warning = 200;
  sagitta_filter_.q_i2b_spike_filter_config.reject_threshold =
    PHYSICAL_CONST_degree_to_radian(1.0f);
  sagitta_filter_.q_i2b_spike_filter_config.amplitude_limit_to_accept_as_step =
    PHYSICAL_CONST_degree_to_radian(0.6f);     // STT精度は8.7 [秒角(1σ)]

  C2A_MATH_ERROR q_i2b_spike_filter_setting_result = C2A_MATH_ERROR_OK;
  C2A_MATH_ERROR filter_setting_result =
    SPIKE_FILTER_init(&APP_SAGITTA_FILTER_q_i2b_spike_, sagitta_filter_.q_i2b_spike_filter_config);

  if (filter_setting_result != C2A_MATH_ERROR_OK)
  {
    q_i2b_spike_filter_setting_result = filter_setting_result;
    return -1;
  }

  return 0;
}

static C2A_MATH_ERROR APP_SAGITTA_FILTER_calc_spike_filter_out_quaternion_(Quaternion q_i2b_in)
{
  // 2つのquaternionの間の離角を計算し，その値と閾値を比較することでspike判定を実施
  Quaternion q_diff = QUATERNION_divide(sagitta_filter_.q_i2b_filtered, q_i2b_in);

  float separation_angle_rad = 0.0f;                //!< 2つのquaternionの間の離角
  float eigen_axis[PHYSICAL_CONST_THREE_DIM];       //!< 2つのquaternionの間の回転軸方向
  QUATERNION_to_euler_axis_angle(eigen_axis, &separation_angle_rad, q_diff);

  float dummy_arg_for_spike_filter = 0.0f;          //!< SPIKE_FILTER_calc_outputのうち，使わない出力を受け取るためのダミー
  sagitta_filter_.q_i2b_filter_error = SPIKE_FILTER_calc_output(&APP_SAGITTA_FILTER_q_i2b_spike_,
                                                                &dummy_arg_for_spike_filter,
                                                                separation_angle_rad);

  if (sagitta_filter_.q_i2b_filter_error == C2A_MATH_ERROR_OK)
  {
    sagitta_filter_.q_i2b_filtered = q_i2b_in;
  }

  return sagitta_filter_.q_i2b_filter_error;
}

CCP_CmdRet Cmd_APP_SAGITTA_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet)
{
  SAGITTA_IDX sensor_id = (SAGITTA_IDX)(CCP_get_param_from_packet(packet, 0, uint8_t));
  if (sensor_id >= SAGITTA_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  SpikeFilter_Config config_received;
  config_received.count_limit_to_accept = CCP_get_param_from_packet(packet, 1, uint8_t);

  config_received.count_limit_to_reject_continued_warning = CCP_get_param_from_packet(packet, 2, uint8_t);

  float reject_threshold_float = CCP_get_param_from_packet(packet, 3, float);
  if (reject_threshold_float < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.reject_threshold = (double)(reject_threshold_float);

  float amplitude_limit_to_accept_as_step_float = CCP_get_param_from_packet(packet, 4, float);
  if (amplitude_limit_to_accept_as_step_float < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.amplitude_limit_to_accept_as_step = (double)(amplitude_limit_to_accept_as_step_float);

   C2A_MATH_ERROR init_error = SPIKE_FILTER_init(&APP_SAGITTA_FILTER_q_i2b_spike_, config_received);

  // 初期化に成功した場合のみ，sagitta_filterのconfigステータスを変更
  if (init_error != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  sagitta_filter_.q_i2b_spike_filter_config = config_received;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_SAGITTA_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet)
{
  SAGITTA_IDX sensor_id = (SAGITTA_IDX)(CCP_get_param_from_packet(packet, 0, uint8_t));
  if (sensor_id >= SAGITTA_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  SPIKE_FILTER_reset(&APP_SAGITTA_FILTER_q_i2b_spike_);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
