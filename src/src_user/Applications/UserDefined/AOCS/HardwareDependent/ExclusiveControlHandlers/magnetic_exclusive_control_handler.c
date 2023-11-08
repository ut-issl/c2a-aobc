#pragma section REPRO
/**
* @file   magnetic_exclusive_control_handler.c
* @brief  MTQ-磁気センサ排他制御ハンドラー
*/

#include "magnetic_exclusive_control_handler.h"

static MagneticExclusiveControlHandler        magnetic_exclusive_control_handler_;
const  MagneticExclusiveControlHandler* const magnetic_exclusive_control_handler = &magnetic_exclusive_control_handler_;

static void APP_MECH_init_(void);
static void APP_MECH_exec_(void);

static void APP_MECH_update_timer_(void);
static void APP_MECH_update_state_(void);

AppInfo APP_MECH_create_app(void)
{
  return AI_create_app_info("MagneticExclusiveControlHandler", APP_MECH_init_, APP_MECH_exec_);
}

void APP_MECH_init_(void)
{
  magnetic_exclusive_control_handler_.previous_obc_time = TMGR_get_master_clock();
  magnetic_exclusive_control_handler_.current_state     = APP_MECH_STATE_STANDBY;
  magnetic_exclusive_control_handler_.state_timer_ms    = 0;
  magnetic_exclusive_control_handler_.observe_duration_ms = 100;
  magnetic_exclusive_control_handler_.control_duration_ms = 800;
  magnetic_exclusive_control_handler_.standby_duration_ms = 100;
}

void APP_MECH_exec_(void)
{
  APP_MECH_update_timer_();
  APP_MECH_update_state_();
}


void APP_MECH_update_timer_(void)
{
  ObcTime current_obc_time = TMGR_get_master_clock();
  uint32_t step_time_ms = OBCT_diff_in_msec(&(magnetic_exclusive_control_handler_.previous_obc_time), &current_obc_time);
  magnetic_exclusive_control_handler_.previous_obc_time = current_obc_time;

  magnetic_exclusive_control_handler_.state_timer_ms += step_time_ms;
}

void APP_MECH_update_state_(void)
{
  switch (magnetic_exclusive_control_handler_.current_state)
  {
  case APP_MECH_STATE_OBSERVE:
    if (magnetic_exclusive_control_handler_.state_timer_ms >= magnetic_exclusive_control_handler_.observe_duration_ms)
    {
      magnetic_exclusive_control_handler_.state_timer_ms = 0;
      magnetic_exclusive_control_handler_.current_state = APP_MECH_STATE_CONTROL;
    }
    break;
  case APP_MECH_STATE_CONTROL:
    if (magnetic_exclusive_control_handler_.state_timer_ms >= magnetic_exclusive_control_handler_.control_duration_ms)
    {
      magnetic_exclusive_control_handler_.state_timer_ms = 0;
      magnetic_exclusive_control_handler_.current_state = APP_MECH_STATE_STANDBY;
    }
    break;
  case APP_MECH_STATE_STANDBY:
    if (magnetic_exclusive_control_handler_.state_timer_ms >= magnetic_exclusive_control_handler_.standby_duration_ms)
    {
      magnetic_exclusive_control_handler_.state_timer_ms = 0;
      magnetic_exclusive_control_handler_.current_state = APP_MECH_STATE_OBSERVE;
    }
    break;
  default:
    // NOT REACHED
    break;
  }
}
