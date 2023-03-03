/**
* @file   sl_rough_three_axis.c
* @brief  粗三軸制御モードのシーケンスリスト
*/

#pragma section REPRO
#include "sl_rough_three_axis.h"

#include "../../../TlmCmd/block_command_definitions.h"
#include "../../../TlmCmd/command_definitions.h"
#include "../../../Library/physical_constants.h"

#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>


void BCL_load_rough_sun_pointing_to_rough_three_axis(void)
{
  uint32_t timing_sec = 1;
  // 自動モード遷移OFF
  BCL_tool_prepare_param_uint8(0);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);

  // 自動モード遷移閾値変更
  timing_sec++;
  BCL_tool_prepare_param_float(PHYSICAL_CONST_degree_to_radian(30.0f));
  BCL_tool_prepare_param_float(5.0f * 60.0f);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_THREE_AXIS_THRESHOLD);

  // モード遷移完了
  timing_sec += 5;
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_MM_FINISH_TRANSITION);

  // 自動モード遷移ON
  timing_sec += (35 * 60);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);
}

#pragma section
