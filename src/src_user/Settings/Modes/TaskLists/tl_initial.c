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
  BCL_tool_register_combine(0, BC_AC_CDH_UPDATE); // 23step以上

  // AOCS必須アプリ
  BCL_tool_register_combine(25, BC_AC_BASIC_SENSOR_UPDATE);  // 7step以上
  BCL_tool_register_app    (35, AR_APP_AOCS_MM_INITIAL);     // 1step以上

  // Initial mode用アプリ
  BCL_tool_register_combine(36, BC_AC_RM3100_UPDATE);  // 4step以上
  BCL_tool_register_combine(41, BC_AC_STIM210_UPDATE); // 1step以上

  BCL_tool_register_combine(45, BC_AC_MTQ_UPDATE);     // 1step以上, MTQ駆動時間カウンタを20 [Hz]周期にするため入れている

  BCL_tool_register_combine(47, BC_AC_SUN_VECTOR_UPDATE);   // 合計4step以上必要ある？
  BCL_tool_register_combine(51, BC_AC_INERTIAL_REF_UPDATE); // 11step以上

  BCL_tool_register_combine(63, BC_AC_STT_UPDATE);     // 4step以上
  BCL_tool_register_combine(78, BC_AC_OEM7600_UPDATE); // 1step以上

  BCL_tool_register_app    (80, AR_DI_RW0003);         // max 6msec
  BCL_tool_register_app    (87, AR_APP_RW0003_FILTER); // 0msec

  // BCL_tool_register_app   (90, AR_APP_MODULE_TEST_BENCH);  // 打ち上げ時は必ずコメントアウトする

  BCL_tool_register_combine(95, BC_AC_MTQ_UPDATE); // 1step以上
}

#pragma section
