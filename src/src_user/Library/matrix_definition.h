/**
 * @file  matrix_definition.h
 * @brief 任意サイズ行列計算の型定義部
 * @note  ユーザー側でこのファイルをincludeする必要はなく，matrix.hのみincludeすればよい
 */
#ifndef MATRIX_DEFINITION_H_
#define MATRIX_DEFINITION_H_

#include "stdint.h"
#include <stddef.h>

/**
 * @enum   MATRIX_TYPE
 * @brief  行列の型指定
 * @note   uint8_tを想定．実装上，一部小文字にする必要があるため，ここでは命名規則を無視する
 */
typedef enum
{
  MATRIX_TYPE_float,
  MATRIX_TYPE_double,
  MATRIX_TYPE_MAX
} MATRIX_TYPE;

/**
 * @struct MATRIX_Parameter
 * @brief  行列の型情報，サイズを格納する
 * @note   行列型のメモリ配置は，MATRIX_Parameterの後にdataが来る．
 */
typedef struct
{
  MATRIX_TYPE matrix_type;
  size_t row;
  size_t col;
  uint64_t dummy; //!< MATRIX_T構造体のdataメンバの開始アドレスが，パディングによって意図しない位置から始まらないようにするためのダミーデータ
} MATRIX_Parameter;

#define MATRIX_T_INNER(row, col, type) MATRIX_ ## row ## _ ## col ## _ ## type ## _t  //!< MATRIX_Tの展開用マクロ．inner functionなので，他では決して使わない．
#define MATRIX_T(row, col, type) MATRIX_T_INNER(row, col, type)  //!< 各行列演算関数の返り値型定義マクロ
#define MATRIX_TYPE_MEMBER(type) MATRIX_TYPE_ ## type //!< 型とMATRIX_TYPE enumの変換マクロ

#define MATRIX_DEFINE_MATRIX_SRTUCT(row, col, type) \
typedef struct \
{ \
  MATRIX_Parameter parameter; \
  type data[row][col]; \
} MATRIX_T(row, col, type) //!< 各種型の行列構造体を宣言するするマクロ

#endif
