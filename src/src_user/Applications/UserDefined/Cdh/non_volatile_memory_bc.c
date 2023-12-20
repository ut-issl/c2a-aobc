#pragma section REPRO
/**
* @file
* @brief BCT のコピーを不揮発メモリに保存する
* @note 使い方
*       0. 起動時はデフォルトで無効化されている
*       1. Cmd_APP_NVM_BC_MANAGER_SET_ENABLE で有効化すると BC が不揮発に自動同期される
*       2. OBC がリセットしてしまったら, 復元したい BC だけ Cmd_APP_NVM_BC_MANAGER_RESTORE_BC_FROM_NVM を用いて1つずつ復元する
*          復元前に Cmd_APP_NVM_BC_MANAGER_SET_ENABLE で有効化するとコピーが消えてしまうので注意
*       3. 復元が終わったら再度有効化する
*/

#include "non_volatile_memory_bc.h"

#include <string.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/block_command_executor.h>

static NonVolatileMemoryBcManager nvm_bc_manager_;
const  NonVolatileMemoryBcManager* const nvm_bc_manager = &nvm_bc_manager_;

static void APP_NVM_BC_MANAGER_init_(void);
static void APP_NVM_BC_MANAGER_exec_(void);

/**
 * @brief 指定された BC を複数まとめて不揮発にコピーする
 * @param[in] begin_bc_id : 最初にコピーする BC の id
 * @param[in] num         : コピーする BC 数
 * @return void
 * @note アプリで定期実行される
 */
static void APP_NVM_BC_MANAGER_copy_bc_(bct_id_t begin_bc_id, uint8_t num);

/**
 * @brief 指定された BC を１つ不揮発から復元する
 * @param[in] bc_id : 復元する BC の id
 * @return APP_NVM_MANAGER_ERROR
 * @note 地上局コマンドで実行される
 */
static APP_NVM_MANAGER_ERROR APP_NVM_BC_MANAGER_restore_bc_from_nvm_(bct_id_t bc_id);

/**
 * @brief 指定した BC が保存されている不揮発メモリ上のアドレスを返す
 * @param[in] bc_id
 * @return 不揮発メモリ上のアドレス
 */
static uint32_t APP_NVM_BC_MANAGER_get_address_from_bc_id_(bct_id_t bc_id);


AppInfo APP_NVM_BC_MANAGER_create_app(void)
{
  return AI_create_app_info("Non-volatile BC Manager", APP_NVM_BC_MANAGER_init_, APP_NVM_BC_MANAGER_exec_);
}

static void APP_NVM_BC_MANAGER_init_(void)
{
  APP_NVM_MANAGER_ERROR ret;

  nvm_bc_manager_.is_active = 0;  // 起動直後は無効化しておく
  nvm_bc_manager_.bc_id_to_copy = 0;
  nvm_bc_manager_.bc_num_to_copy = 10;
  nvm_bc_manager_.address_for_ready_flags = non_volatile_memory_partition->elements[APP_NVM_PARTITION_ID_BCT].start_address;
  nvm_bc_manager_.address_for_bc = nvm_bc_manager_.address_for_ready_flags + sizeof(nvm_bc_manager_.is_ready_to_restore);

  // is_ready_to_restore を不揮発から復元する
  ret = APP_NVM_PARTITION_read_bytes(nvm_bc_manager_.is_ready_to_restore,
                                     APP_NVM_PARTITION_ID_BCT,
                                     nvm_bc_manager_.address_for_ready_flags,
                                     sizeof(nvm_bc_manager_.is_ready_to_restore));
  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    EL_record_event(EL_GROUP_NVM_BC_MANAGER, (uint32_t)APP_NVM_MANAGER_ERROR_RESTORE_READY_FLAG, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
  }

  return;
}

static void APP_NVM_BC_MANAGER_exec_(void)
{
  // 有効化するまでは何もしない
  if (!nvm_bc_manager_.is_active) return;

  // 実行時間を考慮して、1回あたり nvm_bc_manager_.bc_num_to_copy 個ずつコピーを取っていく
  // nvm_bc_manager_.bc_num_to_copy は BCT_MAX_BLOCKS の約数であることを前提とする

  APP_NVM_BC_MANAGER_copy_bc_(nvm_bc_manager_.bc_id_to_copy, nvm_bc_manager_.bc_num_to_copy);

  if (nvm_bc_manager_.bc_id_to_copy + nvm_bc_manager_.bc_num_to_copy >= BCT_MAX_BLOCKS)
  {
    nvm_bc_manager_.bc_id_to_copy = 0;

    // 1周したので is_ready_to_restore の魚拓も取る
    APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                                              nvm_bc_manager_.address_for_ready_flags,
                                                              sizeof(nvm_bc_manager_.is_ready_to_restore),
                                                              nvm_bc_manager_.is_ready_to_restore);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      EL_record_event(EL_GROUP_NVM_BC_MANAGER, (uint32_t)APP_NVM_MANAGER_ERROR_COPY_READY_FLAG, EL_ERROR_LEVEL_LOW, (uint32_t)ret);
    }
  }
  else
  {
    nvm_bc_manager_.bc_id_to_copy += nvm_bc_manager_.bc_num_to_copy;
  }

  return;
}

static void APP_NVM_BC_MANAGER_copy_bc_(bct_id_t begin_bc_id, uint8_t num)
{
  bct_id_t bc_id;
  uint32_t write_address;
  uint8_t data_tmp[sizeof(BCT_Table)];
  APP_NVM_MANAGER_ERROR ret;

  for (uint8_t i = 0; i < num; ++i)
  {
    bc_id = begin_bc_id + i;
    nvm_bc_manager_.is_ready_to_restore[bc_id] = 0;

    // 有効化されている BC のみコピーする
    if (!BCE_is_active(bc_id)) continue;

    write_address = APP_NVM_BC_MANAGER_get_address_from_bc_id_(bc_id);
    if (write_address < nvm_bc_manager_.address_for_bc)
    {
      EL_record_event(EL_GROUP_NVM_BC_MANAGER, (uint32_t)APP_NVM_MANAGER_ERROR_NG_ADDRESS_NVM_BC, EL_ERROR_LEVEL_LOW, (uint32_t)bc_id);
      break;
    }

    memcpy(data_tmp, block_command_table->blocks[bc_id], sizeof(BCT_Table));

    // 不揮発に BC をコピー
    ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                        write_address,
                                        sizeof(BCT_Table),
                                        data_tmp);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      EL_record_event(EL_GROUP_NVM_BC_MANAGER, (uint32_t)ret, EL_ERROR_LEVEL_LOW, bc_id);
    }
    else
    {
      // 正常にコピーできた場合のみ ready flag を立てる
      nvm_bc_manager_.is_ready_to_restore[bc_id] = 1;
    }
  }

  return;
}

static APP_NVM_MANAGER_ERROR APP_NVM_BC_MANAGER_restore_bc_from_nvm_(bct_id_t bc_id)
{
  if (!nvm_bc_manager_.is_ready_to_restore[bc_id]) return APP_NVM_MANAGER_ERROR_NOT_READY_TO_RESTORE;

  uint32_t read_address = APP_NVM_BC_MANAGER_get_address_from_bc_id_(bc_id);

  if (read_address < nvm_bc_manager_.address_for_bc) return APP_NVM_MANAGER_ERROR_NG_ADDRESS_NVM_BC;

  uint8_t data_tmp[sizeof(BCT_Table)];

  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_read_bytes(data_tmp,
                                                           APP_NVM_PARTITION_ID_BCT,
                                                           read_address,
                                                           sizeof(BCT_Table));
  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    return ret;
  }

  BCT_ACK ack = BCT_copy_bct_from_bytes(bc_id, data_tmp);
  if (ack != BCT_SUCCESS)
  {
    return APP_NVM_MANAGER_ERROR_BCT_COPY_FAIL;
  }

  return APP_NVM_MANAGER_ERROR_OK;
}

static uint32_t APP_NVM_BC_MANAGER_get_address_from_bc_id_(bct_id_t bc_id)
{
  if (bc_id >= BCT_MAX_BLOCKS)
  {
    return 0;
  }

  return nvm_bc_manager_.address_for_bc + sizeof(BCT_Table) * bc_id;
}

CCP_CmdRet Cmd_APP_NVM_BC_MANAGER_SET_ENABLE(const CommonCmdPacket* packet)
{
  APP_NVM_MANAGER_ERROR ret;
  uint8_t is_active = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (is_active > 0)
  {
    nvm_bc_manager_.is_active = 1;

    // ready flag を全て下ろす
    memset(nvm_bc_manager_.is_ready_to_restore, 0, sizeof(nvm_bc_manager_.is_ready_to_restore));
    ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                        nvm_bc_manager_.address_for_ready_flags,
                                        sizeof(nvm_bc_manager_.is_ready_to_restore),
                                        nvm_bc_manager_.is_ready_to_restore);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, ret);
    }
  }
  else
  {
    nvm_bc_manager_.is_active = 0;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_NVM_BC_MANAGER_RESTORE_BC_FROM_NVM(const CommonCmdPacket* packet)
{
  bct_id_t bc_id = CCP_get_param_from_packet(packet, 0, bct_id_t);

  if (bc_id >= BCT_MAX_BLOCKS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  APP_NVM_MANAGER_ERROR ret = APP_NVM_BC_MANAGER_restore_bc_from_nvm_(bc_id);

  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, ret);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

// 軌道上で使うことは想定していないが、念のため構造体のメンバ変数をいじれるようにしておく
CCP_CmdRet Cmd_APP_NVM_BC_MANAGER_OTHER_SETTINGS(const CommonCmdPacket* packet)
{
  uint8_t idx = CCP_get_param_from_packet(packet, 0, uint8_t);
  uint32_t value = CCP_get_param_from_packet(packet, 1, uint32_t);

  switch (idx)
  {
  case 0:
    if (value >= BCT_MAX_BLOCKS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    nvm_bc_manager_.bc_id_to_copy = (bct_id_t)value;
  case 1:
    if (value > 255) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    nvm_bc_manager_.bc_num_to_copy = (uint8_t)value;
  case 2:
    nvm_bc_manager_.address_for_ready_flags = value;
  case 3:
    nvm_bc_manager_.address_for_bc = value;
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
