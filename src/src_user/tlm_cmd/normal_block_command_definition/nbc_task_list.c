#pragma section REPRO
/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#include "nbc_header.h"
#include "../telemetry_definitions.h"


void BCL_load_tl_start_up(void)    // STARTUPモード
{
  BCL_tool_register_combine(0, BC_AR_GS_RELATED_PROCESS);
  BCL_tool_register_app(8, AR_TLC_DISPATCHER_GS);
  BCL_tool_register_combine(10, BC_AC_TLM_CMD_HIRATE);
  BCL_tool_register_rotate(30, BC_AR_DRIVERS_UPDATE_INI);
  BCL_tool_register_app(40, AR_EVENT_UTILITY);
  BCL_tool_register_rotate(95, BC_AR_DEBUG_DISPLAY_INI);
}

void BCL_load_tl_initial(void)    // INITIALモード
{
  BCL_tool_register_combine(0, BC_AR_GS_RELATED_PROCESS);
  BCL_tool_register_app(8, AR_TLC_DISPATCHER_GS);
  BCL_tool_register_combine(10, BC_AC_TLM_CMD_HIRATE);
  BCL_tool_register_rotate(30, BC_AR_DRIVERS_UPDATE_INI);
  BCL_tool_register_app(40, AR_EVENT_UTILITY);
  BCL_tool_register_rotate(95, BC_AR_DEBUG_DISPLAY_INI);
}

#pragma section
