/**
* @file   non_volatile_memory_normal_parameters.h
* @brief  不揮発メモリ保存ノーマルパラメータの登録
* @note   下記で管理 TODO_L : コード自動生成
*          https://docs.google.com/spreadsheets/d/1JWxLqdwr3IzQBAkYI4zkRBb0jKZwCUtGaSDt9z3XXPQ/edit#gid=1413802869
*/
#ifndef NON_VOLATILE_MEMORY_NORMAL_PARAMETERS_H_
#define NON_VOLATILE_MEMORY_NORMAL_PARAMETERS_H_

/**
 * @enum  NVM_NORMAL_PARAMS
 * @brief 不揮発メモリで管理する通常パラメータのリスト
 * @note  uint8を想定
 */
typedef enum
{
  // テスト(削除して良い)
  NVM_NORMAL_PARAMS_TEST_UINT8 = 0,
  NVM_NORMAL_PARAMS_TEST_UINT16,
  NVM_NORMAL_PARAMS_TEST_UINT32,
  NVM_NORMAL_PARAMS_TEST_FLOAT,
  NVM_NORMAL_PARAMS_TEST_DOUBLE,
  // これ以降に追加
  // ここより上に追加
  NVM_NORMAL_PARAMS_MAX = 100, // とりあえずこの値に設定
} NVM_NORMAL_PARAMS;


void NVM_NORMAL_PARAMS_register(void);

#endif
