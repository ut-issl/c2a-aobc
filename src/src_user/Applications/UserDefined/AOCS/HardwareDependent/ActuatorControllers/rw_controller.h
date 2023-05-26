/**
* @file   rw_controller.h
* @brief  aocs_managerの目標トルク情報から，RWの出力トルクを計算・設定するアプリ
*/

#ifndef RW_CONTROLLER_H_
#define RW_CONTROLLER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../aocs_manager.h"
#include "../../../../DriverInstances/di_rw0003.h"

AppInfo APP_RW_CONTROLLER_create_app(void);

#endif
