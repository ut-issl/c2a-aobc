/**
* @file   rw0003.h
* @brief  RW0003 Reaction Wheelのドライバ
*/

#ifndef RW0003_H_
#define RW0003_H_

#include <src_core/IfWrapper/i2c.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "../../Library/physical_constants.h"
#include "../../Library/c2a_math.h"

extern const float RW0003_kMaxTorqueNm;         //!< 出力可能最大トルク [Nm]
extern const float RW0003_kMomentOfInertiaKgm2; //!< ホイール慣性モーメント [kgm2]

/**
* @enum  RW0003_REC_CRC_STATE
* @brief 受信CRCが正しいかどうか。TODO_L : 共通enumにする
* @note  uint_8を想定
*/
typedef enum
{
  RW0003_REC_CRC_STATE_OK = 0,
  RW0003_REC_CRC_STATE_NG = 1
} RW0003_REC_CRC_STATE;

/**
 * @struct RW0003_Info
 * @brief  RW0003のテレメトリ情報
 */
typedef struct
{
  float speed_rad_s;        //!< rotation speed [rad/s]
  float torque_Nm;          //!< ordered torque [Nm]
  float temperature_degC;   //!< temperature [degC]
  float vdd_V;              //!< output of +3.3 V DC/DC converter [V]
  float seu_count;          //!< number of errors
  float fault_state;        //!< 1: wheel is in an fault, 0: otherwise
  int32_t diagnostic_reset_reason;      //!< reset reason
  float speed_limit1_rad_s; //!< clipされる回転数  [rad/s] (limit1 < limit2)
  float speed_limit2_rad_s; //!< 異常停止する回転数 [rad/s] (limit1 < limit2)
  float rotation_direction_b[PHYSICAL_CONST_THREE_DIM]; //!< 機体座標系での回転方向単位ベクトル(右手系)
  RW0003_REC_CRC_STATE crc_state;          //!< 受信したCRCが正しいか
} RW0003_Info;

/**
 * @struct RW0003_Driver
 * @brief  RW0003_Driver構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;      //!< DriverSuper class
    I2C_Config i2c_config;  //!< I2C class
  } driver;
  RW0003_Info info;
} RW0003_Driver;

// 基本関数

/**
 * @brief  RW0003初期化
 *
 *         RW0003_Driver構造体のポインタを渡すことでポートを初期化し，RW0003_Driverの各メンバも初期化する
 * @param  rw0003_driver : 初期化するRW0003_Driver構造体へのポインタ
 * @param  ch             : RW0003が接続されているI2Cポート番号
 * @param  i2c_address    : RW0003のI2Cデバイスアドレス
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE RW0003_init(RW0003_Driver* rw0003_driver, uint8_t ch, uint8_t i2c_address, DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  RW0003アプリケーションスタートコマンド
 * @param  rw0003_driver : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_start_app(RW0003_Driver* rw0003_driver);

/**
 * @brief  RW0003 Idleモード指令コマンド
 * @param  rw0003_driver : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_set_idle(RW0003_Driver* rw0003_driver);

/**
 * @brief  RW0003回転数測定コマンド
 * @param  rw0003_driver : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_observe_speed(RW0003_Driver* rw0003_driver);

/**
 * @brief  RW0003温度測定コマンド
 * @param  *rw0003 : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_observe_temperature(RW0003_Driver* rw0003_driver);

/**
 * @brief  RW0003トルク指令コマンド
 * @param  rw0003_driver   : RW0003_Driver構造体へのポインタ
 * @param  torque_Nm : 指令トルク [Nm]
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_drive_torque(RW0003_Driver* rw0003_driver, const float torque_Nm);

/**
 * @brief  RW0003回転数指令コマンド
 * @param  rw0003_driver      : RW0003_Driver構造体へのポインタ
 * @param  *speed_rad_s : 指令回転数 [rad/s]
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_drive_speed(RW0003_Driver* rw0003_driver, const float speed_rad_s);

/**
 * @brief  回転方向ベクトル設定関数
 * @param  rw0003_driver       : RW0003_Driver構造体へのポインタ
 * @param  rotation_direction_b : 機体固定座標系での回転方向ベクトル
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR RW0003_set_rotation_direction_b(RW0003_Driver* rm3100_driver,
                                               const float rotation_direction_b[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  RW0003 VDD読み取りコマンド
 * @param  rw0003_driver : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_read_vdd(RW0003_Driver* rw0003_driver);

/**
 * @brief  RW0003 SEU Count読み取りコマンド
 * @param  rw0003_driver : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_read_seu_count(RW0003_Driver* rw0003_driver);

/**
 * @brief  RW0003 Fault State読み取りコマンド
 * @param  rw0003_driver : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_read_fault_state(RW0003_Driver* rw0003_driver);

/**
 * @brief  RW0003 Diagnosticコマンド
 * @param  rw0003_driver : RW0003_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RW0003_diagnostic(RW0003_Driver* rw0003_driver);

#endif
