#pragma section REPRO
#include "nbc_header.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Applications/DriverInstances/di_oem7600.h>
#include <src_user/Applications/DriverInstances/di_ina260.h>
#include <src_user/Applications/DriverInstances/di_rm3100.h>
#include <src_user/Settings/System/EventHandlerRules/event_handler_rules.h>
#include <src_user/Settings/System/event_logger_group.h>

void BCL_load_power_on_oem7600(void)
{
  cycle_t bc_cycle = 1;

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_OEM7600);
  BCL_tool_prepare_param_float(1000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
#endif
  bc_cycle += OBCT_sec2cycle(1);

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_OEM7600);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(1);

  // Reset GPIO control
  BCL_tool_prepare_param_uint8(OEM7600_IDX_IN_UNIT);
  BCL_tool_prepare_param_uint8(1);  // High
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_OEM7600_SET_RESET_GPIO_HIGHLOW);
  bc_cycle += OBCT_sec2cycle(5);

  BCL_tool_prepare_param_uint8(OEM7600_IDX_IN_UNIT);
  BCL_tool_prepare_param_uint8(0);  // Low
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_OEM7600_SET_RESET_GPIO_HIGHLOW);
  bc_cycle += OBCT_sec2cycle(5);

  BCL_tool_prepare_param_uint8(OEM7600_IDX_IN_UNIT);
  BCL_tool_prepare_param_uint8(1);  // On
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_OEM7600_ONOFF_ANTENNA_POWER);
  bc_cycle += OBCT_sec2cycle(3);

#ifndef SILS_FW  // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(654.19f);
  BCL_tool_prepare_param_float(374.0f);
  BCL_tool_prepare_param_float(348.14f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(223.03f);
  BCL_tool_prepare_param_float(-126.81f);
  BCL_tool_prepare_param_float(202.58f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif

  // Activate EL/EH
  BCL_tool_register_deploy(bc_cycle, BC_ACTIVATE_OEM7600_EL_EH, TLCD_ID_DEPLOY_BC); // 1sec
  bc_cycle += OBCT_sec2cycle(1);

}

void BCL_load_power_off_oem7600(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EL/EH
  BCL_tool_register_deploy(bc_cycle, BC_INACTIVATE_OEM7600_EL_EH, TLCD_ID_DEPLOY_BC); // 1sec
  bc_cycle += OBCT_sec2cycle(1);

  // Power OFF
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_OEM7600);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(1);

#ifndef SILS_FW  // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(654.19f);
  BCL_tool_prepare_param_float(374.0f);
  BCL_tool_prepare_param_float(348.14f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(-223.03f);
  BCL_tool_prepare_param_float(126.81f);
  BCL_tool_prepare_param_float(-202.58f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif
}

void BCL_load_reset_oem7600(void)
{
  BCL_tool_register_deploy(OBCT_sec2cycle(1), BC_POWER_OFF_OEM7600, TLCD_ID_DEPLOY_BC);

  BCL_tool_register_deploy(OBCT_sec2cycle(6), BC_POWER_ON_OEM7600, TLCD_ID_DEPLOY_BC);
}

void BCL_load_activate_oem7600_el_eh(void)
{
  cycle_t bc_cycle = 1;

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_OEM7600_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_OEM7600_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);

  // 1sec
}

void BCL_load_inactivate_oem7600_el_eh(void)
{
  cycle_t bc_cycle = 1;

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_OEM7600_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_OEM7600_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);

  // 1sec
}

#pragma section
