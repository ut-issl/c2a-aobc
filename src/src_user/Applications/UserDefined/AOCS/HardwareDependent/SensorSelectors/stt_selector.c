#pragma section REPRO
/**
* @file   stt_selector.c
* @brief  複数あるSTT出力を組み合わせてQuaternionを求めるアプリ
* @note   現時点では一つしか搭載されていないので、その一つを使う
*/

#include "stt_selector.h"

#include <src_core/Library/print.h>
#include "../../../../DriverInstances/di_sagitta.h"
#include "../../aocs_manager.h"

static SttSelector        stt_selector_;
const  SttSelector* const stt_selector = &stt_selector_;

static void APP_STT_SELECTOR_init_(void);
static void APP_STT_SELECTOR_exec_(void);

AppInfo APP_STT_SELECTOR_create_app(void)
{
  return AI_create_app_info("STT Selector", APP_STT_SELECTOR_init_, APP_STT_SELECTOR_exec_);
}

static void APP_STT_SELECTOR_init_(void)
{
  stt_selector_.state = APP_STT_SELECTOR_STATE_SAGITTA_IN_UNIT;
  return;
}

static void APP_STT_SELECTOR_exec_(void)
{
  switch (stt_selector_.state)
  {
  case APP_STT_SELECTOR_STATE_SAGITTA_IN_UNIT:
    if (sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.is_valid_quaternion == 1)
    {
      AOCS_MANAGER_set_quaternion_obs_i2b(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.quaternion_i2b);
      AOCS_MANAGER_set_stt_availability(AOCS_MANAGER_STT_AVAILABLE);
    }
    else
    {
      AOCS_MANAGER_set_stt_availability(AOCS_MANAGER_STT_UNAVAILABLE);
    }
    break;
  default:
    // Not Reached
    break;
  }
  return;
}

#pragma section
