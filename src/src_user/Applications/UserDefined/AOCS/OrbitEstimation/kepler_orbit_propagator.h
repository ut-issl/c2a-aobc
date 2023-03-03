/**
* @file   kepler_orbit_propagator.h
* @brief  ケプラー軌道の伝搬を行うアプリ
*/
#ifndef KEPLER_ORBIT_PROPAGATOR_H_
#define KEPLER_ORBIT_PROPAGATOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../Library/Orbit/kepler_orbit.h"

/**
 * @struct KeplerOrbitPropagator
 * @brief  ケプラー軌道伝搬を行うためのテレメトリ
 */
typedef struct
{
  KeplerOrbitalElements orbital_elements;         //!< 軌道要素
  KeplerOrbitalElements orbital_elements_cmd_tmp; //!< コマンドで設定途中の軌道要素
  KeplerOrbitConstants  orbit_constants;          //!< 軌道要素から計算される定数
} KeplerOrbitPropagator;

extern const KeplerOrbitPropagator* const kepler_orbit_propagator;

AppInfo APP_KOP_create_app(void);

CCP_EXEC_STS Cmd_APP_KOP_SET_ORBIT_ELEMENTS(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_APP_KOP_SAVE_ORBIT_ELEMENTS(const CommonCmdPacket* packet);

#endif
