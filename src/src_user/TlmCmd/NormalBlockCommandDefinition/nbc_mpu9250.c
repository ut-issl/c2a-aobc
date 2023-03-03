#pragma section REPRO
#include "nbc_header.h"

#include "../block_command_definitions.h"
#include "../command_definitions.h"
#include "../telemetry_definitions.h"

#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include "../../Applications/UserDefined/Power/power_switch_control.h"
#include "../../Settings/System/EventHandlerRules/event_handler_rules.h"
#include "../../Settings/System/event_logger_group.h"

void BCL_load_power_on_mpu9250(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle += OBCT_sec2cycle(1);
#endif

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_MPU9250);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);
  bc_cycle += OBCT_sec2cycle(1);

  // Initialize
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_MPU9250_INIT);
  bc_cycle += OBCT_sec2cycle(3);

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_MPU9250);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_MPU9250);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_MPU9250);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);

  // total: 5.5sec程度
}

void BCL_load_power_off_mpu9250(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_MPU9250);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_MPU9250);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_MPU9250);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  // Power OFF
  bc_cycle += OBCT_sec2cycle(1);
  BCL_tool_prepare_param_uint8(APP_PSC_5V_IDX_MPU9250);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_5V_PORT);

  // total: 2.5sec程度
}

void BCL_load_reset_mpu9250(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  BCL_tool_register_deploy(OBCT_sec2cycle(1), BC_POWER_OFF_MPU9250, TLCD_ID_DEPLOY_BC); // 2.5sec
  bc_cycle += OBCT_sec2cycle(4);

  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_MPU9250, TLCD_ID_DEPLOY_BC);  // 5.5sec

  // total: 15sec程度
}

#pragma section
