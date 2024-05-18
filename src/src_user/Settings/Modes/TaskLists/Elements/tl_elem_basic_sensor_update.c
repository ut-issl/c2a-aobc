/**
* @file   tl_elem_basic_sensor_update.c
* @brief  基本センサ更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_basic_sensor_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_basic_sensor_update(void)
{
  BCL_tool_register_app(0, AR_DI_MPU9250); // 4step以上
  BCL_tool_register_app(1, AR_APP_MPU9250_FILTER); // 1step以上
  BCL_tool_register_app(2, AR_DI_INA260); // 2step以上
  BCL_tool_register_app(3, AR_APP_CURRENT_ANOMALY); // 1step以上
  BCL_tool_register_app(4, AR_APP_THERMO_SENSOR); // step以上
  BCL_tool_register_app(5, AR_APP_AOCS_MANAGER); // 1step以上
}

#pragma section
