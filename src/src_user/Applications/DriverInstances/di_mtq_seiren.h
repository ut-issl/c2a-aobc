/**
* @file   di_mtq_seiren.h
* @brief  MTQ_SEIRENのDriver Instance
*/
#ifndef DI_MTQ_SEIREN_H_
#define DI_MTQ_SEIREN_H_

#include <src_user/Drivers/Aocs/mtq_seiren.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_error.h>
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  MTQ_SEIREN_IDX
 * @brief 複数搭載されているMTQ_SEIRENの番号管理
 * @note  型はuint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  MTQ_SEIREN_IDX_X = 0,
  MTQ_SEIREN_IDX_Y,
  MTQ_SEIREN_IDX_Z,
  MTQ_SEIREN_IDX_MAX,
} MTQ_SEIREN_IDX;


extern const MTQ_SEIREN_Driver* const mtq_seiren_driver[MTQ_SEIREN_IDX_MAX];

/**
 * @brief  MTQ_SEIREN定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_MTQ_SEIREN_update(void);

/**
 * @brief  MTQ_SEIRENデューティー設定関数
 *         mtq_seiren_controllerから呼ばれることを想定
 * @param  axis                    : MTQ_SEIRENのインデックス
 * @param  pwm_signed_duty_percent : 符号付きduty
 * @return AOCS_ERRORに準拠
 */
AOCS_ERROR DI_MTQ_SEIREN_set_pwm_duty(MTQ_SEIREN_IDX axis, int8_t pwm_signed_duty_percent);

// コマンド関数
CCP_CmdRet Cmd_DI_MTQ_SEIREN_SET_PWM_PERIOD_MS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DI_MTQ_SEIREN_SET_PWM_DUTY(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DI_MTQ_SEIREN_SET_MAGNETIC_MOMENT_DIRECTION_VECTOR(const CommonCmdPacket* packet);

#endif
