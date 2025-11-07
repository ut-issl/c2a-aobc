/**
* @file    fine_three_axis_determination.h
* @brief   精三軸姿勢決定アプリ
*
*          将来的にここでEKF関数も呼ぶことを想定して作る
*/
#ifndef FINE_THREE_AXIS_DETERMINATION_H_
#define FINE_THREE_AXIS_DETERMINATION_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/System/TimeManager/time_manager.h>

/**
* @enum  FINE_THREE_AXIS_DETERMINATION_METHOD
* @brief 使用する三軸姿勢決定の手法
* @note  uint_8を想定
*/
typedef enum
{
  APP_FTAD_METHOD_STT = 0, //!< STT出力を直接使って姿勢決定する
  APP_FTAD_METHOD_EKF,     //!< EKFを使って姿勢決定する
  APP_FTAD_METHOD_MAX
} APP_FTAD_METHOD;

/**
 * @struct FineThreeAxisDetermination
 * @brief  精三軸姿勢決定に必要な情報を格納する構造体
 */
typedef struct
{
  ObcTime previous_obc_time; //!< 前回精三軸姿勢決定アプリが呼び出された時の時刻情報
  APP_FTAD_METHOD method;    //!< どの手法を使って精三軸姿勢決定を行うか
} FineThreeAxisDetermination;

extern const FineThreeAxisDetermination* const fine_three_axis_determination;

AppInfo APP_FTAD_create_app(void);

CCP_CmdRet Cmd_APP_FTAD_SET_METHOD(const CommonCmdPacket* packet);

#endif
