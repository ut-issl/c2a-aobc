/**
* @file   sagitta_filter.h
* @brief  SAGITTA (STT) 出力フィルタ
*/
#ifndef SAGITTA_FILTER_H_
#define SAGITTA_FILTER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include "../../../../../Library/SignalProcess/spike_filter.h"
#include "../../../../../Library/quaternion.h"

/**
 * @struct SagittaFilter
 * @brief SAGITTA出力フィルタに関する外部公開情報用構造体
 */
typedef struct
{
  SpikeFilter_Config q_i2b_spike_filter_config;   //!< スパイク除去フィルタ設定パラメータ

  Quaternion q_i2b_filtered;                      //!< スパイク除去フィルタ通過後のq_i2b

  C2A_MATH_ERROR q_i2b_filter_error;              //!< スパイク除去フィルタ入力判定ログ
} SagittaFilter;

extern const SagittaFilter* const sagitta_filter;

AppInfo APP_SAGITTA_FILTER_create_app(void);

CCP_EXEC_STS Cmd_APP_SAGITTA_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_APP_SAGITTA_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet);

#endif
