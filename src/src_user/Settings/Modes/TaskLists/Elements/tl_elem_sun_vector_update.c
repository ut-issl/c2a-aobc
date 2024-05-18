/**
* @file   tl_elem_sun_vector_update.c
* @brief  太陽方向更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_sun_vector_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_sun_vector_update(void)
{
  BCL_tool_register_app(0, AR_DI_NANOSSOC_D60); // 3step以上
  BCL_tool_register_app(1, AR_APP_SUN_SENSOR_SELECTOR); // 1step以上
  BCL_tool_register_app(2, AR_APP_NANOSSOC_D60_FILTER); // 1step以上
  BCL_tool_register_app(3, AR_APP_SUN_VEC_PROPAGATOR); // 1step以上
}

#pragma section
