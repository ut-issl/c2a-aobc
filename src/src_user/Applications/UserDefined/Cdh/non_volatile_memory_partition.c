#pragma section REPRO
/**
* @file   non_volatile_memory_partition.c
* @brief  不揮発メモリアクセス用のパーティションを定義する
*/
#include "non_volatile_memory_partition.h"
#include <src_core/Library/print.h>
#include "../../../Library/stdint.h"
#include <string.h>

static NonVolatileMemoryPartition        non_volatile_memory_partition_;
const  NonVolatileMemoryPartition* const non_volatile_memory_partition = &non_volatile_memory_partition_;

static void  APP_NVM_PARTITION_init_(void);
static void  APP_NVM_PARTITION_exec_(void);

/**
 * @brief パーティションを設定する
 * @param[in] id            : パーティションID
 * @param[in] start_address : パーティションの先頭アドレス
 * @param[in] size_byte     : パーティションのサイズ
 * @note 今の所、ここ以外で設定することを想定していないのでstatic関数にしている
 * @return               : APP_NVM_MANAGER_ERRORに準拠
 */
static APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_set_partition_(APP_NVM_PARTITION_ID id, uint32_t start_address, uint32_t size_byte);
/**
 * @brief パーティション内のアドレスからグローバルなアドレスを計算する
 * @param[out] start_address_in_global   : グローバルなアドレス
 * @param[in] id                         : パーティションID
 * @param[in] start_address_in_partition : パーティション内でのアクセス先先頭アドレス
 * @param[in] size_byte                  : アクセスデータサイズ
 * @return               : APP_NVM_MANAGER_ERRORに準拠
 */
static APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_calc_global_address_(uint32_t* start_address_in_global,
                                                                    APP_NVM_PARTITION_ID id,
                                                                    uint32_t start_address_in_partition,
                                                                    uint32_t size_byte);

AppInfo APP_NVM_PARTITION_create_app(void)
{
  return AI_create_app_info("Non-Volatile Memory Partition", APP_NVM_PARTITION_init_, APP_NVM_PARTITION_exec_);
}

static void APP_NVM_PARTITION_init_(void)
{
  memset(&non_volatile_memory_partition_, 0x00, sizeof(NonVolatileMemoryPartition));

  // 各パーティションの先頭アドレスなどは下記で管理
  // https://docs.google.com/spreadsheets/d/1JWxLqdwr3IzQBAkYI4zkRBb0jKZwCUtGaSDt9z3XXPQ/edit#gid=885360724
  // TODO_L コード自動生成する？
  APP_NVM_MANAGER_ERROR ret;
  APP_NVM_PARTITION_ID id;
  id = APP_NVM_PARTITION_ID_NORMAL_PARAMS;
  ret = APP_NVM_PARTITION_set_partition_(id, 0x00000, 16 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  id = APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_1;
  ret = APP_NVM_PARTITION_set_partition_(id, 0x04000, 16 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  id = APP_NVM_PARTITION_ID_BCT;
  ret = APP_NVM_PARTITION_set_partition_(id, 0x08000, 64 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  id = APP_NVM_PARTITION_ID_DR_1;
  ret = APP_NVM_PARTITION_set_partition_(id, 0x18000, 32 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  id = APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_2;
  ret = APP_NVM_PARTITION_set_partition_(id, 0x20000, 16 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  id = APP_NVM_PARTITION_ID_DR_2;
  ret = APP_NVM_PARTITION_set_partition_(id, 0x24000, 112 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  id = APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_3;
  ret = APP_NVM_PARTITION_set_partition_(id , 0x40000, 16 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  id = APP_NVM_PARTITION_ID_DR_3;
  ret = APP_NVM_PARTITION_set_partition_(id, 0x44000, 240 * 1024);
  if (ret != APP_NVM_MANAGER_ERROR_OK) Printf("Error APP_NVM_PARTITION set id = %d, ret = %d \n", id, ret);

  return;
}

static void APP_NVM_PARTITION_exec_(void)
{
  // 特に何もしない
  return;
}

CCP_EXEC_STS Cmd_APP_NVM_PARTITION_WRITE_BYTES(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t param_idx = 0;

  APP_NVM_PARTITION_ID partition_id;
  partition_id = (APP_NVM_PARTITION_ID)param[0];
  param_idx++;
  if (partition_id >= APP_NVM_PARTITION_ID_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint32_t start_address;
  endian_memcpy(&start_address, param + param_idx, sizeof(uint32_t));
  param_idx += sizeof(uint32_t);
  if (start_address >= APP_NVM_MANAGER_STOP_ADDRESS) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint8_t length;
  endian_memcpy(&length, param + param_idx, sizeof(uint8_t));
  param_idx += sizeof(uint8_t);
  if (length > APP_NVM_MANAGER_MAX_CMD_DATA_SIZE) return CCP_EXEC_ILLEGAL_PARAMETER;

  // 引数がrawなのでサイズチェックが必要
  uint8_t param_len_byte = param_idx + length;
  if (CCP_get_param_len(packet) != param_len_byte) return CCP_EXEC_ILLEGAL_LENGTH;

  uint8_t write_data[APP_NVM_MANAGER_MAX_CMD_DATA_SIZE];
  endian_memcpy(write_data, param + param_idx, sizeof(uint8_t) * length);
  param_idx += length;

  APP_NVM_MANAGER_ERROR error_status = APP_NVM_PARTITION_write_bytes(partition_id, start_address, length, write_data);
  if (error_status != APP_NVM_MANAGER_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  // テレメ用データ保存
  non_volatile_memory_partition_.write_info.partition_id = partition_id;
  non_volatile_memory_partition_.write_info.start_address = start_address;
  non_volatile_memory_partition_.write_info.data_size_byte = length;
  memcpy(non_volatile_memory_partition_.write_info.data, write_data, length);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_NVM_PARTITION_READ_BYTES(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t param_idx = 0;

  APP_NVM_PARTITION_ID partition_id;
  partition_id = (APP_NVM_PARTITION_ID)param[0];
  param_idx++;
  if (partition_id >= APP_NVM_PARTITION_ID_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint32_t start_address;
  endian_memcpy(&start_address, param + param_idx, sizeof(uint32_t));
  param_idx += sizeof(uint32_t);
  if (start_address >= APP_NVM_MANAGER_STOP_ADDRESS) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint8_t length;
  endian_memcpy(&length, param + param_idx, sizeof(uint8_t));
  param_idx += sizeof(uint8_t);
  if (length > APP_NVM_MANAGER_MAX_CMD_DATA_SIZE) return CCP_EXEC_ILLEGAL_PARAMETER;

  non_volatile_memory_partition_.error_status = APP_NVM_PARTITION_read_bytes(non_volatile_memory_partition_.read_info.data,
                                                                             partition_id, start_address, length);
  if (non_volatile_memory_partition_.error_status != APP_NVM_MANAGER_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  // テレメ用データ保存
  non_volatile_memory_partition_.read_info.partition_id = partition_id;
  non_volatile_memory_partition_.read_info.start_address = start_address;
  non_volatile_memory_partition_.read_info.data_size_byte = length;

  return CCP_EXEC_SUCCESS;
}

// 外部公開関数
APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID id, uint32_t start_address, uint32_t size_byte, uint8_t* write_data)
{
  uint32_t global_start_address;
  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_calc_global_address_(&global_start_address, id, start_address, size_byte);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  ret = APP_NVM_MANAGER_write_bytes(global_start_address, size_byte, write_data);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  return APP_NVM_MANAGER_ERROR_OK;
}

APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_read_bytes(uint8_t* read_data, APP_NVM_PARTITION_ID id, uint32_t start_address, uint32_t size_byte)
{
  uint32_t global_start_address;
  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_calc_global_address_(&global_start_address, id, start_address, size_byte);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  ret = APP_NVM_MANAGER_read_bytes(read_data, global_start_address, size_byte);
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  return APP_NVM_MANAGER_ERROR_OK;
}

// static関数
static APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_set_partition_(APP_NVM_PARTITION_ID id, uint32_t start_address, uint32_t size_byte)
{
  if (id >= APP_NVM_PARTITION_ID_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  if (start_address >= APP_NVM_MANAGER_STOP_ADDRESS) return APP_NVM_MANAGER_ERROR_NG_ADDRESS;

  uint32_t stop_address = start_address + size_byte;
  if (stop_address > APP_NVM_MANAGER_STOP_ADDRESS) return APP_NVM_MANAGER_ERROR_NG_LENGTH;

  // TODO_L 他のパーティションとの被りがないかも確認すべき？

  non_volatile_memory_partition_.elements[id].start_address = start_address;
  non_volatile_memory_partition_.elements[id].size_byte = size_byte;

  return APP_NVM_MANAGER_ERROR_OK;
}

static APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_calc_global_address_(uint32_t* start_address_in_global,
                                                                    APP_NVM_PARTITION_ID id,
                                                                    uint32_t start_address_in_partition,
                                                                    uint32_t size_byte)
{
  if (id >= APP_NVM_PARTITION_ID_MAX) return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  const APP_NVM_PARTITION_Elements* element = &non_volatile_memory_partition_.elements[id];

  if (start_address_in_partition > element->size_byte) return APP_NVM_MANAGER_ERROR_NG_ADDRESS;

  uint32_t stop_address = start_address_in_partition + size_byte;
  if (stop_address > element->size_byte) return APP_NVM_MANAGER_ERROR_NG_LENGTH;

  *start_address_in_global = element->start_address + start_address_in_partition;

  return APP_NVM_MANAGER_ERROR_OK;
}

#pragma section
