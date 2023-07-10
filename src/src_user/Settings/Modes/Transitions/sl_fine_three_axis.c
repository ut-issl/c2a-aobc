/**
* @file   sl_fine_three_axis.h
* @brief  精三軸制御モードのシーケンスリスト
*/

#pragma section REPRO
#include "sl_fine_three_axis.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/Applications/UserDefined/AOCS/HardwareDependent/SensorSelectors/gyro_selector.h>
#include <src_user/Library/physical_constants.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/fdir_parameters.h>

void BCL_load_rough_three_axis_rw_to_fine_three_axis(void)
{
  uint32_t timing_sec = 1;
  // 自動モード遷移OFF
  BCL_tool_prepare_param_uint8(0);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);
  timing_sec++;

  // 自動モード遷移閾値変更
  BCL_tool_prepare_param_float(FDIR_PARAMETERS_fine_three_axis_div_limit_rad);
  BCL_tool_prepare_param_float(FDIR_PARAMETERS_fine_three_axis_div_time_limit_s);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_THREE_AXIS_THRESHOLD);
  timing_sec++;

  // STIM ON
  BCL_tool_prepare_param_uint8(TLCD_ID_DEPLOY_BC);
  BCL_tool_prepare_param_uint16(BC_POWER_ON_STIM210);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_TLCD_DEPLOY_BLOCK); // 14sec
  timing_sec += 14;


  // STT ON
  BCL_tool_prepare_param_uint8(TLCD_ID_DEPLOY_BC);
  BCL_tool_prepare_param_uint16(BC_POWER_ON_SAGITTA);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_TLCD_DEPLOY_BLOCK); // 34.5sec
  timing_sec += 35;

  // モード遷移完了
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_MM_FINISH_TRANSITION);

  // 自動モード遷移ON
  timing_sec += FDIR_PARAMETERS_fine_three_axis_start_mode_manager_time_s;
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd(OBCT_sec2cycle(timing_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);
}

#pragma section
