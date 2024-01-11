/**
* @file   magnetic_exclusive_control_timer.h
* @brief  MTQ-磁気センサ排他制御タイマー
*/

#ifndef MAGNETIC_EXCLUSIVE_CONTROL_TIMER_H_
#define MAGNETIC_EXCLUSIVE_CONTROL_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/Library/physical_constants.h>

/**
 * @enum  APP_MECT_EXCLUSIVE_CONTROL
 * @brief 排他制御が有効か無効か
 * @note  uint8_tを想定
*/
typedef enum
{
  APP_MECT_EXCLUSIVE_CONTROL_DISABLE = 0, //!< 排他制御を無効化する
  APP_MECT_EXCLUSIVE_CONTROL_ENABLE       //!< 排他制御を有効化する
} APP_MECT_EXCLUSIVE_CONTROL;

/**
 * @enum  APP_MECT_STATE
 * @brief 排他制御の現在の状態
 * @note  状態は観測 --> 制御 --> スタンバイ --> 観測 --> ... と切り替わる
 *        排他制御が無効であるときは、常に制御状態となる
 *        App間の結合パスを単純化するため、構造体変数はAOCS managerに持たせる
 * @note  uint8_tを想定
*/
typedef enum
{
  APP_MECT_STATE_OBSERVE = 0, //!< 磁気センサで磁場観測する
  APP_MECT_STATE_CONTROL,     //!< MTQ出力によって姿勢制御する
  APP_MECT_STATE_STANDBY      //!< 磁気センサの読みからMTQ出力由来のノイズの影響が抜けるのを待つ
} APP_MECT_STATE;

/**
 * @struct MagneticExclusiveControlTimerConfig
 * @brief  排他制御の時間設定
*/
typedef struct
{
  uint16_t observe_duration_ms; //!< 磁気センサ観測状態の持続時間
  uint16_t control_duration_ms; //!< MTQ出力状態の持続時間
  uint16_t standby_duration_ms; //!< スタンバイ状態の持続時間
} MagneticExclusiveControlTimerConfig;

/**
 * @struct MagneticExclusiveControlTimer
 * @brief  排他制御に必要な情報を管理する構造体
*/
typedef struct
{
  ObcTime  previous_obc_time;           //!< 前回のアプリ実行時刻
  uint16_t state_timer_ms;              //!< 現在の排他制御状態に入ってから経過した時間
  APP_MECT_EXCLUSIVE_CONTROL is_enable; //!< 排他制御が有効か無効か
  MagneticExclusiveControlTimerConfig config;          //!< 排他制御の時間設定
  MagneticExclusiveControlTimerConfig buffered_config; //!< 排他制御の時間設定のバッファ
  bool is_config_buffered;                             //!< バッファに設定があるかどうか
} MagneticExclusiveControlTimer;

extern const MagneticExclusiveControlTimer* const magnetic_exclusive_control_timer;

/**
 * @brief  MTQ-磁気センサ排他制御を有効化・無効化する
 * @param  CommonCmdPacket
 * @return CCP_EXEC_STSに準拠
*/
CCP_CmdRet Cmd_APP_MAGNETIC_EXCLUSIVE_CONTROL_TIMER_SET_ENABLE(const CommonCmdPacket* packet);

/**
* @brief  MTQ-磁気センサ排他制御タイマーの時間設定
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
*/
CCP_CmdRet Cmd_APP_MAGNETIC_EXCLUSIVE_CONTROL_TIMER_SET_DURATION(const CommonCmdPacket* packet);

AppInfo APP_MECT_create_app(void);

#endif
