/**
* @file   sgp4_orbit_propagator.h
* @brief  SGP4軌道伝搬を行うアプリ
*/
#ifndef SGP4_ORBIT_PROPAGATOR_H_
#define SGP4_ORBIT_PROPAGATOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/Orbit/sgp4.h>

/**
 * @struct Sgp4OrbitPropagator
 * @brief  SGP4軌道伝搬関連テレメトリ
 */
typedef struct
{
  TleInput tle_input;         //!< TLE入力
  TleInput tle_input_cmd_tmp; //!< コマンドで設定途中のTLE入力
  Sgp4Constants constants;    //!< SGP4定数
} Sgp4OrbitPropagator;

extern const Sgp4OrbitPropagator* const sgp4_orbit_propagator;

AppInfo APP_SGP4_create_app(void);

CCP_CmdRet Cmd_APP_SGP4_SET_TLE_LINE1(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_SGP4_SET_TLE_LINE2(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_SGP4_SAVE_TLE(const CommonCmdPacket* packet);

#endif
