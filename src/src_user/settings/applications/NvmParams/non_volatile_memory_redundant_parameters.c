#pragma section REPRO
/**
* @file   non_volatile_memory_redundant_parameters.c
* @brief  不揮発メモリ保存冗長パラメータの登録
* @note   下記で管理 TODO_L : コード自動生成
*         https://docs.google.com/spreadsheets/d/1JWxLqdwr3IzQBAkYI4zkRBb0jKZwCUtGaSDt9z3XXPQ/edit#gid=1180200168
*/
#include "non_volatile_memory_redundant_parameters.h"
#include <src_user/Applications/UserDefined/Cdh/non_volatile_memory_parameter.h>

void NVM_REDUNDANT_PARAMS_register(void)
{
  // テスト
  APP_NVM_PARAM_register_redundant_param(NVM_REDUNDANT_PARAMS_TEST_UINT8,
                                         APP_NVM_PARAM_TYPE_UINT8, 0, 2);
  APP_NVM_PARAM_register_redundant_param(NVM_REDUNDANT_PARAMS_TEST_UINT16,
                                         APP_NVM_PARAM_TYPE_UINT16, 2, 1);
  APP_NVM_PARAM_register_redundant_param(NVM_REDUNDANT_PARAMS_TEST_UINT32,
                                         APP_NVM_PARAM_TYPE_UINT32, 4, 1);
  APP_NVM_PARAM_register_redundant_param(NVM_REDUNDANT_PARAMS_TEST_FLOAT,
                                         APP_NVM_PARAM_TYPE_FLOAT, 8, 1);
  APP_NVM_PARAM_register_redundant_param(NVM_REDUNDANT_PARAMS_TEST_DOUBLE,
                                         APP_NVM_PARAM_TYPE_DOUBLE, 12, 1);

  return;
}

#pragma section
