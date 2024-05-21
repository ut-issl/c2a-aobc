#pragma section REPRO
#include "nbc_header.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>

#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Applications/DriverInstances/di_sagitta.h>
#include <src_user/Applications/DriverInstances/di_ina260.h>
#include <src_user/Applications/DriverInstances/di_rm3100.h>
#include <src_user/Settings/System/EventHandlerRules/event_handler_rules.h>
#include <src_user/Settings/System/event_logger_group.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/sagitta_parameters.h>

void BCL_load_power_on_sagitta(void)
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  // Disable HW OC EL
#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
#endif
  bc_cycle++;

  // 一時的にINA260の過電流閾値をあげる
  BCL_tool_prepare_param_uint8(INA260_IDX_SAGITTA);
  BCL_tool_prepare_param_float(1200);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);
  bc_cycle += OBCT_sec2cycle(1);

  // Power ON
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_SAGITTA);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_ON);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(2);

  // Boot
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_BOOT);
  bc_cycle += OBCT_sec2cycle(2);

#ifndef SILS_FW  // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(445.64f);
  BCL_tool_prepare_param_float(478.48f);
  BCL_tool_prepare_param_float(-885.402f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[0]);
  BCL_tool_prepare_param_float(SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[1]);
  BCL_tool_prepare_param_float(SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[2]);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif

  // Set TLM
  BCL_tool_register_deploy(bc_cycle, BC_SET_SAGITTA_PARAMETER, TLCD_ID_DEPLOY_BC);
  bc_cycle += OBCT_sec2cycle(16);

  // Read TLM
  BCL_tool_register_deploy(bc_cycle, BC_READ_SAGITTA_PARAMETER, TLCD_ID_DEPLOY_BC);
  bc_cycle += OBCT_sec2cycle(16);

  // Set Subscription
  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_SUBSCRIPTION);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(3);

  // INA260の過電流閾値をもとに戻す
  BCL_tool_prepare_param_uint8(INA260_IDX_SAGITTA);
  BCL_tool_prepare_param_float(500);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_INA260_SET_OVER_CURRENT_PROTECTION);

  // Activate EL/EH
  BCL_tool_register_deploy(bc_cycle, BC_ACTIVATE_SAGITTA_EL_EH, TLCD_ID_DEPLOY_BC); // 1sec
  bc_cycle += OBCT_sec2cycle(1);

  // Total: 42~43sec程度
}

void BCL_load_power_off_sagitta(void)
{
  cycle_t  bc_cycle = OBCT_sec2cycle(1);

  // Inactivate EL/EH
  BCL_tool_register_deploy(bc_cycle, BC_INACTIVATE_SAGITTA_EL_EH, TLCD_ID_DEPLOY_BC); // 1sec
  bc_cycle += OBCT_sec2cycle(1);

  // Power OFF
  BCL_tool_prepare_param_uint8(APP_PSC_UNREG_IDX_SAGITTA);
  BCL_tool_prepare_param_uint8(APP_PSC_STATE_OFF);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_APP_PSC_CONTROL_UNREG_PORT);
  bc_cycle += OBCT_sec2cycle(1);

#ifndef SILS_FW  // TODO_L S2Eに電源ON/OFFでのバイアス変更機能を追加する
  // 磁気バイアス補正
  BCL_tool_prepare_param_uint8(RM3100_IDX_ON_AOBC);
  BCL_tool_prepare_param_float(-445.64f);
  BCL_tool_prepare_param_float(-478.48f);
  BCL_tool_prepare_param_float(885.402f);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;

  BCL_tool_prepare_param_uint8(RM3100_IDX_EXTERNAL);
  BCL_tool_prepare_param_float(-SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[0]);
  BCL_tool_prepare_param_float(-SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[1]);
  BCL_tool_prepare_param_float(-SAGITTA_PARAMETERS_mag_bias_rm3100_ext_compo_nT[2]);
  BCL_tool_prepare_param_uint8(1); // Add

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_RM3100_SET_MAG_BIAS_COMPO_NT);
  bc_cycle++;
#endif
}

void BCL_load_reset_sagitta(void)
{
  cycle_t bc_cycle = 1;
  BCL_tool_register_deploy(bc_cycle, BC_POWER_OFF_SAGITTA, TLCD_ID_DEPLOY_BC);
  bc_cycle += OBCT_sec2cycle(5);

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_DS_INIT);
  bc_cycle++;

  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_DS_INIT_STREAM_REC_BUFFER);
  bc_cycle++;

  BCL_tool_register_deploy(bc_cycle, BC_POWER_ON_SAGITTA, TLCD_ID_DEPLOY_BC);
}

void BCL_set_sagitta_parameter(void)
{
  cycle_t  bc_cycle = 1;

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_LOG_LEVEL);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_LIMITS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_MOUNTING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_DISTORTION);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_CAMERA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_CENTROIDING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_LISA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_MATCHING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_TRACKING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_VALIDATION);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_ALGO);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_AUTO_THRESHOLD);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_FAST_LISA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_NOISE_LIMITS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_BLOB_FILTER);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_SET_PARAMETER);

  // 16sec
}

void BCL_read_sagitta_parameter(void)
{
  cycle_t  bc_cycle = 1;

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_LOG_LEVEL);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_LIMITS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_MOUNTING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_DISTORTION);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_CAMERA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_CENTROIDING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_LISA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_MATCHING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_TRACKING);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_VALIDATION);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_ALGO);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_SUBSCRIPTION);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_AUTO_THRESHOLD);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_FAST_LISA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_NOISE_LIMITS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_PARAMETER_ID_BLOB_FILTER);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_READ_PARAMETER);

  // 16sec
}


void BCL_load_activate_sagitta_el_eh(void)
{
  cycle_t bc_cycle = 1;

  // Enable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_XXHASH_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;

#ifndef SILS_FW
  BCL_tool_prepare_param_uint32(EL_GROUP_HW_OC);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_ENABLE_LOGGING);
  bc_cycle++;
#endif

  // Activate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_XXHASH_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_SAGITTA_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_SAGITTA_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL);

  // 1sec
}


void BCL_load_inactivate_sagitta_el_eh(void)
{
  cycle_t bc_cycle = 1;

  // Inactivate EH
  BCL_tool_prepare_param_uint16(EH_RULE_TLM_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_XXHASH_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_HW_OC_SAGITTA_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE);
  bc_cycle++;

  BCL_tool_prepare_param_uint16(EH_RULE_SW_OC_SAGITTA_BROKEN);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL);
  bc_cycle++;

  // Disable EL
  BCL_tool_prepare_param_uint32(EL_GROUP_TLM_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_XXHASH_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);
  bc_cycle++;

  BCL_tool_prepare_param_uint32(EL_GROUP_ERROR_SAGITTA);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_EL_DISABLE_LOGGING);

  // 1sec
}

#pragma section
