/**
* @file   di_cube_wheel.h
* @brief  CUBE_WHEELのDriver Instance
*/
#ifndef DI_CUBE_WHEEL_H_
#define DI_CUBE_WHEEL_H_

#include "../../Drivers/ADCS/cube_wheel.h"

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_user/Settings/CmdTlm/common_tlm_cmd_packet_define.h>
#include <src_core/CmdTlm/common_tlm_cmd_packet.h>
#include "../UserDefined/AOCS/aocs_error.h"

typedef enum
{
  CUBE_WHEEL_IDX_ON_X = 0,
  CUBE_WHEEL_IDX_ON_Y,
  CUBE_WHEEL_IDX_ON_Z,
  CUBE_WHEEL_IDX_MAX,
} CUBE_WHEEL_IDX;

extern const CUBE_WHEEL_Driver* const cube_wheel_driver[CUBE_WHEEL_IDX_MAX];

/**
 * @brief  CUBE_WHEEL定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_CUBE_WHEEL_update(void);

CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_Enable(const CommonCmdPacket* packet); // 旧 RW_startup
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_Init(const CommonCmdPacket* packet); // 旧 RW_startup+enable
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_EndProcess(const CommonCmdPacket* packet);


CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_SetSpeed(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_SetDutyCycle(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_SetControlMode(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_SetPwmGain(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_SetMainGain(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_GetStatus(const CommonCmdPacket* packet);


CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_SetBackupmode(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_EnableEncoder(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_EnableHallsensor(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_DI_CUBE_WHEEL_SET_ROTATION_DIRECTION_VECTOR(const CommonCmdPacket* packet);

int DI_CUBE_WHEEL_set_torque_Nm(CUBE_WHEEL_IDX idx, float torque_rw_Nm);
int DI_CUBE_WHEEL_set_speed_rpm(CUBE_WHEEL_IDX idx, float speed_rw_rpm);

#endif
