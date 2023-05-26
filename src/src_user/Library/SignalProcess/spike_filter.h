/**
* @file   spike_filter.h
* @brief  スパイク除去フィルタ関連の関数群の定義
*/

#ifndef SPIKE_FILTER_H_
#define SPIKE_FILTER_H_

#include <stdint.h>
#include "../c2a_math.h"
#include "../physical_constants.h"

/**
 * @struct SpikeFilter_Config
 * @brief  スパイク除去フィルタ設定値構造体
 */
typedef struct
{
  uint8_t count_limit_to_accept;                    //!< スパイク値を真値として受け入れるまでのカウント数
  uint8_t count_limit_to_reject_continued_warning;  //!< 連続して入力値を除外した回数のカウント上限値 (上限値に達した場合に警告)

  double  reject_threshold;   //!< スパイク値判定に用いる閾値

  /*!< スパイクor発振ではなく, ステップ変化として値を受けれる判定のための許容値
     (スパイクの疑いのある値が連続して入力されても，それら各値同士の差分がこの値以下でなければカウント数に依らず除外し続ける) */
  double  amplitude_limit_to_accept_as_step;
} SpikeFilter_Config;


/**
 * @struct SpikeFilter
 * @brief  スパイク除去フィルタ構造体
 */
typedef struct
{
  uint8_t count_to_accept_;                      //!< 現在，スパイク値と見做した連続の回数 (真値として値を受け入れた際にゼロクリア)
  double  last_accept_val_;                      //!< 最後に真値として受けれた値
  double  last_reject_val_;                      //!< 最後にスパイク値として除外した値
  uint8_t count_to_reject_continued_warning_;    //!< スパイクor発振値として連続して入力値を除外した回数
  SpikeFilter_Config config;                     //!< フィルタの設定値
} SpikeFilter;


/**
 * @brief  SPIKE_FILTERを初期化
 * @param[out]  *filter        : 初期化対象
 * @param[in]      filter_setting : フィルタの設定値
 * @return C2A_MATH_ERROR
 */
C2A_MATH_ERROR SPIKE_FILTER_init(SpikeFilter* filter, const SpikeFilter_Config filter_setting);


/**
 * @brief  SPIKE_FILTERの内部状態をゼロクリアする
 * @param[in,out]  *filter  : リセット対象
 */
void SPIKE_FILTER_reset(SpikeFilter* filter);


/**
 * @brief  SPIKE_FILTERの出力を得る
 * @param[in,out]  *filter  : 使用するフィルタ
 * @param[out]  output      : フィルタ出力値
 * @param[in]   input       : フィルタ入力値
 * @return C2A_MATH_ERROR_RANGE_OVER : スパイク値として入力を除外した場合
 * @return C2A_MATH_ERROR_OTHERS     : 入力値除外回数連続値が指定値以上になった場合
 * @return C2A_MATH_ERROR_OK         : それ以外
 */
C2A_MATH_ERROR SPIKE_FILTER_calc_output(SpikeFilter* filter, float* output, const float input);


/**
 * @brief  SPIKE_FILTERの出力を得る (double ver.)
 * @param[in,out]  *filter  : 使用するフィルタ
 * @param[out]  output      : フィルタ出力値
 * @param[in]   input       : フィルタ入力値
 * @return C2A_MATH_ERROR_RANGE_OVER : スパイク値として入力を除外した場合
 * @return C2A_MATH_ERROR_OTHERS     : 入力値除外回数連続値が指定値以上になった場合
 * @return C2A_MATH_ERROR_OK         : それ以外
 */
C2A_MATH_ERROR SPIKE_FILTER_calc_output_double(SpikeFilter* filter, double* output, const double input);


#endif
