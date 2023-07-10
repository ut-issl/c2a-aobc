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

  // AOCSアプリ
  BCL_tool_register_app    (25, AR_APP_AOCS_MANAGER);
  BCL_tool_register_app    (27, AR_APP_THERMO_SENSOR);
  BCL_tool_register_combine(30, BC_AC_OEM7600_UPDATE);
  BCL_tool_register_app    (35, AR_APP_AOCS_MM_INITIAL);
  BCL_tool_register_rotate (50, BC_AR_DRIVERS_UPDATE_INI);
  BCL_tool_register_app    (65, AR_DI_SAGITTA);
  BCL_tool_register_app    (70, AR_DI_RW0003); // max 6msec
  BCL_tool_register_app    (77, AR_DI_INA260);
  BCL_tool_register_app    (80, AR_APP_CURRENT_ANOMALY);
  BCL_tool_register_combine(85, BC_AC_INERTIAL_REF_UPDATE);
  // BCL_tool_register_app   (90, AR_APP_MODULE_TEST_BENCH);  // 打ち上げ時は必ずコメントアウトする
}

#pragma section
