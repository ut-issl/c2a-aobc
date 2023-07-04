/**
* @file   tl_rough_sun_pointing.c
* @brief  粗太陽補足制御時のタスクリスト
*/
#pragma section REPRO
#include "tl_rough_sun_pointing.h"

#include <src_user/Applications/app_registry.h>
#include <src_user/TlmCmd/block_command_definitions.h>

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_rough_sun_pointing_mode(void)
{
  // CDH必須アプリ
  BCL_tool_register_combine(0, BC_AC_CDH_UPDATE); // 23step以上

  // AOCS必須アプリ
  BCL_tool_register_combine(25, BC_AC_BASIC_SENSOR_UPDATE);  // 7step以上
  BCL_tool_register_app    (35, AR_APP_AOCS_MM_SUN_POINTING); // 1step以上

  // 粗太陽補足アプリ
  BCL_tool_register_combine(40, BC_AC_MTQ_UPDATE);  // 1step以上, MTQ駆動時間カウンタを20 [Hz]周期にするため入れている

  BCL_tool_register_combine(45, BC_AC_RM3100_UPDATE); // 4step以上

  BCL_tool_register_app    (60, AR_APP_GYRO_SELECTOR); // 1step以上
  BCL_tool_register_combine(65, BC_AC_SUN_VECTOR_UPDATE); // 合計4step以上

  BCL_tool_register_combine(70, BC_AC_INERTIAL_REF_UPDATE); // 合計11step以上

  // 太陽補足しながら次の三軸姿勢を確認できるようにしておく
  BCL_tool_register_app    (83, AR_APP_ROUGH_THREE_AXIS_DETERMINATION); // 1step以上

  BCL_tool_register_app    (85, AR_APP_SUN_POINTING); // 1step以上

  BCL_tool_register_combine(90, BC_AC_MTQ_UPDATE); // 1step以上
}

#pragma section
