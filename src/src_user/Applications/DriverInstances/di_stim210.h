/**
* @file   di_stim210.h
* @brief  STIM210のDriver Instance
*/
#ifndef DI_STIM210_H_
#define DI_STIM210_H_

#include "../../Drivers/Aocs/stim210.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include "../../Library/SignalProcess/polynomial_approximation.h"

/**
 * @enum  STIM210_IDX
 * @brief 搭載されているSTIM210の管理番号
 * @note  uint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  STIM210_IDX_IN_UNIT = 0,
  STIM210_IDX_MAX,
} STIM210_IDX;

/**
 * @enum  STIM210_EL_NOTE
 * @brief STIM210に関連するELの詳細
 * @note  型はuint8_tを想定
 */
typedef enum
{
  STIM210_EL_NOTE_CRC_ERROR = 0,
  STIM210_EL_NOTE_STATUS_ERROR,
  STIM210_EL_NOTE_NOISE_REMOVED,
  STIM210_EL_NOTE_MAX,
} STIM210_EL_NOTE;

/**
 * @struct DiStim210
 * @brief STIM210 DIに関する外部公開情報用構造体
 */
typedef struct
{
  // 温度補正
  PolynomialApprox bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM];   //!< コンポ座標でのセンサバイアス温度補正パラメータ [rad/s]
  PolynomialApprox scale_factor_compo[PHYSICAL_CONST_THREE_DIM]; //!< コンポ座標でのセンサスケールファクタ温度補正パラメータ [-]
} DiStim210;

extern const STIM210_Driver* const stim210_driver[STIM210_IDX_MAX];
extern const DiStim210* const di_stim210[STIM210_IDX_MAX];

/**
 * @brief  STIM210定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_STIM210_update(void);

CCP_EXEC_STS Cmd_DI_STIM210_INIT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_MODE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_NORMAL_MODE_FORMAT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_GYRO_OUTPUT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_SAMPLE_RATE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_TERMINATION_MODE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_LOW_PASS_FILTER(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_ANG_VEL_BIAS_COMPO_RAD_S(const CommonCmdPacket* packet);
// 温度補正
CCP_EXEC_STS Cmd_DI_STIM210_SET_ANG_VEL_BIAS_TEMP_CALIB(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_STIM210_SET_ANG_VEL_SF_TEMP_CALIB(const CommonCmdPacket* packet);

#endif
