#pragma section REPRO
/**
* @file   non_volatile_memory_normal_parameters.c
* @brief  不揮発メモリ保存ノーマルパラメータの登録
* @note   下記で管理 TODO_L : コード自動生成
*          https://docs.google.com/spreadsheets/d/1JWxLqdwr3IzQBAkYI4zkRBb0jKZwCUtGaSDt9z3XXPQ/edit#gid=1413802869
*/
#include "non_volatile_memory_normal_parameters.h"
#include "../../../Applications/UserDefined/Cdh/non_volatile_memory_parameter.h"

void NVM_NORMAL_PARAMS_register(void)
{
  // テスト
  APP_NVM_PARAM_register_normal_param(NVM_NORMAL_PARAMS_TEST_UINT8,
                                      APP_NVM_PARAM_TYPE_UINT8, 0, 2);
  APP_NVM_PARAM_register_normal_param(NVM_NORMAL_PARAMS_TEST_UINT16,
                                      APP_NVM_PARAM_TYPE_UINT16, 2, 1);
  APP_NVM_PARAM_register_normal_param(NVM_NORMAL_PARAMS_TEST_UINT32,
                                      APP_NVM_PARAM_TYPE_UINT32, 4, 1);
  APP_NVM_PARAM_register_normal_param(NVM_NORMAL_PARAMS_TEST_FLOAT,
                                      APP_NVM_PARAM_TYPE_FLOAT, 8, 1);
  APP_NVM_PARAM_register_normal_param(NVM_NORMAL_PARAMS_TEST_DOUBLE,
                                      APP_NVM_PARAM_TYPE_DOUBLE, 12, 1);

  return;
}

#pragma section
