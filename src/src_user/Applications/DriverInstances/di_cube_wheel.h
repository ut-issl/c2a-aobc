/**
* @file   di_cubewheel.h
* @brief  CUBEWHEELのDriver Instance
*/
#ifndef DI_CUBEWHEEL_H_
#define DI_CUBEWHEEL_H_

#include "../../Drivers/ADCS/cubewheel.h"

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_user/Settings/CmdTlm/common_tlm_cmd_packet_define.h>
#include <src_core/CmdTlm/common_tlm_cmd_packet.h>
#include "../UserDefined/AOCS/aocs_error.h"

typedef enum
{
  CUBEWHEEL_IDX_ON_X = 0,
  CUBEWHEEL_IDX_ON_Y,
  CUBEWHEEL_IDX_ON_Z,
  CUBEWHEEL_IDX_MAX,
} CUBEWHEEL_IDX;

extern const CUBEWHEEL_Driver* const cubewheel_driver[CUBEWHEEL_IDX_MAX];

/**
 * @brief  CUBEWHEEL定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_CUBEWHEEL_update(void);

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_Enable(const CommonCmdPacket* packet); // 旧 RW_startup
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_Init(const CommonCmdPacket* packet); // 旧 RW_startup+enable
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_EndProcess(const CommonCmdPacket* packet);


CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetSpeed(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetDutyCycle(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetControlMode(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetPwmGain(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetMainGain(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_GetStatus(const CommonCmdPacket* packet);


CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetBackupmode(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_EnableEncoder(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_CUBEWHEEL_EnableHallsensor(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SET_ROTATION_DIRECTION_VECTOR(const CommonCmdPacket* packet);

int DI_CUBEWHEEL_set_torque_Nm(CUBEWHEEL_IDX idx, float torque_rw_Nm);
int DI_CUBEWHEEL_set_speed_rpm(CUBEWHEEL_IDX idx, float speed_rw_rpm);

#endif
