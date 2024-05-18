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
  BCL_tool_register_app(0, AR_DI_MOBC); // 1step以上
  BCL_tool_register_app(1, AR_GSC_DISPATCHER); // 1step以上
  BCL_tool_register_app(2, AR_TLC_DISPATCHER_GS); // 1step以上
  BCL_tool_register_app(3, AR_RTC_DISPATCHER); // 1step以上
  BCL_tool_register_app(4, AR_TLC_DISPATCHER_BC); // 2step以上
  BCL_tool_register_app(5, AR_TLC_DISPATCHER_TLM); // 1step以上
  BCL_tool_register_app(6, AR_DI_MOBC_RT_TLM_PH); // 19step以上
  BCL_tool_register_app(7, AR_EVENT_UTILITY); // 2step以上
  BCL_tool_register_app(8, AR_APP_AOCS_DATA_RECORDER); // 1step以上
  BCL_tool_register_app(9, AR_APP_NVM_BC); // 1step以上
}

#pragma section
