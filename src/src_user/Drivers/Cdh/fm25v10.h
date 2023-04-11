/**
* @file
* @brief  FM25V10 FRAMのドライバ
*/

#ifndef FM25V10_H_
#define FM25V10_H_

#include <src_core/IfWrapper/spi.h>
#include <src_core/Drivers/Super/driver_super.h>

#define FM25V10_STOP_ADDRESS (0x20000) //!< FM25V10のSTOPアドレス(排他範囲)
#define FM25V10_MAX_LENGTH  (128)      //!< 読み書きする最大データ数（実行速度などで決める）
/**
 * @struct FM25V10_Info
 * @brief  FM25V10のテレメトリ情報
 */
typedef struct
{
  uint8_t  status;               //!< FM25V10 status registerの情報
  uint32_t last_start_address;   //!< 最後に読み書きアクセスした先頭アドレス
  uint8_t  last_read_data_byte;  //!< 最後に読み取った先頭アドレスの1Byteデータ
  uint8_t  last_write_data_byte; //!< 最後に書き込んだ先頭アドレスの1Byteデータ
} FM25V10_Info;

/**
 * @struct FM25V10_Info
 * @brief  FM25V10ドライバの構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;      //!< DriverSuper class
    SPI_Config spi_config;  //!< SPI class
  } driver;
  FM25V10_Info info;
} FM25V10_Driver;

// 基本関数
/**
 * @brief  FM25V10初期化
 *
 *         FM25V10_Driver構造体のポインタを渡すことでポートを初期化し，FM25V10_Driverの各メンバも初期化する
 * @param  fm25v10_driver : 初期化するFM25V10_Driver構造体へのポインタ
 * @param  comm_ch         : FM25V10が接続されているSPIポート番号
 * @param  gpio_ch         : FM25V10のChip SelectGPIOポート番号
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE FM25V10_init(FM25V10_Driver* fm25v10_driver, uint8_t comm_ch, uint8_t gpio_ch, DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  FM25V10のステータスレジスタへの書き込み
 * @param[in]  fm25v10_driver : 初期化するFM25V10_Driver構造体へのポインタ
 * @param[in]  status          : 書き込むステータス
 * @return                     : DS_CMD_ERR_CODEに従う
 */
DS_CMD_ERR_CODE FM25V10_write_status(FM25V10_Driver* fm25v10_driver, const uint8_t status);

/**
 * @brief  FM25V10のステータスレジスタの読み出し
 * @param[in]  fm25v10_driver : 初期化するFM25V10_Driver構造体へのポインタ
 * @return                     : DS_CMD_ERR_CODEに従う
 */
DS_CMD_ERR_CODE FM25V10_read_status(FM25V10_Driver* fm25v10_driver);

/**
 * @brief  FM25V10への複数Byteのデータの書き込み
 * @param[in]  fm25v10_driver : 初期化するFM25V10_Driver構造体へのポインタ
 * @param[in]  start_address   : 書き込み先先頭アドレス
 * @param[in]  write_bytes     : 書き込むデータ
 * @param[in]  length          : 書き込むデータのデータ長
 * @return                     : DS_CMD_ERR_CODEに従う
 */
DS_CMD_ERR_CODE FM25V10_write_bytes(FM25V10_Driver* fm25v10_driver, const uint32_t start_address,
                                    const uint8_t* write_bytes, const uint16_t length);

/**
 * @brief  FM25V10からの複数Byteのデータ読み出し
 * @param[in]  fm25v10_driver : 初期化するFM25V10_Driver構造体へのポインタ
 * @param[in]  start_address   : 書き込み先先頭アドレス
 * @param[out] read_bytes      : 読み出したデータを書き込む先
 * @param[in]  length          : 読み出すデータのデータ長
 * @return                     : DS_CMD_ERR_CODEに従う
 */
DS_CMD_ERR_CODE FM25V10_read_bytes(FM25V10_Driver* fm25v10_driver, const uint32_t start_address,
                                   uint8_t* read_bytes, const uint16_t length);

#endif
