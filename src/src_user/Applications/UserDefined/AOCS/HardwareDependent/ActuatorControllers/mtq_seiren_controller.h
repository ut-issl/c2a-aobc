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
#include <src_user/Library/ControlUtility/cross_product_control.h>
#include <src_user/Applications/DriverInstances/di_mtq_seiren.h>

/**
 * @struct MtqSeirenController
 * @brief  MTQ_SEIRENの制御に必要な情報を管理する構造体
 */
typedef struct
{
  uint32_t mtq_output_duration_ms[MTQ_SEIREN_IDX_MAX];         //!< 一回の制御期間中にMTQが出力する時間 [ms]
  MTQ_SEIREN_POLARITY mtq_output_polarity[MTQ_SEIREN_IDX_MAX]; //!< この制御期間中に出力するMTQ電流極性

  // 消磁中に更新された指令トルクに関する積分関連パラメータ
  float integrated_torque_Nms[PHYSICAL_CONST_THREE_DIM]; //!< 消磁中に更新された指令トルクを積分して角運動量指令に換算するためのバッファ [Nms]
  ObcTime  previous_torque_integration_obc_time;         //!< 前回積分した時間

  CROSS_PRODUCT_CONTROL_ERROR cross_product_error;    //!< CrossProductの実行エラー記録 (TODO_L: 残すか要検討，デバッグ用)
  float cross_product_error_ratio;
} MtqSeirenController;

extern const MtqSeirenController* const mtq_seiren_controller;

AppInfo APP_MTQ_SEIREN_CONTROLLER_create_app(void);

/**
 * @brief  クロスプロダクト制御時MTQ目標磁気モーメント出力関数
 */
void APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_output(const CrossProductControl cross_product_cntrl);

/**
 * @brief  MTQ出力比を手動で設定する
 * @note   地上での電気試験で使うことを想定。出力比は [-1.0, +1.0] の範囲で指定する。
 * @param  CommonCmdPacket
 * @return CCP_CmdRetに準拠
*/
CCP_CmdRet Cmd_APP_MTQ_SEIREN_CONTROLLER_SET_OUTPUT_RATIO_MANUALLY(const CommonCmdPacket* packet);

#endif
