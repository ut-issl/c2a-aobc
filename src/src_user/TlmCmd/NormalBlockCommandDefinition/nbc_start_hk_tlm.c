#pragma section REPRO
#include "nbc_header.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

#include <src_core/Applications/timeline_command_dispatcher_id_define.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_user/Applications/DriverInstances/di_sagitta.h>



void BCL_load_start_hk_tlm()
{
  // HK General
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK_GEN);
  BCL_tool_register_cmd       (1, Cmd_CODE_TG_GENERATE_RT_TLM);

  // HK Components
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK_COMPO);
  BCL_tool_register_cmd       (3, Cmd_CODE_TG_GENERATE_RT_TLM);

  // SAGITTA1
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_SAGITTA1);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd       (5, Cmd_CODE_GENERATE_TLM);

  // SAGITTA2
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_SAGITTA2);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd       (9, Cmd_CODE_GENERATE_TLM);

  // SAGITTA3
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_SAGITTA3);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd       (13, Cmd_CODE_GENERATE_TLM);

  // SAGITTA4
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_SAGITTA4);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd       (17, Cmd_CODE_GENERATE_TLM);

  BCL_tool_register_deploy (OBCT_sec2cycle(2), BC_HK_CYCLIC_TLM, TLCD_ID_DEPLOY_TLM);
}

void BCL_load_stt_synchronous()
{
  cycle_t bc_cycle = OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_POWER);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_SOLUTION);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_TEMPERATURE);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_HISTOGRAM);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_BLOBS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_CENTROIDS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_AUTO_BLOB);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_prepare_param_uint8(SAGITTA_TLM_ID_MATCHED_CENTROIDS);
  BCL_tool_register_cmd(bc_cycle, Cmd_CODE_DI_SAGITTA_REQUEST_SYNCHRONOUS_TELEMETRY);
  bc_cycle += OBCT_sec2cycle(1);

  BCL_tool_register_deploy(bc_cycle, BC_STT_SYNCHRONOUS_TLM, TLCD_ID_DEPLOY_TLM);
}

#pragma section
