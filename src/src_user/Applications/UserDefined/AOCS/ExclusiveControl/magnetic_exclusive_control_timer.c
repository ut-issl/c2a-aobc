#pragma section REPRO
/**
* @file   magnetic_exclusive_control_timer.c
* @brief  MTQ-磁気センサ排他制御タイマー
*/

#include "magnetic_exclusive_control_timer.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

static MagneticExclusiveControlTimer        magnetic_exclusive_control_timer_;
const  MagneticExclusiveControlTimer* const magnetic_exclusive_control_timer = &magnetic_exclusive_control_timer_;

static void APP_MECT_init_(void);
static void APP_MECT_exec_(void);

static void APP_MECT_update_timer_(void);
static void APP_MECT_update_state_(void);

/*
 * @brief  MTQ-磁気センサ排他制御タイマーの時間設定をロードし、今のコンフィグに反映する
 * @note   MTQ駆動中に外部からコンフィグをロードすると制御が崩れるので、
 *         コマンドによる時間設定の変更は一度バッファリングし、MTQ駆動が終わったタイミングで反映する
 */
static void APP_MECT_load_buffered_config_(void);

AppInfo APP_MECT_create_app(void)
{
  return AI_create_app_info("MagneticExclusiveControlTimer", APP_MECT_init_, APP_MECT_exec_);
}

void APP_MECT_init_(void)
{
  magnetic_exclusive_control_timer_.previous_obc_time = TMGR_get_master_clock();
  magnetic_exclusive_control_timer_.state_timer_ms    = 0;
  magnetic_exclusive_control_timer_.config.observe_duration_ms = 100;
  magnetic_exclusive_control_timer_.config.control_duration_ms = 800;
  magnetic_exclusive_control_timer_.config.standby_duration_ms = 100;
  magnetic_exclusive_control_timer_.buffered_config.observe_duration_ms = 100;
  magnetic_exclusive_control_timer_.buffered_config.control_duration_ms = 800;
  magnetic_exclusive_control_timer_.buffered_config.standby_duration_ms = 100;
}

void APP_MECT_exec_(void)
{
  APP_MECT_update_timer_();
  APP_MECT_update_state_();
}


void APP_MECT_update_timer_(void)
{
  ObcTime current_obc_time = TMGR_get_master_clock();
  uint32_t step_time_ms = OBCT_diff_in_msec(&(magnetic_exclusive_control_timer_.previous_obc_time), &current_obc_time);
  magnetic_exclusive_control_timer_.previous_obc_time = current_obc_time;

  magnetic_exclusive_control_timer_.state_timer_ms += step_time_ms;
}

void APP_MECT_update_state_(void)
{
  switch (aocs_manager->magnetic_exclusive_control_timer_state)
  {
  case APP_MECT_STATE_OBSERVE:
    if (magnetic_exclusive_control_timer_.state_timer_ms >= magnetic_exclusive_control_timer_.config.observe_duration_ms)
    {
      magnetic_exclusive_control_timer_.state_timer_ms = 0;
      AOCS_MANAGER_set_magnetic_exclusive_control_timer_state(APP_MECT_STATE_CONTROL);
    }
    break;
  case APP_MECT_STATE_CONTROL:
    if (magnetic_exclusive_control_timer_.state_timer_ms >= magnetic_exclusive_control_timer_.config.control_duration_ms)
    {
      magnetic_exclusive_control_timer_.state_timer_ms = 0;
      AOCS_MANAGER_set_magnetic_exclusive_control_timer_state(APP_MECT_STATE_STANDBY);
    }
    break;
  case APP_MECT_STATE_STANDBY:
    APP_MECT_load_buffered_config_();
    if (magnetic_exclusive_control_timer_.state_timer_ms >= magnetic_exclusive_control_timer_.config.standby_duration_ms)
    {
      magnetic_exclusive_control_timer_.state_timer_ms = 0;
      AOCS_MANAGER_set_magnetic_exclusive_control_timer_state(APP_MECT_STATE_OBSERVE);
    }
    break;
  default:
    // NOT REACHED
    break;
  }
}

static void APP_MECT_load_buffered_config_(void)
{
  magnetic_exclusive_control_timer_.config = magnetic_exclusive_control_timer_.buffered_config;
  return;
}

CCP_CmdRet Cmd_APP_MAGNETIC_EXCLUSIVE_CONTROL_TIMER_SET_DURATION(const CommonCmdPacket* packet)
{
  const uint16_t kAcceptableMaxDuration_ms = 10000;
  const uint16_t kAcceptableMinDuration_ms = 10;

  uint16_t observe_duration_ms = CCP_get_param_from_packet(packet, 0, uint16_t);
  if (observe_duration_ms > kAcceptableMaxDuration_ms || observe_duration_ms < kAcceptableMinDuration_ms)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  uint16_t control_duration_ms = CCP_get_param_from_packet(packet, 1, uint16_t);
  if (control_duration_ms > kAcceptableMaxDuration_ms || control_duration_ms < kAcceptableMinDuration_ms)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  uint16_t standby_duration_ms = CCP_get_param_from_packet(packet, 2, uint16_t);
  if (standby_duration_ms > kAcceptableMaxDuration_ms || standby_duration_ms < kAcceptableMinDuration_ms)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  magnetic_exclusive_control_timer_.buffered_config.observe_duration_ms = observe_duration_ms;
  magnetic_exclusive_control_timer_.buffered_config.control_duration_ms = control_duration_ms;
  magnetic_exclusive_control_timer_.buffered_config.standby_duration_ms = standby_duration_ms;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
