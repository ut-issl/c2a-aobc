#pragma section REPRO
/**
* @file  rw0003_filter.c
* @brief RW0003回転数フィルタ
*/

#include "rw0003_filter.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../aocs_manager.h"

// Satellite Parameters
#include "../../../../../Settings/SatelliteParameters/rw0003_parameters.h"

static Rw0003Filter        rw0003_filter_;
const  Rw0003Filter* const rw0003_filter = &rw0003_filter_;

static SpikeFilter APP_RW0003_FILTER_spike_[RW0003_IDX_MAX];  //!< スパイク除去フィルタ

static void APP_RW0003_FILTER_init_(void);
static void APP_RW0003_FILTER_exec_(void);

AppInfo APP_RW0003_FILTER_create_app(void)
{
  return AI_create_app_info("RW0003 Filter", APP_RW0003_FILTER_init_, APP_RW0003_FILTER_exec_);
}

static void APP_RW0003_FILTER_init_(void)
{
  for (uint8_t axis_id = 0; axis_id < RW0003_IDX_MAX; axis_id++)
  {
    rw0003_filter_.filter_error[axis_id] = C2A_MATH_ERROR_OK;
    rw0003_filter_.spike_filter_config[axis_id].count_limit_to_accept = RW0003_PARAMETERS_spike_filter_config_count_limit_to_accept[axis_id];
    rw0003_filter_.spike_filter_config[axis_id].count_limit_to_reject_continued_warning = RW0003_PARAMETERS_spike_filter_config_count_limit_to_reject_continued_warning[axis_id];
    rw0003_filter_.spike_filter_config[axis_id].reject_threshold = RW0003_PARAMETERS_spike_filter_config_reject_threshold_rad_s[axis_id];
    rw0003_filter_.spike_filter_config[axis_id].amplitude_limit_to_accept_as_step = RW0003_PARAMETERS_spike_filter_config_amplitude_limit_to_accept_as_step_rad_s[axis_id];
    C2A_MATH_ERROR ret = SPIKE_FILTER_init(&APP_RW0003_FILTER_spike_[axis_id],
                                           rw0003_filter_.spike_filter_config[axis_id]);
    if (ret != C2A_MATH_ERROR_OK)
    {
      Printf("RW0003 SpikeFilter init Failed at Axis %d ! \n", axis_id);
    }
  }

  return;
}

static void APP_RW0003_FILTER_exec_(void)
{
  float filtered_rotation_speed_rad_s[RW0003_IDX_MAX];
  for (uint8_t axis_id = 0; axis_id < RW0003_IDX_MAX; axis_id++)
  {
    rw0003_filter_.filter_error[axis_id] =
    SPIKE_FILTER_calc_output(&APP_RW0003_FILTER_spike_[axis_id],
                             &filtered_rotation_speed_rad_s[axis_id],
                             aocs_manager->rw_angular_velocity_rad_s[axis_id]);

    // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
    if (rw0003_filter_.filter_error[axis_id] == C2A_MATH_ERROR_OTHERS)
    {
      EL_record_event((EL_GROUP)(EL_GROUP_ERROR_RW0003X + axis_id),
                      (uint32_t)(RW0003_IDX_ON_X + axis_id),
                      EL_ERROR_LEVEL_HIGH,
                      (uint32_t)RW0003_EL_NOTE_NOISE_REMOVED);
    }
    else if (rw0003_filter_.filter_error[axis_id] == C2A_MATH_ERROR_RANGE_OVER)
    {
      // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
      // EL_record_event((EL_GROUP)(EL_GROUP_ERROR_RW0003X + axis_id),
      //                 (uint32_t)(RW0003_IDX_ON_X + axis_id),
      //                 EL_ERROR_LEVEL_LOW,
      //                 (uint32_t)RW0003_EL_NOTE_NOISE_REMOVED);
    }
    else
    {
      // 何もしない
    }
  }

  AOCS_MANAGER_set_rw_angular_velocity_rad_s(filtered_rotation_speed_rad_s);

  return;
}


CCP_CmdRet Cmd_APP_RW0003_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;
  SpikeFilter_Config config_received;

  RW0003_IDX axis_id = (RW0003_IDX)(param[read_out_offset]);
  if (axis_id >= RW0003_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  config_received.count_limit_to_accept = param[read_out_offset];
  read_out_offset++;

  config_received.count_limit_to_reject_continued_warning = param[read_out_offset];
  read_out_offset++;

  float reject_threshold;
  ENDIAN_memcpy(&reject_threshold, param + read_out_offset, sizeof(float));
  read_out_offset += sizeof(float);
  if (reject_threshold < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.reject_threshold = reject_threshold;

  float amplitude_limit_to_accept_as_step;
  ENDIAN_memcpy(&amplitude_limit_to_accept_as_step, param + read_out_offset, sizeof(float));
  read_out_offset += sizeof(float);
  if (amplitude_limit_to_accept_as_step < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.amplitude_limit_to_accept_as_step = amplitude_limit_to_accept_as_step;

  C2A_MATH_ERROR init_error = SPIKE_FILTER_init(&APP_RW0003_FILTER_spike_[axis_id], config_received);
  if (init_error != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  rw0003_filter_.spike_filter_config[axis_id] = config_received;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_RW0003_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  RW0003_IDX axis_id = (RW0003_IDX)(param[read_out_offset]);
  read_out_offset++;
  if (axis_id >= RW0003_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  SPIKE_FILTER_reset(&APP_RW0003_FILTER_spike_[axis_id]);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
