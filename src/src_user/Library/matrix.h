/**
 * @file  matrix.h
 * @brief 任意サイズ行列計算用の関数群の定義
 */
#ifndef MATRIX_H_
#define MATRIX_H_

#include "c2a_math.h"
#include "matrix_user_settings.h"
#include <stddef.h>

/**
 * @enum   MATRIX_ERROR
 * @brief  行列計算処理に関するエラーコード
 * @note   uint8_tを想定
 */
typedef enum
{
  MATRIX_ERROR_OK  = 0,        //!< 数値エラーなし
  MATRIX_ERROR_NULL,           //!< 計算対象の行列がNULL
  MATRIX_ERROR_SINGULAR,       //!< callされた関数の処理内で極端に大きい or 小さい値が発生している
  MATRIX_ERROR_TYPE_MISMATCH,  //!< 行列計算中の型不一致
  MATRIX_ERROR_SIZE_MISMATCH,  //!< 行列計算中のサイズ不一致
  MATRIX_ERROR_SIZE_OVER,      //!< 行列サイズが最大許容サイズを超えている
  MATRIX_ERROR_SIZE_SHORTAGE,  //!< 行や列のサイズが1未満
  MATRIX_ERROR_OTHERS          //!< その他
} MATRIX_ERROR;

/**
 * @brief     行列の初期化をする関数マクロ
 * @param[in] row       : 行数
 * @param[in] col       : 列数
 * @param[in] type      : 型名
 * @return    MATRIX_T(n, n, type)型構造体
 * @note      使用例
 *            MATRIX_T(2, 2, float) matrix22 = MATRIX_MAKE_INIT_MATRIX(2, 2, float);
 */
#define MATRIX_MAKE_INIT_MATRIX(row, col, type) \
(*(MATRIX_T(row, col, type)*)MATRIX_make_init_matrix(row, col, MATRIX_TYPE_MEMBER(type)))

/**
 * @brief      任意サイズ行列同士の足し算 C = A + B
 * @param[out] out  : 行列C
 * @param[in]  in_a : 行列A
 * @param[in]  in_b : 行列B
 * @return          : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_add(void* out, const void* in_a, const void* in_b);

/**
 * @brief      任意サイズ行列同士の引き算 C = A - B
 * @param[out] out  : 行列C
 * @param[in]  lhs  : 行列A
 * @param[in]  rhs  : 行列B
 * @return          : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_subtract(void* out, const void* lhs, const void* rhs);

/**
 * @brief      任意サイズ行列同士の積 C = A * B
 * @param[out] out  : 行列C
 * @param[in]  lhs  : 行列A
 * @param[in]  rhs  : 行列B
 * @return          : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_multiply(void* out, const void* lhs, const void* rhs);

/**
 * @brief      任意サイズ行列のスカラー倍 B = c * A
 * @param[out] out    : 行列B
 * @param[in]  scalar : 定数c（最も大きいdouble型で確保しているが，float型のスカラーを入れても良い）
 * @param[in]  in     : 行列A
 * @return            : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_multiply_scalar(void* out, const double scalar, const void* in);

/**
 * @brief      任意サイズ行列のトレースの計算 b = tr(A)
 * @param[out] trace : トレースb（float型などのトレースを計算したい場合，(double*)にキャストして入力する）
 * @param[in]  mat   : 行列A
 * @return           : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_trace(double* trace, const void* mat);

/**
 * @brief      任意サイズ行列の転置行列の計算 B = A^T
 * @param[in]  in    : 行列A
 * @param[out] out   : 転置行列B
 * @return           : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_transpose(void* out, const void* in);

/**
 * @brief      任意サイズ行列の行列式の計算 b = det(A)
 * @param[out] determinant   : 行列式b
 * @param[in]  mat           : 行列A
 * @return                   : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_determinant(double* determinant, const void* mat);

/**
 * @brief      任意サイズ行列の逆行列の計算 B = A^(-1)
 * @param[in]  in    : 行列A
 * @param[out] out   : 逆行列B
 * @return           : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_inverse(void* out, const void* in);

/**
 * @brief      任意サイズ行列のコピー B = A
 * @param[in]  in    : コピー元行列A
 * @param[out] out   : コピー先行列B
 * @return           : C2A_MATH_ERRORに準拠
 */
MATRIX_ERROR MATRIX_copy(void* out, const void* in);

// ============= 以下の関数は，ユーザーは使わない =================

/**
 * @brief     任意サイズ行列の生成
 * @param[in] row       : 行数
 * @param[in] col       : 列数
 * @param[in] type_enum : MATRIX_TYPEに準拠
 * @note MATRIX_GET_INIT_MATRIX を使う．この関数は直接使わない．
 */
void* MATRIX_make_init_matrix(const size_t row, const size_t col, MATRIX_TYPE type_enum);
#endif
