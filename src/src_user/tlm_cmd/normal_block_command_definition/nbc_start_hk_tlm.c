#pragma section REPRO
#include "nbc_header.h"

#include "../block_command_definitions.h"
#include "../command_definitions.h"
#include "../telemetry_definitions.h"

#include <src_core/applications/timeline_command_dispatcher_id_define.h>
#include <src_core/tlm_cmd/block_command_loader.h>


void BCL_load_start_hk_tlm()
{
  // HK General
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK);
  BCL_tool_register_cmd       (1, Cmd_CODE_TG_GENERATE_RT_TLM);

  // // HK Components
  // BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK_COMPO);
  // BCL_tool_register_cmd       (2, Cmd_CODE_TG_GENERATE_RT_TLM);

  // // HK Algorithm
  // BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK_ALGO);
  // BCL_tool_register_cmd       (4, Cmd_CODE_TG_GENERATE_RT_TLM);

  BCL_tool_register_deploy (OBCT_sec2cycle(1), BC_HK_CYCLIC_TLM, TLCD_ID_DEPLOY_TLM);
}

#pragma section
