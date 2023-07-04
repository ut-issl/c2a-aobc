/**
* @file   tl_elem_oem7600_update.c
* @brief  OEM7600出力更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_oem7600_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_oem7600_update(void)
{
  BCL_tool_register_app(0, AR_DI_OEM7600);
  BCL_tool_register_app(1, AR_APP_GPSR_SELECTOR);
  BCL_tool_register_app(2, AR_APP_OEM7600_FILTER);
}

#pragma section
