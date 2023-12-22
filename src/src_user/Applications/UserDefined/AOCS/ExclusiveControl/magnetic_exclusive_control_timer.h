/**
* @file   magnetic_exclusive_control_timer.h
* @brief  MTQ-磁気センサ排他制御タイマー
*/

#ifndef MAGNETIC_EXCLUSIVE_CONTROL_TIMER_H_
#define MAGNETIC_EXCLUSIVE_CONTROL_TIMER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/Library/physical_constants.h>

/**
 * @enum  APP_MECT_STATE
 * @brief 排他制御の現在の状態
 * @note  状態は観測 --> 制御 --> スタンバイ --> 観測 --> ... と切り替わる
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
  uint32_t observe_duration_ms; //!< 磁気センサ観測状態の持続時間
  uint32_t control_duration_ms; //!< MTQ出力状態の持続時間
  uint32_t standby_duration_ms; //!< スタンバイ状態の持続時間
} MagneticExclusiveControlTimerConfig;

/**
 * @struct MagneticExclusiveControlTimer
 * @brief  排他制御に必要な情報を管理する構造体
*/
typedef struct
{
  ObcTime  previous_obc_time;   //!< 前回のアプリ実行時刻
  APP_MECT_STATE current_state; //!< 現在の排他制御状態
  uint32_t state_timer_ms;      //!< 現在の排他制御状態に入ってから経過した時間
  MagneticExclusiveControlTimerConfig config;          //!< 排他制御の時間設定
  MagneticExclusiveControlTimerConfig buffered_config; //!< 排他制御の時間設定のバッファ
} MagneticExclusiveControlTimer;

extern const MagneticExclusiveControlTimer* const magnetic_exclusive_control_timer;

/*
* @brief  MTQ-磁気センサ排他制御タイマーの時間設定
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
*/
CCP_CmdRet Cmd_APP_MAGNETIC_EXCLUSIVE_CONTROL_TIMER_SET_DURATION(const CommonCmdPacket* packet);

AppInfo APP_MECt_create_app(void);

#endif
