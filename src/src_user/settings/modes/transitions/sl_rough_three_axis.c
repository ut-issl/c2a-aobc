/**
* @file   sl_rough_three_axis.c
* @brief  粗三軸制御モードのシーケンスリスト
*/

#pragma section REPRO
#include "sl_rough_three_axis.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/Library/physical_constants.h>

#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/fdir_parameters.h>

void BCL_load_rough_sun_pointing_to_rough_three_axis(void)
{
  uint32_t timing_sec = 1;
  // 自動モード遷移OFF
  BCL_tool_prepare_param_uint8(0);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);

  // 自動モード遷移閾値変更
  timing_sec++;
  BCL_tool_prepare_param_float(FDIR_PARAMETERS_rough_three_axis_mtq_div_limit_rad);
  BCL_tool_prepare_param_float(FDIR_PARAMETERS_rough_three_axis_mtq_div_time_limit_s);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_THREE_AXIS_THRESHOLD);

  // モード遷移完了
  timing_sec += 5;
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_MM_FINISH_TRANSITION);

  // 自動モード遷移ON
  timing_sec += FDIR_PARAMETERS_rough_three_axis_mtq_start_mode_manager_time_s;
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);
}

#pragma section
