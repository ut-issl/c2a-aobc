/**
* @file   magnetometer_selector.h
* @brief  複数ある磁気センサ出力を組み合わせて一つの観測磁場ベクトルを計算するアプリ
*/
#ifndef MAGNETOMETER_SELECTOR_H_
#define MAGNETOMETER_SELECTOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../../Library/stdint.h"

/**
 * @enum   APP_MAG_SELECTOR_STATE
 * @brief  どの磁気センサを選んでいるかなどの状態を表す
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_MAG_SELECTOR_STATE_RM_AOBC = 0,   //!< AOBC上のRM3100を使う
  APP_MAG_SELECTOR_STATE_RM_EXT,        //!< 外部のRM3100を使う
  APP_MAG_SELECTOR_STATE_MPU,           //!< MPU9250を使う
  APP_MAG_SELECTOR_STATE_FUSION,        //!< 色々加味して合成する
  APP_MAG_SELECTOR_STATE_MAX
} APP_MAG_SELECTOR_STATE;

/**
 * @struct MagnetmeterSelector
 * @brief 磁気センサ統合に関する外部公開情報用構造体
 */
typedef struct
{
  APP_MAG_SELECTOR_STATE state;
  uint8_t auto_flag; //!< 0以外なら電源状態などを加味してどれを使うか自動選択する
} MagnetometerSelector;

extern const MagnetometerSelector* const magnetometer_selector;

AppInfo APP_MAG_SELECTOR_create_app(void);

CCP_EXEC_STS Cmd_APP_MAG_SELECTOR_SET_STATE(const CommonCmdPacket* packet);


#endif
