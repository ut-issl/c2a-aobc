/**
 * @file
 * @brief  行列計算用の関数群の定義
 */
#ifndef MATRIX33_H_
#define MATRIX33_H_

#include "physical_constants.h"
#include "c2a_math.h"

/**
  * @enum   MATRIX33_ROTATION_AXIS
  * @brief  回転行列を求める回転軸
  */
typedef enum
{
  MATRIX33_ROTATION_AXIS_X = 0,    //!< x軸周りの回転
  MATRIX33_ROTATION_AXIS_Y = 1,    //!< y軸周りの回転
  MATRIX33_ROTATION_AXIS_Z = 2,    //!< z軸周りの回転
  MATRIX33_ROTATION_MAX
} MATRIX33_ROTATION_AXIS;

/**
 * @brief      (3,3)の行列同士の足し算 C = A + B
 * @param[out] out  : 行列C
 * @param[in]  in_a : 行列A
 * @param[in]  in_b : 行列B
 */
void MATRIX33_add(float out [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
            const float in_a[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
            const float in_b[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      (3,3)の行列同士の引き算 C = A - B
 * @param[out] out : 行列C
 * @param[in]  lhs : 行列A
 * @param[in]  rhs : 行列B
 */
void MATRIX33_subtract(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                 const float lhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                 const float rhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  (3,3)の行列のスカラー倍 B = c * A
 * @param[out] out    : 行列B
 * @param[in]  scalar : 定数c
 * @param[in]  in     : 行列A
 */
void MATRIX33_multiply_scalar_matrix(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float scalar,
                               const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  (3,3)の行列とベクトルの掛け算 c = Ab
 * @param[out] vec_out : ベクトルc
 * @param[in]  mat     : 行列A
 * @param[in]  vec_in  : ベクトルb
 */
void MATRIX33_multiply_matrix_vector(float vec_out[PHYSICAL_CONST_THREE_DIM],
                               const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float vec_in[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  double版(3,3)の行列とベクトルの掛け算 c = Ab
 * @param[out] vec_out : ベクトルc
 * @param[in]  mat     : 行列A
 * @param[in]  vec_in  : ベクトルb
 */
void MATRIX33_multiply_matrix_vector_double(double vec_out[PHYSICAL_CONST_THREE_DIM],
                                      const double mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                      const double vec_in[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  (3,3)の行列Aとベクトルbの二次形式の計算 c = b^t*A*b
 * @param[in]  mat     : 行列A
 * @param[in]  vec_in  : ベクトルb
 * @return             : スカラーc
 */
float MATRIX33_multiply_matrix_vector_quadratic(const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                const float vec[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      (3,3)の行列同士の掛け算 C = A * B
 * @param[out] out : 行列C
 * @param[in]  lhs : 行列A
 * @param[in]  rhs : 行列B
 */
void MATRIX33_multiply_matrix_matrix(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float lhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float rhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      double版(3,3)の行列同士の掛け算 C = A * B
 * @param[out] out : 行列C
 * @param[in]  lhs : 行列A
 * @param[in]  rhs : 行列B
 */
void MATRIX33_multiply_matrix_matrix_double(double out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                      const double lhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                      const double rhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      (3,3)の行列のトレースの計算 b = tr(A)
 * @param[in]  mat   : 行列A
 * @return     trace : スカラーb
 */
float MATRIX33_trace(const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      (3,3)の行列の転置行列の計算 B = A^T
 * @param[in]  in    : 行列A
 * @param[out] out   : 転置行列B
 */
void MATRIX33_transpose(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                  const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      (3,3)の行列の転置行列の計算 B = A^T
 * @param[in]  in    : 行列A
 * @param[out] out   : 転置行列B
 */
void MATRIX33_transpose_double(double out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                         const double in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      (3,3)の行列の行列式の計算 b = det(A)
 * @param[in]  mat : 行列A
 * @return     det : スカラーb
 */
float MATRIX33_determinant(const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  (3,3)の行列の余因子行列の計算 B = adj(A)
 * @param[out] out : 行列B
 * @param[in]  in  : 行列A
 */
void MATRIX33_adjugate(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                 const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  (3,3)の行列の逆行列の計算 B = inv(A)
 * @param[out] inv : 行列B
 * @param[in]  mat : 行列A
 * @return     AOCS_MATH_ERROR
 * @note       Aの行列式が微小量のとき余因子のみを計算し，Aの行列式での割り算を行わない
 */
C2A_MATH_ERROR MATRIX33_inverse(float inv[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                          const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief (3次元) 対角行列の生成 M = diag(v[0], v[1], v[2])
 * @param [out] matrix     : 対角行列の格納先
 * @param [in] seed_vector : 対角行列の元にするベクトル，M = diag(v[0], v[1], v[2])のv
 */
void MATRIX33_make_diagonal(float matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                      const float seed_vector[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief double版(3次元) 対角行列の生成 M = diag(v[0], v[1], v[2])
 * @param [out] matrix     : 対角行列の格納先
 * @param [in] seed_vector : 対角行列の元にするベクトル，M = diag(v[0], v[1], v[2])のv
 */
void MATRIX33_make_diagonal_double(double matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                             const double seed_vector[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief (3次元) 単位行列の生成 M = diag(1, 1, 1)
 * @param matrix : 単位行列の格納先
 */
void MATRIX33_make_unit(float matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief double版(3次元) 単位行列の生成 M = diag(1, 1, 1)
 * @param matrix : 単位行列の格納先
 */
void MATRIX33_make_unit_double(double matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief (3次元) 歪対象行列 (skew matrix) の生成 M = ~v
 * @param [out] matrix      : 歪対象行列 (skew matrix) の格納先
 * @param [in]  seed_vector : skewの元になる3次元ベクトル，M = ~vのv
 */
void MATRIX33_make_skew(float matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                  const float seed_vector[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief     回転行列の計算 R_axis(theta)
 * @param[out] mat   : 回転行列R
 * @param[in]  theta : 回転角[rad]
 * @param[in]  axis  : MATRIX_ROTATION_AXIS
 */
void MATRIX33_make_rotation(float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM], const float theta, const MATRIX33_ROTATION_AXIS axis);

/**
 * @brief     double版回転行列の計算 R_axis(theta)
 * @param[out] mat   : 回転行列R
 * @param[in]  theta : 回転角[rad]
 * @param[in]  axis  : MATRIX_ROTATION_AXIS
 */
void MATRIX33_make_rotation_double(double mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                   const double theta,
                                   const MATRIX33_ROTATION_AXIS axis);

/**
 * @brief      2つの線形独立なベクトルから座標変換行列を計算する
 * @param[out] dcm_out       : direct cosine matrix
 * @param[in]  first_vec_in  : first vector
 * @param[in]  second_vec_in : second vector
 * @return     C2A_MATH_ERROR (first vectorとsecond vectorが線形独立でないとき，エラーを返す)
 * @note       入力された2つのベクトルは関数内で正規化される
 */
C2A_MATH_ERROR MATRIX33_make_dcm_from_two_vectors(float dcm_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                  float first_vec_in[PHYSICAL_CONST_THREE_DIM],
                                                  float second_vec_in[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief 行列のコピー
 * @param [out] matrix_out : コピー先行列
 * @param [in]  matrix_in  : コピー元
 */
void MATRIX33_copy(float matrix_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
             const float matrix_in [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief double版行列のコピー
 * @param [out] matrix_out : コピー先行列
 * @param [in]  matrix_in  : コピー元
 */
void MATRIX33_copy_double(double matrix_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                    const double matrix_in [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  行列が規格化 (行列式が1) されているかの判定
 * @param  in : 規格化判定対象の行列
 * @return C2A_MATH_ERROR
 */
C2A_MATH_ERROR MATRIX33_is_normalized(const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief 行列の規格化 (行列式が1になる様に各成分を等倍する)
 * @param [out] matrix_out : 規格化後行列
 * @param [in]  matrix_in  : 規格化対象
 * @return C2A_MATH_ERROR
 * @note   行列式がゼロ近傍の場合には，単位行列を返す
 */
C2A_MATH_ERROR MATRIX33_normalize(float matrix_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                            const float matrix_in [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  行列が対称行列かの判定
 * @param  in: 判定対象の行列
 * @return C2A_MATH_ERROR
 */
C2A_MATH_ERROR MATRIX33_is_symmetry(const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

#endif
