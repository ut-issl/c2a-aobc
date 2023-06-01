/**
* @file  rw0003_filter.h
* @brief RW0003回転数フィルタ
*/
#ifndef RW0003_FILTER_H_
#define RW0003_FILTER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../DriverInstances/di_rw0003.h"
#include "../../../../../Library/physical_constants.h"
#include "../../../../../Library/SignalProcess/spike_filter.h"

/**
 * @struct Rw0003Filter
 * @brief  RW0003回転数フィルタに関する外部公開情報用構造体
 */
typedef struct
{
  SpikeFilter_Config spike_filter_config[RW0003_IDX_MAX];  //!< スパイク除去フィルタ設定パラメータ
  C2A_MATH_ERROR filter_error[RW0003_IDX_MAX];             //!< スパイク除去フィルタ入力判定ログ
} Rw0003Filter;

extern const Rw0003Filter* const rw0003_filter;

AppInfo APP_RW0003_FILTER_create_app(void);

CCP_CmdRet Cmd_APP_RW0003_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_RW0003_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet);
#endif
