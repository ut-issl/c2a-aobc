#pragma section REPRO
#include "nbc_header.h"

#include "../block_command_definitions.h"
#include "../command_definitions.h"
#include "../telemetry_definitions.h"

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include "../../Applications/UserDefined/Power/power_switch_control.h"
#include "../../Applications/DriverInstances/di_ina260.h"
#include "../../Settings/System/EventHandlerRules/event_handler_rules.h"
#include "../../Settings/System/event_logger_group.h"

void BCL_load_power_on_mtq(void)
{
  cycle_t bc_cycle = 1;

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_MTQ);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle ++;

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
#endif
  bc_cycle += OBCT_sec2cycle(1);

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_MTQ_SEIREN);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);
  bc_cycle += OBCT_sec2cycle(3);

  // Initialize
  // 今のところ無し

  // Enable EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_MTQ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_MTQ_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_MTQ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_MTQ_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);

  // Total: 4.5sec程度
}

void BCL_load_power_off_mtq(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_MTQ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_MTQ_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_MTQ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_MTQ_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);

  // Power OFF
  bc_cycle += OBCT_sec2cycle(1);
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_MTQ_SEIREN);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);

  // Total: 2.5sec程度
}

void BCL_load_reset_mtq(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_OFF_MTQ, TLCD_ID_DEPLOY_BC); // 2.5sec
  bc_cycle += OBCT_sec2cycle(5);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_MTQ, TLCD_ID_DEPLOY_BC);
}

#pragma section
