/**
* @file   sl_bdot.c
* @brief  Bdot制御モードのシーケンスリスト
*/

#pragma section REPRO
#include "sl_bdot.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Applications/DriverInstances/di_rm3100.h>
#include <src_user/Applications/UserDefined/AOCS/HardwareDependent/SensorSelectors/magnetometer_selector.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/fdir_parameters.h>
#include <src_user/Settings/SatelliteParameters/component_selector_parameters.h>

void BCL_load_initial_to_bdot(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // MPU9250 ON
  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_MPU9250, TLCD_ID_DEPLOY_BC); // 5.5sec
  bc_cycle += OBCT_sec2cycle(7);

  // RM3100 ON
  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_RM3100, TLCD_ID_DEPLOY_BC); // 8.5sec
  bc_cycle += OBCT_sec2cycle(10);

  // MTQ ON
  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_MTQ, TLCD_ID_DEPLOY_BC); // 4.5sec
  bc_cycle += OBCT_sec2cycle(5);

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_MM_FINISH_TRANSITION);
}

void BCL_load_any_to_bdot(void)
{
  uint32_t time_sec = 1;
  // 自動モード遷移OFF
  BCL_tool_prepare_param_uint8(0);
  BCL_tool_register_cmd(OBCT_sec2cycle(time_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);
  time_sec += 1;

  // RW X OFF
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_POWER_OFF_RWX, TLCD_ID_DEPLOY_BC);
  time_sec += 4;  // OFF BCで2秒使う

  // RW Y OFF
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_POWER_OFF_RWY, TLCD_ID_DEPLOY_BC);
  time_sec += 4;  // OFF BCで2秒使う

  // RW Z OFF
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_POWER_OFF_RWZ, TLCD_ID_DEPLOY_BC);
  time_sec += 4;  // OFF BCで2秒使う

  // GPS-R OFF
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_POWER_OFF_OEM7600, TLCD_ID_DEPLOY_BC);
  time_sec += 4;  // OFF BCで3.5秒使う

  // STIM OFF
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_POWER_OFF_STIM210, TLCD_ID_DEPLOY_BC);
  time_sec += 4;  // OFF BCで3.5秒使う

  // STT OFF
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_POWER_OFF_SAGITTA, TLCD_ID_DEPLOY_BC);
  time_sec += 4;  // OFF BCで3.5秒使う

  // SS OFF
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_POWER_OFF_NANOSSOC_D60, TLCD_ID_DEPLOY_BC);
  time_sec += 7;  // OFF BCで4秒使う

  // INA Reset
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_RESET_INA260, TLCD_ID_DEPLOY_BC);
  time_sec += 16;  // Reset BCで15秒使う

  // MTQ Reset
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_RESET_MTQ, TLCD_ID_DEPLOY_BC);
  time_sec += 10;  // Reset BCで8.5秒使う

  // MPU9250 reset
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_RESET_MPU9250, TLCD_ID_DEPLOY_BC);
  time_sec += 16;  // Reset BCで15秒使う

  // RM3100 reset
  BCL_tool_register_deploy(OBCT_sec2cycle(time_sec), BC_RESET_RM3100, TLCD_ID_DEPLOY_BC);
  time_sec += 18;  // Reset BCで16秒使う

  // External磁気センサバイアスデフォルト値にクリア
  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_uint8(2); // Clear default

  BCL_tool_register_cmd(OBCT_sec2cycle(time_sec), Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  time_sec += 1;

  // 使う磁気センサを設定
  BCL_tool_prepare_param_uint8(COMPONENT_SELECTOR_PARAMETERS_initial_selected_magnetometer);
  BCL_tool_register_cmd(OBCT_sec2cycle(time_sec), Cmd_CODE_APP_MAG_SELECTOR_SET_STATE);
  time_sec += 1;

  BCL_tool_register_cmd(OBCT_sec2cycle(time_sec), Cmd_CODE_MM_FINISH_TRANSITION);

  // 自動モード遷移ON
  time_sec += FDIR_PARAMETERS_bdot_start_mode_manager_time_s;
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd(OBCT_sec2cycle(time_sec), Cmd_CODE_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION);

  // 16個ギリギリなので注意
}

#pragma section
