/**
* @file   sl_rough_sun_pointing.c
* @brief  粗太陽補足制御モードのシーケンスリスト
*/

#pragma section REPRO
#include "sl_rough_sun_pointing.h"

#include "../../../TlmCmd/block_command_definitions.h"
#include "../../../TlmCmd/command_definitions.h"
#include "../../../Applications/UserDefined/Power/power_switch_control.h"

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>

void BCL_load_bdot_to_rough_sun_pointing(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // 自動モード遷移OFF
  BCL_tool_prepare_param_uint8(0);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);
  bc_cycle += OBCT_sec2cycle(1);

  // NanoSSOC-D60 ON
  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_NANOSSOC_D60, TLCD_ID_DEPLOY_BC); // 6.5sec
  bc_cycle += OBCT_sec2cycle(10);

  // モード遷移完了
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_MM_FINISH_TRANSITION);

  // 自動モード遷移ON
  bc_cycle += OBCT_sec2cycle(25 * 60);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);
}

#pragma section
