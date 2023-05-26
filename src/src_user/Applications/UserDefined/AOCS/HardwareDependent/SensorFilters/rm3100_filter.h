/**
* @file   rm3100_filter.h
* @brief  RM3100磁気センサ出力フィルタ
*/
#ifndef RM3100_FILTER_H_
#define RM3100_FILTER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../../Library/physical_constants.h"
#include "../../../../../Library/SignalProcess/z_filter.h"
#include "../../../../../Library/SignalProcess/spike_filter.h"
#include "../../../../DriverInstances/di_rm3100.h"

/**
 * @struct Rm3100Filter
 * @brief RM3100フィルタに関する外部公開情報用構造体
 */
typedef struct
{
  // ZFilterの設定パラメータ
  float sampling_freq_Hz;     //!< zfilter (1次LPFとしての使用を想定) 用サンプリング周波数 [Hz] (アプリ実行周期に合わせる)
  float cut_off_freq_lpf_1st_Hz[RM3100_IDX_MAX][PHYSICAL_CONST_THREE_DIM];          //!< 1次LPF用カットオフ周波数 [Hz]

  SpikeFilter_Config spike_filter_config[RM3100_IDX_MAX][PHYSICAL_CONST_THREE_DIM]; //!< スパイク除去フィルタ設定パラメータ

  float mag_est_body_nT[RM3100_IDX_MAX][PHYSICAL_CONST_THREE_DIM];                  //!< フィルタリング後の機体固定座標推定磁場ベクトル [nT]

  C2A_MATH_ERROR filter_error[RM3100_IDX_MAX][PHYSICAL_CONST_THREE_DIM];            //!< スパイク除去フィルタ入力判定ログ
} Rm3100Filter;

extern const Rm3100Filter* const rm3100_filter;

AppInfo APP_RM3100_FILTER_create_app(void);

CCP_CmdRet Cmd_APP_RM3100_FILTER_SET_ZFILTER_PARAM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_APP_RM3100_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet);

#endif
