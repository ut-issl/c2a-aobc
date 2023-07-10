/**
* @file   b_dot.h
* @brief  Bdot則制御アプリケーション
*/
#ifndef BDOT_H_
#define BDOT_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_error.h>

/**
 * @struct Bdot
 * @brief  Bdot則に必要な情報を管理するための構造体
 */
typedef struct
{
  float control_gain[PHYSICAL_CONST_THREE_DIM];                   //!< 制御ゲイン[ ]．
  uint32_t minimum_time_derivative_step_ms;                       //!< 磁場微分計算のための最小インターバル[ms]
  uint32_t mtq_output_time_length_ms;                             //!< 1回の駆動でMTQのON状態を維持する時間[ms]
  ObcTime  mtq_output_turned_on_obc_time;                         //!< MTQの出力をONにした瞬間のOBC Time
  struct
  {
    ObcTime previous_obc_time;                                    //!< 磁場ベクトル微分計算に使うOBC time
    ObcTime current_obc_time;                                     //!< 磁場ベクトル微分計算に使うOBC time
    float mag_vec_before_nT[PHYSICAL_CONST_THREE_DIM];            //!< 磁場ベクトル微分計算に使う磁場ベクトル情報[nT]
    float mag_vec_after_nT[PHYSICAL_CONST_THREE_DIM];             //!< 磁場ベクトル微分計算に使う磁場ベクトル情報[nT]
    uint8_t num_of_mag_observation;                               //!< 磁場の観測回数．2回観測すると磁場ベクトル微分が計算され，0に戻る．
    float mag_vec_time_derivative_nT_s[PHYSICAL_CONST_THREE_DIM]; //!< 磁場ベクトルの時間微分[nT/s]
  } time_derivative_variables;
} Bdot;

extern const Bdot* const bdot;

/**
 * @brief  BDOT定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_BDOT_create_app(void);

/**
 * @brief  Bdotゲイン設定
 *
 *         機体固定座標系の指定した軸に関するBdot制御のゲインを設定する
 * @param  axis_body     : ゲインを設定する軸
 * @param  control_gain  : 設定するゲイン値．負の値に限る
 * @return AOCS_ERRORに準じる
 */
AOCS_ERROR APP_BDOT_set_control_gain(float* control_gain);

CCP_CmdRet Cmd_APP_BDOT_SET_CONTROL_GAIN(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_APP_BDOT_SET_TIMING(const CommonCmdPacket* packet);

#endif
