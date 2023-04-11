/**
* @file   nanossoc_d60_filter.h
* @brief  Nano-SSOC D60太陽センサ出力フィルタ
*/
#ifndef NANOSSOC_D60_FILTER_H_
#define NANOSSOC_D60_FILTER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../../Library/stdint.h"
#include "../../../../../Library/physical_constants.h"
#include "../../../../../Library/SignalProcess/z_filter.h"
#include "../../../../../Library/SignalProcess/spike_filter.h"
#include "../../../../DriverInstances/di_nanossoc_d60.h"

/**
 * @struct NanoSsocD60Filter
 * @brief  Nano-SSOC D60フィルタに関する外部公開情報用構造体
 * @note   スパイク判定は統合後のセンサ出力に対して実施するため，構造体内の各変数は1個ずつで良い
 */
typedef struct
{
  SpikeFilter_Config spike_filter_config;           //!< スパイク除去フィルタ設定パラメータ

  float sun_vec_est_body[PHYSICAL_CONST_THREE_DIM]; //!< フィルタリング後の機体固定座標推定太陽方向ベクトル

  C2A_MATH_ERROR filter_error;                      //!< スパイク除去フィルタ入力判定ログ
} NanoSsocD60Filter;

extern const NanoSsocD60Filter* const nanossoc_d60_filter;

AppInfo APP_NANOSSOC_D60_FILTER_create_app(void);

CCP_CmdRet Cmd_APP_NANOSSOC_D60_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet);

#endif
