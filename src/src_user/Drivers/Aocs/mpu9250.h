/**
* @file   mpu9250.h
* @brief  MPU9250 9軸センサのドライバ
*/

#ifndef MPU9250_H_
#define MPU9250_H_

#include <src_core/IfWrapper/i2c.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "../../Library/quaternion.h"
#include "../../Library/physical_constants.h"

/**
 * @enum   MPU9250_ANG_VEL_RANGE
 * @brief  MPU9250の取りうる角速度範囲
 * @note   uint8_tを想定
 */
typedef enum
{
  MPU9250_ANG_VEL_RANGE_250_DEG_S = 0,
  MPU9250_ANG_VEL_RANGE_500_DEG_S,
  MPU9250_ANG_VEL_RANGE_1000_DEG_S,
  MPU9250_ANG_VEL_RANGE_2000_DEG_S,
  MPU9250_ANG_VEL_RANGE_MAX
} MPU9250_ANG_VEL_RANGE;


/**
 * @enum   MPU9250_ACCEL_RANGE
 * @brief  MPU9250の取りうる加速度範囲
 * @note   uint8_tを想定
 */
typedef enum
{
  MPU9250_ACCEL_RANGE_2_G  = 0,
  MPU9250_ACCEL_RANGE_4_G,
  MPU9250_ACCEL_RANGE_8_G,
  MPU9250_ACCEL_RANGE_16_G,
  MPU9250_ACCEL_RANGE_MAX
} MPU9250_ACCEL_RANGE;


/**
 * @struct MPU9250_Info
 * @brief  MPU9250の観測データやパラメータを格納する構造体
 */
typedef struct
{
  // angular velocity
  float ang_vel_raw_compo_rad_s[PHYSICAL_CONST_THREE_DIM];   //!< uncalibrated angular velocity uncaliblated value @ gyroscope and accelerometer frame [rad/s]
  float ang_vel_compo_rad_s[PHYSICAL_CONST_THREE_DIM];       //!< calibrated angular velocity @ gyroscope and accelerometer frame [rad/s]
  float ang_vel_body_rad_s [PHYSICAL_CONST_THREE_DIM];       //!< calibrated angular velocity @ body frame [rad/s]
  float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM];  //!< bias correction value of angular velocity @ gyroscope and accelerometer frame [rad/s]
  float ang_vel_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< scale factor of angular velocity @ gyroscope and accelerometer frame [-]
  // accelerometer
  float accel_compo_m_s2[PHYSICAL_CONST_THREE_DIM];  //!< acceleration @ gyroscope and accelerometer frame [m/s2]
  float accel_body_m_s2 [PHYSICAL_CONST_THREE_DIM];  //!< acceleration @ body frame [m/s2]
  // magnetometer
  uint8_t mag_status_overrun;                         //!< status of magnetometer (0: ready to read, 1: reading register, 2,3: data overrun)
  uint8_t mag_status_overflow;                        //!< status of magnetometer (0,16: OK, 8,24: data overflow)
  uint8_t is_mag_enabled;                             //!< magnetometer is enabled or not (0: OFF, 1:ON)
  float mag_raw_compo_nT[PHYSICAL_CONST_THREE_DIM];   //!< uncalibrated magnetic flux density uncaliblated value @ gyroscope and accelerometer frame [nT] (not magnetometer frame)
  float mag_compo_nT[PHYSICAL_CONST_THREE_DIM];       //!< calibrated magnetic flux density @ gyroscope and accelerometer frame [nT] (not magnetometer frame)
  float mag_body_nT [PHYSICAL_CONST_THREE_DIM];       //!< calibrated magnetic flux density @ body frame [nT] (not magnetometer frame)
  float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM];  //!< bias correction value of magnetic flux density @ gyroscope and accelerometer frame [nT] (not magnetometer frame) (output = observation - this value)
  float mag_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< scale factor of magnetic flux density @ gyroscope and accelerometer frame [-]
  // others
  float temperature_degC;          //!< temperature of the component [degC]
  Quaternion frame_transform_c2b;  //!< frame transformation quaternion from component(gyro sensor) frame to body frame
} MPU9250_Info;

/**
 * @struct MPU9250_Driver
 * @brief  MPU9250にはMPU9250本体とAK8963でI2Cアドレスが異なるため、それぞれ構造体を持たせている。
 */
typedef struct
{
  struct
  {
    DriverSuper super;      //!< DriverSuper class
    I2C_Config i2c_config;  //!< I2C class
  } driver_mpu9250;

  struct
  {
    DriverSuper super;      //!< DriverSuper class
    I2C_Config i2c_config;  //!< I2C class
  } driver_ak8963;

  MPU9250_Info info;
} MPU9250_Driver;

// 基本関数

/**
 * @brief  MPU9250初期化
 *
 *         MPU9250_Driver構造体のポインタを渡すことでポートを初期化し，MPU9250_Driverの各メンバも初期化する
 * @param  mpu9250_driver     : 初期化するMPU9250_Driver構造体へのポインタ
 * @param  ch                 : MPU9250が接続されているI2Cポート番号
 * @param  i2c_address_mpu9250: MPU9250のI2Cデバイスアドレス
 * @param  i2c_address_ak8963 : AK8963のI2Cデバイスアドレス
 * @param  rx_buffer_mpu9250  : MPU9250 の受信バッファ
 * @param  rx_buffer_ak8963   : AK8963 の受信バッファ
 * @return DS_ERR_CODE
 */
DS_INIT_ERR_CODE MPU9250_init(MPU9250_Driver* mpu9250_driver,
                         uint8_t ch,
                         uint8_t i2c_address_mpu9250,
                         uint8_t i2c_address_ak8963,
                         DS_StreamRecBuffer* rx_buffer_mpu9250,
                         DS_StreamRecBuffer* rx_buffer_ak8963);

/**
 * @brief  MPU9250により角速度・温度・加速度・磁場の観測を行う
 * @param  mpu9250_driver    : 初期化するMPU9250_Driver構造体へのポインタ
 * @return 0                  : 正常終了
 * @return 1                  : 角速度・加速度・温度観測異常終了
 * @return 2                  : 磁気観測異常終了
 */
int MPU9250_rec(MPU9250_Driver* mpu9250_driver);

/**
 * @brief  MPU9250の磁気センサ(AK8963)をONして連続観測を開始する
 * @param  mpu9250_driver    : MPU9250_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE MPU9250_enable_mag(MPU9250_Driver* mpu9250_driver);

/**
 * @brief  MPU9250の角速度・温度・加速度センサをONして連続観測を開始する
 * @param  mpu9250_driver    : MPU9250_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE MPU9250_enable_gyro(MPU9250_Driver* mpu9250_driver);

/**
 * @brief  MPU9250の角速度・磁場観測値のローパスフィルタのカットオフ周波数を設定する
 *
 *         TODO_L : 現状カットオフ周波数は固定しているので変更できるようにする
 * @param  mpu9250_driver    : MPU9250_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE MPU9250_set_lpf(MPU9250_Driver* mpu9250_driver);

/**
 * @brief  MPU9250の角速度観測範囲を設定する
 * @param  mpu9250_driver    : MPU9250_Driver構造体へのポインタ
 * @param  range       : MPU9250_ANG_VEL_RANGEにある角速度範囲
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE MPU9250_set_ang_vel_range(MPU9250_Driver* mpu9250_driver, MPU9250_ANG_VEL_RANGE range);

/**
 * @brief  MPU9250の加速度観測範囲を設定する
 * @param  mpu9250_driver    : MPU9250_Driver構造体へのポインタ
 * @param  range       : MPU9250_ACCEL_RANGEにある角速度範囲
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE MPU9250_set_accel_range(MPU9250_Driver* mpu9250_driver, MPU9250_ACCEL_RANGE range);

/**
 * @brief  座標変換行列設定関数
 * @param  mpu9250_driver  : MPU9250_Driver構造体へのポインタ
 * @param  q_c2b            : コンポ座標からボディ座標への座標変換Quaternion
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR MPU9250_set_frame_transform_c2b(MPU9250_Driver* mpu9250_driver, const Quaternion q_c2b);

/**
 * @brief  角速度バイアス補正値設定関数
 * @param  mpu9250_driver  : MPU9250_Driver構造体へのポインタ
 * @param  ang_vel_bias_compo_rad_s : コンポ座標での角速度バイアス補正値(計測値からこの値を差し引く)
 * @return C2A_MATH_ERROR_RANGE_OVER : 補正値が大きすぎる場合
 */
C2A_MATH_ERROR MPU9250_set_ang_vel_bias_compo_rad_s(MPU9250_Driver* mpu9250_driver, const float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  磁場ベクトルバイアス補正値設定関数
 * @param  mpu9250_driver  : MPU9250_Driver構造体へのポインタ
 * @param  mag_bias_compo_nT : コンポ座標での磁場バイアス補正値(計測値からこの値を差し引く)
 * @return C2A_MATH_ERROR_RANGE_OVER : 補正値が大きすぎる場合
 */
C2A_MATH_ERROR MPU9250_set_mag_bias_compo_nT(MPU9250_Driver* mpu9250_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  磁場ベクトルバイアス補正値設定関数(現在値への加算)
 * @param  mpu9250_driver  : MPU9250_Driver構造体へのポインタ
 * @param  mag_bias_compo_nT : コンポ座標での磁場バイアス補正値(計測値からこの値を差し引く)
 * @return C2A_MATH_ERROR_RANGE_OVER : 補正値が大きすぎる場合
 */
C2A_MATH_ERROR MPU9250_add_mag_bias_compo_nT(MPU9250_Driver* mpu9250_driver, const float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  角速度スケールファクタ行列設定関数
 * @param  mpu9250_driver  : MPU9250_Driver構造体へのポインタ
 * @param  ang_vel_scale_factor_compo : コンポ座標でのスケールファクタ行列
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR MPU9250_set_ang_vel_scale_factor_compo(MPU9250_Driver* mpu9250_driver,
                                                      const float ang_vel_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  磁場ベクトルスケールファクタ行列設定関数
 * @param  mpu9250_driver  : MPU9250_Driver構造体へのポインタ
 * @param  mag_scale_factor_compo : コンポ座標でのスケールファクタ行列
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR MPU9250_set_mag_scale_factor_compo(MPU9250_Driver* mpu9250_driver,
                                                  const float mag_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

#endif
