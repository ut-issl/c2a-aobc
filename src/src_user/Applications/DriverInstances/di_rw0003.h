/**
* @file   di_rw0003.h
* @brief  RW0003のDriver Instance
*/
#ifndef DI_RW0003_H_
#define DI_RW0003_H_

#include "../../Drivers/Aocs/rw0003.h"
#include "../UserDefined/AOCS/aocs_error.h"
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  RW0003_IDX
 * @brief 複数搭載されているRW0003の番号管理
 * @note  型はint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  RW0003_IDX_ON_X = 0,
  RW0003_IDX_ON_Y,
  RW0003_IDX_ON_Z,
  RW0003_IDX_MAX,
} RW0003_IDX;

/**
 * @enum  RW0003_EL_NOTE
 * @brief RW0003に関連するELの詳細
 * @note  型はuint8_tを想定
 */
typedef enum
{
  RW0003_EL_NOTE_CRC_ERROR = 0,
  RW0003_EL_NOTE_NOISE_REMOVED,
  RW0003_EL_NOTE_TORQUE_CMD,
  RW0003_EL_NOTE_MAX,
} RW0003_EL_NOTE;

extern const RW0003_Driver* const rw0003_driver[RW0003_IDX_MAX];

/**
 * @brief  RW0003定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_RW0003_update(void);

// Setter
AOCS_ERROR DI_RW0003_set_torque_Nm(const RW0003_IDX idx, const float torque_Nm);
AOCS_ERROR DI_RW0003_set_speed_rad_s(const RW0003_IDX idx, const float speed_rad_s);

// コマンド関数
CCP_EXEC_STS Cmd_DI_RW0003_INIT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_RW0003_SET_ROTATION_DIRECTION_VECTOR(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_RW0003_SET_IDLE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_RW0003_DRIVE_TORQUE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_RW0003_DRIVE_SPEED(const CommonCmdPacket* packet);

#endif
