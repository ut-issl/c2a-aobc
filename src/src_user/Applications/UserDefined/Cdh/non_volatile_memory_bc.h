/**
* @file
* @brief BCT のコピーを不揮発メモリに保存する
* @note 使い方は .c を参照
*/
#ifndef NON_VOLATILE_MEMORY_BC_H_
#define NON_VOLATILE_MEMORY_BC_H_

#include "non_volatile_memory_partition.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/block_command_table.h>


/**
 * @struct
 * @brief 不揮発BC管理アプリの構造体
 */
typedef struct
{
  uint8_t is_active;                           //!< アプリが有効かどうか
  uint8_t is_ready_to_restore[BCT_MAX_BLOCKS]; //!< 各 BC が復元できる状態かどうか
  bct_id_t bc_id_to_copy;                      //!< 次にコピーする BC の ID
  uint8_t bc_num_to_copy;                      /*!< 一度に何個の BC をコピーするか
                                                    必ず BCT_MAX_BLOCKS の約数にすること！！ */
  uint32_t address_for_ready_flags;            //!< is_ready_to_restore を保存している領域の開始アドレス
  uint32_t address_for_bc;                     //!< BCT を保存している領域の開始アドレス
} NonVolatileMemoryBc;

extern const NonVolatileMemoryBc* const nvm_bc;

AppInfo APP_NVM_BC_create_app(void);

CCP_CmdRet Cmd_APP_NVM_BC_SET_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVM_BC_RESTORE_BC_FROM_NVM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVM_BC_OTHER_SETTINGS(const CommonCmdPacket* packet);

#endif
