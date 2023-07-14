/**
* @file   tl_elem_rw_update.c
* @brief  RW出力更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_rw_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_rw_update(void)
{
  BCL_tool_register_app(0, AR_APP_AOCS_RW_CONTROLLER); // max 3msec
  BCL_tool_register_app(1, AR_DI_RW0003); // max 6msec
  BCL_tool_register_app(3, AR_APP_RW0003_FILTER); // 0msec
}

#pragma section
