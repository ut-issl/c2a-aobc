/**
* @file   tl_elem_cdh_update.c
* @brief  CDH関連処理TL Element
*/
#pragma section REPRO
#include "tl_elem_cdh_update.h"

#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/Applications/app_registry.h>

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_cdh_update()
{
  BCL_tool_register_app(0, AR_DI_MOBC);
  BCL_tool_register_app(1, AR_GSC_DISPATCHER);
  BCL_tool_register_app(2, AR_TLC_DISPATCHER_GS);
  BCL_tool_register_app(3, AR_RTC_DISPATCHER);
  BCL_tool_register_app(4, AR_TLC_DISPATCHER_BC);
  BCL_tool_register_app(5, AR_TLC_DISPATCHER_TLM);
  BCL_tool_register_app(6, AR_DI_MOBC_MST_PH);
  BCL_tool_register_app(7, AR_EVENT_UTILITY);
  BCL_tool_register_app(8, AR_APP_AOCS_DATA_RECORDER);
}

#pragma section
