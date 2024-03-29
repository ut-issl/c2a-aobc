#pragma section REPRO
/**
* @file
* @brief BCT のコピーを不揮発メモリに保存する
* @note 詳細は .h を参照
*/

#include "non_volatile_memory_bc.h"

#include <string.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/block_command_executor.h>

static NonVolatileMemoryBc nvm_bc_;
const  NonVolatileMemoryBc* const nvm_bc = &nvm_bc_;

static void APP_NVM_BC_init_(void);
static void APP_NVM_BC_exec_(void);

/**
 * @brief 指定された BC を複数まとめて不揮発にコピーする
 * @param[in] begin_bc_id : 最初にコピーする BC の id
 * @param[in] num         : コピーする BC 数
 * @return void
 * @note アプリで定期実行される
 */
static void APP_NVM_BC_copy_bcs_(bct_id_t begin_bc_id, uint8_t num);

/**
 * @brief 指定された BC を不揮発にコピーする
 * @param[in] bc_id : コピーする BC の id
 * @param[in] write_address : コピー先の不揮発メモリアドレス
 * @note APP_NVM_BC_copy_bcs_ にて呼ばれる
 */
static APP_NVM_MANAGER_ERROR APP_NVM_BC_copy_bc_(bct_id_t bc_id, uint32_t write_address);

/**
 * @brief 指定された BC を１つ不揮発から復元する
 * @param[in] bc_id : 復元する BC の id
 * @return APP_NVM_MANAGER_ERROR
 * @note 地上局コマンドで実行される
 */
static APP_NVM_MANAGER_ERROR APP_NVM_BC_restore_bc_from_nvm_(bct_id_t bc_id);

/**
 * @brief 指定した BC が保存されている不揮発メモリ上のアドレスを返す
 * @param[in] bc_id
 * @return 不揮発メモリ上のアドレス
 */
static uint32_t APP_NVM_BC_get_address_from_bc_id_(bct_id_t bc_id);


AppInfo APP_NVM_BC_create_app(void)
{
  return AI_create_app_info("Non-volatile BC", APP_NVM_BC_init_, APP_NVM_BC_exec_);
}

static void APP_NVM_BC_init_(void)
{
  nvm_bc_.is_active = 0;  // 起動直後は無効化しておく
  nvm_bc_.bc_id_to_copy = 0;
  nvm_bc_.bc_num_to_copy = 1;  // 2 以上だと 3-3 アノマリが出るので基本 1 にする
  nvm_bc_.address_for_ready_flags = 0;  // パーティッション内でのアドレス
  nvm_bc_.address_for_bc = nvm_bc_.address_for_ready_flags + sizeof(nvm_bc_.is_ready_to_restore);  // パーティッション内でのアドレス

  // is_ready_to_restore を不揮発から復元する
  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_read_bytes(nvm_bc_.is_ready_to_restore,
                                                           APP_NVM_PARTITION_ID_BCT,
                                                           nvm_bc_.address_for_ready_flags,
                                                           sizeof(nvm_bc_.is_ready_to_restore));
  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    EL_record_event(EL_GROUP_NVM_BC, (uint32_t)APP_NVM_MANAGER_ERROR_RESTORE_READY_FLAG, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
  }

  return;
}

static void APP_NVM_BC_exec_(void)
{
  // 有効化するまでは何もしない
  if (!nvm_bc_.is_active) return;

  // 実行時間を考慮して、1回あたり nvm_bc_.bc_num_to_copy 個ずつコピーを取っていく
  APP_NVM_BC_copy_bcs_(nvm_bc_.bc_id_to_copy, nvm_bc_.bc_num_to_copy);

  if (nvm_bc_.bc_id_to_copy + nvm_bc_.bc_num_to_copy >= BCT_MAX_BLOCKS)
  {
    nvm_bc_.bc_id_to_copy = 0;

    // 1周したので is_ready_to_restore の魚拓も取る
    APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                                              nvm_bc_.address_for_ready_flags,
                                                              sizeof(nvm_bc_.is_ready_to_restore),
                                                              nvm_bc_.is_ready_to_restore);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      EL_record_event(EL_GROUP_NVM_BC, (uint32_t)APP_NVM_MANAGER_ERROR_COPY_READY_FLAG, EL_ERROR_LEVEL_LOW, (uint32_t)ret);
    }
  }
  else
  {
    nvm_bc_.bc_id_to_copy += nvm_bc_.bc_num_to_copy;
  }

  return;
}

static void APP_NVM_BC_copy_bcs_(bct_id_t begin_bc_id, uint8_t num)
{
  for (uint8_t i = 0; i < num; ++i)
  {
    bct_id_t bc_id = begin_bc_id + i;
    if (bc_id >= BCT_MAX_BLOCKS)
    {
      // bc_num_to_copy が BCT_MAX_BLOCKS の約数になっていないと最終周回でここに到達する
      // ただ, 動作に問題があるわけではないので EL 登録せずに return する
      return;
    }

    nvm_bc_.is_ready_to_restore[bc_id] = 0;

    // 有効化されている BC のみコピーする
    if (!BCE_is_active(bc_id)) continue;

    uint32_t write_address = APP_NVM_BC_get_address_from_bc_id_(bc_id);
    if (write_address < nvm_bc_.address_for_bc)
    {
      EL_record_event(EL_GROUP_NVM_BC, (uint32_t)APP_NVM_MANAGER_ERROR_NG_ADDRESS_NVM_BC, EL_ERROR_LEVEL_LOW, (uint32_t)bc_id);
      break;
    }

    // BC をコピー
    APP_NVM_MANAGER_ERROR ret = APP_NVM_BC_copy_bc_(bc_id, write_address);

    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      EL_record_event(EL_GROUP_NVM_BC, (uint32_t)ret, EL_ERROR_LEVEL_LOW, bc_id);
    }
    else
    {
      // 正常にコピーできた場合のみ ready flag を立てる
      nvm_bc_.is_ready_to_restore[bc_id] = 1;
    }
  }

  return;
}

static APP_NVM_MANAGER_ERROR APP_NVM_BC_copy_bc_(bct_id_t bc_id, uint32_t write_address)
{
  uint32_t current_addr = write_address;
  uint8_t len = block_command_table->blocks[bc_id]->length;

  // BC のコマンド数をコピー
  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                                            current_addr,
                                                            sizeof(len),
                                                            &len);
  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    EL_record_event(EL_GROUP_NVM_BC, APP_NVM_MANAGER_ERROR_COPY_BC_LEN, EL_ERROR_LEVEL_LOW, (uint32_t)ret);
    return ret;
  }
  current_addr = current_addr + sizeof(len);

  // BC のコマンドを1つずつコピーしていく
  for (uint8_t i = 0; i < len; ++i)
  {
    BCT_CmdData cmd = block_command_table->blocks[bc_id]->cmds[i];
    APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                                              current_addr,
                                                              sizeof(BCT_CmdData),
                                                              (uint8_t*)&cmd);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      EL_record_event(EL_GROUP_NVM_BC, APP_NVM_MANAGER_ERROR_COPY_BC_CMD, EL_ERROR_LEVEL_LOW, (uint32_t)ret);
      return ret;
    }
    current_addr = current_addr + sizeof(BCT_CmdData);
  }

  return APP_NVM_MANAGER_ERROR_OK;
}

static APP_NVM_MANAGER_ERROR APP_NVM_BC_restore_bc_from_nvm_(bct_id_t bc_id)
{
  if (!nvm_bc_.is_ready_to_restore[bc_id]) return APP_NVM_MANAGER_ERROR_NOT_READY_TO_RESTORE;

  uint32_t current_addr = APP_NVM_BC_get_address_from_bc_id_(bc_id);
  if (current_addr < nvm_bc_.address_for_bc) return APP_NVM_MANAGER_ERROR_NG_ADDRESS_NVM_BC;

  BCT_clear_block(bc_id);

  // BC の長さを復元
  uint8_t len;
  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_read_bytes(&len,
                                                           APP_NVM_PARTITION_ID_BCT,
                                                           current_addr,
                                                           sizeof(len));
  if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

  current_addr = current_addr + sizeof(len);

  // BC のコマンドを 1つずつ復元
  CommonCmdPacket packet;
  for (uint8_t i = 0; i < len; ++i)
  {
    APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_read_bytes(packet.packet,
                                                             APP_NVM_PARTITION_ID_BCT,
                                                             current_addr,
                                                             sizeof(BCT_CmdData));
    if (ret != APP_NVM_MANAGER_ERROR_OK) return ret;

    // 復元したコマンドを BCT に登録
    BCT_ACK ack = BCT_register_cmd(&packet);
    if (ack != BCT_SUCCESS) return APP_NVM_MANAGER_ERROR_BCT_FAIL;

    current_addr = current_addr + sizeof(BCT_CmdData);
  }

  // 全て問題なく復元できた場合、有効化する
  BCE_activate_block_by_id(bc_id);

  return APP_NVM_MANAGER_ERROR_OK;
}

static uint32_t APP_NVM_BC_get_address_from_bc_id_(bct_id_t bc_id)
{
  if (bc_id >= BCT_MAX_BLOCKS)
  {
    return 0;
  }

  // 余裕を持たせて sizeof(BCT_Table) ずつ確保する
  return nvm_bc_.address_for_bc + sizeof(BCT_Table) * bc_id;
}

CCP_CmdRet Cmd_APP_NVM_BC_SET_ENABLE(const CommonCmdPacket* packet)
{
  uint8_t is_active = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (is_active > 0)
  {
    nvm_bc_.is_active = 1;

    // ready flag を全て下ろす
    memset(nvm_bc_.is_ready_to_restore, 0, sizeof(nvm_bc_.is_ready_to_restore));
    APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                                              nvm_bc_.address_for_ready_flags,
                                                              sizeof(nvm_bc_.is_ready_to_restore),
                                                              nvm_bc_.is_ready_to_restore);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, ret);
    }
  }
  else
  {
    nvm_bc_.is_active = 0;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_NVM_BC_RESTORE_BC_FROM_NVM(const CommonCmdPacket* packet)
{
  bct_id_t bc_id = CCP_get_param_from_packet(packet, 0, bct_id_t);

  if (bc_id >= BCT_MAX_BLOCKS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  APP_NVM_MANAGER_ERROR ret = APP_NVM_BC_restore_bc_from_nvm_(bc_id);

  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, ret);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

// 軌道上で使うことは想定していないが、念のため構造体のメンバ変数をいじれるようにしておく
CCP_CmdRet Cmd_APP_NVM_BC_OTHER_SETTINGS(const CommonCmdPacket* packet)
{
  uint8_t idx = CCP_get_param_from_packet(packet, 0, uint8_t);
  uint32_t value = CCP_get_param_from_packet(packet, 1, uint32_t);

  switch (idx)
  {
  case 0:
    if (value >= BCT_MAX_BLOCKS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    nvm_bc_.bc_id_to_copy = (bct_id_t)value;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case 1:
    if (value > 255) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    nvm_bc_.bc_num_to_copy = (uint8_t)value;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case 2:
    nvm_bc_.address_for_ready_flags = value;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case 3:
    nvm_bc_.address_for_bc = value;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
}

#pragma section
