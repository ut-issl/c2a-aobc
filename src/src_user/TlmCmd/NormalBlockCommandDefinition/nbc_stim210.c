#pragma section REPRO
#include "nbc_header.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/EventManager/event_handler.h>

#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Applications/UserDefined/AOCS/HardwareDependent/SensorSelectors/gyro_selector.h>
#include <src_user/Applications/DriverInstances/di_ina260.h>
#include <src_user/Applications/DriverInstances/di_rm3100.h>
#include <src_user/Settings/System/EventHandlerRules/event_handler_rules.h>
#include <src_user/Settings/System/event_logger_group.h>
#include <src_user/Applications/DriverInstances/di_stim210.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/stim210_parameters.h>


void BCL_load_power_on_stim210(void)
{
  cycle_t bc_cycle = 1;

  // HW OC EH発動後にラッチ解除を行う。メモリの制約からHW OC EH専用のBCが作れないためここで対処。
  BCL_tool_prepare_param_uint8(INA260_IDX_STIM210);
  BCL_tool_prepare_param_float(1200.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
#endif
  bc_cycle += OBCT_sec2cycle(1);

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_STIM210);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(2);

#ifndef SILS_FW // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(STIM210_mag_bias_rm3100_ext_compo_nT[0]);
  BCL_tool_prepare_param_float(STIM210_mag_bias_rm3100_ext_compo_nT[1]);
  BCL_tool_prepare_param_float(STIM210_mag_bias_rm3100_ext_compo_nT[2]);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif

  // Initialize
  BCL_tool_prepare_param_uint8(STIM210_OPERATION_SERVICE_MODE);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_STIM210_SET_MODE);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(STIM210_NORMAL_MODE_RATE_TEMPERATURE);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_STIM210_SET_NORMAL_MODE_FORMAT);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(STIM210_GYRO_OUTPUT_AVERAGE_ANGULAR_RATE);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_STIM210_SET_GYRO_OUTPUT);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(STIM210_SAMPLE_RATE_EXTERNAL_TRIGGER);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_STIM210_SET_SAMPLE_RATE);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(STIM210_LPF_16HZ);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_STIM210_SET_LOW_PASS_FILTER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(STIM210_OPERATION_NORMAL_MODE);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_STIM210_SET_MODE);
  bc_cycle += OBCT_sec2cycle(3);

  // Gyro Selector
  BCL_tool_prepare_param_uint8(APP_GYRO_SELECTOR_STATE_STIM);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_GYRO_SELECTOR_SET_STATE);
  bc_cycle += OBCT_sec2cycle(1);

  // EL EH Activte
  BCL_tool_register_deploy(bc_cycle, BC_ACTIVATE_STIM210_EL_EH, TLCD_ID_DEPLOY_BC); // 1.2sec
  bc_cycle++;

  // 一時的に上げたINA260の過電流閾値を元に戻す
  BCL_tool_prepare_param_uint8(INA260_IDX_STIM210);
  BCL_tool_prepare_param_float(1000.0f);  // mA
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle++;

  // total: 14sec程度
}

void BCL_load_power_off_stim210(void)
{
  cycle_t bc_cycle = 1;

  // Inactivate EH/EL
  BCL_tool_register_deploy(bc_cycle, BC_INACTIVATE_STIM210_EL_EH, TLCD_ID_DEPLOY_BC); // 1.2sec
  bc_cycle += OBCT_sec2cycle(2);

  // Change selector
  BCL_tool_prepare_param_uint8(APP_GYRO_SELECTOR_STATE_MPU);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_GYRO_SELECTOR_SET_STATE);
  bc_cycle += OBCT_sec2cycle(1);

  // Power OFF
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_STIM210);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle++;

#ifndef SILS_FW  // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_float(0.0f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(-STIM210_mag_bias_rm3100_ext_compo_nT[0]);
  BCL_tool_prepare_param_float(-STIM210_mag_bias_rm3100_ext_compo_nT[1]);
  BCL_tool_prepare_param_float(-STIM210_mag_bias_rm3100_ext_compo_nT[2]);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif

  // 5sec
}

void BCL_load_reset_stim210(void)
{
  BCL_tool_register_deploy(OBCT_sec2cycle(1), BC_POWER_OFF_STIM210, TLCD_ID_DEPLOY_BC);

  BCL_tool_register_deploy(OBCT_sec2cycle(6), BC_POWER_ON_STIM210, TLCD_ID_DEPLOY_BC);
}

void BCL_load_activate_stim210_el_eh(void)
{
  cycle_t bc_cycle = 1;
  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_CRC_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);

  // 1.2sec
}

void BCL_load_inactivate_stim210_el_eh(void)
{
  cycle_t bc_cycle = 1;

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_CRC_ERROR_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_STIM210_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_CRC_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_STIM210);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  // 1.2sec
}

#pragma section
