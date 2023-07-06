/**
* @file   tl_elem_driver_update.c
* @brief  ドライバ更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_drivers_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_drivers_update_initial()
{
  BCL_tool_register_app(0, AR_DI_NANOSSOC_D60);
  BCL_tool_register_app(1, AR_NOP);
  BCL_tool_register_app(2, AR_DI_STIM210);
  BCL_tool_register_app(3, AR_DI_RM3100);
  BCL_tool_register_app(4, AR_NOP);
  BCL_tool_register_app(5, AR_NOP);
  BCL_tool_register_app(6, AR_DI_MPU9250);
  BCL_tool_register_app(7, AR_DI_MTQ_SEIREN);
  BCL_tool_register_app(8, AR_NOP);
  BCL_tool_register_app(9, AR_NOP);
}



#pragma section
