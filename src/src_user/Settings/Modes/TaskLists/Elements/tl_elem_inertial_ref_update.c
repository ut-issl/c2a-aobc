/**
* @file   tl_elem_inertial_reference_update.c
* @brief  慣性系リファレンス更新用TL Element
*/
#pragma section REPRO
#include "tl_elem_inertial_ref_update.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_inertial_ref_update()
{
  BCL_tool_register_app(0, AR_APP_TIME_SPACE_CALCULATOR); // 3step以上
  BCL_tool_register_app(1, AR_APP_ORBIT_CALCULATOR);   //4step以上
  BCL_tool_register_app(2, AR_APP_SUN_DIR_ECI_CALCULATOR);  //2step以上
  BCL_tool_register_app(3, AR_APP_GEOMAG_ECI_CALCULATOR);  //7step以上
  BCL_tool_register_app(4, AR_APP_TARGET_ATTITUDE_CALCULATOR);  //1step以上

  // total: 11step
}

#pragma section
