#pragma section REPRO
/**
 * @file  matrix.c
 * @brief 任意サイズ行列計算用の関数群の定義
 */

#include "matrix.h"
#include "math_constants.h"
#include <string.h>
#include <math.h>

#define MATRIX_FUNC(name, type) MATRIX_ ## name ## _ ## type ## _  //!< 各種型に対応する関数定義マクロ

static MATRIX_T(MATRIX_MAX_ROW, MATRIX_MAX_COL, double) tmp_matrix_; //!< 計算用のバッファプールとしての行列．

/**
 * @brief     行列の和の計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 */
#define MATRIX_DEFINE_ADD_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(add, type)(void* out, const void* in_a, const void* in_b) \
{ \
  MATRIX_Parameter* parameter_a = (MATRIX_Parameter*)(in_a); \
  const size_t row = parameter_a->row; \
  const size_t col = parameter_a->col; \
  size_t i, j; \
  const type* a_data = (const type*)MATRIX_get_matrix_data_poitner_(in_a); \
  const type* b_data = (const type*)MATRIX_get_matrix_data_poitner_(in_b); \
  type* out_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(out); \
  \
  for (i = 0; i < row; i++) \
  { \
    for (j = 0; j < col; j++) \
    { \
      size_t idx = MATRIX_get_serialized_index_(i, j, in_a); \
      out_data[idx] = a_data[idx] + b_data[idx]; \
    } \
  } \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列の差の計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 */
#define MATRIX_DEFINE_SUBTRACT_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(subtract, type)(void* out, const void* lhs, const void* rhs) \
{ \
  MATRIX_Parameter* parameter_out = (MATRIX_Parameter*)(out); \
  const size_t row = parameter_out->row; \
  const size_t col = parameter_out->col; \
  size_t i, j; \
  const type* lhs_data = (const type*)MATRIX_get_matrix_data_poitner_(lhs); \
  const type* rhs_data = (const type*)MATRIX_get_matrix_data_poitner_(rhs); \
  type* out_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(out); \
  \
  for (i = 0; i < row; i++) \
  { \
    for (j = 0; j < col; j++) \
    { \
      size_t idx = MATRIX_get_serialized_index_(i, j, lhs); \
      out_data[idx] = lhs_data[idx] - rhs_data[idx]; \
    } \
  } \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列の積の計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 */
#define MATRIX_DEFINE_MULTIPLY_MATRIX_MATRIX_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(multiply_matrix_matrix, type)(void* out, const void* lhs, const void* rhs) \
{ \
  MATRIX_Parameter* parameter_out = (MATRIX_Parameter*)(out); \
  MATRIX_Parameter* parameter_lhs = (MATRIX_Parameter*)(lhs); \
  const size_t row = parameter_out->row; \
  const size_t col = parameter_out->col; \
  const size_t idx_size = parameter_lhs->col; \
  /* lhsやrhsとoutとが同じときlhsやrhsが書き換えられるのを防ぐため，バッファプールを使う */ \
  tmp_matrix_.parameter.row = row; \
  tmp_matrix_.parameter.col = col; \
  size_t i, j, k; \
  const type* lhs_data = (const type*)MATRIX_get_matrix_data_poitner_(lhs); \
  const type* rhs_data = (const type*)MATRIX_get_matrix_data_poitner_(rhs); \
  type* out_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(out); \
  type* tmp_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(&tmp_matrix_); \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  \
  for (i = 0; i < row; i++) \
  { \
    for (j = 0; j < col; j++) \
    { \
      size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
      for (k = 0; k < idx_size; k++) \
      { \
        size_t lhs_idx = MATRIX_get_serialized_index_(i, k, lhs); \
        size_t rhs_idx = MATRIX_get_serialized_index_(k, j, rhs); \
        tmp_data[tmp_idx] += lhs_data[lhs_idx] * rhs_data[rhs_idx]; \
      } \
    } \
  } \
  for (i = 0; i < row; i++) \
  { \
    for (j = 0; j < col; j++) \
    { \
      size_t out_idx = MATRIX_get_serialized_index_(i, j, out); \
      size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
      out_data[out_idx] = tmp_data[tmp_idx]; \
    } \
  } \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列のスカラー倍の計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 */
#define MATRIX_DEFINE_MULTIPLY_SCALAR_MATRIX_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(multiply_scalar_matrix, type)(void* out, double scalar, const void* in) \
{ \
  MATRIX_Parameter* parameter_out = (MATRIX_Parameter*)(out); \
  const size_t row = parameter_out->row; \
  const size_t col = parameter_out->col; \
  size_t i, j; \
  const type* in_data = (const type*)MATRIX_get_matrix_data_poitner_(in); \
  type* out_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(out); \
  \
  for (i = 0; i < row; i++) \
  { \
    for (j = 0; j < col; j++) \
    { \
      size_t idx = MATRIX_get_serialized_index_(i, j, in); \
      out_data[idx] = (type)scalar * in_data[idx]; \
    } \
  } \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列のトレース計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 * @note      行サイズと列サイズが等しいことは呼び出し元でチェックしていることを前提としている
 */
#define MATRIX_DEFINE_TRACE_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(trace, type)(type* trace, const void* mat) \
{ \
  MATRIX_Parameter* parameter_mat = (MATRIX_Parameter*)(mat); \
  const size_t size = parameter_mat->row; \
  size_t i; \
  const type* data = (const type*)MATRIX_get_matrix_data_poitner_(mat); \
  type tmp_trace = 0.0; \
  for (i = 0; i < size; i++) \
  { \
    size_t idx = MATRIX_get_serialized_index_(i, i, mat); \
    tmp_trace += data[idx]; \
  } \
  *trace = tmp_trace; \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列の転置計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 */
#define MATRIX_DEFINE_TRANSPOSE_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(transpose, type)(void* out, const void* in) \
{ \
  MATRIX_Parameter* parameter_in = (MATRIX_Parameter*)(in); \
  const size_t row = parameter_in->row; \
  const size_t col = parameter_in->col; \
  size_t i, j; \
  /* 入力した行列をバッファプールにコピー */ \
  tmp_matrix_.parameter.row = row; \
  tmp_matrix_.parameter.col = col; \
  const type* in_data = (const type*)MATRIX_get_matrix_data_poitner_(in); \
  type* tmp_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(&tmp_matrix_); \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  for (i = 0; i < row; i++) \
  { \
     for (j = 0; j < col; j++) \
     { \
        size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
        tmp_data[tmp_idx] = in_data[tmp_idx]; \
     } \
  } \
  type* out_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(out); \
  for (i = 0; i < row; i++) \
  { \
     for (j = 0; j < col; j++) \
    { \
      size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
      size_t out_idx = MATRIX_get_serialized_index_(j, i, out); \
      out_data[out_idx] = tmp_data[tmp_idx]; \
    } \
  } \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列式計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 * @note      行サイズと列サイズが等しいことは呼び出し元でチェックしていることを前提としている
 */
#define MATRIX_DEFINE_DETERMINANT_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(determinant, type)(type* determinant, const void* mat) \
{ \
  MATRIX_Parameter* parameter_mat = (MATRIX_Parameter*)(mat); \
  const size_t size = parameter_mat->row; \
  size_t i, j; \
  /* 入力した行列をバッファプールにコピー */ \
  tmp_matrix_.parameter.row = size; \
  tmp_matrix_.parameter.col = size; \
  const type* in_data = (const type*)MATRIX_get_matrix_data_poitner_(mat); \
  type* tmp_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(&tmp_matrix_); \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  for (i = 0; i < size; i++) \
  { \
     for (j = 0; j < size; j++) \
     { \
        size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
        tmp_data[tmp_idx] = in_data[tmp_idx]; \
     } \
  } \
  /* バッファプールの行列を変形して上三角行列にする */ \
  /* 上三角部分以外の要素を順番に見ていって，それぞれ0になるように行基本変形を行う */ \
  for (i = 0; i < size; i++) \
  { \
    for (j = 0; j < i; j++) \
    { \
      size_t idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
      size_t diagonal_idx = MATRIX_get_serialized_index_(j, j, &tmp_matrix_); \
      /* 既に要素が0のときはスキップして次の要素を見に行く */ \
      if (fabs(tmp_data[idx]) < MATH_CONST_PROHIBIT_DIVISION_VALUE) \
      { \
        break; \
      } \
      /* ここで見ている要素に対応する対角成分が0のときは行基本変形時に0除算が起きるので，行基本変形で対角成分を0でなくする*/ \
      if (fabs(tmp_data[diagonal_idx]) < MATH_CONST_PROHIBIT_DIVISION_VALUE) \
      { \
        for (size_t l = 0; l < size; l++) \
        { \
          size_t augend_idx = MATRIX_get_serialized_index_(j, l, &tmp_matrix_); \
          size_t addend_idx = MATRIX_get_serialized_index_(i, l, &tmp_matrix_); \
          tmp_data[augend_idx] += tmp_data[addend_idx]; \
        } \
      } \
      /* ここで見ている要素を，行基本変形で0にする */ \
      type ratio = (tmp_data[idx] / tmp_data[diagonal_idx]); \
      for (size_t l = 0; l < size; l++) \
      { \
        size_t augend_idx = MATRIX_get_serialized_index_(i, l, &tmp_matrix_); \
        size_t addend_idx = MATRIX_get_serialized_index_(j, l, &tmp_matrix_); \
        tmp_data[augend_idx] -= ratio * tmp_data[addend_idx]; \
      } \
    } \
  } \
  /* 上三角行列の対角成分を掛け合わせて，行列式を得る */ \
  type tmp_determinant = 1.0; \
  for (i = 0; i < size; i++) \
  { \
    size_t diagonal_idx = MATRIX_get_serialized_index_(i, i, &tmp_matrix_); \
    tmp_determinant *= tmp_data[diagonal_idx]; \
  } \
  *determinant = tmp_determinant; \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     逆行列計算を定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 * @note      行サイズと列サイズが等しいことは呼び出し元でチェックしていることを前提としている
 */
#define MATRIX_DEFINE_INVERSE_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(inverse, type)(void* out, const void* in) \
{ \
  MATRIX_Parameter* parameter_in = (MATRIX_Parameter*)(in); \
  const size_t size = parameter_in->row; \
  size_t i, j, k; \
  /* 入力した行列をバッファプールにコピー */ \
  tmp_matrix_.parameter.row = size; \
  tmp_matrix_.parameter.col = size * 2; \
  const type* in_data = (const type*)MATRIX_get_matrix_data_poitner_(in); \
  type* out_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(out); \
  type* tmp_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(&tmp_matrix_); \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  for (i = 0; i < size; i++) \
  { \
     for (j = 0; j < size; j++) \
     { \
        size_t in_idx  = MATRIX_get_serialized_index_(i, j, in); \
        size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
        tmp_data[tmp_idx] = in_data[in_idx]; \
     } \
  } \
  /* 単位行列を入力行列の横に結合する */ \
  for (i = 0; i < size; i++) \
  { \
     for (j = size; j < size * 2; j++) \
     { \
      size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
      if (i + size == j) \
      { \
        tmp_data[tmp_idx] = (type)1.0; \
      } \
      else \
      { \
        tmp_data[tmp_idx] = (type)0.0; \
      } \
     } \
  } \
  type max_element = tmp_data[0]; /* 掃き出し法のピボット選択に用いる，最大の要素*/ \
  type tmp; /* 掃き出し法のピボット選択に用いる，一時変数*/ \
  for (i = 0; i < size; i++) \
  { \
    for (k = i; k < size; k++) \
    { \
      size_t tmp_idx = MATRIX_get_serialized_index_(k, i, &tmp_matrix_); \
      if (fabs(max_element) < fabs(tmp_data[tmp_idx])) \
      { \
        for (j = 0; j < size * 2; j++) \
        { \
          size_t swap_target_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
          size_t swap_source_idx = MATRIX_get_serialized_index_(k, j, &tmp_matrix_); \
          tmp = tmp_data[swap_target_idx]; \
          tmp_data[swap_target_idx] = tmp_data[swap_source_idx]; \
          tmp_data[swap_source_idx] = tmp; \
        } \
      } \
    } \
    if (fabs(max_element) < MATH_CONST_PROHIBIT_DIVISION_VALUE) \
    { \
      /* 行列の対角成分から0がなくならない場合，行列式0であるから逆行列が存在しない */ \
      return MATRIX_ERROR_SINGULAR; \
    } \
    /* 行正規化 */ \
    size_t diagonal_idx = MATRIX_get_serialized_index_(i, i, &tmp_matrix_); \
    type diagonal_element = tmp_data[diagonal_idx]; \
    for (j = i; j < size * 2; j++) \
    { \
      size_t tmp_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
      tmp_data[tmp_idx] /= diagonal_element; \
    } \
    /* 掃き出し */ \
    for (k = 0; k < size; k++) \
    { \
      if (k != i) \
      { \
        size_t ratio_idx = MATRIX_get_serialized_index_(k, i, &tmp_matrix_); \
        type ratio = tmp_data[ratio_idx]; \
        for (j = i; j < size * 2; j++) \
        { \
          size_t augend_idx = MATRIX_get_serialized_index_(k, j, &tmp_matrix_); \
          size_t addend_idx = MATRIX_get_serialized_index_(i, j, &tmp_matrix_); \
          tmp_data[augend_idx] += -ratio * tmp_data[addend_idx]; \
        } \
      } \
    } \
  } \
  for (i = 0; i < size; i++) \
  { \
    for (j = 0; j < size; j++) \
    { \
      size_t out_idx = MATRIX_get_serialized_index_(i, j, out); \
      size_t tmp_idx = MATRIX_get_serialized_index_(i, j + size, &tmp_matrix_); \
      out_data[out_idx] = tmp_data[tmp_idx]; \
    } \
  } \
  memset(tmp_data, 0x00, sizeof(tmp_matrix_.data)); \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列のコピーを定義する関数マクロ
 * @param     type : 行列要素の型
 * @return    MATRIX_TYPEに準拠
 * @note      行列のサイズチェックと型チェックは呼び出し元で済ませていることを前提としている
 */
#define MATRIX_DEFINE_COPY_FUNC(type) \
static MATRIX_ERROR MATRIX_FUNC(copy, type)(void* out, const void* in) \
{ \
  MATRIX_Parameter* parameter_in = (MATRIX_Parameter*)(in); \
  const size_t row = parameter_in->row; \
  const size_t col = parameter_in->col; \
  size_t i, j; \
  const type* in_data = (const type*)MATRIX_get_matrix_data_poitner_(in); \
  type* out_data = (type*)MATRIX_get_matrix_data_non_const_poitner_(out); \
  \
  for (i = 0; i < row; i++) \
  { \
    for (j = 0; j < col; j++) \
    { \
      size_t idx = MATRIX_get_serialized_index_(i, j, in); \
      out_data[idx] = in_data[idx];\
    } \
  } \
  return MATRIX_ERROR_OK; \
} \


/**
 * @brief     行列の先頭要素が格納されているポインタを取得する
 * @param     matrix : 行列構造体へのポインタ
 * @return    行列データのポインタ
 */
static const uint8_t* MATRIX_get_matrix_data_poitner_(const void* matrix);


/**
  * @brief     行列の各要素が格納されている非constなポインタを取得する
  * @param     matrix : 行列構造体へのポインタ
  * @return    非constな行列データのポインタ
  * @note      入出力のインデックスは0起算．
  */
static uint8_t* MATRIX_get_matrix_data_non_const_poitner_(const void* matrix);


/**
  * @brief     1次元配列に直した行列のインデックスを取得する
  * @param     row_index : 行番号
  * @param     col_index : 列番号
  * @param     matrix    : 行列へのポインタ
  * @return    行列要素へのインデックス
  */
static size_t MATRIX_get_serialized_index_(size_t row_index, size_t col_index, const void* matrix);


/**
  * @brief     行列のサイズが最大サイズをオーバーしていないかどうか，1未満でないかどうかをチェックする
  * @param     matrix    : 行列へのポインタ
  * @return    MATRIX_ERRORに準拠
  */
static MATRIX_ERROR MATRIX_validate_size_(const void* matrix);

/**
  * @brief     2つの行列のサイズが一致しているかどうかチェックする
  * @param     in_a    : 行列へのポインタ
  * @param     in_b    : 行列へのポインタ
  * @return    MATRIX_ERRORに準拠
  */
static MATRIX_ERROR MATRIX_validate_size_matching_(const void* in_a, const void* in_b);

/**
  * @brief     2つの行列の型が一致しているかどうかチェックする
  * @param     in_a    : 行列へのポインタ
  * @param     in_b    : 行列へのポインタ
  * @return    MATRIX_ERRORに準拠
  */
static MATRIX_ERROR MATRIX_validate_type_matching_(const void* in_a, const void* in_b);


#ifdef MATRIX_USE_TYPE_FLOAT
MATRIX_DEFINE_ADD_FUNC(float)
MATRIX_DEFINE_SUBTRACT_FUNC(float)
MATRIX_DEFINE_MULTIPLY_MATRIX_MATRIX_FUNC(float)
MATRIX_DEFINE_MULTIPLY_SCALAR_MATRIX_FUNC(float)
MATRIX_DEFINE_TRACE_FUNC(float)
MATRIX_DEFINE_TRANSPOSE_FUNC(float)
MATRIX_DEFINE_DETERMINANT_FUNC(float)
MATRIX_DEFINE_INVERSE_FUNC(float)
MATRIX_DEFINE_COPY_FUNC(float)
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
MATRIX_DEFINE_ADD_FUNC(double)
MATRIX_DEFINE_SUBTRACT_FUNC(double)
MATRIX_DEFINE_MULTIPLY_MATRIX_MATRIX_FUNC(double)
MATRIX_DEFINE_MULTIPLY_SCALAR_MATRIX_FUNC(double)
MATRIX_DEFINE_TRACE_FUNC(double)
MATRIX_DEFINE_TRANSPOSE_FUNC(double)
MATRIX_DEFINE_DETERMINANT_FUNC(double)
MATRIX_DEFINE_INVERSE_FUNC(double)
MATRIX_DEFINE_COPY_FUNC(double)
#endif


MATRIX_ERROR MATRIX_add(void* out, const void* in_a, const void* in_b)
{
  MATRIX_ERROR ret;

  if (in_a == NULL) return MATRIX_ERROR_NULL;
  if (in_b == NULL) return MATRIX_ERROR_NULL;
  if (out == NULL)  return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_a = (const MATRIX_Parameter*)in_a;

  ret = MATRIX_validate_size_(in_a);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(in_b);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(out);
  if (ret != MATRIX_ERROR_OK) return ret;

  ret = MATRIX_validate_size_matching_(in_a, in_b);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_matching_(in_a, out);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(in_a, in_b);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(in_a, out);
  if (ret != MATRIX_ERROR_OK) return ret;

  switch (parameter_a->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(add, float)(out, in_a, in_b);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(add, double)(out, in_a, in_b);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


MATRIX_ERROR MATRIX_subtract(void* out, const void* lhs, const void* rhs)
{
  MATRIX_ERROR ret;

  if (lhs == NULL) return MATRIX_ERROR_NULL;
  if (rhs == NULL) return MATRIX_ERROR_NULL;
  if (out == NULL) return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_a = (const MATRIX_Parameter*)lhs;

  ret = MATRIX_validate_size_(lhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(rhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(out);
  if (ret != MATRIX_ERROR_OK) return ret;

  ret = MATRIX_validate_size_matching_(lhs, rhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_matching_(out, lhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(lhs, rhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(out, lhs);
  if (ret != MATRIX_ERROR_OK) return ret;

  switch (parameter_a->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(subtract, float)(out, lhs, rhs);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(subtract, double)(out, lhs, rhs);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


MATRIX_ERROR MATRIX_multiply(void* out, const void* lhs, const void* rhs)
{
  MATRIX_ERROR ret;

  if (lhs == NULL) return MATRIX_ERROR_NULL;
  if (rhs == NULL) return MATRIX_ERROR_NULL;
  if (out == NULL) return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_lhs = (const MATRIX_Parameter*)lhs;
  const MATRIX_Parameter* parameter_rhs = (const MATRIX_Parameter*)rhs;
  const MATRIX_Parameter* parameter_out = (const MATRIX_Parameter*)out;

  ret = MATRIX_validate_size_(lhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(rhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(out);
  if (ret != MATRIX_ERROR_OK) return ret;

  if (parameter_lhs->row != parameter_out->row) return MATRIX_ERROR_SIZE_MISMATCH;
  if (parameter_rhs->col != parameter_out->col) return MATRIX_ERROR_SIZE_MISMATCH;
  if (parameter_lhs->col != parameter_rhs->row) return MATRIX_ERROR_SIZE_MISMATCH;

  ret = MATRIX_validate_type_matching_(lhs, rhs);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(out, lhs);
  if (ret != MATRIX_ERROR_OK) return ret;

  switch (parameter_lhs->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(multiply_matrix_matrix, float)(out, lhs, rhs);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(multiply_matrix_matrix, double)(out, lhs, rhs);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


MATRIX_ERROR MATRIX_multiply_scalar(void* out, const double scalar, const void* in)
{
  MATRIX_ERROR ret;

  if (out == NULL) return MATRIX_ERROR_NULL;
  if (in == NULL) return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_in = (const MATRIX_Parameter*)in;

  ret = MATRIX_validate_size_(out);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(in);
  if (ret != MATRIX_ERROR_OK) return ret;

  ret = MATRIX_validate_size_matching_(out, in);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(out, in);
  if (ret != MATRIX_ERROR_OK) return ret;

  switch (parameter_in->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(multiply_scalar_matrix, float)(out, scalar, in);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(multiply_scalar_matrix, double)(out, scalar, in);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


MATRIX_ERROR MATRIX_trace(double* trace, const void* mat)
{
  MATRIX_ERROR ret;

  if (mat == NULL) return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_mat = (const MATRIX_Parameter*)mat;

  ret = MATRIX_validate_size_(mat);
  if (ret != MATRIX_ERROR_OK) return ret;

  if (parameter_mat->row != parameter_mat->col) return MATRIX_ERROR_SIZE_MISMATCH;

  switch (parameter_mat->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(trace, float)((float*)trace, mat);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(trace, double)(trace, mat);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


MATRIX_ERROR MATRIX_transpose(void* out, const void* in)
{
  MATRIX_ERROR ret;

  if (out == NULL) return MATRIX_ERROR_NULL;
  if (in == NULL) return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_out = (const MATRIX_Parameter*)out;
  const MATRIX_Parameter* parameter_in = (const MATRIX_Parameter*)in;

  ret = MATRIX_validate_size_(out);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(in);
  if (ret != MATRIX_ERROR_OK) return ret;

  if (parameter_out->row != parameter_in->col) return MATRIX_ERROR_SIZE_MISMATCH;
  if (parameter_out->col != parameter_in->row) return MATRIX_ERROR_SIZE_MISMATCH;

  ret = MATRIX_validate_type_matching_(out, in);
  if (ret != MATRIX_ERROR_OK) return ret;

  switch (parameter_in->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(transpose, float)(out, in);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(transpose, double)(out, in);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


MATRIX_ERROR MATRIX_determinant(double* determinant, const void* mat)
{
  MATRIX_ERROR ret;

  if (mat == NULL) return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_mat = (const MATRIX_Parameter*)mat;

  ret = MATRIX_validate_size_(mat);
  if (ret != MATRIX_ERROR_OK) return ret;

  if (parameter_mat->row != parameter_mat->col) return MATRIX_ERROR_SIZE_MISMATCH;

  switch (parameter_mat->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(determinant, float)((float*)determinant, mat);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(determinant, double)(determinant, mat);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}

MATRIX_ERROR MATRIX_inverse(void* out, const void* in)
{
  MATRIX_ERROR ret;

  if (in == NULL) return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_in = (const MATRIX_Parameter*)in;

  ret = MATRIX_validate_size_(in);
  if (ret != MATRIX_ERROR_OK) return ret;

  ret = MATRIX_validate_size_matching_(out, in);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(out, in);
  if (ret != MATRIX_ERROR_OK) return ret;

  if (parameter_in->row != parameter_in->col) return MATRIX_ERROR_SIZE_MISMATCH;

  switch (parameter_in->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(inverse, float)(out, in);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(inverse, double)(out, in);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


MATRIX_ERROR MATRIX_copy(void* out, const void* in)
{
  MATRIX_ERROR ret;

  if (in == NULL) return MATRIX_ERROR_NULL;
  if (out == NULL)  return MATRIX_ERROR_NULL;

  const MATRIX_Parameter* parameter_in = (const MATRIX_Parameter*)in;

  ret = MATRIX_validate_size_(in);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_size_(out);
  if (ret != MATRIX_ERROR_OK) return ret;

  ret = MATRIX_validate_size_matching_(in, out);
  if (ret != MATRIX_ERROR_OK) return ret;
  ret = MATRIX_validate_type_matching_(in, out);
  if (ret != MATRIX_ERROR_OK) return ret;

  switch (parameter_in->matrix_type)
  {
#ifdef MATRIX_USE_TYPE_FLOAT
  case MATRIX_TYPE_float:
    return MATRIX_FUNC(copy, float)(out, in);
#endif
#ifdef MATRIX_USE_TYPE_DOUBLE
  case MATRIX_TYPE_double:
    return MATRIX_FUNC(copy, double)(out, in);
#endif
  default:
    return MATRIX_ERROR_OTHERS;
  }
}


void* MATRIX_make_init_matrix(const size_t row, const size_t col, MATRIX_TYPE type_enum)
{
  static MATRIX_T(MATRIX_MAX_ROW, MATRIX_MAX_COL, double) ret;
  memset(&ret, 0x00, sizeof(ret));
  MATRIX_Parameter* parameter_ret = (MATRIX_Parameter*)&ret;

  // サイズが0以下や許容最大サイズ以上の場合，また定義していない型が入力されたとき，0×0のdouble型行列を返す．
  parameter_ret->row = 0;
  parameter_ret->col = 0;
  parameter_ret->dummy = 0;
  parameter_ret->matrix_type = MATRIX_TYPE_double;
  if (row < 1 || row > MATRIX_MAX_ROW)
  {
    return (void*)&ret;
  }
  if (col < 1 || col > MATRIX_MAX_COL)
  {
    return (void*)&ret;
  }
  if (type_enum < (MATRIX_TYPE)0 || type_enum > MATRIX_TYPE_MAX)
  {
    return (void*)&ret;
  }

  parameter_ret->row = row;
  parameter_ret->col = col;
  parameter_ret->matrix_type = type_enum;
  return (void*)&ret;
}


static const uint8_t* MATRIX_get_matrix_data_poitner_(const void* matrix)
{
  const uint8_t* pointer = (const uint8_t*)matrix;
  return pointer + sizeof(MATRIX_Parameter);
}


static uint8_t* MATRIX_get_matrix_data_non_const_poitner_(const void* matrix)
{
  uint8_t* pointer = (uint8_t*)matrix;
  return pointer + sizeof(MATRIX_Parameter);
}


static size_t MATRIX_get_serialized_index_(size_t row_index, size_t col_index, const void* matrix)
{
  const MATRIX_Parameter* parameter_mat = (const MATRIX_Parameter*)matrix;
  return row_index * parameter_mat->col + col_index;
}


static MATRIX_ERROR MATRIX_validate_size_(const void* matrix)
{
  const MATRIX_Parameter* parameter_mat = (const MATRIX_Parameter*)matrix;

  if (parameter_mat->row > MATRIX_MAX_ROW) return MATRIX_ERROR_SIZE_OVER;
  if (parameter_mat->col > MATRIX_MAX_COL) return MATRIX_ERROR_SIZE_OVER;

  if (parameter_mat->row < 1 || parameter_mat->col < 1) return MATRIX_ERROR_SIZE_SHORTAGE;

  return MATRIX_ERROR_OK;
}

MATRIX_ERROR MATRIX_validate_size_matching_(const void* in_a, const void* in_b)
{
  const MATRIX_Parameter* parameter_a = (const MATRIX_Parameter*)in_a;
  const MATRIX_Parameter* parameter_b = (const MATRIX_Parameter*)in_b;

  if (parameter_a->row != parameter_b->row) return MATRIX_ERROR_SIZE_MISMATCH;
  if (parameter_a->col != parameter_b->col) return MATRIX_ERROR_SIZE_MISMATCH;

  return MATRIX_ERROR_OK;
}

MATRIX_ERROR MATRIX_validate_type_matching_(const void* in_a, const void* in_b)
{
  const MATRIX_Parameter* parameter_a = (const MATRIX_Parameter*)in_a;
  const MATRIX_Parameter* parameter_b = (const MATRIX_Parameter*)in_b;

  if (parameter_a->matrix_type != parameter_b->matrix_type) return MATRIX_ERROR_TYPE_MISMATCH;

  return MATRIX_ERROR_OK;
}

#pragma section
