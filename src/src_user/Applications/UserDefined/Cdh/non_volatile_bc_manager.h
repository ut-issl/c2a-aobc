/**
* @file
* @brief BCT のコピーを FRAM にとって不揮発化する
*/
#ifndef NON_VOLATILE_BC_MANAGER_H_
#define NON_VOLATILE_BC_MANAGER_H_

#include <src_core/System/ApplicationManager/app_info.h>

#include "non_volatile_memory_partition.h"

/**
 * @struct
 * @brief 不揮発BC管理アプリの構造体
 */
typedef struct
{
  uint8_t is_active;
  bct_id_t bc_id_to_copy;  // 次にコピーする BC の ID
  uint8_t bc_num_to_copy;  // 一度に何個の BC をコピーするか
} NonVolatileBCManager;

extern const NonVolatileBCManager* const nv_bc_manager;

AppInfo APP_NVBC_MGR_create_app(void);

CCP_CmdRet Cmd_APP_NVBC_MGR_SET_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVBC_MGR_RESTORE_FROM_NVM(const CommonCmdPacket* packet);

#endif
