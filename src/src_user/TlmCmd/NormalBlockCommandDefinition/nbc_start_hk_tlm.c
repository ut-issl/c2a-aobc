#pragma section REPRO
#include "nbc_header.h"

#include "../block_command_definitions.h"
#include "../command_definitions.h"
#include "../telemetry_definitions.h"

#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_start_hk_tlm()
{
  // HK General
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK_GEN);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd       (1, Cmd_CODE_GENERATE_TLM);

  // HK Components
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK_COMPO);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd       (2, Cmd_CODE_GENERATE_TLM);

  // HK Algorithm
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK_ALGO);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd       (4, Cmd_CODE_GENERATE_TLM);

  BCL_tool_register_deploy (OBCT_sec2cycle(1), BC_HK_CYCLIC_TLM, TLCD_ID_DEPLOY_TLM);
}

#pragma section
