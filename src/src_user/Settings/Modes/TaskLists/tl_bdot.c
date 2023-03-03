/**
* @file   tl_bdot.c
* @brief  Bdot制御時のタスクリスト
*/
#pragma section REPRO
#include "tl_bdot.h"

#include "../../../Applications/app_registry.h"
#include "../../../TlmCmd/block_command_definitions.h"

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_bdot_mode(void)
{
  // CDH必須アプリ
  BCL_tool_register_combine(0, BC_AC_CDH_UPDATE); // 23step以上

  // AOCS必須アプリ
  BCL_tool_register_combine(25, BC_AC_BASIC_SENSOR_UPDATE); // 7step以上
  BCL_tool_register_app    (35, AR_APP_AOCS_MM_BDOT); // 1step以上

  // Bdotアプリ
  BCL_tool_register_combine(40, BC_AC_MTQ_UPDATE);  // 1step以上, MTQ駆動時間カウンタを20 [Hz]周期にするため入れている

  BCL_tool_register_combine(45, BC_AC_RM3100_UPDATE); // 4step以上

  BCL_tool_register_app    (60, AR_APP_GYRO_SELECTOR); // 1step以上


  BCL_tool_register_app    (80, AR_APP_BDOT); // 1step以上

  BCL_tool_register_combine(90, BC_AC_MTQ_UPDATE); // 1step以上
}

#pragma section
