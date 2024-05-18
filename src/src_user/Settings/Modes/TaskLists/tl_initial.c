/**
* @file   tl_initial.c
* @brief  初期モードのタスクリスト
*/
#pragma section REPRO
#include "tl_initial.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_initial_mode(void)
{
  // CDH必須アプリ
  BCL_tool_register_combine(0, BC_AC_CDH_UPDATE); // 30step以上

  // AOCS必須アプリ
  BCL_tool_register_combine(30, BC_AC_BASIC_SENSOR_UPDATE);  // 10step以上
  BCL_tool_register_app    (40, AR_APP_AOCS_MM_INITIAL);     // 1step以上

  // Initial mode用アプリ
  BCL_tool_register_combine(41, BC_AC_RM3100_UPDATE);  // 6step以上
  BCL_tool_register_combine(47, BC_AC_STIM210_UPDATE); // 3step以上

  BCL_tool_register_combine(50, BC_AC_MTQ_UPDATE);     // 2step以上, MTQ駆動時間カウンタを20 [Hz]周期にするため入れている

  BCL_tool_register_combine(52, BC_AC_SUN_VECTOR_UPDATE);    //6step以上
  BCL_tool_register_combine(58, BC_AC_INERTIAL_REF_UPDATE); //17step以上

  BCL_tool_register_combine(75, BC_AC_STT_UPDATE);     // 13step以上
  BCL_tool_register_combine(88, BC_AC_OEM7600_UPDATE); // 3step以上

  BCL_tool_register_app    (91, AR_DI_RW0003);         // max 6msec, 6step以上
  BCL_tool_register_app    (97, AR_APP_RW0003_FILTER); //1step以上

  // BCL_tool_register_app   (90, AR_APP_MODULE_TEST_BENCH);  // 打ち上げ時は必ずコメントアウトする

  BCL_tool_register_combine(98, BC_AC_MTQ_UPDATE); // 2step以上
}

#pragma section
