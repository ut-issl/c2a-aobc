/**
* @file   tl_elem_rm3100_update.c
* @brief  RM3100出力更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_rm3100_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_rm3100_update(void)
{
  BCL_tool_register_app(0, AR_DI_RM3100);
  BCL_tool_register_app(1, AR_APP_RM3100_FILTER);
  BCL_tool_register_app(2, AR_APP_MAG_SELECTOR);
}

#pragma section
