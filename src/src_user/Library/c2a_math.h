/**
* @file   c2a_math.h
* @brief  各種数学処理用関数群の定義
* @note   基本，ここで提供するのはスカラー処理とし，ベクトル等の配列への対応はvector3.h/.c等に実装
* @note   基本的には入力はfloat想定．double入力の場合は関数末尾に_doubleとついているものを使用．
*/
#ifndef C2A_MATH_H_
#define C2A_MATH_H_

#include "stdint.h"

/**
 * @enum   C2A_MATH_ERROR
 * @brief  数学関連処理に関するエラーコード
 * @note   uint8_tを想定
 */
typedef enum
{
  C2A_MATH_ERROR_OK              = 0,    //!< 数値エラーなし
  C2A_MATH_ERROR_NAN             = 1,    //!< 数値が非数値
  C2A_MATH_ERROR_SINGULAR        = 2,    //!< callされた関数の処理内で極端に大きい or 小さい値が発生している
  C2A_MATH_ERROR_UNNORMALIZED    = 3,    //!< ベクトル等が正規化されていない
  C2A_MATH_ERROR_RANGE_OVER      = 4,    //!< 値が指定された値域を超えている
  C2A_MATH_ERROR_RANGE_OVER_HIGH = 5,    //!< 値が指定された値域の上限を超えている
  C2A_MATH_ERROR_RANGE_OVER_LOW  = 6,    //!< 値が指定された値域の下限を超えている
  C2A_MATH_ERROR_ENUM_IRREGULAR  = 7,    //!< enum上で定義されていない値の発生 (数値系引数のレンジオーバーと区別)
  C2A_MATH_ERROR_OTHERS                  //!< その他
} C2A_MATH_ERROR;


// 特定値域へのクリップ，NaN判定などの数値操作系ユーティリティ処理

/**
 * @brief  float型変数のNaN, Inf等の非数値判定
 * @param  float_input : NaN, Inf判定の対象とするfloat型変数
 * @return C2A_MATH_ERROR
 */
C2A_MATH_ERROR C2A_MATH_check_nan_inf(const float input);

/**
 * @brief  double型変数のNaN, Inf等の非数値判定
 * @param  double_input : NaN, Inf判定の対象とするdouble型変数
 * @return C2A_MATH_ERROR
 */
C2A_MATH_ERROR C2A_MATH_check_nan_inf_double(const double input);

/**
 * @brief  float型変数のクリップ処理
 * @param  float_input : クリップ処理をかける対象 (入力値)
 * @param  upper_limit : 上限値
 * @param  lower_limit : 下限値
 * @return クリップ処理後の入力値
 * @note   片方を無制約にしたい場合は，想定範囲より十分大きな有限の値をlimitに設定すること
 */
float C2A_MATH_clip_value(const float input, const float upper_limit, const float lower_limit);

/**
 * @brief  float型変数を指定した絶対値内に収めるクリップ処理
 * @param  float_input : クリップ処理をかける対象 (入力値)
 * @param  abs_limit   : 絶対値上限値
 * @return クリップ処理後の入力値 (入力値の符号は保存する)
 */
float C2A_MATH_clip_value_abs(const float input, const float abs_limit);

/**
 * @brief  非数値除去も含めたfloat型変数のクリップ処理
 *
 *         主としてアクチュエータ指令値や制御器への偏差入力に対する適用を想定
 * @param  float_input        : クリップ処理をかける対象 (入力値)
 * @param  upper_limit        : 上限値
 * @param  lower_limit        : 下限値
 * @param  substitutional_ans : 入力値がNaNやInf等の非数値だった場合の戻り値を指定
 * @return クリップ処理後の入力値
 * @note   片方を無制約にしたい場合は，想定範囲より十分大きな有限の値をlimitに設定すること
 */
float C2A_MATH_clip_value_reject_nan(const float input, const float upper_limit,
                                     const float lower_limit, const float substitutional_ans);

/**
 * @brief  double型変数のクリップ処理
 *
 *         主として軌道推定値等に対するFault Isolation等への適用を想定
 * @param  double_input       : クリップ処理をかける対象 (入力値)
 * @param  upper_limit        : 上限値
 * @param  lower_limit        : 下限値
 * @param  substitutional_ans : 入力値がNaNやInf等の非数値だった場合の戻り値を指定
 * @return クリップ処理後の入力値
 * @note   片方を無制約にしたい場合は，想定範囲より十分大きな有限の値をlimitに設定すること
 */
double C2A_MATH_clip_value_double(const double input, const double upper_limit,
                                  const double lower_limit, const double substitutional_ans);

/**
 * @brief  float型角度 [rad] の [0, 2pi) へのクリップ処理
 *
 *         主として回転機械や軌道等における角度演算への適用を想定
 * @param  float_input_angle_rad   : クリップ処理をかける対象である入力角度 [rad]
 * @return クリップ処理後の入力角度 [0, 2pi) [rad]
 */
float C2A_MATH_normalize_zero_2pi(const float input_angle_rad);

/**
 * @brief  double型角度 [rad] の [0, 2pi) へのクリップ処理
 *
 *         主として回転機械や軌道等における角度演算への適用を想定
 * @param  float_input_angle_rad   : クリップ処理をかける対象である入力角度 [rad]
 * @return クリップ処理後の入力角度 [0, 2pi) [rad]
 */
double C2A_MATH_normalize_zero_2pi_double(const double input_angle_rad);

/**
 * @brief  float型変数が値域内に収まっているか否かの判定
 * @param  float_input : 判定対象 (入力値)
 * @param  upper_limit : 上限値
 * @param  lower_limit : 下限値
 * @return C2A_MATH_ERRORに則したエラーコード
 * @note   片方を無制約にしたい場合は，想定範囲より十分大きな有限の値をlimitに設定すること
 */
C2A_MATH_ERROR C2A_MATH_check_range_violation(const float input,
                                              const float upper_limit,
                                              const float lower_limit);

/**
 * @brief  float型変数が指定した絶対値内に収まっているか否かの判定
 * @param  float_input : 判定対象 (入力値)
 * @param  abs_limit   : 絶対値上限値
 * @return C2A_MATH_ERRORに則したエラーコード
 */
C2A_MATH_ERROR C2A_MATH_check_abs_range_violation(const float input, const float abs_limit);

/**
 * @brief  スカラーの時間微分 c = (a - b) / (ta - tb)
 * @param[in]  after    : スカラーa
 * @param[in]  before   : スカラーb
 * @param[in]  t_after  : スカラーta
 * @param[in]  t_before : スカラーtb
 * @param[out] out      : スカラーc
 */
C2A_MATH_ERROR C2A_MATH_calculate_time_derivative(float out,
                                                  const float after,
                                                  const float before,
                                                  const float t_after,
                                                  const float t_before);

// math.hの拡張関数

/**
 * @brief  入力が値域 [-1,1] を超えている場合でも値を返す様に拡張したarc cosine
 * @param  float_input_cos         : 入力値
 * @return 入力値のarc cosine [rad]
 */
float C2A_MATH_acos_rad(const float input_cos);

#endif
