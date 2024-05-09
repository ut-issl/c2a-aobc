#pragma section REPRO
#include "nbc_header.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Applications/DriverInstances/di_ina260.h>
#include <src_user/Applications/DriverInstances/di_rm3100.h>
#include <src_user/Settings/System/EventHandlerRules/event_handler_rules.h>
#include <src_user/Settings/System/event_logger_group.h>

void BCL_load_power_on_nanossoc_d60(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_NANOSSOC_D60);
  BCL_tool_prepare_param_float(150.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

#ifndef SILS_FW
  // Disable HW OC EL
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle += OBCT_sec2cycle(1);
#endif

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_NANOSSOC_D60);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);
  bc_cycle += OBCT_sec2cycle(3);

#ifndef SILS_FW // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(28.94f);
  BCL_tool_prepare_param_float(-17.68f);
  BCL_tool_prepare_param_float(77.99f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(97.61f);
  BCL_tool_prepare_param_float(-48.16f);
  BCL_tool_prepare_param_float(44.57f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_NANOSSOC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_NANOSSOC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle ++;
#endif

  // ACTIVATE EH
  BCL_tool_register_deploy(bc_cycle, BC_ACTIVATE_NANOSSOC_D60_EH, TLCD_ID_DEPLOY_BC); // 1.2sec

  // Total: 6.5sec程度
}

void BCL_load_power_off_nanossoc_d60(void)
{
  cycle_t bc_cycle = 10;

  // Inactivate EH
  BCL_tool_register_deploy(bc_cycle, BC_INACTIVATE_NANOSSOC_D60_EH, TLCD_ID_DEPLOY_BC); // 1.2sec
  bc_cycle += OBCT_sec2cycle(2);

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_NANOSSOC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_NANOSSOC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle += OBCT_sec2cycle(1);

  // Power OFF
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_NANOSSOC_D60);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);
  bc_cycle += OBCT_sec2cycle(1);

#ifndef SILS_FW // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(-28.94f);
  BCL_tool_prepare_param_float(17.68f);
  BCL_tool_prepare_param_float(-77.99f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(-97.61f);
  BCL_tool_prepare_param_float(48.16f);
  BCL_tool_prepare_param_float(-44.57f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif

  // Total: 4.5sec程度
}

void BCL_load_reset_nanossoc_d60(void)
{
  cycle_t bc_cycle = 1;

  BCL_tool_register_deploy(bc_cycle, BC_POWER_OFF_NANOSSOC_D60, TLCD_ID_DEPLOY_BC);  // 4.5sec
  bc_cycle += OBCT_sec2cycle(5);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_NANOSSOC_D60, TLCD_ID_DEPLOY_BC);
}

void BCL_load_activate_nanossoc_d60_eh(void)
{
  cycle_t bc_cycle = 1;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_0);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_1);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_2);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_3);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_0);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_1);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_2);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_3);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_NANOSSOC_D60);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_NANOSSOC_D60_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_NANOSSOC_D60);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_NANOSSOC_D60_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);

  // Total: 1.2sec程度
}

void BCL_load_inactivate_nanossoc_d60_eh(void)
{
  cycle_t bc_cycle = 1;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_0);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_1);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_2);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_NANOSSOC_D60_3);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_0);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_1);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_2);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_3);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_NANOSSOC_D60);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_NANOSSOC_D60_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_NANOSSOC_D60);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_NANOSSOC_D60_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  // Total: 1.2sec程度
}

#pragma section
