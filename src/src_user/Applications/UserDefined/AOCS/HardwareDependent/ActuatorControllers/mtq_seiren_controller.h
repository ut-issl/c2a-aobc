/**
* @file   mtq_seiren_controller.h
* @brief  aocs_managerの目標磁気情報から，MTQ_SEIRENのPWMデューティー比を計算・設定するアプリ
* @note   本来なら，PWMデューティー比 (DI等の下位側でやること) とMTQ駆動・消磁の比率 (ここでやること) は別物となる
* @note   ここでやるべきはMTQ駆動・消磁の比率の計算か，PWMデューティー比の計算のどちらかのはずだが，両者が混ざっている
* @note   とはいえ，PWM周期の遅い現状では，両者の周期を近づけざるを得ず，その状態でDI側のSWを現状維持するために，下記制約を課す
* @note   mtq_output_time_length_msとmtq_demagnetization_required_time_msの和が，遅いPWM周期 (1sec)の整数倍 -100msec と合致する様に定める
* @note   上記制約が満たされない場合，DI側と上位側 (3軸制御やこのアプリ) が思うMTQ駆動動作に齟齬が発生する (100msecはcase処理による余分な分岐で生じる時間)
*/
#ifndef MTQ_SEIREN_CONTROLLER_H_
#define MTQ_SEIREN_CONTROLLER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_core/System/TimeManager/time_manager.h>
#include "../../../../../Library/stdint.h"
#include "../../../../../Library/physical_constants.h"
#include "../../../../../Library/ControlUtility/cross_product_control.h"

/**
 * @struct MtqSeirenController
 * @brief  MTQ_SEIRENの制御に必要な情報を管理する構造体
 * @note   PWM周期が遅いことへの暫定処置として，mtq_output_time_length_msと
 * @note   mtq_demagnetization_required_time_msの和が，遅いPWM周期 (1sec)の整数倍 -10msec と合致する様に定める
 */
typedef struct
{
  ObcTime  previous_obc_time;                     //!< 前回のアプリ実行時間
  uint32_t internal_timer_ms;                     //!< 実行状態制御用カウンタ[ms], 駆動 + 消磁動作1cycleに相当する周期 (OBCTime) でroll over
  uint32_t max_count_internal_timer_ms;           //!< 実行状態制御用カウンタ[ms]の最大値 = 駆動+消磁動作1cycleに相当する周期 (OBCTime)
  uint32_t mtq_output_time_length_ms;             //!< 1回の駆動でMTQのON状態を維持する時間[ms]
  uint32_t mtq_demagnetization_required_time_ms;  //!< MTQ消磁までに必要な時間[ms]
  ObcTime  mtq_output_turned_on_obc_time;         //!< MTQがONされたOBC時間

  // 消磁中に更新された指令トルクに関する積分関連パラメータ
  float integrated_trq_Nms[PHYSICAL_CONST_THREE_DIM]; //!< 消磁中に更新された指令トルクを積分して角運動量指令に換算するためのバッファ [Nms]
  ObcTime  previous_trq_integration_obc_time;         //!< 前回積分した時間

  CROSS_PRODUCT_CONTROL_ERROR cross_product_error;    //!< CrossProductの実行エラー記録 (TODO_L: 残すか要検討，デバッグ用)
  float cross_product_error_ratio;
} MtqSeirenController;

extern const MtqSeirenController* const mtq_seiren_controller;

AppInfo APP_MTQ_SEIREN_CONTROLLER_create_app(void);

/**
 * @brief  クロスプロダクト制御時MTQ目標磁気モーメント出力関数
 * @note   MTQの出力がONのときに呼び出され，所定の時間だけMTQ出力を維持する役割を持つ
 * @note   所定のMTQ出力時間が過ぎたら，MTQ出力を切る
 */
void APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_output(const CrossProductControl cross_product_cntrl);

CCP_EXEC_STS Cmd_APP_MTQ_SEIREN_CONTROLLER_SET_DEMAGNITIZATION_TIME(const CommonCmdPacket* packet);

#endif
