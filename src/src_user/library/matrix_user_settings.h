/**
 * @file  matrix_user_settings.h
 * @brief 任意サイズ行列計算のユーザーセッティング部
 * @note  ユーザー側でこのファイルをincludeする必要はなく，matrix.hのみincludeすればよい
 */
#ifndef MATRIX_USER_SETTINGS_H_
#define MATRIX_USER_SETTINGS_H_
#include "matrix_definition.h"

// 型の設定．使用したい型を増やす場合，matrix_definition.hのMATRIX_Parameter構造体のメンバも増やす
#define MATRIX_USE_TYPE_FLOAT       //!< float型を使うかどうか
#define MATRIX_USE_TYPE_DOUBLE      //!< double型を使うかどうか

// 行列最大サイズの設定．
#define MATRIX_MAX_ROW 7            //!< 行列型に許容する最大の行数
#define MATRIX_MAX_COL 14           //!< 行列型に許容する最大の列数 （NOTE：逆行列計算で掃き出し法を用いるため列数は行数の2倍必要）


MATRIX_DEFINE_MATRIX_STRUCT(MATRIX_MAX_ROW, MATRIX_MAX_COL, double); //!< 最大サイズの行列．行列計算時の中間変数として用いる

#endif
