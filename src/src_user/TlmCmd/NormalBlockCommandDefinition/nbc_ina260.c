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

void BCL_load_power_on_ina260()
{
  cycle_t bc_cycle = 1;
  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_INA260);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);
  bc_cycle += OBCT_sec2cycle(1);

  // Initialize
  BCL_tool_prepare_param_uint8(INA260_IDX_PIC);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_STIM210);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_SAGITTA);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_OEM7600);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RM3100);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_1MS);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_1MS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_NANOSSOC_D60);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_MTQ);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_X);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_Y);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_Z);
  BCL_tool_prepare_param_uint8(INA260_AVERAGING_MODE_16);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_prepare_param_uint8(INA260_CONVERSION_TIME_140US);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_INIT);
  bc_cycle++;

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_INA260);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  BCL_tool_prepare_param_uint32(EL_GROUP_SW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  // Activate EH
  BCL_tool_register_deploy(bc_cycle, BC_ACTIVATE_INA260_EH, TLCD_ID_DEPLOY_BC); // 1.1 sec

  // Total: 4 sec程度
}

void BCL_load_set_ina260_oc_limit()
{
  cycle_t bc_cycle = 1;

  BCL_tool_prepare_param_uint8(INA260_IDX_PIC);
  BCL_tool_prepare_param_float(200.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_STIM210);
  BCL_tool_prepare_param_float(1000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_SAGITTA);
  BCL_tool_prepare_param_float(500.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_OEM7600);
  BCL_tool_prepare_param_float(1000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RM3100);
  BCL_tool_prepare_param_float(200.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_NANOSSOC_D60);
  BCL_tool_prepare_param_float(150.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_MTQ);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_X);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_Y);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_Z);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // Total: 1 sec程度
}

void BCL_load_power_off_ina260(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_PIC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_NANOSSOC_D60);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_MTQ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RW0003_X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RW0003_Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RW0003_Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_INA260);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;
#endif

  BCL_tool_prepare_param_uint32(EL_GROUP_SW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  // Power OFF
  bc_cycle += OBCT_sec2cycle(1);
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_INA260);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);

  // Total: 2 sec程度
}

void BCL_load_reset_ina260(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_OFF_INA260, TLCD_ID_DEPLOY_BC);  // 2sec
  bc_cycle += OBCT_sec2cycle(7);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_INA260, TLCD_ID_DEPLOY_BC);  // 4sec
  bc_cycle += OBCT_sec2cycle(5);

  // INA set over current Limit
  BCL_tool_register_deploy(bc_cycle, BC_SET_INA260_OC_LIMIT, TLCD_ID_DEPLOY_BC);

  // Total: 15 sec程度
}

void BCL_load_activate_ina260_eh(void)
{
  cycle_t bc_cycle = 1;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_PIC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_OEM7600);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RM3100);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_NANOSSOC_D60);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_MTQ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RW0003_X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RW0003_Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_INA260_RW0003_Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  // Total: 1 sec程度
}

#pragma section
