/**
* @file   di_mpu9250.h
* @brief  MPU9250のDriver Instance
*/
#ifndef DI_MPU9250_H_
#define DI_MPU9250_H_

#include "../../Drivers/Aocs/mpu9250.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include "../../Library/SignalProcess/polynomial_approximation.h"

/**
 * @enum  MPU9250_IDX
 * @brief 複数搭載されているMPU9250の番号管理
 * @note  型はuint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  MPU9250_IDX_ON_AOBC = 0,
  MPU9250_IDX_MAX,
} MPU9250_IDX;

/**
 * @enum  MPU9250_EL_NOTE
 * @brief MPU9250に関連するELの詳細
 * @note  型はuint8_tを想定
 */
typedef enum
{
  MPU9250_EL_NOTE_NOISE_REMOVED_MAG = 0,
  MPU9250_EL_NOTE_NOISE_REMOVED_GYRO,
  MPU9250_EL_NOTE_MAX,
} MPU9250_EL_NOTE;

/**
 * @struct DiMpu9250
 * @brief MPU9250 DIに関する外部公開情報用構造体
 */
typedef struct
{
  // 温度補正
  PolynomialApprox gyro_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM];   //!< コンポ座標でのGyroセンサバイアス温度補正パラメータ [rad/s]
  PolynomialApprox gyro_scale_factor_compo[PHYSICAL_CONST_THREE_DIM]; //!< コンポ座標でのGyroセンサスケールファクタ温度補正パラメータ [-]
} DiMpu9250;

extern const MPU9250_Driver* const mpu9250_driver[MPU9250_IDX_MAX];
extern const DiMpu9250* const di_mpu9250[MPU9250_IDX_MAX];

/**
 * @brief  MPU9250定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_MPU9250_update(void);

CCP_EXEC_STS Cmd_DI_MPU9250_INIT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_MPU9250_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_MPU9250_SET_ANG_VEL_BIAS_COMPO_RAD_S(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_MPU9250_SET_MAG_BIAS_COMPO_NT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_MPU9250_SET_ANG_VEL_BIAS_TEMP_CALIB(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_MPU9250_SET_ANG_VEL_SF_TEMP_CALIB(const CommonCmdPacket* packet);

#endif
