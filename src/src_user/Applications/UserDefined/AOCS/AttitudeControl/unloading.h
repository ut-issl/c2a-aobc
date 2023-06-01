/**
* @file   unloading.h
* @brief  アンローディングのためのアプリケーション
*/
#ifndef UNLOADING_H_
#define UNLOADING_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../Library/physical_constants.h"
#include "../../../../Library/ControlUtility/cross_product_control.h"
#include "../aocs_manager.h"

/**
 * @enum   APP_UNLOADING_STATE
 * @brief  そのホイールが外力トルク入力によるアンローディング中であるかどうか
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_UNLOADING_STATE_OFF = 0,
  APP_UNLOADING_STATE_ON_INCREASE,
  APP_UNLOADING_STATE_ON_DECREASE
} APP_UNLOADING_STATE;

/**
 * @enum   APP_UNLOADING_EXEC
 * @brief  アンローディングを実行するかどうか
 *
 *         地上局ではじめてRW運用をするときは，アンローディングが入ってRWの動作確認がしにくくなったりしないようにDISABLEにしておく
 *         うまく動いていることを確認したら，以降は基本的にENABLEのままで使うことを想定
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_UNLOADING_EXEC_ENABLE = 0,
  APP_UNLOADING_EXEC_DISABLE = 1
} APP_UNLOADING_EXEC;

/**
 * @struct Unloading
 * @brief  アンローディング制御に必要な情報を管理するための構造体
 */
typedef struct
{
  float angular_velocity_upper_threshold_rad_s;           //!< アンローディングを始める上側RW回転角速度．これを上回ったらRW回転数を減らす
  float angular_velocity_lower_threshold_rad_s;           //!< アンローディングを終わるRW回転角速度．これを下回ったらRW回転数を増やす
  float angular_velocity_target_rad_s;                    //!< アンローディング目標角速度
  float control_gain;                                     //!< RWの余剰回転数に対するMTQ出力トルクのゲイン．負の値を想定
  APP_UNLOADING_EXEC exec_is_enable;                      //!< アンローディング処理が有効であるかどうか
  APP_UNLOADING_STATE unloading_state[RW0003_IDX_MAX];    //!< 各ホイールがアンローディング中であるかどうか
  float output_torque_body_Nm[PHYSICAL_CONST_THREE_DIM];  //!< アンローディングによる出力目標トルク
  uint8_t number_of_unloading_wheels;                     //!< アンローディング中のホイールの個数
  CrossProductControl cross_product_cntrl;                //!< CrossProduct制御インスタンス
} Unloading;

extern const Unloading* const unloading;

/**
 * @brief  アンローディング定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_UNLOADING_create_app(void);

CCP_CmdRet Cmd_APP_UNLOADING_SET_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_UNLOADING_SET_ANGULAR_VEROCITY_THRESHOLD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_UNLOADING_SET_CONTROL_GAIN(const CommonCmdPacket* packet);


#endif
