#pragma section REPRO
/**
* @file   nanossoc_d60_filter.c
* @brief  Nano-SSOC D60太陽センサ出力フィルタ
*/

#include "nanossoc_d60_filter.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../aocs_manager.h"
#include "../../../../../Library/vector3.h"
#include "../../../../../Library/c2a_math.h"
#include "../../../../../Library/math_constants.h"

// Satellite Parameters
#include "../../../../../Settings/SatelliteParameters/nanossoc_d60_parameters.h"

static NanoSsocD60Filter        nanossoc_d60_filter_;
const  NanoSsocD60Filter* const nanossoc_d60_filter = &nanossoc_d60_filter_;

static SpikeFilter APP_NANOSSOC_D60_FILTER_spike_;

static void APP_NANOSSOC_D60_FILTER_init_(void);
static void APP_NANOSSOC_D60_FILTER_exec_(void);

AppInfo APP_NANOSSOC_D60_FILTER_create_app(void)
{
  return AI_create_app_info("NanoSsocD60Filter", APP_NANOSSOC_D60_FILTER_init_, APP_NANOSSOC_D60_FILTER_exec_);
}

static void APP_NANOSSOC_D60_FILTER_init_(void)
{
  nanossoc_d60_filter_.filter_error = C2A_MATH_ERROR_OK;

  VECTOR3_initialize(nanossoc_d60_filter_.sun_vec_est_body, 0.0f);
  nanossoc_d60_filter_.sun_vec_est_body[2] = 1.0f; // 規格時等にエラーにならない様に，Z軸に1.0fを代入

  nanossoc_d60_filter_.spike_filter_config.count_limit_to_accept = NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_accept;
  nanossoc_d60_filter_.spike_filter_config.count_limit_to_reject_continued_warning = NANOSSOC_D60_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning;
  nanossoc_d60_filter_.spike_filter_config.reject_threshold = NANOSSOC_D60_PARAMETERS_spike_filter_config_reject_threshold_rad;
  nanossoc_d60_filter_.spike_filter_config.amplitude_limit_to_accept_as_step =NANOSSOC_D60_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad;

  C2A_MATH_ERROR filter_setting_result = SPIKE_FILTER_init(&APP_NANOSSOC_D60_FILTER_spike_,
                                                            nanossoc_d60_filter_.spike_filter_config);

  if (filter_setting_result != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSsocD60Filter init Failed ! \n");
  }

  return;
}

static void APP_NANOSSOC_D60_FILTER_exec_(void)
{
  // 太陽センサのプレゼンスがない場合には，直近のプレゼンスに備え，伝播値をフィルタ過去値に反映
  // 伝播値そのものに対してはフィルタ処理を実施しない
  if (aocs_manager->sun_visibility == AOCS_MANAGER_SUN_INVISIBILE)
  {
    VECTOR3_copy(nanossoc_d60_filter_.sun_vec_est_body, aocs_manager->sun_vec_est_body);
    return;
  }

  // 2つのベクトルの間の離角を計算し，その値と閾値を比較することでspike判定を実施
  float cos_separation_angle = VECTOR3_inner_product(nanossoc_d60_filter_.sun_vec_est_body, aocs_manager->sun_vec_obs_body);
  float separation_angle_rad = C2A_MATH_acos_rad(cos_separation_angle);
  float dummy_arg_for_spike_filter = 0.0f;          //!< SPIKE_FILTER_calc_outputのうち，使わない出力を受け取るためのダミー
  nanossoc_d60_filter_.filter_error = SPIKE_FILTER_calc_output(&APP_NANOSSOC_D60_FILTER_spike_,
                                                               &dummy_arg_for_spike_filter,
                                                               separation_angle_rad);

  if (nanossoc_d60_filter_.filter_error == C2A_MATH_ERROR_OK)
  {
    // スパイクと判定されなかった場合は,フィルタ過去値をaocs_manager->sun_vec_obs_bodyで更新
    VECTOR3_copy(nanossoc_d60_filter_.sun_vec_est_body, aocs_manager->sun_vec_obs_body);
  }
  else if (nanossoc_d60_filter_.filter_error == C2A_MATH_ERROR_OTHERS)
  {
    // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
    EL_record_event(EL_GROUP_ERROR_NANOSSOC, (uint32_t)0, EL_ERROR_LEVEL_HIGH, (uint32_t)NANOSSOC_D60_EL_NOTE_NOISE_REMOVED);
  }
  else if (nanossoc_d60_filter_.filter_error == C2A_MATH_ERROR_RANGE_OVER)
  {
    // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
    // EL_record_event(EL_GROUP_ERROR_NANOSSOC, (uint32_t)0, EL_ERROR_LEVEL_LOW, (uint32_t)NANOSSOC_D60_EL_NOTE_NOISE_REMOVED);
  }
  else
  {
    // 何もしない
  }

  AOCS_MANAGER_set_sun_vec_est_body(nanossoc_d60_filter_.sun_vec_est_body);

  return;
}

CCP_CmdRet Cmd_APP_NANOSSOC_D60_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet)
{
  SpikeFilter_Config config_received;

  config_received.count_limit_to_accept = CCP_get_param_from_packet(packet, 0, uint8_t);
  config_received.count_limit_to_reject_continued_warning = CCP_get_param_from_packet(packet, 1, uint8_t);

  float reject_threshold_rad = CCP_get_param_from_packet(packet, 2, float);

  C2A_MATH_ERROR range_check_result = C2A_MATH_check_range_violation(reject_threshold_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  config_received.reject_threshold = (double)(reject_threshold_rad);

  float amplitude_limit_to_accept_as_step_rad = CCP_get_param_from_packet(packet, 3, float);

  range_check_result = C2A_MATH_check_range_violation(amplitude_limit_to_accept_as_step_rad, MATH_CONST_PI, 0.0f);
  if (range_check_result != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  config_received.amplitude_limit_to_accept_as_step = (double)(amplitude_limit_to_accept_as_step_rad);

  C2A_MATH_ERROR init_error = SPIKE_FILTER_init(&APP_NANOSSOC_D60_FILTER_spike_, config_received);

  if (init_error != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  nanossoc_d60_filter_.spike_filter_config = config_received;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
