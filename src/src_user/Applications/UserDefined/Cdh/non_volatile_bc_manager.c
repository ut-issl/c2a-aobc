#pragma section REPRO
/**
* @file
* @brief BCT のコピーを FRAM にとって不揮発化する
*/

#include "non_volatile_bc_manager.h"

#include <string.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_user/TlmCmd/block_command_user_settings.h>

static NonVolatileBCManager nv_bc_manager_;
const  NonVolatileBCManager* const nv_bc_manager = &nv_bc_manager_;

static void  APP_NVBC_MGR_init_(void);
static void  APP_NVBC_MGR_exec_(void);


AppInfo APP_NVBC_MGR_create_app(void)
{
  return AI_create_app_info("AOCS Data Recorder", APP_NVBC_MGR_init_, APP_NVBC_MGR_exec_);
}

static void APP_NVBC_MGR_init_(void)
{
  nv_bc_manager_.is_active = 0; // 起動直後は無効化しておく
  nv_bc_manager_.bc_id_to_copy = 0;
  nv_bc_manager_.bc_num_to_copy = 10;

  return;
}

static void APP_NVBC_MGR_exec_(void)
{
  // 有効化するまではコピーしない
  if (!nv_bc_manager_.is_active) return;

  // データ書き込み
  APP_NVBC_MGR_copy_bc_(nv_bc_manager_.bc_id_to_copy, nv_bc_manager_.bc_num_to_copy);

  // 次にコピーする BC の ID を更新
  if (nv_bc_manager_.bc_id_to_copy + nv_bc_manager_.bc_num_to_copy >= BCT_MAX_BLOCKS)
  {
    nv_bc_manager_.bc_id_to_copy = 0;
  }
  else
  {
    nv_bc_manager_.bc_id_to_copy += nv_bc_manager_.bc_num_to_copy;
  }

  return;
}

static void APP_NVBC_MGR_copy_bc_(bct_id_t begin_bc_id, uint8_t len)
{
  uint32_t start_address = non_volatile_memory_partition->elements[APP_NVM_PARTITION_ID_BCT].start_address;
  uint32_t write_address;
  bct_id_t bc_id;
  BCT_Table block;
  APP_NVM_MANAGER_ERROR ret;

  for (uint8_t i = 0; i < len; ++i)
  {
    bc_id = begin_bc_id + i;
    block = *block_command_table->blocks[bc_id];  // TODO: アサーション入れる
    write_address = start_address + sizeof(BCT_Table) * i;  // TODO: bc_id を入力すると address が出力される関数を作る

    ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                        write_address,
                                        sizeof(BCT_Table),
                                        &block);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      EL_record_event(EL_GROUP_NVBC_WRITE, (uint32_t)ret, EL_ERROR_LEVEL_LOW, bc_id);
    }
  }

  return;
}

static void APP_NVBC_MGR_recover_bc_from_nvm_()
{
  // aaa
}

CCP_CmdRet Cmd_APP_NVBC_MGR_SET_ENABLE(const CommonCmdPacket* packet)
{
  uint8_t flag = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (flag > 0)
  {
    nv_bc_manager_.is_active = 1;
  }
  else
  {
    nv_bc_manager_.is_active = 0;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
