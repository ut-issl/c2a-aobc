/**
* @file   nanossoc_d60.h
* @brief  nanoSSOC-D60サンセンサのドライバ
*/

#ifndef NANOSSOC_D60_H_
#define NANOSSOC_D60_H_

#include <src_core/IfWrapper/i2c.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "../../Library/quaternion.h"
#include "../../Library/physical_constants.h"

/**
* @enum  NANOSSOC_D60_ErrorCode
* @brief 太陽検出率および角度出力に応じたエラーコード
* @note  uint_8を想定
*/
typedef enum
{
  NANOSSOC_D60_ERROR_CODE_NO_ERROR                 = 0x0,
  NANOSSOC_D60_ERROR_CODE_NOT_ENOUGH_RADIATION     = 0x10,
  NANOSSOC_D60_ERROR_CODE_ONLY_ALBEDO_DETECTED     = 0x11,
  NANOSSOC_D60_ERROR_CODE_BOTH_SUN_ALBEDO_DETECTED = 0x12,
  NANOSSOC_D60_ERROR_CODE_OUT_OF_FIELD_OF_VIEW     = 0x13
} NANOSSOC_D60_ERROR_CODE;

/**
* @enum  NANOSSOC_D60_ANGLE_ELEMENT
* @brief サンセンサの角度要素
* @note  uint_8を想定
*/
typedef enum
{
  NANOSSOC_D60_ANGLE_ELEMENT_ALPHA = 0,
  NANOSSOC_D60_ANGLE_ELEMENT_BETA,
  NANOSSOC_D60_ANGLE_ELEMENT_MAX
} NANOSSOC_D60_ANGLE_ELEMENT;

/**
* @enum  NANOSSOC_D60_CHECKSUM_STATE
* @brief サンセンサから送られてきたチェックサムとドライバで計算したチェックサムが一致しているかどうか
* @note  uint_8を想定
*/
typedef enum
{
  NANOSSOC_D60_CHECKSUM_STATE_OK = 0,
  NANOSSOC_D60_CHECKSUM_STATE_NG = 1
} NANOSSOC_D60_CHECKSUM_STATE;

/**
 * @struct NANOSSOC_D60_Info
 * @brief NANOSSOC_D60内部状態
 */
typedef struct
{
  float sun_angle_compo_deg[NANOSSOC_D60_ANGLE_ELEMENT_MAX];    //!< sun direction angle @ component frame [deg] (alpha, beta)
  float sun_vec_compo[PHYSICAL_CONST_THREE_DIM];                //!< sun direction vector @ component frame
  float sun_intensity_percent;                                  //!< sun luminous intensity compared to solar constant [%]
  NANOSSOC_D60_ERROR_CODE err_code;                             //!< error code
  NANOSSOC_D60_CHECKSUM_STATE checksum_state;                   //!< checksum match or not
  float sun_vec_body[PHYSICAL_CONST_THREE_DIM];                 //!< sun direction vector @ body frame [-]
  Quaternion frame_transform_c2b;                               //!< frame transformation quaternion from component frame to body frame
} NANOSSOC_D60_Info;

/**
 * @struct NANOSSOC_D60_Driver
 * @brief NANOSSOC_D60ドライバ構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;      //!< DriverSuper class
    I2C_Config i2c_config;  //!< I2C class
  } driver;
  NANOSSOC_D60_Info info;
} NANOSSOC_D60_Driver;

// 基本関数

/**
 * @brief  NANOSSOC_D60初期化
 *
 *         NANOSSOC_D60_Driver構造体のポインタを渡すことでポートを初期化し，NANOSSOC_D60_Driverの各メンバも初期化する
 * @param  nanossoc_d60_driver : 初期化するNANOSSOC_D60_Driver構造体へのポインタ
 * @param  ch                   : NANOSSOC_D60が接続されているI2Cポート番号
 * @param  i2c_address          : NANOSSOC_D60のI2Cデバイスアドレス
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE NANOSSOC_D60_init(NANOSSOC_D60_Driver* nanossoc_d60_driver, uint8_t ch, uint8_t i2c_address, DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  NANOSSOC_D60観測
 *
 *         太陽方向，太陽検出率，エラーコードを読み出す
 * @param  nanossoc_d60_driver : 初期化するNANOSSOC_D60_Driver構造体へのポインタ
 * @return DS_ERR_CODEに準拠
 */
DS_CMD_ERR_CODE NANOSSOC_D60_observe(NANOSSOC_D60_Driver* nanossoc_d60_driver);

/**
 * @brief  座標変換行列設定関数
 * @param  nanossoc_d60_driver  : NANOSSOC_D60_Driver構造体へのポインタ
 * @param  q_c2b                 : コンポ座標からボディ座標への座標変換Quaternion
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR NANOSSOC_D60_set_frame_transform_c2b(NANOSSOC_D60_Driver* nanossoc_d60_driver, const Quaternion q_c2b);

#endif
