/**
* @file   oem7600.h
* @brief  OEM7600 GPS受信機のドライバ
*/

#ifndef OEM7600_H_
#define OEM7600_H_

#include <src_core/IfWrapper/uart.h>
#include "../../IfWrapper/GPIO.h"
#include <src_core/Drivers/Super/driver_super.h>
#include <src_core/System/TimeManager/obc_time.h>

#define OEM7600_MAX_SAT_NUM_RANGE_STORE (12) //!< rangeテレメの格納対象とする同時可視最大衛星数

/**
* @enum  OEM7600_DATA_UPDATE_STATUS
* @brief レンジデータが最新値か（可視衛星不足により）過去値が残ったままか．
* @note  uint_8を想定
*/
typedef enum
{
  OEM7600_DATA_NOT_UPDATED = 0,
  OEM7600_DATA_UPDATED     = 1
} OEM7600_DATA_UPDATE_STATUS;

/**
 * @enum   OEM7600_TLM_IDX
 * @brief  OEM7600からのTLM内についている，TLMの内容を識別するためのIDの定義
 * @note  （基本的には）uint32_tを想定
 */
typedef enum
{
  // hwmonitor tlm内部の識別ID
  OEM7600_TLM_ID_TEMPERATURE     = 0x00000001,
  OEM7600_TLM_ID_ANTENNA_CURRENT = 0x00000002,
  OEM7600_TLM_ID_3V3_VOLTAGE     = 0x00000006,
  OEM7600_TLM_ID_ANTENNA_VOLTAGE = 0x00000007,
  OEM7600_TLM_ID_SUPPLY_VOLTAGE  = 0x0000000F,
  // binary format系列
  OEM7600_TLM_ID_RANGE            = 0x0000002B, // cmd/log manual 3.144項
  OEM7600_TLM_ID_BEST_XYZ         = 0x000000F1, // cmd/log manual 3.21項
  OEM7600_TLM_ID_HARDWARE_MONITOR = 0x000003C3, // cmd/log manual 3.76項
  // 仕様書上はbinary/asciiで同一IDだが、CMD引数にIDを使うために、binary系列ID + 0x1000をascii系列として定義する
  OEM7600_TLM_ID_RANGE_ASCII            = OEM7600_TLM_ID_RANGE + 0x1000,
  OEM7600_TLM_ID_BEST_XYZ_ASCII         = OEM7600_TLM_ID_BEST_XYZ + 0x1000,
  OEM7600_TLM_ID_HARDWARE_MONITOR_ASCII = OEM7600_TLM_ID_HARDWARE_MONITOR + 0x1000

} OEM7600_TLM_ID;

/**
 * @struct OEM7600_Time
 * @brief OEM7600が提供する時刻情報
 */
typedef struct
{
  // TODO_L: 時刻とPPS利用ユースケースが確定し次第，それらに合わせて定義変更
  cycle_t pps_received_aobc_time;  //!< GPSRからのPPS (Pulse Per Second) を受け取った時のAOBC内TI
  cycle_t tlm_received_aobc_time;  //!< GPSRからのテレメデータを受け取った時のAOBC内TI (DHとのIF用)
  ObcTime  obct_gps_time_obs;      //!< GPSRからのテレメデータを受け取った時のC2Aマスタークロック [-] (AOCS内部用)
  uint16_t gps_time_week;          //!< GPS時刻のうち，週番号 [week]
  uint32_t gps_time_ms;            //!< GPS時刻のうち，週内の経過秒 [msec]
  float time_from_last_fix_s;      //!< 現在の最新航法解が得られたタイミングからの経過時間 (受信機出力) [sec]
} OEM7600_Times;

/**
 * @struct OEM7600_Hardware_Status
 * @brief OEM7600受信機ハードウェア状態の詳細テレメ
 */
typedef struct
{
  float temperature_degC;  //!< reciever temperature [degC]
  float antenna_current_A; //!< antenna current [A]
  float voltage_3v3_V;     //!< 3V3 voltage regulated in the receiver and delivered mainly to the receiver's processor [V]
  float antenna_voltage_V; //!< antenna voltage [V]
  float supply_voltage_V;  //!< supply voltage from power source [V]
} OEM7600_HardwareStatus;

/**
 * @struct OEM7600_RangeData
 * @brief OEM7600が提供するRANGEテレメデータ構造体
 */
typedef struct
{
  uint16_t prn_slot;                          //!< Satellite PRN number of range measurement
  double   pseudo_range_m;                    //!< Pseudorange measurement [m]
  float    pseudo_range_std_deviation_m;      //!< Pseudorange measurement standard deviation [m]
  double   carrier_phase_cycle;               //!< Carrier phase [cycles] (accumulated Doppler range)
  float    carrier_phase_std_deviation_cycle; //!< Estimated carrier phase standard deviation [cycles]
  float    carrier_doppler_Hz;                //!< Instantaneous carrier Doppler frequency [Hz]
  float    carrier_to_noise_ratio_dB_Hz;      //!< Carrier to noise density ratio [dB-Hz]
  float    continuous_tracking_time_sec;      //!< Number of seconds of continuous tracking (no cycle slipping)
  uint32_t tracking_status;                   //!< Tracking status (see cmd/log manual p704)
  OEM7600_DATA_UPDATE_STATUS  is_updated;     //!< データ更新の有無(可視衛星数が少ない時に，バッファに残った過去データを識別するため)
} OEM7600_RangeData;

/**
 * @enum  OEM7600_CRC_STATE
 * @brief
 * OEM7600から送られてきたデータのCRC計算結果
 * @note  uint_8を想定
 */
typedef enum
{
  OEM7600_CRC_STATE_OK = 0,
  OEM7600_CRC_STATE_NG = 1
} OEM7600_CRC_STATE;

/**
 * @struct OEM7600_Info
 * @brief  OEM7600のテレメトリ情報
 */
typedef struct
{
  double pos_antenna_ecef_m[3];      //!< position of GPS antenna @ ECEF frame [m]
  double vel_antenna_ecef_m_s[3];    //!< velocity of GPS antenna @ ECEF frame [m/s]
  uint8_t  tlm_receive_counter;      //!< テレメデータ受信回数カウンタ
  uint8_t  num_of_visible_sats;      //!< 可視衛星数
  uint32_t receiver_status_summary;  //!< 異常有無等の受信機状態サマリ
  uint32_t uart_baudrate;
  OEM7600_Times times;
  OEM7600_HardwareStatus hardware_status;
  OEM7600_RangeData range_tlm[OEM7600_MAX_SAT_NUM_RANGE_STORE];
  OEM7600_CRC_STATE crc_state;       //!< 受信したCRCが正しいか
  // range_tlm受信用特殊パラメータ
  struct oem7600
  {
    uint8_t is_receiving;             //!< flag to check whether the driver is under range tlm receiving or not
    uint16_t recieved_tlm_len;        //!< length of the range data stored to the range tlm buffer
    uint16_t expected_data_length;    //!< expected data length of range tlm sentence under receiving
    uint16_t range_tlm_wait_counter;  //!< range tlmの受信処理call回数（閾値以上になった場合、range tlmの受信処理をリセット）
  } range_tlm_status;
} OEM7600_Info;

/**
 * @struct OEM7600_Driver
 * @brief  OEM7600_Driver構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;       //!< DriverSuper class
    UART_Config uart_config;  //!< UART class
    uint8_t ch_gpio_pps;     //!< GPIO port for receiving PPS (Pulse Per Seconds)
    uint8_t ch_gpio_reset;   //!< GPIO port for sending reset signal to OEM7600
  } driver;
  OEM7600_Info info;
} OEM7600_Driver;

// 基本関数

/**
 * @brief  OEM7600初期化
 *
 *         OEM7600_Driver構造体のポインタを渡すことでポートを初期化し，OEM7600_Driverの各メンバも初期化する
 * @param  oem7600     : 初期化するOEM7600_Driver構造体へのポインタ
 * @param  ch_uart      : OEM7600が接続されているUARTポート番号
 * @param  ch_gpio_pps  : OEM7600が接続されているPPS信号受信用GPIOポート番号
 * @param  ch_gpio_reset: OEM7600が接続されているReset信号送信用GPIOポート番号
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE OEM7600_init(OEM7600_Driver* oem7600_driver,
                              uint8_t ch_uart,
                              uint8_t ch_gpio_pps,
                              uint8_t ch_gpio_reset,
                              DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  OEM7600からのテレメデータ受信
 * @param  oem7600 : OEM7600_Driver構造体へのポインタ
 * @retval DS_REC_ERR_CODEに準拠
 */
DS_REC_ERR_CODE OEM7600_rec(OEM7600_Driver* oem7600_driver);


/**
 * @brief  OEM7600付属アンテナの電源操作コマンドパケット送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @param  onoff:    on(1)/off(0)の指定
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_onoff_antenna_power(OEM7600_Driver* oem7600_driver, const uint8_t onoff);

/**
 * @brief  OEM7600のsoftwareリセットコマンドパケット送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @param  time_for_reset_sec:コマンド送信からリセットまでの間の猶予時間 [sec]
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_software_reset(OEM7600_Driver* oem7600_driver, const uint8_t time_for_reset_sec);

/**
 * @brief  OEM7600のsoftwareリセットコマンドパケット送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @param  high_low: リセット用GPIOのHigh/Lowの指定
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_set_reset_gpio_high_low(OEM7600_Driver* oem7600_driver, const GPIO_HL high_low);

/**
 * @brief  OEM7600の出力TLM設定コマンドパケット送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @param  *tlm_name:TLMの名称 (ASCII)
 * @param  name_length: TLMの名称の文字数
 * @param  out_interval_sec:出力頻度 (seconds)
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_set_tlm_contents(OEM7600_Driver* oem7600_driver, const uint32_t oem7600_tlm_id, const uint8_t out_interval_sec);

/**
 * @brief  OEM7600の出力TLM設定保存コマンドパケット送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_save_tlm_setting(OEM7600_Driver* oem7600_driver);

/**
 * @brief  OEM7600のUART伝送速度設定コマンドパケット送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @param  uint32_t: baudrate
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_set_uart_baudrate(OEM7600_Driver* oem7600_driver, const uint32_t baudrate, DS_StreamRecBuffer* rx_buffer);


/**
 * @brief  OEM7600のレンジテレメ取得開始コマンド送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_start_rec_range_tlm(OEM7600_Driver* oem7600_driver);


/**
 * @brief  OEM7600のレンジテレメ取得終了コマンド送信処理
 * @param  oem7600: OEM7600_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE OEM7600_end_rec_range_tlm(OEM7600_Driver* oem7600_driver);

#endif
