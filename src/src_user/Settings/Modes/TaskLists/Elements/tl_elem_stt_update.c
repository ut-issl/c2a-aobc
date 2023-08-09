/**
* @file   tl_elem_stt210_update.c
* @brief  STT出力更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_stt_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_stt_update(void)
{
  BCL_tool_register_app(0, AR_DI_SAGITTA);
  BCL_tool_register_app(1, AR_APP_STT_SELECTOR);
  BCL_tool_register_app(2, AR_APP_SAGITTA_FILTER);
}

#pragma section
