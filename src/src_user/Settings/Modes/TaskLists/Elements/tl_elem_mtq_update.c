/**
* @file   tl_elem_mtq_update.c
* @brief  MTQ出力更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_mtq_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_mtq_update(void)
{
  BCL_tool_register_app(0, AR_APP_MAGNETIC_EXCLUSIVE_CONTROL_TIMER);
  BCL_tool_register_app(1, AR_APP_AOCS_MTQ_SEIREN_CONTROLLER);
}

#pragma section
