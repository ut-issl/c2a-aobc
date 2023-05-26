/**
* @file   polynomial_approximation.h
* @brief  多項式近似関連関数群ライブラリ
*/

#ifndef POLYNOMIAL_APPROXIMATION_H_
#define POLYNOMIAL_APPROXIMATION_H_

#include <stdint.h>

#define POLYNOMIAL_APPROX_MAX_COEFF_NUM (10) //!< 係数数最大数

/**
 * @struct PolynomialCoeff
 * @brief  多項式構造体
 */
typedef struct
{
  uint8_t num_coeff_;    //!< 係数数
  float   coefficients_[POLYNOMIAL_APPROX_MAX_COEFF_NUM]; //!< 係数配列
  float   range_low_;    //!< 近似適応可能範囲(下)
  float   range_high_;   //!< 近似適応可能範囲(上)
} PolynomialApprox;

/**
 * @brief 多項式近似構造体の初期化
 * @param[out] polynomial_approx : 多項式近似構造体
 * @param[in]  num_coeff    : 係数数
 * @param[in]  coefficients : 係数配列先頭アドレス
 * @param[in]  range_low    : 近似適応可能範囲(下)
 * @param[in]  range_high   : 近似適応可能範囲(上)
 * @return 0:初期化成功、-1:初期化失敗
 */
int POLYNOMIAL_APPROX_initialize(PolynomialApprox* polynomial_approx,
                          const uint8_t num_coeff,
                          const float* coefficients,
                          const float range_low,
                          const float range_high);

/**
 * @brief 多項式近似計算
 * @param[out] output : 近似出力
 * @param[in]  polynomial_approx : 多項式近似構造体
 * @param[in]  input  : 独立変数
 * @return 0:計算成功、-1:失敗(適応可能範囲外)
 */
int POLYNOMIAL_APPROX_calc_output(float* output, const PolynomialApprox polynomial_approx, const float input);

#endif
