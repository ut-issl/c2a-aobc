/**
* @file   current_anomaly.c
* @brief  電流異常検知アプリ
*/
#ifndef CURRENT_ANOMALY_H_
#define CURRENT_ANOMALY_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../DriverInstances/di_ina260.h"

/**
 * @struct CurrentAnomaly
 * @brief  電流異常に関する外部公開情報用構造体
 */
typedef struct
{
  float hw_oc_event_logger_threshold_V[INA260_IDX_MAX]; //!< voltage threshold for H/W OC event logger
  uint16_t sw_oc_threshold_mA[INA260_IDX_MAX];          //!< S/W OC threshold
} CurrentAnomaly;

extern const CurrentAnomaly* const current_anomaly;

AppInfo APP_CURRENT_ANOMALY_create_app(void);

CCP_CmdRet Cmd_APP_CURRENT_ANOMALY_SET_HW_OC_EL_THRESHOLD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_CURRENT_ANOMALY_SET_SW_OVER_CURRENT_PROTECTION(const CommonCmdPacket* packet);

#endif
