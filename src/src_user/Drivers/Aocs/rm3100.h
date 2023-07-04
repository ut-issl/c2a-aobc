/**
* @file   rm3100.h
* @brief  RM3100精磁気センサのドライバ
*/

#ifndef RM3100_H_
#define RM3100_H_

#include <src_core/IfWrapper/i2c.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_user/Library/quaternion.h>
#include <src_user/Library/physical_constants.h>

extern const uint8_t RM3100_kCmmMode; //!< Continuous Measure Modeに入れるための設定値

/**
 * @struct RM3100_Info
 * @brief  RM3100のテレメトリ情報
 */
typedef struct
{
  float mag_raw_compo_nT[PHYSICAL_CONST_THREE_DIM];   //!< uncalibrated magnetic flux density @ component frame [nT]
  float mag_compo_nT[PHYSICAL_CONST_THREE_DIM];       //!< calibrated magnetic flux density @ component frame [nT]
  float mag_body_nT[PHYSICAL_CONST_THREE_DIM];        //!< calibrated magnetic flux density @ body frame [nT]
  float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM];  //!< bias of magnetic flux density @ component frame [nT] (output = observation - this value)
  Quaternion frame_transform_c2b;                     //!< frame transformation quaternion from component frame to body frame
} RM3100_Info;

/**
 * @struct RM3100_Driver
 * @brief  RM3100_Driver構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;      //!< DriverSuper class
    I2C_Config i2c_config;  //!< I2C class
  } driver;
  RM3100_Info info;
} RM3100_Driver;

// 基本関数

/**
 * @brief  RM3100初期化
 *
 *         RM3100_Driver構造体のポインタを渡すことでポートを初期化し，RM3100_Driverの各メンバも初期化する
 * @param  rm3100_driver : 初期化するRM3100_Driver構造体へのポインタ
 * @param  ch             : RM3100が接続されているI2Cポート番号
 * @param  i2c_address    : RM3100のI2Cデバイスアドレス
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE RM3100_init(RM3100_Driver* rm3100_driver, uint8_t ch, uint8_t i2c_address, DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  RM3100モード設定コマンド
 *
 *         RM3100へモード設定コマンドを送る
 * @param  rm3100_driver : RM3100_Driver構造体へのポインタ
 * @param  mode           : RM3100に設定する各種モード情報を束ねたもの
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RM3100_set_mode(RM3100_Driver* rm3100_driver, uint8_t mode);

/**
 * @brief  RM3100磁場観測
 *
 *         RM3100から磁場観測値を読み取る
 * @param  rm3100_driver  : RM3100_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE RM3100_observe_mag(RM3100_Driver* rm3100_driver);

/**
 * @brief  座標変換行列設定関数
 * @param  rm3100_driver : RM3100_Driver構造体へのポインタ
 * @param  q_c2b          : コンポ座標からボディ座標への座標変換Quaternion
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR RM3100_set_frame_transform_c2b(RM3100_Driver* rm3100_driver, const Quaternion q_c2b);

/**
 * @brief  磁場ベクトルバイアス補正値設定関数
 * @param  rm3100_driver : RM3100_Driver構造体へのポインタ
 * @param  mag_bias_compo_nT : コンポ座標での磁場バイアス補正値(計測値からこの値を差し引く)
 * @return C2A_MATH_ERROR_RANGE_OVER : 補正値が大きすぎる場合
 */
C2A_MATH_ERROR RM3100_set_mag_bias_compo_nT(RM3100_Driver* rm3100_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  磁場ベクトルバイアス補正値設定関数(現在値への加算)
 * @param  rm3100_driver : RM3100_Driver構造体へのポインタ
 * @param  mag_bias_compo_nT : コンポ座標での磁場バイアス補正値(計測値からこの値を差し引く)
 * @return C2A_MATH_ERROR_RANGE_OVER : 補正値が大きすぎる場合
 */
C2A_MATH_ERROR RM3100_add_mag_bias_compo_nT(RM3100_Driver* rm3100_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM]);

#endif
