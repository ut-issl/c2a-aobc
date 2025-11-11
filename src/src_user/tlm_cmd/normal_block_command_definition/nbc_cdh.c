#pragma section REPRO
/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#include "nbc_header.h"
#include "../telemetry_definitions.h"


void BCL_load_hk_cyclic_tlm(void)
{
  BCL_tool_prepare_param_uint8(Tlm_CODE_AOBC_HK);
  BCL_tool_register_cmd(1, Cmd_CODE_TG_GENERATE_RT_TLM);
  BCL_tool_register_deploy(10, BC_HK_CYCLIC_TLM, TLCD_ID_DEPLOY_TLM);
}

#pragma section
