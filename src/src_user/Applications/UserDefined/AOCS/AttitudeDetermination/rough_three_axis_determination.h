/**
* @file    rough_three_axis_determination.h
* @brief   粗三軸姿勢決定アプリ
*
*          TRIADとQmethodを使い分ける
*/
#ifndef ROUGH_THREE_AXIS_DETERMINATION_H_
#define ROUGH_THREE_AXIS_DETERMINATION_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/System/TimeManager/time_manager.h>

/**
* @enum  APP_RTAD_METHOD
* @brief 使用する三軸姿勢決定の手法
* @note  uint_8を想定
*/
typedef enum
{
  APP_RTAD_METHOD_TRIAD = 0,  //!< TRIADを使って姿勢決定する
  APP_RTAD_METHOD_Q_METHOD,   //!< Qmethodを使って姿勢決定する
  APP_RTAD_METHOD_MAX
} APP_RTAD_METHOD;

/**
* @enum  APP_RTAD_SUN_INVISIBLE_PROPAGATION
* @brief 太陽センサ非可視時にどのあたいを伝搬するかの設定
* @note  uint_8を想定
*/
typedef enum
{
  APP_RTAD_SUN_INVISIBLE_PROPAGATION_SUN_VECTOR = 0,  //!< 伝搬した太陽ベクトルを利用する
  APP_RTAD_SUN_INVISIBLE_PROPAGATION_QUATERNION,      //!< Quaternionを伝搬する
  APP_RTAD_SUN_INVISIBLE_PROPAGATION_MAX
} APP_RTAD_SUN_INVISIBLE_PROPAGATION;

/**
* @struct QMethod
* @brief  Qmethod関連の情報を格納する構造体（TODO_L:もし必要になったら，TRIAD関連の情報を格納する構造体も作る）
*/
typedef struct
{
  float sun_vec_weight; //!< 推定における，太陽方向ベクトルの重み
  float mag_vec_weight; //!< 推定における，磁場方向ベクトルの重み
} QMethodInfo;

/**
 * @struct ROUGH_THREE_AXIS_DETERMINATION
 * @brief  粗三軸姿勢決定に必要な情報を格納する構造体
 */
typedef struct
{
  ObcTime previous_obc_time;                                    //!< 前回粗三軸姿勢決定アプリが呼び出された時の時刻情報
  APP_RTAD_METHOD method;                                       //!< 粗三軸姿勢決定手法
  APP_RTAD_SUN_INVISIBLE_PROPAGATION sun_invisible_propagation; //!< 太陽センサ非可視時にどのあたいを伝搬するかの設定
  QMethodInfo q_method_info;                                    //!< Q methodパラメータ
} RoughThreeAxisDetermination;

extern const RoughThreeAxisDetermination* const rough_three_axis_determination;

/**
 * @brief  粗三軸姿勢決定アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_RTAD_create_app(void);

CCP_CmdRet Cmd_APP_RTAD_SET_METHOD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_RTAD_SET_QMETHOD_SUN_VEC_WEIGHT(const CommonCmdPacket* packet);

#endif
