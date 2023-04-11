/**
* @file   ina260.h
* @brief  INA260 電流電圧センサのドライバ
*/

#ifndef INA260_H_
#define INA260_H_

#include <src_core/IfWrapper/i2c.h>
#include <src_core/Drivers/Super/driver_super.h>


/**
 * @enum  INA260_CONVERSION_TIME
 * @brief 電流・電圧共通変換時間(仕様書参照)
 * @note  型はuint8_tを想定
 */
typedef enum
{
  INA260_CONVERSION_TIME_140US = 0,
  INA260_CONVERSION_TIME_204US,
  INA260_CONVERSION_TIME_332US,
  INA260_CONVERSION_TIME_588US,
  INA260_CONVERSION_TIME_1MS,
  INA260_CONVERSION_TIME_2MS,
  INA260_CONVERSION_TIME_4MS,
  INA260_CONVERSION_TIME_8MS,
  INA260_CONVERSION_TIME_MAX,
} INA260_CONVERSION_TIME;

/**
 * @enum  INA260_AVERAGING_MODE
 * @brief 電流・電圧共通平均化モード設定(数値は平均化要素数)
 * @note  型はuint8_tを想定
 */
typedef enum
{
  INA260_AVERAGING_MODE_1 = 0,
  INA260_AVERAGING_MODE_4,
  INA260_AVERAGING_MODE_16,
  INA260_AVERAGING_MODE_64,
  INA260_AVERAGING_MODE_128,
  INA260_AVERAGING_MODE_256,
  INA260_AVERAGING_MODE_512,
  INA260_AVERAGING_MODE_1024,
  INA260_AVERAGING_MODE_MAX,
} INA260_AVERAGING_MODE;


/**
 * @struct INA260_Info
 * @brief  INA260のテレメトリ情報
 *
 *         TLM用に生値も持たせている
 */
typedef struct
{
  // observed value
  float current_mA;          //!< current [mA]
  float voltage_V;           //!< voltage [V]
  int16_t  current_raw;      //!< current raw value
  uint16_t voltage_raw;      //!< voltage raw value

  // setting value
  uint16_t oc_threshold_raw;                      //!< over current threshold raw value
  INA260_AVERAGING_MODE  averaging_mode;          //!< averaging mode for observation
  INA260_CONVERSION_TIME voltage_conversion_time; //!< voltage conversion time
  INA260_CONVERSION_TIME current_conversion_time; //!< current conversion time
} INA260_Info;

/**
 * @struct INA260_Driver
 * @brief INA260の観測データやパラメータを格納する構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;      //!< DriverSuper class
    I2C_Config i2c_config;  //!< I2C class
  } driver;
  INA260_Info info;
} INA260_Driver;

// 基本関数

/**
 * @brief  INA260初期化
 *
 *         INA260_Driver構造体のポインタを渡すことでポートを初期化し，INA260_Driverの各メンバも初期化する
 * @param  ina260_driver : 初期化するINA260_Driver構造体へのポインタ
 * @param  ch             : INA260が接続されているI2Cポート番号
 * @param  i2c_address    : INA260のI2Cデバイスアドレス
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE INA260_init(INA260_Driver* ina260_driver, uint8_t ch, uint8_t i2c_address, DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  INA260モード設定コマンド
 * @param  ina260_driver : INA260_Driver構造体へのポインタ
 * @param  mode           : INA260に設定する各種モード情報を束ねたもの
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE INA260_set_mode(INA260_Driver* ina260_driver,
                                INA260_AVERAGING_MODE averaging_mode,
                                INA260_CONVERSION_TIME voltage_conversion_time,
                                INA260_CONVERSION_TIME current_conversion_time);

/**
 * @brief  INA260過電流閾値設定コマンド
 * @param  ina260_driver : INA260_Driver構造体へのポインタ
 * @param  mode           : INA260に設定する過電流閾値[mA]
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE INA260_set_over_current_threshold(INA260_Driver* ina260_driver, float threshold_current_mA);

/**
 * @brief  INA260過電流保護有効化コマンド
 * @param  ina260_driver : INA260_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE INA260_enable_over_current_protection(INA260_Driver* ina260_driver);

/**
 * @brief  INA260電流観測コマンド
 * @param  ina260_driver : INA260_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE INA260_observe_current(INA260_Driver* ina260_driver);

/**
 * @brief  INA260電圧観測コマンド
 * @param  ina260_driver : INA260_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE INA260_observe_voltage(INA260_Driver* ina260_driver);


/**
 * @brief  INA260マスクレジスタの読み込み
 *
 *         読むことでOCラッチを外すことができる
 * @param  ina260_driver : INA260_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE INA260_read_mask_register(INA260_Driver* ina260_driver);

#endif
