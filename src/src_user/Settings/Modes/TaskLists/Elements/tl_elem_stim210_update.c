/**
* @file   tl_elem_stim210_update.c
* @brief  STIM210出力更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_stim210_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_stim210_update(void)
{
  BCL_tool_register_app(0, AR_DI_STIM210);
  BCL_tool_register_app(1, AR_APP_STIM210_FILTER);
  BCL_tool_register_app(2, AR_APP_GYRO_SELECTOR);
}

#pragma section
