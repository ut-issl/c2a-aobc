/**
* @file
* @brief MOBC の DI. MOBC における GS との DI に相当する
*/
#ifndef DI_MOBC_H_
#define DI_MOBC_H_

#include <src_user/Drivers/Etc/mobc.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

extern const MOBC_Driver* const mobc_driver;           //!< MOBC driver

/**
 * @brief  MOBC updateアプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_MOBC_update(void);

/**
 * @brief  MOBC RT TLM (Realtime telemetry) PH (packet handler) アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_MOBC_rt_tlm_packet_handler(void);

#endif
