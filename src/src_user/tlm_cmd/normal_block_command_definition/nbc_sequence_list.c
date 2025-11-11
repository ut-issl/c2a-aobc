#pragma section REPRO
/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#include "nbc_header.h"
#include "../telemetry_definitions.h"


void BCL_load_sl_start_up_to_initial(void)
{
  BCL_tool_register_deploy(10, BC_HK_CYCLIC_TLM, TLCD_ID_DEPLOY_TLM);
  // 遷移自体はここで終了
  BCL_tool_register_cmd(100, Cmd_CODE_MM_FINISH_TRANSITION);
}

void BCL_load_sl_nop(void)
{
}

#pragma section
