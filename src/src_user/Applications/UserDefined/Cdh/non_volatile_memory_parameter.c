#pragma section REPRO
/**
* @file   non_volatile_memory_parameter.h
* @brief  不揮発メモリパラメータの定義
*/

#include <stdint.h>
#include "non_volatile_memory_parameter.h"
#include <src_core/Library/print.h>
#include <src_core/Library/majority_vote_for3.h>
#include <src_core/System/EventManager/event_logger.h>
#include <string.h>
#include <stdlib.h>

static NonVolatileMemoryParameter        non_volatile_memory_parameter_;
const  NonVolatileMemoryParameter* const non_volatile_memory_parameter = &non_volatile_memory_parameter_;

static uint8_t APP_NVM_PARAMETER_calc_data_size_(const APP_NVM_PARAM_TYPE data_type);
static APP_NVM_MANAGER_ERROR APP_NVM_PARAMETER_check_registration_(const APP_NVM_PARTITION_ID partition_id,
                                                                   const APP_NVM_PARAM_TYPE data_type,
                                                                   const uint32_t start_address,
                                                                   const uint8_t length);

static void  APP_NVM_PARAMETER_init_(void);
static void  APP_NVM_PARAMETER_exec_(void);

AppInfo APP_NVM_PARAMETER_create_app(void)
{
  return AI_create_app_info("Non-Volatile Memory Parameter", APP_NVM_PARAMETER_init_, APP_NVM_PARAMETER_exec_);
}

static void APP_NVM_PARAMETER_init_(void)
{
  memset(&non_volatile_memory_parameter_, 0x00, sizeof(NonVolatileMemoryParameter));

  // パラメータ登録
  NVM_NORMAL_PARAMS_register();
  NVM_REDUNDANT_PARAMS_register();

  APP_NVM_MANAGER_ERROR ret;

  // テスト用パラメータの初期値書き込み
  uint8_t w_test_u8[2] = {50, 200};
  ret = APP_NVM_PARAM_write_normal_param(NVM_NORMAL_PARAMS_TEST_UINT8, w_test_u8);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  uint16_t w_test_u16 = 500;
  ret = APP_NVM_PARAM_write_normal_param(NVM_NORMAL_PARAMS_TEST_UINT16, &w_test_u16);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  uint32_t w_test_u32 = 2000;
  ret = APP_NVM_PARAM_write_normal_param(NVM_NORMAL_PARAMS_TEST_UINT32, &w_test_u32);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  float w_test_f = 3.14f;
  ret = APP_NVM_PARAM_write_normal_param(NVM_NORMAL_PARAMS_TEST_FLOAT, &w_test_f);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  double w_test_d = -4.2e-7;
  ret = APP_NVM_PARAM_write_normal_param(NVM_NORMAL_PARAMS_TEST_DOUBLE, &w_test_d);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);

  // テスト用パラメータの読み出し
  uint8_t r_test_u8[2];
  ret = APP_NVM_PARAM_read_normal_param(r_test_u8, NVM_NORMAL_PARAMS_TEST_UINT8);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  uint16_t r_test_u16;
  ret = APP_NVM_PARAM_read_normal_param(&r_test_u16, NVM_NORMAL_PARAMS_TEST_UINT16);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  uint32_t r_test_u32;
  ret = APP_NVM_PARAM_read_normal_param(&r_test_u32, NVM_NORMAL_PARAMS_TEST_UINT32);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  float r_test_f;
  ret = APP_NVM_PARAM_read_normal_param(&r_test_f, NVM_NORMAL_PARAMS_TEST_FLOAT);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);
  double r_test_d;
  ret = APP_NVM_PARAM_read_normal_param(&r_test_d, NVM_NORMAL_PARAMS_TEST_DOUBLE);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Normal Param Test Error: %d \n", ret);

  Printf("NVM test read data: u8_1 = %d, u8_2 = %d, u16 = %d, u32 = %d, f = %f, d = %e. \n",
                              r_test_u8[0], r_test_u8[1], r_test_u16, r_test_u32, r_test_f, r_test_d);

  // テスト用三重冗長パラメータの初期値書き込み
  uint8_t w_red_test_u8[2] = {200, 50};
  ret = APP_NVM_PARAM_write_redundant_param(NVM_REDUNDANT_PARAMS_TEST_UINT8, w_red_test_u8);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  uint16_t w_red_test_u16 = 420;
  ret = APP_NVM_PARAM_write_redundant_param(NVM_REDUNDANT_PARAMS_TEST_UINT16, &w_red_test_u16);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  uint32_t w_red_test_u32 = 5000;
  ret = APP_NVM_PARAM_write_redundant_param(NVM_REDUNDANT_PARAMS_TEST_UINT32, &w_red_test_u32);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  float w_red_test_f = 5.0e-6f;
  ret = APP_NVM_PARAM_write_redundant_param(NVM_REDUNDANT_PARAMS_TEST_FLOAT, &w_red_test_f);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  double w_red_test_d = -3.1415;
  ret = APP_NVM_PARAM_write_redundant_param(NVM_REDUNDANT_PARAMS_TEST_DOUBLE, &w_red_test_d);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);

  // テスト用三重冗長パラメータの読み出し
  uint8_t r_red_test_u8[2];
  ret = APP_NVM_PARAM_read_redundant_param(r_red_test_u8, NVM_REDUNDANT_PARAMS_TEST_UINT8);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  uint16_t r_red_test_u16;
  ret = APP_NVM_PARAM_read_redundant_param(&r_red_test_u16, NVM_REDUNDANT_PARAMS_TEST_UINT16);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  uint32_t r_red_test_u32;
  ret = APP_NVM_PARAM_read_redundant_param(&r_red_test_u32, NVM_REDUNDANT_PARAMS_TEST_UINT32);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  float r_red_test_f;
  ret = APP_NVM_PARAM_read_redundant_param(&r_red_test_f, NVM_REDUNDANT_PARAMS_TEST_FLOAT);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);
  double r_red_test_d;
  ret = APP_NVM_PARAM_read_redundant_param(&r_red_test_d, NVM_REDUNDANT_PARAMS_TEST_DOUBLE);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error Redundant Param Test Error: %d \n", ret);

  Printf("NVM test read redundant data: u8_1 = %d, u8_2 = %d, u16 = %d, u32 = %d, f = %f, d = %e. \n",
         r_red_test_u8[0], r_red_test_u8[1], r_red_test_u16, r_red_test_u32, r_red_test_f, r_red_test_d);

  return;
}

static void APP_NVM_PARAMETER_exec_(void)
{
  // 特に何もしない
  return;
}

// 外部公開関数
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_register_normal_param(const NVM_NORMAL_PARAMS param_id,
                                                          const APP_NVM_PARAM_TYPE data_type,
                                                          const uint32_t start_address,
                                                          const uint8_t length)
{
  if (param_id >= NVM_NORMAL_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  APP_NVM_MANAGER_ERROR ret;
  ret = APP_NVM_PARAMETER_check_registration_(APP_NVM_PARTITION_ID_NORMAL_PARAMS,
                                              data_type, start_address, length);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  non_volatile_memory_parameter_.normal_params[param_id].data_type = data_type;
  non_volatile_memory_parameter_.normal_params[param_id].start_address = start_address;
  non_volatile_memory_parameter_.normal_params[param_id].length = length;
  non_volatile_memory_parameter_.normal_params[param_id].registered_flag = 1;

  return APP_NVM_MANAGER_ERROR_OK;
}

APP_NVM_MANAGER_ERROR APP_NVM_PARAM_clear_normal_param(const NVM_NORMAL_PARAMS param_id)
{
  if (param_id >= NVM_NORMAL_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  non_volatile_memory_parameter_.normal_params[param_id].registered_flag = 0;
  return APP_NVM_MANAGER_ERROR_OK;
}

APP_NVM_MANAGER_ERROR APP_NVM_PARAM_write_normal_param(const NVM_NORMAL_PARAMS param_id, const void* write_data_arg)
{
  if (param_id >= NVM_NORMAL_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  if (non_volatile_memory_parameter_.normal_params[param_id].registered_flag == 0) return APP_NVM_MANAGER_ERROR_NG_OTHERS;

  uint8_t* write_data = (uint8_t*)write_data_arg;

  size_t single_data_size = APP_NVM_PARAMETER_calc_data_size_(non_volatile_memory_parameter_.normal_params[param_id].data_type);
  uint32_t size_byte = single_data_size * non_volatile_memory_parameter_.normal_params[param_id].length;

  APP_NVM_MANAGER_ERROR ret;
  ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_NORMAL_PARAMS,
                                      non_volatile_memory_parameter_.normal_params[param_id].start_address,
                                      size_byte, write_data);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  return APP_NVM_MANAGER_ERROR_OK;
}

APP_NVM_MANAGER_ERROR APP_NVM_PARAM_read_normal_param(void* read_data_arg, const NVM_NORMAL_PARAMS param_id)
{
  if (param_id >= NVM_NORMAL_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  if (non_volatile_memory_parameter_.normal_params[param_id].registered_flag == 0) return APP_NVM_MANAGER_ERROR_NG_OTHERS;

  uint8_t* read_data = (uint8_t*)read_data_arg;

  size_t single_data_size = APP_NVM_PARAMETER_calc_data_size_(non_volatile_memory_parameter_.normal_params[param_id].data_type);
  uint32_t size_byte = single_data_size * non_volatile_memory_parameter_.normal_params[param_id].length;

  APP_NVM_MANAGER_ERROR ret;
  ret = APP_NVM_PARTITION_read_bytes(read_data, APP_NVM_PARTITION_ID_NORMAL_PARAMS,
                                     non_volatile_memory_parameter_.normal_params[param_id].start_address,
                                     size_byte);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  return APP_NVM_MANAGER_ERROR_OK;
}


APP_NVM_MANAGER_ERROR APP_NVM_PARAM_register_redundant_param(const NVM_REDUNDANT_PARAMS param_id,
                                                             const APP_NVM_PARAM_TYPE data_type,
                                                             const uint32_t start_address,
                                                             const uint8_t length)
{
  if (param_id >= NVM_REDUNDANT_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  APP_NVM_MANAGER_ERROR ret;
  ret = APP_NVM_PARAMETER_check_registration_(APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_1,
                                              data_type, start_address, length);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  non_volatile_memory_parameter_.redundant_params[param_id].data_type = data_type;
  non_volatile_memory_parameter_.redundant_params[param_id].start_address = start_address;
  non_volatile_memory_parameter_.redundant_params[param_id].length = length;
  non_volatile_memory_parameter_.redundant_params[param_id].registered_flag = 1;

  return APP_NVM_MANAGER_ERROR_OK;
}

APP_NVM_MANAGER_ERROR APP_NVM_PARAM_clear_redundant_param(const NVM_REDUNDANT_PARAMS param_id)
{
  if (param_id >= NVM_REDUNDANT_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  non_volatile_memory_parameter_.redundant_params[param_id].registered_flag = 0;
  return APP_NVM_MANAGER_ERROR_OK;
}

APP_NVM_MANAGER_ERROR APP_NVM_PARAM_write_redundant_param(const NVM_REDUNDANT_PARAMS param_id, const void* write_data_arg)
{
  if (param_id >= NVM_REDUNDANT_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  if (non_volatile_memory_parameter_.redundant_params[param_id].registered_flag == 0) return APP_NVM_MANAGER_ERROR_NG_OTHERS;

  uint8_t* write_data = (uint8_t*)write_data_arg;

  const uint8_t length = non_volatile_memory_parameter_.redundant_params[param_id].length;
  const uint32_t start_address = non_volatile_memory_parameter_.redundant_params[param_id].start_address;
  size_t single_data_size = APP_NVM_PARAMETER_calc_data_size_(non_volatile_memory_parameter_.redundant_params[param_id].data_type);
  uint32_t size_byte = single_data_size * length;

  APP_NVM_MANAGER_ERROR ret;
  ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_1,
                                      start_address, size_byte, write_data);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;
  ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_2,
                                      start_address, size_byte, write_data);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;
  ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_3,
                                      start_address, size_byte, write_data);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  return APP_NVM_MANAGER_ERROR_OK;
}

APP_NVM_MANAGER_ERROR APP_NVM_PARAM_read_redundant_param(void* read_data_arg, const NVM_REDUNDANT_PARAMS param_id)
{
  if (param_id >= NVM_REDUNDANT_PARAMS_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  if (non_volatile_memory_parameter_.redundant_params[param_id].registered_flag == 0) return APP_NVM_MANAGER_ERROR_NG_OTHERS;

  const uint8_t length = non_volatile_memory_parameter_.redundant_params[param_id].length;
  const uint32_t start_address = non_volatile_memory_parameter_.redundant_params[param_id].start_address;
  size_t single_data_size = APP_NVM_PARAMETER_calc_data_size_(non_volatile_memory_parameter_.redundant_params[param_id].data_type);
  uint32_t size_byte = single_data_size * length;

  uint8_t error_count = 0;
  uint8_t* read_data = (uint8_t*)read_data_arg;
  uint8_t* read_data_1 = (uint8_t*)malloc(size_byte);
  uint8_t* read_data_2 = (uint8_t*)malloc(size_byte);
  uint8_t* read_data_3 = (uint8_t*)malloc(size_byte);
  if ((read_data_1 == NULL) || (read_data_2 == NULL) || (read_data_3 == NULL))
  {
    free(read_data_1);
    free(read_data_2);
    free(read_data_3);
    return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  }

  APP_NVM_MANAGER_ERROR ret;
  ret = APP_NVM_PARTITION_read_bytes(read_data_1, APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_1,
                                     start_address, size_byte);
  if (ret != APP_NVM_MANAGER_ERROR_OK) error_count++;
  ret = APP_NVM_PARTITION_read_bytes(read_data_2, APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_2,
                                     start_address, size_byte);
  if (ret != APP_NVM_MANAGER_ERROR_OK) error_count++;
  ret = APP_NVM_PARTITION_read_bytes(read_data_3, APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_3,
                                     start_address, size_byte);
  if (ret != APP_NVM_MANAGER_ERROR_OK) error_count++;

  MAJORITY_VOTE_FOR3_STATUS ret_majority_sts;
  ret_majority_sts = majority_vote_for3_copy(read_data, read_data_1, read_data_2, read_data_3,
                                             single_data_size, length);
  if (ret_majority_sts != MAJORITY_VOTE_MATCH)
  {
    EL_record_event(EL_GROUP_NVM_TRIPLE_REDUNDANT, (uint32_t)param_id, EL_ERROR_LEVEL_HIGH, (uint32_t)ret_majority_sts);
    error_count++;
  }

  free(read_data_1);
  free(read_data_2);
  free(read_data_3);
  if (error_count != 0) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  return APP_NVM_MANAGER_ERROR_OK;
}

// static関数
static uint8_t APP_NVM_PARAMETER_calc_data_size_(const APP_NVM_PARAM_TYPE data_type)
{
  switch (data_type)
  {
  case APP_NVM_PARAM_TYPE_UINT8:
    return sizeof(uint8_t);
  case APP_NVM_PARAM_TYPE_UINT16:
    return sizeof(uint16_t);
  case APP_NVM_PARAM_TYPE_UINT32:
    return sizeof(uint32_t);
  case APP_NVM_PARAM_TYPE_INT8:
    return sizeof(int8_t);
  case APP_NVM_PARAM_TYPE_INT16:
    return sizeof(int16_t);
  case APP_NVM_PARAM_TYPE_INT32:
    return sizeof(int32_t);
  case APP_NVM_PARAM_TYPE_FLOAT:
    return sizeof(float);
  case APP_NVM_PARAM_TYPE_DOUBLE:
    return sizeof(double);
  default:
    return 0;
  }
}

static APP_NVM_MANAGER_ERROR APP_NVM_PARAMETER_check_registration_(const APP_NVM_PARTITION_ID partition_id,
                                                                   const APP_NVM_PARAM_TYPE data_type,
                                                                   const uint32_t start_address,
                                                                   const uint8_t length)
{
  if (partition_id >= APP_NVM_PARTITION_ID_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  if (data_type >= APP_NVM_PARAM_TYPE_MAX)  return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  if (start_address > non_volatile_memory_partition->elements[partition_id].size_byte)
  {
    return APP_NVM_MANAGER_ERROR_NG_ADDRESS;
  }
  uint8_t single_data_size = APP_NVM_PARAMETER_calc_data_size_(data_type);
  if (single_data_size == 0) return APP_NVM_MANAGER_ERROR_NG_LENGTH;
  uint32_t stop_address = start_address + single_data_size * length;
  if (stop_address > non_volatile_memory_partition->elements[partition_id].size_byte)
  {
    return APP_NVM_MANAGER_ERROR_NG_LENGTH;
  }

  return APP_NVM_MANAGER_ERROR_OK;
}

#pragma section
