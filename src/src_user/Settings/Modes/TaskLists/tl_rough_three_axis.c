/**
* @file   tl_rough_three_axis.c
* @brief  粗三軸制御時のタスクリスト
*/
#pragma section REPRO
#include "tl_rough_three_axis.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_rough_three_axis_mode(void)
{
  // CDH必須アプリ
  BCL_tool_register_combine(0, BC_AC_CDH_UPDATE); // 30step以上

  // AOCS必須アプリ
  BCL_tool_register_combine(35, BC_AC_BASIC_SENSOR_UPDATE);  // 10step以上
  BCL_tool_register_app    (46, AR_APP_AOCS_MM_ROUGH_THREE_AXIS); // 1step以上

  // 基本使わないがユーザーが使いたい時に使えるように入れておく
  BCL_tool_register_combine(48, BC_AC_STIM210_UPDATE); // 3step以上

  // 粗三軸制御アプリ
  BCL_tool_register_combine(52, BC_AC_MTQ_UPDATE);  // 2step以上, MTQ駆動時間カウンタを20 [Hz]周期にするため入れている

  BCL_tool_register_combine(55, BC_AC_RM3100_UPDATE); // 6step以上

  BCL_tool_register_combine(62, BC_AC_SUN_VECTOR_UPDATE); // 6step以上
  BCL_tool_register_combine(69, BC_AC_INERTIAL_REF_UPDATE); // 17step以上
  BCL_tool_register_app    (87, AR_APP_ROUGH_THREE_AXIS_DETERMINATION); // 1step以上

  BCL_tool_register_app    (89, AR_APP_THREE_AXIS_CONTROL_MTQ); // 1step以上

  BCL_tool_register_combine(91, BC_AC_MTQ_UPDATE); // 2tep以上

  // 基本使わないがユーザーが使いたい時に使えるように入れておく
  BCL_tool_register_combine(94, BC_AC_OEM7600_UPDATE); // 3step以上
}

#pragma section
