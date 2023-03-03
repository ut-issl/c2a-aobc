/**
* @file   stim210_filter.h
* @brief  STIM210ジャイロセンサ出力フィルタ
*/
#ifndef STIM210_FILTER_H_
#define STIM210_FILTER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../../Library/stdint.h"
#include "../../../../../Library/physical_constants.h"
#include "../../../../../Library/SignalProcess/z_filter.h"
#include "../../../../../Library/SignalProcess/spike_filter.h"

/**
 * @struct Stim210Filter
 * @brief STIM210フィルタに関する外部公開情報用構造体
 */
typedef struct
{
  // ZFilterの設定パラメータ
  float sampling_freq_Hz;     //!< zfilter (1次LPFとしての使用を想定) 用サンプリング周波数 [Hz] (アプリ実行周期に合わせる)
  float cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM];   //!< 1次LPF用カットオフ周波数 [Hz]

  SpikeFilter_Config spike_filter_config[PHYSICAL_CONST_THREE_DIM];  //!< スパイク除去フィルタ設定パラメータ

  float ang_vel_est_body_rad_s[PHYSICAL_CONST_THREE_DIM];           //!< フィルタリング後の機体固定座標推定角速度 [rad/s]

  C2A_MATH_ERROR filter_error[PHYSICAL_CONST_THREE_DIM];        //!< スパイク除去フィルタ入力判定ログ
} Stim210Filter;

extern const Stim210Filter* const stim210_filter;

AppInfo APP_STIM210_FILTER_create_app(void);

CCP_EXEC_STS Cmd_APP_STIM210_FILTER_SET_ZFILTER_PARAM(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_APP_STIM210_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_APP_STIM210_FILTER_RESET_ZFILTER(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_APP_STIM210_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet);

#endif
