/**
* @file   gyro_selector.h
* @brief  複数あるジャイロセンサ出力を組み合わせて一つの観測角速度ベクトルを計算するアプリ
*/
#ifndef GYRO_SELECTOR_H_
#define GYRO_SELECTOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../../Library/stdint.h"

/**
 * @enum   APP_GYRO_SELECTOR_STATE
 * @brief  どのジャイロセンサを選んでいるかなどの状態を表す
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_GYRO_SELECTOR_STATE_MPU = 0,   //!< MPU9250を使う
  APP_GYRO_SELECTOR_STATE_STIM,      //!< STIM210を使う
  APP_GYRO_SELECTOR_STATE_FUSION,    //!< 色々加味して合成する
  APP_GYRO_SELECTOR_STATE_MAX,
} APP_GYRO_SELECTOR_STATE;

/**
 * @struct GyroSelector
 * @brief ジャイロセンサ統合に関する外部公開情報用構造体
 */
typedef struct
{
  APP_GYRO_SELECTOR_STATE state;
  uint8_t auto_flag; //!< 0以外なら電源状態などを加味してどれを使うか自動選択する
} GyroSelector;

extern const GyroSelector* const gyro_selector;

AppInfo APP_GYRO_SELECTOR_create_app(void);

CCP_CmdRet Cmd_APP_GYRO_SELECTOR_SET_STATE(const CommonCmdPacket* packet);

#endif
