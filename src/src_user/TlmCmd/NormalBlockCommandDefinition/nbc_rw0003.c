#pragma section REPRO
#include "nbc_header.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Applications/DriverInstances/di_rw0003.h>
#include <src_user/Applications/DriverInstances/di_ina260.h>
#include <src_user/Settings/System/EventHandlerRules/event_handler_rules.h>
#include <src_user/Settings/System/event_logger_group.h>

void BCL_load_power_on_rwx(void)
{
  cycle_t bc_cycle = 1;

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_X);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
#endif
  bc_cycle += OBCT_sec2cycle(1);

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_RWX);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(2);

  // Initialize
  BCL_tool_prepare_param_uint8(RW0003_IDX_ON_X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RW0003_INIT);
  bc_cycle += OBCT_sec2cycle(3);

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RWX);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_RWX);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003X_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003X_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
}

void BCL_load_power_on_rwy(void)
{
  cycle_t bc_cycle = 1;

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_Y);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
#endif
  bc_cycle += OBCT_sec2cycle(1);

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_RWY);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(2);

  // Initialize
  BCL_tool_prepare_param_uint8(RW0003_IDX_ON_Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RW0003_INIT);
  bc_cycle += OBCT_sec2cycle(3);

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RWY);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_RWY);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Y_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Y_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
}

void BCL_load_power_on_rwz(void)
{
  cycle_t bc_cycle = 1;

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_RW0003_Z);
  BCL_tool_prepare_param_float(2000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
#endif
  bc_cycle += OBCT_sec2cycle(1);

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_RWZ);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(2);

  // Initialize
  BCL_tool_prepare_param_uint8(RW0003_IDX_ON_Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RW0003_INIT);
  bc_cycle += OBCT_sec2cycle(3);

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RWZ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_RWZ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Z_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Z_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
}

void BCL_load_power_off_rwx(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RWX);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_RWX);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003X_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003X_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RW0003X);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  // Power OFF
  bc_cycle += OBCT_sec2cycle(1);
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_RWX);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
}

void BCL_load_power_off_rwy(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RWY);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_RWY);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Y_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Y_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RW0003Y);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  // Power OFF
  bc_cycle += OBCT_sec2cycle(1);
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_RWY);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
}

void BCL_load_power_off_rwz(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_RWZ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_RWZ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_RW0003Z_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_RW0003Z_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_RW0003Z);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  // Power OFF
  bc_cycle += OBCT_sec2cycle(1);
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_RWZ);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
}

void BCL_load_reset_rwx(void)
{
  BCL_tool_register_deploy(OBCT_sec2cycle(1), BC_POWER_OFF_RWX, TLCD_ID_DEPLOY_BC);

  BCL_tool_register_deploy(OBCT_sec2cycle(6), BC_POWER_ON_RWX, TLCD_ID_DEPLOY_BC);
}

void BCL_load_reset_rwy(void)
{
  BCL_tool_register_deploy(OBCT_sec2cycle(1), BC_POWER_OFF_RWY, TLCD_ID_DEPLOY_BC);

  BCL_tool_register_deploy(OBCT_sec2cycle(6), BC_POWER_ON_RWY, TLCD_ID_DEPLOY_BC);
}

void BCL_load_reset_rwz(void)
{
  BCL_tool_register_deploy(OBCT_sec2cycle(1), BC_POWER_OFF_RWZ, TLCD_ID_DEPLOY_BC);

  BCL_tool_register_deploy(OBCT_sec2cycle(6), BC_POWER_ON_RWZ, TLCD_ID_DEPLOY_BC);
}

#pragma section
