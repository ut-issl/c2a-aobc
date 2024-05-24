/**
* @file   tl_bdot.c
* @brief  Bdot制御時のタスクリスト
*/
#pragma section REPRO
#include "tl_bdot.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

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

  BCL_tool_register_app    (45, AR_DI_MPU9250);         // 3step以上
  BCL_tool_register_app    (50, AR_APP_MPU9250_FILTER); // 1step以上

  BCL_tool_register_combine(55, BC_AC_RM3100_UPDATE); // 4step以上

  BCL_tool_register_app    (60, AR_APP_GYRO_SELECTOR); // 1step以上

  BCL_tool_register_app    (80, AR_APP_BDOT); // 1step以上

  BCL_tool_register_app    (85, AR_APP_AOCS_MTQ_SEIREN_CONTROLLER); // 排他制御モードがOBSERVEのうちに目標出力計算するために入れている

  BCL_tool_register_combine(90, BC_AC_MTQ_UPDATE); // 1step以上

  // 基本使わないがユーザーが使いたい時に使えるように入れておく
  BCL_tool_register_combine(92, BC_AC_OEM7600_UPDATE); // 1step以上
}

#pragma section
