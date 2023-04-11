/**
* @file   stim210.h
* @brief  STIM210ジャイロセンサのドライバ
*/

#ifndef STIM210_H_
#define STIM210_H_

#include <src_core/IfWrapper/uart.h>
#include <src_core/Drivers/Super/driver_super.h>

#include "../../IfWrapper/GPIO.h"
#include "../../Library/physical_constants.h"
#include "../../Library/quaternion.h"

/**
 * @enum   STIM210_OPERATION_MODE
 * @brief  STIM210の運用モード
 * @note   uint8_tを想定
 */
typedef enum
{
  STIM210_OPERATION_INIT_MODE = 0,
  STIM210_OPERATION_NORMAL_MODE,
  STIM210_OPERATION_SERVICE_MODE,
  STIM210_OPERATION_MODE_MAX
} STIM210_OPERATION_MODE;

/**
 * @enum   STIM210_NORMAL_MODE_FORMAT
 * @brief  ノーマルモードの出力設定
 * @note   uint8_tを想定
 */
typedef enum
{
  STIM210_NORMAL_MODE_STANDARD = 0,
  STIM210_NORMAL_MODE_EXTENDED,
  STIM210_NORMAL_MODE_RATE_TEMPERATURE,
  STIM210_NORMAL_MODE_RATE_COUNT,
  STIM210_NORMAL_MODE_RATE_LATENCY,
  STIM210_NORMAL_MODE_RATE_COUNT_LATENCY,
  STIM210_NORMAL_MODE_RATE_TEMPERATURE_COUNT,
  STIM210_NORMAL_MODE_RATE_TEMPERATURE_LATENCY,
  STIM210_NORMAL_MODE_RATE_TEMPERATURE_COUNT_LATENCY,
  STIM210_NORMAL_MODE_MAX
} STIM210_NORMAL_MODE_FORMAT;

/**
 * @enum   STIM210_GYRO_OUTPUT_MODE
 * @brief  ジャイロ出力モード
 * @note   uint8_tを想定
 */
typedef enum
{
  STIM210_GYRO_OUTPUT_ANGULAR_RATE = 0,
  STIM210_GYRO_OUTPUT_INCREMENTAL_ANGLE,
  STIM210_GYRO_OUTPUT_AVERAGE_ANGULAR_RATE,
  STIM210_GYRO_OUTPUT_INTEGRATED_ANGLE,
  STIM210_GYRO_OUTPUT_MODE_MAX
} STIM210_GYRO_OUTPUT_MODE;

/**
 * @enum   STIM210_TERMINATION_MODE
 * @brief  終端子の設定（0: OFF, 1: CRLF）
 * @note   uint8_tを想定
 */
typedef enum
{
  STIM210_TERMINATION_OFF = 0,
  STIM210_TERMINATION_CRLF,
  STIM210_TERMINATION_MODE_MAX
} STIM210_TERMINATION_MODE;

/**
 * @enum   STIM210_LPF
 * @brief  ジャイロ出力ローパスフィルタの指定可能なカットオフ周波数
 * @note   uint8_tを想定
 */
typedef enum
{
  STIM210_LPF_16HZ = 0,
  STIM210_LPF_33HZ,
  STIM210_LPF_66HZ,
  STIM210_LPF_131HZ,
  STIM210_LPF_262HZ,
  STIM210_LPF_MAX
} STIM210_LPF;

/**
 * @enum   STIM210_SAMPLE_RATE
 * @brief  ジャイロ出力のサンプルレート
 * @note   uint8_tを想定
 */
typedef enum
{
  STIM210_SAMPLE_RATE_EXTERNAL_TRIGGER = 0,
  STIM210_SAMPLE_RATE_125HZ,
  STIM210_SAMPLE_RATE_250HZ,
  STIM210_SAMPLE_RATE_500HZ,
  STIM210_SAMPLE_RATE_1000HZ,
  STIM210_SAMPLE_RATE_2000HZ,
  STIM210_SAMPLE_RATE_MAX
} STIM210_SAMPLE_RATE;

/**
 * @enum  STIM210_CRC_STATE
 * @brief
 * STIM210から送られてきたデータのCRC計算結果
 * @note  uint_8を想定
 */
typedef enum
{
  STIM210_CRC_STATE_OK = 0,
  STIM210_CRC_STATE_NG = 1
} STIM210_CRC_STATE;

/**
 * @struct STIM210_Info
 * @brief  STIM210のモードや変数などを格納する
 * @note   uint8_tを想定
 */
typedef struct
{
  float ang_vel_raw_compo_rad_sec[PHYSICAL_CONST_THREE_DIM];  //!< 補正前角速度 @ component frame [rad/s]
  float ang_vel_compo_rad_sec[PHYSICAL_CONST_THREE_DIM];      //!< 補正後角速度 @ component frame [rad/s]
  float ang_vel_body_rad_sec [PHYSICAL_CONST_THREE_DIM];      //!< 補正後角速度 @ body frame [rad/s]
  float ang_vel_bias_compo_rad_sec[PHYSICAL_CONST_THREE_DIM]; //!< 角速度バイアス @ component frame [rad/s]
  float ang_vel_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< scale factor of angular velocity @ component frame [-]

  float ang_compo_rad[PHYSICAL_CONST_THREE_DIM];           //!< 前回の観測から今回の観測までの各軸の角速度積分値 @ component frame[rad]
  float temperature_compo_degC[PHYSICAL_CONST_THREE_DIM];  //!< 温度 @ component frame [degC]
  Quaternion frame_transform_c2b;                          //!< frame transformation quaternion from component frame to body frame

  uint8_t status;     //!< ジャイロ出力のステータス (0: OK, >1: NG)
  uint8_t counter;    //!< STIM210内部のサンプル数 (2000Hz) にあわせてインクリメントされるカウンタ。
  uint16_t latency_sec;   //!< 外部トリガによってデータを下す場合の観測からの遅延時間
  uint8_t buffer[3];  //!< buffer for future use
  uint8_t crc;        //!< CRCチェック用の変数

  STIM210_OPERATION_MODE operation_mode;          //!< 運用モード
  STIM210_NORMAL_MODE_FORMAT normal_mode_format;  //!< ノーマルモード時の出力フォーマット
  STIM210_GYRO_OUTPUT_MODE gyro_output_mode;      //!< ジャイロ出力モード
  STIM210_TERMINATION_MODE termination_mode;      //!< 終端子設定
  STIM210_LPF low_pass_filter_frequency;          //!< ローパスフィルタカットオフ周波数設定
  STIM210_SAMPLE_RATE sample_rate;                //!< 出力のサンプリングレート設定
  STIM210_CRC_STATE crc_state;                    //!< 受信したCRCが正しいか

} STIM210_Info;

/**
 * @struct STIM210_Driver
 * @brief STIM210_Driver構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;        //!< DriverSuper class
    UART_Config uart_config;  //!< UART class
    uint8_t ch_gpio_trig;     //!< GPIO port for external trigger
    uint8_t ch_gpio_reset;    //!< GPIO port for sending reset signal to STIM210
  } driver;
  STIM210_Info info;
} STIM210_Driver;

// 基本関数

/**
 * @brief  STIM210初期化
 *
 *         STIM210_Driver構造体のポインタを渡すことでポートを初期化し，STIM210_Driverの各メンバも初期化する
 * @param  stim210_driver    : 初期化するSTIM210_Driver構造体へのポインタ
 * @param  ch      : STIM210が接続されているUARTポート番号
 * @param  ch_gpio_trig  : STIM210が接続されているテレメトリ送出信号送信用GPIOポート番号
 * @param  ch_gpio_reset : STIM210が接続されているReset信号送信用GPIOポート番号
 * @param  rx_buffer: 受信バッファ
 * @retval 0       : 正常終了
 * @retval 1       : DS 異常終了
 * @retval 2       : GPIO入出力設定異常終了
 * @retval 3       : GPIO初期化異常終了
 */
int STIM210_init(STIM210_Driver* stim210_driver,
                 uint8_t ch,
                 uint8_t ch_gpio_trig,
                 uint8_t ch_gpio_reset,
                 DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  STIM210のパラメータを初期値にリセットする。
 *
 *         電源OFF時のみ実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver    : STIM210_Driver構造体へのポインタ
 * @retval 0       : 正常終了
 * @retval 0以外   : 異常終了
 */
int STIM210_reset_param(STIM210_Driver* stim210_driver);

/**
 * @brief  STIM210のデータ（テレメ）受信
 *
 *         ノーマルモードのみ実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @retval DS_REC_ERR_CODEに準拠
 */
DS_REC_ERR_CODE STIM210_rec(STIM210_Driver* stim210_driver);

/**
 * @brief  GPIOを用いたSTIM210のテレメ送信
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @retval 0     : 正常終了
 * @retval 0以外 : 異常終了
 * @note   GPIO LOWを250ns以上キープする。テレメ送信は86us後に始まる。
 */
int STIM210_send_tlm_by_gpio(STIM210_Driver* stim210_driver);

/**
 * @brief  GPIOを用いたSTIM210のリセット
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @retval 0     : 正常終了
 * @retval 0以外 : 異常終了
 * @note   GPIO HIGHを1us以上キープする。パラメータもリセットする。
 */
int STIM210_reset_by_gpio(STIM210_Driver* stim210_driver);

/**
 * @brief  STIM210の運用モードをサービスモードにする
 *
 *         ノーマルモードと初期モードで実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE STIM210_set_service_mode(STIM210_Driver* stim210_driver);

/**
 * @brief  STIM210のノーマルモードでの出力フォーマットを変更する
 *
 *         サービスモードのみ実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @param  normal_mode_format : 出力フォーマット
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE STIM210_set_normal_mode_format(STIM210_Driver* stim210_driver, STIM210_NORMAL_MODE_FORMAT normal_mode_format);

/**
 * @brief  STIM210のノーマルモードのサンプルレートを変更する
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @param  sample_rate : サンプルレート
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE STIM210_set_sample_rate(STIM210_Driver* stim210_driver, STIM210_SAMPLE_RATE sample_rate);

/**
 * @brief  STIM210のノーマルモードでのジャイロ出力を変更する
 *
 *         サービスモードのみ実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @param  gyro_output_mode : ジャイロ出力
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE STIM210_set_gyro_output(STIM210_Driver* stim210_driver, STIM210_GYRO_OUTPUT_MODE gyro_output_mode);

/**
 * @brief  STIM210のノーマルモードでの終端子を変更する
 *
 *         サービスモードのみ実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @param  gyro_termination_mode : 終端子
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE STIM210_set_termination_mode(STIM210_Driver* stim210_driver, STIM210_TERMINATION_MODE gyro_termination_mode);

/**
 * @brief  STIM210のLPFのカットオフ周波数を変更する
 *
 *         サービスモードのみ実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @param  low_pass_filter_frequency : LPFのカットオフ周波数
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE STIM210_set_low_pass_filter(STIM210_Driver* stim210_driver, STIM210_LPF low_pass_filter_frequency);

/**
 * @brief  STIM210をノーマルモードに移行する
 *
 *         サービスモードのみ実行可能。それ以外の時は異常終了する。
 * @param  stim210_driver : STIM210_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE STIM210_set_normal_mode(STIM210_Driver* stim210_driver);

/**
 * @brief  座標変換行列設定関数
 * @param  stim210_driver  : STIM210_Driver構造体へのポインタ
 * @param  q_c2b            : コンポ座標からボディ座標への座標変換Quaternion
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR STIM210_set_frame_transform_c2b(STIM210_Driver* stim210_driver, const Quaternion q_c2b);

/**
 * @brief  角速度バイアス補正値設定関数
 * @param  stim210_driver  : STIM210_Driver構造体へのポインタ
 * @param  ang_vel_bias_compo_rad_s : コンポ座標での角速度バイアス補正値(計測値からこの値を差し引く)
 * @return C2A_MATH_ERROR_RANGE_OVER : 補正値が大きすぎる場合
 */
C2A_MATH_ERROR STIM210_set_ang_vel_bias_compo_rad_s(STIM210_Driver* stim210_driver, const float ang_vel_bias_compo_rad_s[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  角速度スケールファクタ行列設定関数
 * @param  stim210_driver  : STIM210_Driver構造体へのポインタ
 * @param  ang_vel_scale_factor_compo : コンポ座標でのスケールファクタ行列
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR STIM210_set_ang_vel_scale_factor_compo(STIM210_Driver* stim210_driver,
                                                      const float ang_vel_scale_factor_compo[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

#endif
