#pragma section REPRO
#include "nbc_header.h"

#include "../block_command_definitions.h"
#include "../command_definitions.h"
#include "../telemetry_definitions.h"

#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include "../../Applications/UserDefined/Power/power_switch_control.h"
#include "../../Applications/DriverInstances/di_rm3100.h"
#include "../../Applications/DriverInstances/di_ina260.h"
#include "../../Applications/UserDefined/AOCS/HardwareDependent/SensorSelectors/magnetometer_selector.h"
#include "../../Settings/System/EventHandlerRules/event_handler_rules.h"
#include "../../Settings/System/event_logger_group.h"

void BCL_load_power_on_rm3100()
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_RM3100);
  BCL_tool_prepare_param_float(200.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle += OBCT_sec2cycle(1);
#endif

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_RM3100);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);
  bc_cycle += OBCT_sec2cycle(2);

  // Initialize
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_INIT);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_INIT);
  bc_cycle += OBCT_sec2cycle(3);

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_ON_AOBC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_ON_AOBC_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_EXTERNAL);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_EXTERNAL_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RM3100_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RM3100_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);

  // Total: 8~9sec程度
}

void BCL_load_power_off_rm3100(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_ON_AOBC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_ON_AOBC_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_EXTERNAL);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RM3100_EXTERNAL_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RM3100_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RM3100_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);

  // Power OFF
  bc_cycle += OBCT_sec2cycle(1);
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_RM3100);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);

  // Total: 3sec程度
}

void BCL_load_reset_rm3100(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_OFF_RM3100, TLCD_ID_DEPLOY_BC); // 3sec
  bc_cycle += OBCT_sec2cycle(5);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_RM3100, TLCD_ID_DEPLOY_BC);  // 10sec

  // 16sec
}

void BCL_load_select_rm3100_on_aobc(void)
{
  BCL_tool_prepare_param_uint8(APP_MAG_SELECTOR_STATE_RM_AOBC);
  BCL_tool_register_cmd(OBCT_sec2cycle(1), Cmd_CODE_APP_MAG_SELECTOR_SET_STATE);
}

void BCL_load_select_rm3100_external(void)
{
  BCL_tool_prepare_param_uint8(APP_MAG_SELECTOR_STATE_RM_EXT);
  BCL_tool_register_cmd(OBCT_sec2cycle(1), Cmd_CODE_APP_MAG_SELECTOR_SET_STATE);
}

#pragma section
