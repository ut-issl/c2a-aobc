/**
* @file
* @brief BCT のコピーを不揮発メモリに保存する
* @note 使い方は .c を参照
*/
#ifndef NON_VOLATILE_MEMORY_BC_MANAGER_H_
#define NON_VOLATILE_MEMORY_BC_MANAGER_H_

#include "non_volatile_memory_partition.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/block_command_table.h>

/**
 * @enum  APP_NVBC_MGR_ERROR
 * @brief エラー用enum
 * @note  APP_NVM_MANAGER_ERROR の値を共用したいので値続きにしている
 */
typedef enum
{
  APP_NVBC_MGR_ERR_OK = 0,
  APP_NVBC_MGR_ERR_BCT_COPY_FAIL = APP_NVM_MANAGER_ERROR_NG_OTHERS + 1,  //!< BCT の関数でエラー発生
  APP_NVBC_MGR_ERR_NOT_READY_TO_RESTORE,                                 //!< 復元できる状態ではない
  APP_NVBC_MGR_ERR_INVALID_ADDRESS,                                      //!< アドレスがおかしい
  APP_NVBC_MGR_ERR_COPY_READY_FLAG,                                      //!< ready flag のコピーに失敗
  APP_NVBC_MGR_ERR_RESTORE_READY_FLAG                                    //!< ready flag の復元に失敗
} APP_NVBC_MGR_ERROR;

/**
 * @struct
 * @brief 不揮発BC管理アプリの構造体
 */
typedef struct
{
  uint8_t is_active;
  uint8_t is_ready_to_restore[BCT_MAX_BLOCKS]; // 各 BC が復元できる状態かどうか
  bct_id_t bc_id_to_copy;                      // 次にコピーする BC の ID
  uint8_t bc_num_to_copy;                      // 一度に何個の BC をコピーするか
  uint32_t address_for_ready_flags;            // is_ready_to_restore を保存している領域の開始アドレス
  uint32_t address_for_bc;                     // BCT を保存している領域の開始アドレス
} NonVolatileBCManager;

extern const NonVolatileBCManager* const nv_bc_manager;

AppInfo APP_NVBC_MGR_create_app(void);

CCP_CmdRet Cmd_APP_NVBC_MGR_SET_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVBC_MGR_RESTORE_BC_FROM_NVM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVBC_MGR_OTHER_SETTINGS(const CommonCmdPacket* packet);

#endif
