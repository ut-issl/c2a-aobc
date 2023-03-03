/**
 * @file
 * @brief  ベクトル計算（3次元）用の関数群の定義
 */
#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "physical_constants.h"
#include "c2a_math.h"

/**
 * @brief  ベクトルの初期化
 * @param[out] out   : 出力ベクトル
 * @param[in]  value : 初期化時に埋める値
 */
void VECTOR3_initialize(float out[PHYSICAL_CONST_THREE_DIM],
                  const float value);

/**
 * @brief  double版ベクトルの初期化
 * @param[out] out   : 出力ベクトル
 * @param[in]  value : 初期化時に埋める値
 */
void VECTOR3_initialize_double(double out[PHYSICAL_CONST_THREE_DIM],
                         const double value);

/**
 * @brief  ベクトルの内積計算 c = a ・ b
 * @param[in] in_a : ベクトルa
 * @param[in] in_b : ベクトルb
 * @return    ans  : スカラーc
 */
float VECTOR3_inner_product(const float in_a[PHYSICAL_CONST_THREE_DIM],
                            const float in_b[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの外積計算 c = a × b
 * @param[out] out : ベクトルc
 * @param[in]  lhs : ベクトルa
 * @param[in]  rhs : ベクトルb
 * @note       破壊的関数 (outはlhs，rhsと異なるベクトルとすること)
 */
void VECTOR3_outer_product(float out[PHYSICAL_CONST_THREE_DIM],
                     const float lhs[PHYSICAL_CONST_THREE_DIM],
                     const float rhs[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの外積計算 c = a × b
 * @param[out] out : ベクトルc
 * @param[in]  lhs : ベクトルa
 * @param[in]  rhs : ベクトルb
 * @note       破壊的関数 (outはlhs，rhsと異なるベクトルとすること)
 */
void VECTOR3_outer_product_double(double out[PHYSICAL_CONST_THREE_DIM],
                            const double lhs[PHYSICAL_CONST_THREE_DIM],
                            const double rhs[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルのアダマール積計算 c = a ○ b
 * @param[in]  lhs : ベクトルa
 * @param[in]  rhs : ベクトルb
 * @param[out] out : ベクトルc
 */
void VECTOR3_hadamard_product(float out[PHYSICAL_CONST_THREE_DIM],
                              const float lhs[PHYSICAL_CONST_THREE_DIM],
                              const float rhs[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの定数倍計算 b = c * a
 * @param[out] out    : ベクトルb
 * @param[in]  scalar : スカラーc
 * @param[in]  in     : ベクトルa
 */
void VECTOR3_scalar_product(float out[PHYSICAL_CONST_THREE_DIM],
                      const float scalar,
                      const float in[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの加算 c = a + b
 * @param[out] out  : ベクトルc
 * @param[in]  in_a : ベクトルa
 * @param[in]  in_b : ベクトルb
 */
void VECTOR3_add(float out[PHYSICAL_CONST_THREE_DIM],
           const float in_a[PHYSICAL_CONST_THREE_DIM],
           const float in_b[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの加算 c = a + b
 * @param[out] out  : ベクトルc
 * @param[in]  in_a : ベクトルa
 * @param[in]  in_b : ベクトルb
 */
void VECTOR3_add_double(double out[PHYSICAL_CONST_THREE_DIM],
                  const double in_a[PHYSICAL_CONST_THREE_DIM],
                  const double in_b[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの減算 c = a - b
 * @param[out] out : ベクトルc
 * @param[in]  lhs : ベクトルa
 * @param[in]  rhs : ベクトルb
 */
void VECTOR3_subtract(float out[PHYSICAL_CONST_THREE_DIM],
                const float lhs[PHYSICAL_CONST_THREE_DIM],
                const float rhs[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの減算 c = a - b
 * @param[out] out : ベクトルc
 * @param[in]  lhs : ベクトルa
 * @param[in]  rhs : ベクトルb
 */
void VECTOR3_subtract_double(double out[PHYSICAL_CONST_THREE_DIM],
                       const double lhs[PHYSICAL_CONST_THREE_DIM],
                       const double rhs[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの時間微分 c = (a - b) / (ta - tb)
 * @param[in]  after    : ベクトルa
 * @param[in]  before   : ベクトルb
 * @param[in]  t_after  : スカラーta
 * @param[in]  t_before : スカラーtb
 * @param[out] out      : ベクトルc
 * @return              : C2A_MATH_ERROR
 */
C2A_MATH_ERROR VECTOR3_time_derivative(float out[PHYSICAL_CONST_THREE_DIM],
                                       const float after[PHYSICAL_CONST_THREE_DIM],
                                       const float before[PHYSICAL_CONST_THREE_DIM],
                                       const float t_after,
                                       const float t_before);

/**
 * @brief  ベクトルのコピー b = a
 * @param[out] out : ベクトルb
 * @param[in]  in  : ベクトルa
 */
void VECTOR3_copy(float out[PHYSICAL_CONST_THREE_DIM],
            const float in[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  double型ベクトルのコピー b = a
 * @param[out] out : ベクトルb
 * @param[in]  in  : ベクトルa
 */
void VECTOR3_copy_double(double out[PHYSICAL_CONST_THREE_DIM],
                  const double in [PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルのノルム計算 b = norm(a)
 * @param[in] vec  : ベクトルa
 * @return    norm : スカラーb
 */
float VECTOR3_norm(const float vec[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルの正規化 b = a / norm(a)
 * @param[out] out : ベクトルb
 * @param[in]  in  : ベクトルa
 * @return     C2A_MATH_ERROR
 * @note       ベクトルaのノルムが微小量のとき正規化を行わず，ベクトルaをそのまま返す
 */
C2A_MATH_ERROR VECTOR3_normalize(float out[PHYSICAL_CONST_THREE_DIM],
                           const float in[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  正規化ベクトルの判定
 * @param[in]  in
 * @return     C2A_MATH_ERROR_OK           : 正規化されている
 * @return     C2A_MATH_ERROR_UNNORMALIZED : 正規化されていない
 * @return     C2A_MATH_ERROR_SINGULAR     : 正規化できない
 */
C2A_MATH_ERROR VECTOR3_is_normalized(const float in[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  ベクトルのテンソル積 (内積の転置の順序を入れ替えた演算 M = v1(v2^t), Mは行列)
 * @param [out] matrix : 演算結果である行列M = v1(v2^t)のMの格納先
 * @param [in]  v_in1  : M = v1(v2^t)のv1
 * @param [in]  v_in2  : M = v1(v2^t)のv2
 */
void VECTOR3_tensor_product(float matrix[][PHYSICAL_CONST_THREE_DIM],
                      const float v_in1[PHYSICAL_CONST_THREE_DIM],
                      const float v_in2[PHYSICAL_CONST_THREE_DIM]);

#endif
