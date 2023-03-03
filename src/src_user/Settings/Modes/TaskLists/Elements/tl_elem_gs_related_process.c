/**
* @file   tl_elem_gs_related_process.c
* @brief  GS関連処理TL Element
*/
#pragma section REPRO
#include "tl_elem_gs_related_process.h"

#include "../../../../TlmCmd/block_command_definitions.h"
#include "../../../../Applications/app_registry.h"

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_gs_related_process()
{
  // BCL_tool_register_combine(0, BC_AC_GS_GSTOS_DI);
  BCL_tool_register_app    (0, AR_DI_MOBC);
  BCL_tool_register_app    (1, AR_GSC_DISPATCHER);
}

#pragma section
