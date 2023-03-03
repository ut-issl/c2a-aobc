#pragma section REPRO
/**
* @file   non_volatile_memory_manager.c
* @brief  不揮発メモリ管理をするアプリ
*/
#include "non_volatile_memory_manager.h"
#include "../../../Library/stdint.h"
#include <string.h>

/**
 * @brief ハードウェア依存のエラーコードを不揮発メモリ管理アプリのエラーコードに変換する
 * @param[in] error_code : ハードウェア依存のエラーコード
 * @return               : APP_NVM_MANAGER_ERRORに準拠
 */
static APP_NVM_MANAGER_ERROR APP_NVM_MANAGER_convert_error_code_(DI_FM25V10_ERROR error_code);

static NonVolatileMemoryManager        non_volatile_memory_manager_;
const  NonVolatileMemoryManager* const non_volatile_memory_manager = &non_volatile_memory_manager_;

static void  APP_NVM_MANAGER_init_(void);
static void  APP_NVM_MANAGER_exec_(void);

AppInfo APP_NVM_MANAGER_create_app(void)
{
  return AI_create_app_info("Non-Volatile Memory Manager", APP_NVM_MANAGER_init_, APP_NVM_MANAGER_exec_);
}

static void APP_NVM_MANAGER_init_(void)
{
  memset(&non_volatile_memory_manager_, 0x00, sizeof(NonVolatileMemoryManager));
  non_volatile_memory_manager_.error_status = APP_NVM_MANAGER_ERROR_OK;

  return;
}

static void APP_NVM_MANAGER_exec_(void)
{
  // 特に何もしない
  return;
}

CCP_EXEC_STS Cmd_APP_NVM_MANAGER_WRITE_BYTES(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t param_idx = 0;

  uint32_t start_address;
  endian_memcpy(&start_address, param + param_idx, sizeof(uint32_t));
  param_idx += sizeof(uint32_t);
  if (start_address >= APP_NVM_MANAGER_STOP_ADDRESS) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint8_t length;
  endian_memcpy(&length, param + param_idx, sizeof(uint8_t));
  param_idx += sizeof(uint8_t);
  if (length > APP_NVM_MANAGER_MAX_CMD_DATA_SIZE) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (length == 0) return CCP_EXEC_ILLEGAL_PARAMETER;

  // 引数がrawなのでサイズチェックが必要
  uint8_t param_len_byte = param_idx + length;
  if (CCP_get_param_len(packet) != param_len_byte) return CCP_EXEC_ILLEGAL_LENGTH;

  uint8_t write_data[APP_NVM_MANAGER_MAX_CMD_DATA_SIZE];
  endian_memcpy(write_data, param + param_idx, sizeof(uint8_t) * length);
  param_idx += length;

  non_volatile_memory_manager_.error_status = APP_NVM_MANAGER_write_bytes(start_address, length, write_data);
  if (non_volatile_memory_manager_.error_status != APP_NVM_MANAGER_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  // テレメ用データ保存
  non_volatile_memory_manager_.write_info.start_address = start_address;
  non_volatile_memory_manager_.write_info.data_size_byte = length;
  memcpy(non_volatile_memory_manager_.write_info.data, write_data, length);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_NVM_MANAGER_READ_BYTES(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t param_idx = 0;

  uint32_t start_address;
  endian_memcpy(&start_address, param + param_idx, sizeof(uint32_t));
  param_idx += sizeof(uint32_t);
  if (start_address >= APP_NVM_MANAGER_STOP_ADDRESS) return CCP_EXEC_ILLEGAL_PARAMETER;

  uint8_t length;
  endian_memcpy(&length, param + param_idx, sizeof(uint8_t));
  param_idx += sizeof(uint8_t);
  if (length > APP_NVM_MANAGER_MAX_CMD_DATA_SIZE) return CCP_EXEC_ILLEGAL_PARAMETER;

  non_volatile_memory_manager_.error_status = APP_NVM_MANAGER_read_bytes(non_volatile_memory_manager_.read_info.data, start_address, length);
  if (non_volatile_memory_manager_.error_status != APP_NVM_MANAGER_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  // テレメ用データ保存
  non_volatile_memory_manager_.read_info.start_address = start_address;
  non_volatile_memory_manager_.read_info.data_size_byte = length;

  return CCP_EXEC_SUCCESS;
}

// 以降、使うハードウェアに応じて修正する関数
APP_NVM_MANAGER_ERROR APP_NVM_MANAGER_write_bytes(const uint32_t start_address, const uint32_t length, const uint8_t* write_data)
{
  DI_FM25V10_ERROR ret = DI_FM25V10_global_write_bytes(start_address, length, write_data);
  return APP_NVM_MANAGER_convert_error_code_(ret);
}

APP_NVM_MANAGER_ERROR APP_NVM_MANAGER_read_bytes(uint8_t* read_data, const uint32_t start_address, const uint32_t length)
{
  DI_FM25V10_ERROR ret = DI_FM25V10_global_read_bytes(read_data, start_address, length);
  return APP_NVM_MANAGER_convert_error_code_(ret);
}

static APP_NVM_MANAGER_ERROR APP_NVM_MANAGER_convert_error_code_(DI_FM25V10_ERROR error_code)
{
  switch (error_code)
  {
  case DI_FM25V10_ERROR_OK:
    return APP_NVM_MANAGER_ERROR_OK;
  case DI_FM25V10_ERROR_NG_ADDRESS:
    return APP_NVM_MANAGER_ERROR_NG_ADDRESS;
  case DI_FM25V10_ERROR_NG_LENGTH:
    return APP_NVM_MANAGER_ERROR_NG_LENGTH;
  case DI_FM25V10_ERROR_NG_READ:
    return APP_NVM_MANAGER_ERROR_NG_READ;
  case DI_FM25V10_ERROR_NG_WRITE:
    return APP_NVM_MANAGER_ERROR_NG_WRITE;
  default:
    return APP_NVM_MANAGER_ERROR_NG_OTHERS;
  }
}

#pragma section
