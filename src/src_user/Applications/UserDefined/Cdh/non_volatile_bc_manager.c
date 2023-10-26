#pragma section REPRO
/**
* @file
* @brief BCT のコピーを FRAM にとって不揮発化する
*/

#include "non_volatile_bc_manager.h"
#include "non_volatile_memory_partition.h"

#include <string.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>

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
  nv_bc_manager_.start_address = non_volatile_memory_partition->elements[APP_NVM_PARTITION_ID_BCT].start_address;

  return;
}

static void APP_NVBC_MGR_exec_(void)
{
  // 有効化するまではコピーしない
  if (!nv_bc_manager_.is_active) return;

  // FRAM に BCT をコピー
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
  uint32_t write_address;
  bct_id_t bc_id;
  BCT_Table block;
  APP_NVM_MANAGER_ERROR ret;

  for (uint8_t i = 0; i < len; ++i)
  {
    bc_id = begin_bc_id + i;
    block = *block_command_table->blocks[bc_id];  // TODO: アサーション入れる
    write_address = APP_NVBC_MGR_get_address_from_bc_id_(bc_id);

    ret = APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID_BCT,
                                        write_address,
                                        sizeof(BCT_Table),
                                        &block);
    if (ret != APP_NVM_MANAGER_ERROR_OK)
    {
      EL_record_event(EL_GROUP_NV_BC_MGR, (uint32_t)ret, EL_ERROR_LEVEL_LOW, bc_id);
    }
  }

  return;
}

static void APP_NVBC_MGR_restore_bc_from_nvm_(bct_id_t bc_id)
{
  BCT_Table block;
  APP_NVM_MANAGER_ERROR ret;
  uint32_t read_address = APP_NVBC_MGR_get_address_from_bc_id_(bc_id);

  ret = APP_NVM_PARTITION_read_bytes(&block,
                                     APP_NVM_PARTITION_ID_BCT,
                                     read_address,
                                     sizeof(BCT_Table));
  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    EL_record_event(EL_GROUP_NV_BC_MGR, (uint32_t)ret, EL_ERROR_LEVEL_HIGH, bc_id);
  }

  memcpy(block_command_table->blocks[bc_id], &block, sizeof(BCT_Table));  // FIXME: BCT は static const 確保されているので無理な気がする

  return;
}

static uint32_t APP_NVBC_MGR_get_address_from_bc_id_(bct_id_t bc_id)
{
  uint32_t write_address;

  if (bc_id >= BCT_MAX_BLOCKS)
  {
    return nv_bc_manager_.start_address;
  }

  write_address = nv_bc_manager_.start_address + sizeof(BCT_Table) * bc_id;

  return write_address;
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

CCP_CmdRet Cmd_APP_NVBC_MGR_RESTORE_BC_FROM_NVM(const CommonCmdPacket* packet)
{
  bct_id_t bc_id = CCP_get_param_from_packet(packet, 0, bct_id_t);

  APP_NVBC_MGR_restore_bc_from_nvm_(bc_id);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
