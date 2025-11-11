#pragma section REPRO
/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#include "nbc_header.h"
#include "../telemetry_definitions.h"


void BCL_load_ar_drivers_update_ini(void)
{
  BCL_tool_register_app(0, AR_NOP);
}

void BCL_load_ar_debug_display_ini(void)
{
  BCL_tool_register_app(0, AR_APP_DBG_PRINT_GIT_REV);
  BCL_tool_register_app(1, AR_APP_DBG_PRINT_TIMESTAMP);
  BCL_tool_register_app(2, AR_APP_DBG_PRINT_CMD_STATUS);
  BCL_tool_register_app(3, AR_APP_DBG_PRINT_EVENT_LOGGER0);
  BCL_tool_register_app(4, AR_APP_DBG_PRINT_EVENT_LOGGER1);
  BCL_tool_register_app(5, AR_APP_DBG_PRINT_EVENT_HANDLER);
  BCL_tool_register_app(6, AR_NOP);
  BCL_tool_register_app(7, AR_NOP);
  BCL_tool_register_app(8, AR_NOP);
  BCL_tool_register_app(9, AR_APP_DBG_FLUSH_SCREEN);
}

void BCL_load_ar_gs_related_process(void)
{
  BCL_tool_register_app(0, AR_CSRV_MOBC_CMD_PH);
  BCL_tool_register_app(1, AR_GSC_DISPATCHER);
}

void BCL_load_ac_tlm_cmd_hirate(void)
{
  BCL_tool_register_app(0, AR_RTC_DISPATCHER);
  BCL_tool_register_app(1, AR_TLC_DISPATCHER_BC);
  BCL_tool_register_app(2, AR_TLC_DISPATCHER_TLM);
  BCL_tool_register_app(3, AR_CSRV_MOBC_RT_TLM_PH);
}

#pragma section
