/**
* @file   mtq_seiren_controller.h
* @brief  aocs_managerの目標磁気情報から，MTQ_SEIRENのPWMデューティー比を計算・設定するアプリ
*/
#ifndef MTQ_SEIREN_CONTROLLER_H_
#define MTQ_SEIREN_CONTROLLER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Drivers/Aocs/mtq_seiren.h>

/**
 * @struct MtqSeirenController
 * @brief  MTQ_SEIRENの制御に必要な情報を管理する構造体
 */
typedef struct
{
  float mtq_output_duration_ms[PHYSICAL_CONST_THREE_DIM];            //!< 一回の制御期間中にMTQが出力する時間 [ms]
  MTQ_SEIREN_POLARITY mtq_output_polarity[PHYSICAL_CONST_THREE_DIM]; //!< この制御期間中に出力するMTQ電流極性

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

#endif
