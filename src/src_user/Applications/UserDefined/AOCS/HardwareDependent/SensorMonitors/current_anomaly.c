#pragma section REPRO
/**
* @file   current_anomaly.c
* @brief  電流異常検知アプリ
*/

#include "current_anomaly.h"

#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../../Power/power_switch_control.h"

static CurrentAnomaly        current_anomaly_;
const  CurrentAnomaly* const current_anomaly = &current_anomaly_;

static void APP_CURRENT_ANOMALY_init_(void);
static void APP_CURRENT_ANOMALY_exec_(void);

/**
 * @brief  電源スイッチ状態を確認する
 * @param[in]      idx : INA260_IDX
 * @return         電源スイッチ状態
 */
APP_PSC_STATE APP_CURRENT_ANOMALY_get_switch_state_(INA260_IDX idx);

AppInfo APP_CURRENT_ANOMALY_create_app(void)
{
  return AI_create_app_info("CurrentAnomaly", APP_CURRENT_ANOMALY_init_, APP_CURRENT_ANOMALY_exec_);
}

static void APP_CURRENT_ANOMALY_init_(void)
{
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_PIC]          = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_STIM210]      = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_SAGITTA]      = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_OEM7600]      = 1.0;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_RM3100]       = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_NANOSSOC_D60] = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_MTQ]          = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_RW0003_X]     = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_RW0003_Y]     = 0.5;
  current_anomaly_.hw_oc_event_logger_threshold_V[INA260_IDX_RW0003_Z]     = 0.5;

  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_PIC]          = 200;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_STIM210]      = 1000;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_SAGITTA]      = 500;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_OEM7600]      = 1000;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_RM3100]       = 200;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_NANOSSOC_D60] = 150;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_MTQ]          = 2000;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_RW0003_X]     = 2000;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_RW0003_Y]     = 2000;
  current_anomaly_.sw_oc_threshold_mA[INA260_IDX_RW0003_Z]     = 2000;

  return;
}

static void APP_CURRENT_ANOMALY_exec_(void)
{
  INA260_IDX idx = *DI_INA260_idx_counter;

  APP_PSC_STATE switch_state = APP_CURRENT_ANOMALY_get_switch_state_(idx);

  if (switch_state == APP_PSC_STATE_ON)
  {
    // H/W 過電流検知
    // PIC電流は正しく計測できていない
    if (ina260_driver[idx]->info.voltage_V < current_anomaly_.hw_oc_event_logger_threshold_V[idx])
    {
      if (idx != INA260_IDX_PIC) // TODO_L PIC電流を計測できるようになったらPICもEL対象に入れる
      {
        EL_record_event(EL_GROUP_HW_OC, (uint32_t)idx, EL_ERROR_LEVEL_LOW, 0);
      }
    }

    // S/W 過電流検知
    if (ina260_driver[idx]->info.current_mA > current_anomaly_.sw_oc_threshold_mA[idx])
    {
      EL_record_event(EL_GROUP_SW_OC, (uint32_t)(idx), EL_ERROR_LEVEL_LOW, 0);
    }
  }

  return;
}

APP_PSC_STATE APP_CURRENT_ANOMALY_get_switch_state_(INA260_IDX idx)
{
  switch (idx)
  {
  case INA260_IDX_PIC:
    return power_switch_control->switch_state_5v[APP_PSC_5V_IDX_INA260];
  case INA260_IDX_STIM210:
    return power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_STIM210];
  case INA260_IDX_SAGITTA:
    return power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_SAGITTA];
  case INA260_IDX_OEM7600:
    return power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_OEM7600];
  case INA260_IDX_RM3100:
    return power_switch_control->switch_state_5v[APP_PSC_5V_IDX_RM3100];
  case INA260_IDX_NANOSSOC_D60:
    return power_switch_control->switch_state_5v[APP_PSC_5V_IDX_NANOSSOC_D60];
  case INA260_IDX_MTQ:
    return power_switch_control->switch_state_5v[APP_PSC_5V_IDX_MTQ_SEIREN];
  case INA260_IDX_RW0003_X:
    return power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWX];
  case INA260_IDX_RW0003_Y:
    return power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWY];
  case INA260_IDX_RW0003_Z:
    return power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWZ];
  default:
    // NOT REACHED
    return APP_PSC_STATE_OFF;
  }
}

CCP_EXEC_STS Cmd_APP_CURRENT_ANOMALY_SET_HW_OC_EL_THRESHOLD(const CommonCmdPacket* packet)
{
  INA260_IDX idx = (INA260_IDX)CCP_get_param_from_packet(packet, 0, uint8_t);
  if (idx >= INA260_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint16_t hw_oc_event_logger_threshold_V = CCP_get_param_from_packet(packet, 1, uint16_t);
  if (hw_oc_event_logger_threshold_V < 0.0f) return CCP_EXEC_ILLEGAL_PARAMETER;

  current_anomaly_.hw_oc_event_logger_threshold_V[idx] = hw_oc_event_logger_threshold_V;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_CURRENT_ANOMALY_SET_SW_OVER_CURRENT_PROTECTION(const CommonCmdPacket* packet)
{
  INA260_IDX idx = (INA260_IDX)CCP_get_param_from_packet(packet, 0, uint8_t);
  if (idx >= INA260_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint16_t sw_oc_threshold_mA = CCP_get_param_from_packet(packet, 1, uint16_t);
  if (sw_oc_threshold_mA < 0.0f) return CCP_EXEC_ILLEGAL_PARAMETER;

  current_anomaly_.sw_oc_threshold_mA[idx] = sw_oc_threshold_mA;

  return CCP_EXEC_SUCCESS;
}

#pragma section
