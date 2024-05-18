/**
* @file   tl_fine_three_axis.h
* @brief 精三軸制御時のタスクリスト
*/
#pragma section REPRO
#include "tl_fine_three_axis.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_fine_three_axis_mode(void)
{
  // CDH必須アプリ
  BCL_tool_register_combine(0, BC_AC_CDH_UPDATE); // 30step以上(initial mode)→29step以上(fine three axis)

  // AOCS必須アプリ
  BCL_tool_register_combine(29, BC_AC_BASIC_SENSOR_UPDATE);  // 10step以上
  BCL_tool_register_app    (39, AR_APP_AOCS_MM_FINE_THREE_AXIS); // 1step以上

  // 精三軸制御アプリ
  BCL_tool_register_combine(40, BC_AC_RM3100_UPDATE); // 6step以上

  BCL_tool_register_combine(46, BC_AC_STIM210_UPDATE); // 3step以上→1step以上

  BCL_tool_register_combine(47, BC_AC_MTQ_UPDATE);  // 2step以上, MTQ駆動時間カウンタを20 [Hz]周期にするため入れている→1step以上

  BCL_tool_register_combine(48, BC_AC_SUN_VECTOR_UPDATE); // 6step以上
  BCL_tool_register_combine(54, BC_AC_INERTIAL_REF_UPDATE); // 17step以上(initial mode)→16step以上(fine three axis)

  BCL_tool_register_combine(70, BC_AC_STT_UPDATE); // 13step以上
  BCL_tool_register_app    (83, AR_APP_FINE_THREE_AXIS_DETERMINATION); // 2step以上

  BCL_tool_register_combine(85, BC_AC_OEM7600_UPDATE); // 3step以上→1step以上

  BCL_tool_register_app    (86, AR_APP_THREE_AXIS_CONTROL_RW); // 1step以上
  BCL_tool_register_app    (87, AR_APP_UNLOADING); // 1step以上

  BCL_tool_register_combine(88, BC_AC_RW_UPDATE); // 10step以上
  BCL_tool_register_combine(98, BC_AC_MTQ_UPDATE); // 2step以上→1step以上
}

#pragma section
