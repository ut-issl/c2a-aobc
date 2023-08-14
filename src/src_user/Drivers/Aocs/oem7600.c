#pragma section REPRO
/**
* @file   oem7600.c
* @brief  OEM7600のDriver
*/

#include "oem7600.h"
#include <src_user/Library/physical_constants.h>
#include <string.h> // for memcpy
#include <stdio.h>  // for snprintf

// #define DRIVER_OEM7600_DEBUG_SHOW_REC_DATA
// #define DRIVER_OEM7600_DEBUG_SHOW_SEND_DATA
// #define DRIVER_OEM7600_DEBUG_SHOW_GPIO_DATA
// #define DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF

#define OEM7600_RX_CRC_SIZE          (4)    //!< CRC size
#define OEM7600_RX_MAX_FRAME_SIZE    (144)  //!< 1回の受信テレメサイズ最大値 (暫定値)
#define OEM7600_TX_MAX_FLAME_SIZE    (36)   //!< 1回の送信コマンドサイズ最大値
#define OEM7600_STREAM_TLM_CMD       (0)    //!< テレコマで使うストリーム
#define OEM7600_FRAME_LENGTH_POS     (8)    //!< 可変長テレメデータのうち、フレームサイズデータの出現位置 (先頭からのbyte数)
#define OEM7600_COMMON_HEADER_SIZE   (28)   //!< 上記フレームサイズデータに含まれない、固定長部分のテレメサイズ
#define OEM7600_RANGE_TLM_FULL_SIZE  (564)  //!< max length of range tlm data

// TODO_L : NMEAを使うかどうかも含め要検討．(NMEA形式のTLMを受信したい場合，2種類のヘッダが混在することになる．)
static const uint8_t OEM7600_rx_header_[] = { 0xAA, 0x44, 0x12 };    //!< 受信ヘッダ, フッタはなし．
static const uint8_t OEM7600_tx_footer_[] = { 0x0A };                //!< 送信フッタ ('LF')

static uint8_t OEM7600_tx_frame_[OEM7600_TX_MAX_FLAME_SIZE]; //!< 送信フレーム

// range tl,
#define OEM7600_RANGE_TLM_IF_RX_BUFFER_SIZE  (512)    //!< 一度に IF_RX するサイズ．これが大きいほど，ボーレートをあげられるがメモリを消費する
static uint8_t OEM7600_range_tlm_if_rx_buffer_[OEM7600_RANGE_TLM_IF_RX_BUFFER_SIZE];
static uint8_t OEM7600_range_tlm_buff_[OEM7600_RANGE_TLM_FULL_SIZE]; //!< buffer for storing range tlm data

static DS_ERR_CODE OEM7600_load_driver_super_init_settings_(DriverSuper* p_super);
static DS_ERR_CODE OEM7600_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver);
static DS_ERR_CODE OEM7600_analyze_rec_data_for_range_tlm_(DS_StreamConfig* p_stream_config, void* p_driver);
static DS_CMD_ERR_CODE   OEM7600_send_cmd_(OEM7600_Driver* oem7600_driver, const size_t cmd_length, const char* cmd_payload);
// TODO_L : CRC計算はいずれsrc_core/Library/CRC.h/.cに移動した方が良いのかも(今はCRC32は非対応っぽいので一旦ここで実装)．要相談？
static uint32_t OEM7600_calculate_crc32_(const uint8_t* tlm_data_for_crc, const size_t data_length);  //!< CRC計算処理
static uint32_t OEM7600_calculate_crc32_subroutine_(const uint32_t initial_value);                    //!< CRC計算処理サブルーチン

// ###### テレメデコード作業用関数 ######
//!< 受信機の全テレメに共通して付いている先頭データ部分の解釈処理
static uint16_t OEM7600_analyze_common_header_tlm_(OEM7600_Driver* oem7600_driver, const uint8_t* tlm_payload);
//!< カルテシアン位置・速度テレメの解釈処理
static DS_ERR_CODE OEM7600_analyze_bestxyz_tlm_(OEM7600_Driver* oem7600_driver, const uint8_t* tlm_payload, const uint16_t tlm_length);
//!< 受信機ハードウェア詳細ステータステレメの解釈処理
static DS_ERR_CODE OEM7600_analyze_hardware_status_tlm_(OEM7600_Driver* oem7600_driver,
                                                                  const uint8_t* tlm_payload, const uint16_t tlm_length);
//!< RANGEテレメの解釈処理
static DS_ERR_CODE OEM7600_analyze_range_tlm_(OEM7600_Driver* oem7600_driver, const uint8_t* tlm_payload, const uint16_t tlm_length);
//!< テレメのIDからコマンド用のテレメ文字列を生成する関数
static uint8_t convert_tlm_id_to_tlm_name_(char* copy_target, const uint32_t oem7600_tlm_id);

static DS_ERR_CODE OEM7600_store_range_tlm_chunk_(OEM7600_Driver* oem7600_driver);
static DS_ERR_CODE OEM7600_store_range_tlm_chunk_pickup_(OEM7600_Driver* oem7600_driver, uint16_t if_rec_len, uint16_t* p_rx_buffer_read_pos);


DS_INIT_ERR_CODE OEM7600_init(OEM7600_Driver* oem7600_driver,
                              uint8_t ch_uart,
                              uint8_t ch_gpio_pps,
                              uint8_t ch_gpio_reset,
                              DS_StreamRecBuffer* rx_buffer)
{
  DS_ERR_CODE ret_driver_super_setting;
  GPIO_ERR_CODE ret_gpio_pps_setting, ret_gpio_reset_setting;

  oem7600_driver->info.uart_baudrate = 115200; // cmd可変

  oem7600_driver->driver.uart_config.ch = ch_uart;
  oem7600_driver->driver.uart_config.baudrate = oem7600_driver->info.uart_baudrate;
  oem7600_driver->driver.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  oem7600_driver->driver.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  oem7600_driver->driver.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  oem7600_driver->driver.ch_gpio_pps   = ch_gpio_pps;
  oem7600_driver->driver.ch_gpio_reset = ch_gpio_reset;

  // 受信機側に合わせ，1980/1/5 midnight (GPS時刻のゼロ点) で受信機時刻を初期化  TODO_L : ユースケースに合わせてベターな初期値を定める
  oem7600_driver->info.times.gps_time_week = 0;
  oem7600_driver->info.times.gps_time_ms   = 0;
  oem7600_driver->info.times.pps_received_aobc_time = 0;
  oem7600_driver->info.times.tlm_received_aobc_time = 0;
  oem7600_driver->info.times.time_from_last_fix_s   = 0.0f;
  oem7600_driver->info.tlm_receive_counter = 0;
  oem7600_driver->info.crc_state = OEM7600_CRC_STATE_OK;
  oem7600_driver->info.times.obct_gps_time_obs = OBCT_create(0, 0, 0);
  oem7600_driver->info.range_tlm_status.is_receiving = 0;
  oem7600_driver->info.range_tlm_status.received_tlm_len = 0;
  oem7600_driver->info.range_tlm_status.expected_data_length = 0;
  oem7600_driver->info.range_tlm_status.range_tlm_wait_counter = 0;

  for (uint8_t sat_id = 0; sat_id < OEM7600_MAX_SAT_NUM_RANGE_STORE; sat_id++)
  {
    oem7600_driver->info.range_tlm[sat_id].is_updated = OEM7600_DATA_NOT_UPDATED;
  }

  ret_driver_super_setting = DS_init(&(oem7600_driver->driver.super),
                                     &(oem7600_driver->driver.uart_config),
                                     rx_buffer,
                                     OEM7600_load_driver_super_init_settings_);
  ret_gpio_pps_setting     = (GPIO_ERR_CODE)GPIO_set_direction(ch_gpio_pps, GPIO_INPUT);
  ret_gpio_reset_setting   = (GPIO_ERR_CODE)GPIO_set_direction(ch_gpio_reset, GPIO_OUTPUT);
  ret_gpio_reset_setting   = (GPIO_ERR_CODE)GPIO_set_output(oem7600_driver->driver.ch_gpio_reset, GPIO_LOW); // LOWで初期化する

  if ((ret_driver_super_setting != DS_ERR_CODE_OK) ||
     ((ret_gpio_pps_setting != GPIO_OK) ||
     (ret_gpio_reset_setting != GPIO_OK)))
  {
#ifdef DRIVER_OEM7600_DEBUG_SHOW_GPIO_DATA
    Printf("GPIO ERR CODE: PPS, %d, RESET, %d \n", ret_gpio_pps_setting, ret_gpio_reset_setting);
#endif
    return DS_INIT_DS_INIT_ERR;
  }
  return DS_INIT_OK;
}


static DS_ERR_CODE OEM7600_load_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* p_stream_config;

  p_super->interface = UART;

  // streamは0のみ
  p_stream_config = &(p_super->stream_config[OEM7600_STREAM_TLM_CMD]);

  DSC_set_rx_buffer_size_in_if_rx(p_super, DS_IF_RX_BUFFER_SIZE_OEM7600);

  DSSC_enable(p_stream_config);

  // 送信はする
  DSSC_set_tx_frame(p_stream_config, OEM7600_tx_frame_);  // 送る直前に中身をmemcpyする
  DSSC_set_tx_frame_size(p_stream_config, 0);             // 送る直前に値をセットする

  // 定期的な受信はする
  DSSC_set_rx_header(p_stream_config, OEM7600_rx_header_, (uint16_t)sizeof(OEM7600_rx_header_));
  DSSC_set_rx_frame_size(p_stream_config, -1);    // 可変
  DSSC_set_rx_framelength_pos(p_stream_config, OEM7600_FRAME_LENGTH_POS);
  DSSC_set_rx_framelength_type_size(p_stream_config, 1); // uint_8でカウント
  DSSC_set_rx_framelength_offset(p_stream_config, OEM7600_COMMON_HEADER_SIZE + OEM7600_RX_CRC_SIZE);
  DSSC_set_data_analyzer(p_stream_config, OEM7600_analyze_rec_data_);

  // 定期TLMの監視機能の有効化しない → ので設定上書きなし

  return DS_ERR_CODE_OK;
}


DS_REC_ERR_CODE OEM7600_rec(OEM7600_Driver* oem7600_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;

  if (oem7600_driver->info.range_tlm_status.is_receiving)
  {
    ret = OEM7600_store_range_tlm_chunk_(oem7600_driver);
    if (ret != DS_ERR_CODE_OK) return DS_REC_OTHER_ERR;

    return DS_REC_OK;
  }

  ret = DS_receive(&(oem7600_driver->driver.super));
  if (ret != DS_ERR_CODE_OK) return DS_REC_DS_RECEIVE_ERR;

  p_stream_config = &(oem7600_driver->driver.super.stream_config[OEM7600_STREAM_TLM_CMD]);

  if (DSSC_get_rec_status(p_stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME) return DS_REC_OK;

  ret = DS_analyze_rec_data(&(oem7600_driver->driver.super), OEM7600_STREAM_TLM_CMD, oem7600_driver);

  if (ret != DS_ERR_CODE_OK) return DS_REC_ANALYZE_ERR;

  return DS_REC_OK;
}


// CMD関数の出力はすべて文字列に変換するので、memcopy使用時もbyteオーダーは不問
DS_CMD_ERR_CODE OEM7600_onoff_antenna_power(OEM7600_Driver* oem7600_driver, const uint8_t onoff)
{
  const char    kCmdPrefixChar[]     = "antennapower ";
  const uint8_t kCmdPrefixCharLength = (uint8_t)(sizeof(kCmdPrefixChar) / sizeof(kCmdPrefixChar[0])) -1; // NULL文字分を消す
  const char    kCmdOffChar[]        = "off";
  const uint8_t kCmdOffCharLength    = (uint8_t)(sizeof(kCmdOffChar) / sizeof(kCmdOffChar[0])) - 1;      // NULL文字分を消す
  const char    kCmdOnChar[]         = "on";
  const uint8_t kCmdOnCharLength     = (uint8_t)(sizeof(kCmdOnChar) / sizeof(kCmdOnChar[0])) - 1;        // NULL文字分を消す
  char    cmd_payload[16];
  uint8_t current_cmd_length = 0;

  memcpy(&(cmd_payload), &(kCmdPrefixChar), (size_t)(kCmdPrefixCharLength));
  current_cmd_length = kCmdPrefixCharLength;

  if (onoff == 0)
  {
    memcpy(&(cmd_payload[current_cmd_length]), &(kCmdOffChar), (size_t)(kCmdOffCharLength));
    current_cmd_length += kCmdOffCharLength;
  }
  else
  {
    memcpy(&(cmd_payload[current_cmd_length]), &(kCmdOnChar), (size_t)(kCmdOnCharLength));
    current_cmd_length += kCmdOnCharLength;
  }

  return OEM7600_send_cmd_(oem7600_driver, (size_t)(current_cmd_length), cmd_payload);
}


// CMD関数の出力はすべて文字列に変換するので、memcopy使用時もbyteオーダーは不問
DS_CMD_ERR_CODE OEM7600_software_reset(OEM7600_Driver* oem7600_driver, const uint8_t time_for_reset_sec)
{
  const char kCmdPrefixChar[]     = "reset ";
  const uint8_t kCmdPrefixCharLength = (uint8_t)(sizeof(kCmdPrefixChar) / sizeof(kCmdPrefixChar[0])) - 1; // NULL文字分を消す
  char  cmd_payload[8];
  uint8_t current_cmd_length = 0;
  uint8_t time_for_reset_sec_char_length    = 0;

  // 3桁以上の数字が来ないようにDIで呼び出す際にアサーション済みだがいちおうガード
  if (time_for_reset_sec > 90) return DS_CMD_ILLEGAL_PARAMETER;

  memcpy(&(cmd_payload), &(kCmdPrefixChar), (size_t)(kCmdPrefixCharLength));
  current_cmd_length = kCmdPrefixCharLength;

  // snprintfはNULL文字が強制的に書き込まれるので、文字数+1個分の長さの確保が必要
  time_for_reset_sec_char_length = (time_for_reset_sec > 9) ? 3 : 2;
  snprintf(&(cmd_payload[current_cmd_length]), (size_t)(time_for_reset_sec_char_length), "%d", time_for_reset_sec);
  current_cmd_length += time_for_reset_sec_char_length - 1; // NULL文字分を消す

  return OEM7600_send_cmd_(oem7600_driver, (size_t)(current_cmd_length), cmd_payload);
}


DS_CMD_ERR_CODE OEM7600_set_reset_gpio_high_low(OEM7600_Driver* oem7600_driver, const GPIO_HL high_low)
{
  GPIO_ERR_CODE ret_gpio_reset_setting;

#ifdef DRIVER_OEM7600_DEBUG_SHOW_GPIO_DATA
  Printf("GPIO PARAM %d \n", high_low);
#endif

  if ((high_low != GPIO_HIGH) && (high_low != GPIO_LOW))
  {
#ifdef DRIVER_OEM7600_DEBUG_SHOW_GPIO_DATA
    Printf("GPIO CMD PARAMETER ERR \n");
#endif
    return DS_CMD_ILLEGAL_PARAMETER;
  }

  ret_gpio_reset_setting = (GPIO_ERR_CODE)GPIO_set_output(oem7600_driver->driver.ch_gpio_reset, high_low);

  if (ret_gpio_reset_setting != GPIO_OK)
  {
#ifdef DRIVER_OEM7600_DEBUG_SHOW_GPIO_DATA
    Printf("GPIO ERR CODE: %d \n", ret_gpio_reset_setting);
#endif
    return DS_CMD_DRIVER_SUPER_ERR;
  }

#ifdef DRIVER_OEM7600_DEBUG_SHOW_GPIO_DATA
  Printf("GPIO ERR NONE \n");
#endif

  return DS_CMD_OK;
}


// CMD関数の出力はすべて文字列に変換するので、memcopy使用時もbyteオーダーは不問
DS_CMD_ERR_CODE OEM7600_set_tlm_contents(OEM7600_Driver* oem7600_driver, const uint32_t oem7600_tlm_id, const uint8_t out_interval_sec)
{
  const char    kCmdPrefixChar[]     = "log com1 ";
  const uint8_t kCmdPrefixCharLength = (uint8_t)(sizeof(kCmdPrefixChar) / sizeof(kCmdPrefixChar[0])) - 1; // NULL文字分を消す
  const char    kCmdOnceChar[]       = "once";
  const uint8_t kCmdOnceCharLength   = (uint8_t)(sizeof(kCmdOnceChar) / sizeof(kCmdOnceChar[0])) - 1;     // NULL文字分を消す
  const char    kCmdOntimeChar[]     = "ontime ";
  const uint8_t kCmdOntimeCharLength = (uint8_t)(sizeof(kCmdOntimeChar) / sizeof(kCmdOntimeChar[0])) - 1; // NULL文字分を消す
  uint8_t current_cmd_length = 0;
  uint8_t out_interval_sec_char_length = 0;
  char cmd_payload[OEM7600_TX_MAX_FLAME_SIZE];
  char tlm_name_buffer[OEM7600_TX_MAX_FLAME_SIZE];

  memcpy(&(cmd_payload), &(kCmdPrefixChar), (size_t)(kCmdPrefixCharLength));
  current_cmd_length = kCmdPrefixCharLength;

  uint8_t tlm_name_length = convert_tlm_id_to_tlm_name_(&(tlm_name_buffer[0]), oem7600_tlm_id);
  if (tlm_name_length == 0) return DS_CMD_ILLEGAL_PARAMETER;

  // 3桁以上の数字が来ないようにDIで呼び出す際にアサーション済みだがいちおうガード
  if (out_interval_sec > 99) return DS_CMD_ILLEGAL_PARAMETER;

  memcpy(&(cmd_payload[current_cmd_length]), &(tlm_name_buffer), (size_t)(tlm_name_length));
  current_cmd_length += tlm_name_length;

  cmd_payload[current_cmd_length] = 0x20; // 半角スペース
  current_cmd_length += 1;

  if (out_interval_sec == 0)
  {
    memcpy(&(cmd_payload[current_cmd_length]), &(kCmdOnceChar), (size_t)(kCmdOnceCharLength));
    current_cmd_length += kCmdOnceCharLength;
  }
  else
  {
    memcpy(&(cmd_payload[current_cmd_length]), &(kCmdOntimeChar), (size_t)(kCmdOntimeCharLength));
    current_cmd_length += kCmdOntimeCharLength;

    // snprintfはNULL文字が強制的に書き込まれるので、文字数+1個分の長さの確保が必要
    out_interval_sec_char_length = (out_interval_sec > 9) ? 3 : 2;
    snprintf(&(cmd_payload[current_cmd_length]), (size_t)(out_interval_sec_char_length), "%d", out_interval_sec);
    current_cmd_length += out_interval_sec_char_length - 1; // NULL文字分を消す
  }

  return OEM7600_send_cmd_(oem7600_driver, (size_t)(current_cmd_length), cmd_payload);
}


// CMD関数の出力はすべて文字列に変換するので、memcopy使用時もbyteオーダーは不問
DS_CMD_ERR_CODE OEM7600_save_tlm_setting(OEM7600_Driver* oem7600_driver)
{
  const char    kCmdPayload[]     = "saveconfig";
  const uint8_t kCmdPayloadLength = (uint8_t)(sizeof(kCmdPayload) / sizeof(kCmdPayload[0])) - 1; // NULL文字分を消す

  return OEM7600_send_cmd_(oem7600_driver, (size_t)(kCmdPayloadLength), kCmdPayload);
}

DS_CMD_ERR_CODE OEM7600_set_uart_baudrate(OEM7600_Driver* oem7600_driver, const uint32_t baudrate, DS_StreamRecBuffer* rx_buffer)
{
  const char    kCmdPrefixChar[] = "serialconfig com1 ";
  const uint8_t kCmdPrefixCharLength = (uint8_t)(sizeof(kCmdPrefixChar) / sizeof(kCmdPrefixChar[0])) - 1; // NULL文字分を消す
  const char    kCmdPrefixArg[] = " n 8 1 n on";
  const uint8_t kCmdOnceCharLength = (uint8_t)(sizeof(kCmdPrefixArg) / sizeof(kCmdPrefixArg[0])) - 1;     // NULL文字分を消す
  uint8_t current_cmd_length = 0;
  uint8_t baudrate_char_length = 0;
  char cmd_payload[OEM7600_TX_MAX_FLAME_SIZE];

  memcpy(&(cmd_payload), &(kCmdPrefixChar), (size_t)(kCmdPrefixCharLength));
  current_cmd_length = kCmdPrefixCharLength;

  // 引数を文字列として解釈する際の文字数カウント
  // snprintfはNULL文字が強制的に書き込まれるので、文字数+1個分の長さの確保が必要
  baudrate_char_length = 1;
  uint32_t baudrate_for_digit_count = baudrate;
  while (baudrate_for_digit_count / 10 > 0)
  {
    baudrate_for_digit_count = baudrate_for_digit_count / 10;
    baudrate_char_length++;
  }

  snprintf(&(cmd_payload[current_cmd_length]), (size_t)(baudrate_char_length), "%d", baudrate);
  current_cmd_length += baudrate_char_length - 1; // NULL文字分を消す

  memcpy(&(cmd_payload[current_cmd_length]), &(kCmdPrefixArg), (size_t)(kCmdOnceCharLength));
  current_cmd_length += kCmdOnceCharLength;

  DS_CMD_ERR_CODE cmd_status =  OEM7600_send_cmd_(oem7600_driver, (size_t)(current_cmd_length), cmd_payload);

  // DS側も変更しないとデータ撮れない
  if (cmd_status != DS_CMD_OK) return cmd_status;

  oem7600_driver->info.uart_baudrate = baudrate;
  DS_ERR_CODE ret_driver_super_setting = DS_init(&(oem7600_driver->driver.super),
                                                 &(oem7600_driver->driver.uart_config),
                                                 rx_buffer,
                                                 OEM7600_load_driver_super_init_settings_);

  if (ret_driver_super_setting != DS_ERR_CODE_OK) return DS_CMD_UNKNOWN_ERR;

  return DS_CMD_OK;
}


static DS_CMD_ERR_CODE OEM7600_send_cmd_(OEM7600_Driver* oem7600_driver, const size_t cmd_length, const char* cmd_payload)
{
  uint8_t index_in_tx_framex = 0;
  DS_StreamConfig* p_stream_config;

#ifdef DRIVER_OEM7600_DEBUG_SHOW_SEND_DATA
  // 最初に初期化しておかないと、前回値が残る
  memset(&(OEM7600_tx_frame_[0]), 0, (size_t)(OEM7600_TX_MAX_FLAME_SIZE));
#endif

  if (cmd_length > OEM7600_TX_MAX_FLAME_SIZE - 1) return DS_CMD_ILLEGAL_LENGTH;

  memcpy(&(OEM7600_tx_frame_[0]), cmd_payload, cmd_length);
  index_in_tx_framex += (uint8_t)(cmd_length);

  OEM7600_tx_frame_[index_in_tx_framex] = OEM7600_tx_footer_[0];
  index_in_tx_framex++;

  p_stream_config = &(oem7600_driver->driver.super.stream_config[OEM7600_STREAM_TLM_CMD]);
  DSSC_set_tx_frame_size(p_stream_config, (uint16_t)(index_in_tx_framex));

#ifdef DRIVER_OEM7600_DEBUG_SHOW_SEND_DATA
  Printf("oem7600 cmd data:");
  for (int i = 0; i < OEM7600_TX_MAX_FLAME_SIZE; i++)
  {
    Printf("%d ", OEM7600_tx_frame_[i]);
  }
  Printf("\n");
#endif

  if (DS_send_general_cmd(&(oem7600_driver->driver.super), OEM7600_STREAM_TLM_CMD) != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  return DS_CMD_OK;
}

// この関数内のmemcpyは文字列コピーなのでbyteオーダー不問
static uint8_t convert_tlm_id_to_tlm_name_(char* copy_target, const uint32_t oem7600_tlm_id)
{
  uint8_t tlm_name_length = 0;
  // binary format系列
  const char kBestxyzbTlmChar[]   = "bestxyzb";
  const char kHwmonitorbTlmChar[] = "hwmonitorb";
  const char kRangebTlmChar[]     = "rangeb";
  // short ascii format系列
  const char kBestxyzaTlmChar[]   = "bestxyza";
  const char kHwmonitoraTlmChar[] = "hwmonitora";
  const char kRangeaTlmChar[]     = "rangea";

  switch (oem7600_tlm_id)
  {
  case OEM7600_TLM_ID_BEST_XYZ:
    tlm_name_length = (uint8_t)(sizeof(kBestxyzbTlmChar) / sizeof(kBestxyzbTlmChar[0])) - 1;     // NULL文字分を消す
    memcpy(copy_target, &(kBestxyzbTlmChar), (size_t)(tlm_name_length));
    break;
  case OEM7600_TLM_ID_HARDWARE_MONITOR:
    tlm_name_length = (uint8_t)(sizeof(kHwmonitorbTlmChar) / sizeof(kHwmonitorbTlmChar[0])) - 1; // NULL文字分を消す
    memcpy(copy_target, &(kHwmonitorbTlmChar), (size_t)(tlm_name_length));
    break;
  case OEM7600_TLM_ID_RANGE:
    tlm_name_length = (uint8_t)(sizeof(kRangebTlmChar) / sizeof(kRangebTlmChar[0])) - 1; // NULL文字分を消す
    memcpy(copy_target, &(kRangebTlmChar), (size_t)(tlm_name_length));
    break;

  case OEM7600_TLM_ID_BEST_XYZ_ASCII:
    tlm_name_length = (uint8_t)(sizeof(kBestxyzaTlmChar) / sizeof(kBestxyzaTlmChar[0])) - 1;     // NULL文字分を消す
    memcpy(copy_target, &(kBestxyzaTlmChar), (size_t)(tlm_name_length));
    break;
  case OEM7600_TLM_ID_HARDWARE_MONITOR_ASCII:
    tlm_name_length = (uint8_t)(sizeof(kHwmonitoraTlmChar) / sizeof(kHwmonitoraTlmChar[0])) - 1; // NULL文字分を消す
    memcpy(copy_target, &(kHwmonitoraTlmChar), (size_t)(tlm_name_length));
    break;
  case OEM7600_TLM_ID_RANGE_ASCII:
    tlm_name_length = (uint8_t)(sizeof(kRangeaTlmChar) / sizeof(kRangeaTlmChar[0])) - 1; // NULL文字分を消す
    memcpy(copy_target, &(kRangeaTlmChar), (size_t)(tlm_name_length));
    break;

  default:
    // 何もしないことで、対応する文字列の長さをゼロのままとする
    break;
  }

  return tlm_name_length;
}

static DS_ERR_CODE OEM7600_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver)
{
  DS_ERR_CODE ret;
  OEM7600_Driver* oem7600_driver = (OEM7600_Driver*)p_driver;
  uint16_t received_tlm_size;                          //!< 受信データ長
  uint16_t received_tlm_size_for_crc_calc;             //!< CRC計算範囲として用いるデータ長
  uint32_t received_tlm_id;                            //!< 受け取ったテレメデータがどのテレメ項目に該当するかを指し示すID
  const uint8_t kCountForDebugOut = 10;                //!< テレメデータのデバッグアウト頻度（何回データ受信したらコンソール出力するか）
  const uint8_t* oem7600_rx_data  = DSSC_get_rx_frame(p_stream_config);

  // CRC計算のために、先にデータ長を読んでおく
  received_tlm_size = (uint16_t)(oem7600_rx_data[OEM7600_FRAME_LENGTH_POS]) + ((uint16_t)(oem7600_rx_data[OEM7600_FRAME_LENGTH_POS + 1]) << 8);
  received_tlm_size_for_crc_calc = received_tlm_size + OEM7600_COMMON_HEADER_SIZE + OEM7600_RX_CRC_SIZE;

  if (OEM7600_calculate_crc32_(oem7600_rx_data, received_tlm_size_for_crc_calc) != 0)
  {
    oem7600_driver->info.crc_state = OEM7600_CRC_STATE_NG;
#ifdef DRIVER_OEM7600_DEBUG_SHOW_REC_DATA
    Printf("CRC ERORR FOUND... \n");
#endif
  }
  else
  {
    oem7600_driver->info.crc_state = OEM7600_CRC_STATE_OK;
  }

  oem7600_driver->info.tlm_receive_counter++;
  received_tlm_id = OEM7600_analyze_common_header_tlm_(oem7600_driver, oem7600_rx_data);

  switch (received_tlm_id)
  {
  case OEM7600_TLM_ID_BEST_XYZ:
    ret = OEM7600_analyze_bestxyz_tlm_(oem7600_driver, oem7600_rx_data, received_tlm_size);
    break;
  case OEM7600_TLM_ID_HARDWARE_MONITOR:
    ret = OEM7600_analyze_hardware_status_tlm_(oem7600_driver, oem7600_rx_data, received_tlm_size);
    break;
  default:
    ret = DS_ERR_CODE_ERR;
    break;
  }

#ifdef DRIVER_OEM7600_DEBUG_SHOW_REC_DATA
  if ((oem7600_driver->info.tlm_receive_counter % kCountForDebugOut) == 1)
  {
    Printf("oem status, week, msec; %x, %d, %d \n",
           oem7600_driver->info.receiver_status_summary,
           oem7600_driver->info.times.gps_time_week,
           oem7600_driver->info.times.gps_time_ms);

    Printf("oem posx,velx, visiblesat; %f %f %d\n",
           oem7600_driver->info.pos_antenna_ecef_m[0],
           oem7600_driver->info.vel_antenna_ecef_m_s[0],
           oem7600_driver->info.num_of_visible_sats);

    Printf("oem hw status; %f %f %f %f \n",
           oem7600_driver->info.hardware_status.temperature_degC,
           oem7600_driver->info.hardware_status.antenna_current_A,
           oem7600_driver->info.hardware_status.voltage_3v3_V,
           oem7600_driver->info.hardware_status.supply_voltage_V);
  }
#endif

  return ret;
}


static uint32_t OEM7600_calculate_crc32_(const uint8_t* tlm_data_for_crc, const size_t data_length)
{
  uint32_t temporary_value1_for_crc_calcx;
  uint32_t temporary_value2_for_crc_calcx;
  uint32_t calculated_crc32x = 0;
  uint32_t data_length_for_crc_calc = data_length;

  while (data_length_for_crc_calc-- != 0)
  {
    temporary_value1_for_crc_calcx = (calculated_crc32x >> 8) & 0x00FFFFFF;
    temporary_value2_for_crc_calcx = OEM7600_calculate_crc32_subroutine_(((uint32_t)calculated_crc32x ^ *tlm_data_for_crc++) & 0xFF);
    calculated_crc32x = temporary_value1_for_crc_calcx ^ temporary_value2_for_crc_calcx;
  }

  return calculated_crc32x;
}


static uint32_t OEM7600_calculate_crc32_subroutine_(const uint32_t initial_value)
{
  const uint32_t kCrc32Polynominal = 0xEDB88320;

  uint32_t uint32_crcx = initial_value;
  for (int j = 8; j > 0; j--)
  {
    if (uint32_crcx & 1)
    {
      uint32_crcx = (uint32_crcx >> 1) ^ kCrc32Polynominal;
    }
    else
    {
      uint32_crcx >>= 1;
    }
  }

  return uint32_crcx;
}


// TODO_L: この関数内のmemcpyはbyteオーダーの影響を受けるため、機器側がlittle endianの場合に対応したendian_memcpy相当の関数で置換が必要
static uint16_t OEM7600_analyze_common_header_tlm_(OEM7600_Driver* oem7600_driver, const uint8_t* tlm_payload)
{
  const uint8_t kTlmIdPosition  = 4;  //!< 後続テレメの種別識別用IDデータ出現位置
  const uint8_t kStatusPosition = 20; //!< 受信機ステータス出現位置
  const uint8_t kWeekPosition   = 14; //!< GPS週番号出現位置
  const uint8_t kMsecPosition   = 16; //!< GPSミリ秒出現位置
  uint16_t id_of_received_tlm;        //!< 共通ヘッダ部分に含まれる，後続テレメの内容を識別するためのID

  memcpy(&id_of_received_tlm, &(tlm_payload[kTlmIdPosition]), sizeof(id_of_received_tlm));

  memcpy(&(oem7600_driver->info.receiver_status_summary), &(tlm_payload[kStatusPosition]), sizeof(oem7600_driver->info.receiver_status_summary));
  memcpy(&(oem7600_driver->info.times.gps_time_week), &(tlm_payload[kWeekPosition]), sizeof(oem7600_driver->info.times.gps_time_week));
  memcpy(&(oem7600_driver->info.times.gps_time_ms), &(tlm_payload[kMsecPosition]), sizeof(oem7600_driver->info.times.gps_time_ms));

  const ObcTime* obc_time = DSC_get_rx_time(&oem7600_driver->driver.super);
  oem7600_driver->info.times.tlm_received_aobc_time = obc_time->total_cycle;
  oem7600_driver->info.times.obct_gps_time_obs = OBCT_create(obc_time->total_cycle, obc_time->mode_cycle, obc_time->step);

  return id_of_received_tlm;
}


// TODO_L: この関数内のmemcpyはbyteオーダーの影響を受けるため、機器側がlittle endianの場合に対応したendian_memcpy相当の関数で置換が必要
static DS_ERR_CODE OEM7600_analyze_bestxyz_tlm_(OEM7600_Driver* oem7600_driver, const uint8_t* tlm_payload, const uint16_t tlm_length)
{
  const uint8_t kPositionDataStartPosition      = 36;  //!< 位置情報データの出現位置
  const uint8_t kVelocityDataStartPosition      = 80;  //!< 速度情報データの出現位置
  const uint8_t kTimeFromLastFixStartPosition   = 128; //!< 最終測位タイミングからの経過時間データ出現位置
  const uint8_t kVisibleSatNumDataStartPosition = 132; //!< 可視衛星数データの出現位置
  const uint8_t kSizeOfPositionVelocityData     = 8;   //!< 位置・速度データの1軸あたりのサイズ
  uint8_t offset_for_read_out = 0;                     //!< 読み出し位置をずらすためのオフセット
  uint8_t axis_idx = 0;                                //!< 軸数のカウンタ

  if ((OEM7600_COMMON_HEADER_SIZE + tlm_length) < (kPositionDataStartPosition + PHYSICAL_CONST_THREE_DIM * kSizeOfPositionVelocityData))
  {
    return DS_ERR_CODE_ERR;
  }

  for (axis_idx = 0; axis_idx < PHYSICAL_CONST_THREE_DIM; axis_idx++)
  {
    memcpy(&(oem7600_driver->info.pos_antenna_ecef_m[axis_idx]),
           &tlm_payload[kPositionDataStartPosition + offset_for_read_out],
           (size_t)(kSizeOfPositionVelocityData));
    offset_for_read_out += kSizeOfPositionVelocityData;
  }
  offset_for_read_out = 0;

  if ((OEM7600_COMMON_HEADER_SIZE + tlm_length) < (kVelocityDataStartPosition + PHYSICAL_CONST_THREE_DIM * kSizeOfPositionVelocityData))
  {
    return DS_ERR_CODE_ERR;
  }

  for (axis_idx = 0; axis_idx < PHYSICAL_CONST_THREE_DIM; axis_idx++)
  {
    memcpy(&(oem7600_driver->info.vel_antenna_ecef_m_s[axis_idx]),
           &tlm_payload[kVelocityDataStartPosition + offset_for_read_out],
           (size_t)(kSizeOfPositionVelocityData));
    offset_for_read_out += kSizeOfPositionVelocityData;
  }

  memcpy(&(oem7600_driver->info.times.time_from_last_fix_s),
         &tlm_payload[kTimeFromLastFixStartPosition],
         sizeof(oem7600_driver->info.times.time_from_last_fix_s));
  oem7600_driver->info.num_of_visible_sats = tlm_payload[kVisibleSatNumDataStartPosition];

  return DS_ERR_CODE_OK;
}


// TODO_L: この関数内のmemcpyはbyteオーダーの影響を受けるため、機器側がlittle endianの場合に対応したendian_memcpy相当の関数で置換が必要
static DS_ERR_CODE OEM7600_analyze_hardware_status_tlm_(OEM7600_Driver* oem7600_driver,
                                                                  const uint8_t* tlm_payload,
                                                                  const uint16_t tlm_length)
{
  const uint8_t kNumOfContensPosition = OEM7600_COMMON_HEADER_SIZE; //!< 出力内容の個数データ出現位置
  const uint8_t kSizeOfEachChunk      = 8; //!< ステータスデータ+ステータスIDの1塊あたりのサイズ
  const uint8_t kOffsetIdBeginPos     = 5; //!< 上記1セットの塊の中で、ステータスID
  uint32_t max_number_of_chunks;           //!< 今回のテレメで出力される塊の個数
  uint32_t number_of_chunk;                //!< 読み取り完了した塊の数
  uint32_t contents_id_obtained;           //!< 読み取ったステータスID(読み取った内容がどのステータスに対応するかを指すID)
  uint32_t offset_for_read_outx = 0;       //!< TLMデータの現在読み出し位置を指定するオフセット
  float obtained_value;                    //!< 読み取ったステータスデータ

  memcpy(&max_number_of_chunks, &(tlm_payload[kNumOfContensPosition]), sizeof(max_number_of_chunks));
  offset_for_read_outx = kNumOfContensPosition + 4;

#ifdef DRIVER_OEM7600_DEBUG_SHOW_REC_DATA
  Printf("oem hw status num of contents; %d \n", max_number_of_chunks);
#endif

  for (number_of_chunk = 0; number_of_chunk < max_number_of_chunks; number_of_chunk++)
  {
    if (offset_for_read_outx > (uint32_t)(OEM7600_COMMON_HEADER_SIZE + tlm_length)) return DS_ERR_CODE_ERR;

    contents_id_obtained = ((uint32_t)(tlm_payload[offset_for_read_outx + kOffsetIdBeginPos + 2]) << 16) +
                           ((uint32_t)(tlm_payload[offset_for_read_outx + kOffsetIdBeginPos + 1]) << 8)  +
                           ((uint32_t)(tlm_payload[offset_for_read_outx + kOffsetIdBeginPos]));

    memcpy(&obtained_value, &(tlm_payload[offset_for_read_outx]), sizeof(obtained_value));

    switch (contents_id_obtained)
    {
    case OEM7600_TLM_ID_TEMPERATURE:
      oem7600_driver->info.hardware_status.temperature_degC = obtained_value;
      break;
    case OEM7600_TLM_ID_ANTENNA_CURRENT:
      oem7600_driver->info.hardware_status.antenna_current_A = obtained_value;
      break;
    case OEM7600_TLM_ID_3V3_VOLTAGE:
      oem7600_driver->info.hardware_status.voltage_3v3_V = obtained_value;
      break;
    case OEM7600_TLM_ID_ANTENNA_VOLTAGE:
      oem7600_driver->info.hardware_status.antenna_voltage_V = obtained_value;
      break;
    case OEM7600_TLM_ID_SUPPLY_VOLTAGE:
      oem7600_driver->info.hardware_status.supply_voltage_V = obtained_value;
      break;
    default:
      // 他のテレメ項目は不要なため，無視する
      break;
    }

    offset_for_read_outx += kSizeOfEachChunk;
  }

  return DS_ERR_CODE_OK;
}


// TODO_L: この関数内のmemcpyはbyteオーダーの影響を受けるため、機器側がlittle endianの場合に対応したendian_memcpy相当の関数で置換が必要
static DS_ERR_CODE OEM7600_analyze_range_tlm_(OEM7600_Driver* oem7600_driver, const uint8_t* tlm_payload, const uint16_t tlm_length)
{
  const uint8_t kNumOfContensPosition = OEM7600_COMMON_HEADER_SIZE; //!< 出力内容の個数データ出現位置
  uint32_t max_number_of_chunks = 0;   //!< 今回のテレメで出力される塊の個数=観測衛星数
  uint32_t offset_for_read_out  = 0;   //!< TLMデータの現在読み出し位置を指定するオフセット

  // range tlmは分割受信処理の都合により，この中でCRCチェックとヘッダ解釈もするしかない
  if (OEM7600_calculate_crc32_(tlm_payload, tlm_length) != 0)
  {
    oem7600_driver->info.crc_state = OEM7600_CRC_STATE_NG;
  }
  else
  {
    oem7600_driver->info.crc_state = OEM7600_CRC_STATE_OK;
  }

  oem7600_driver->info.tlm_receive_counter++;
  OEM7600_analyze_common_header_tlm_(oem7600_driver, tlm_payload);

  memcpy(&max_number_of_chunks, &(tlm_payload[kNumOfContensPosition]), sizeof(max_number_of_chunks));
  offset_for_read_out = kNumOfContensPosition + 4;

  if (max_number_of_chunks > OEM7600_MAX_SAT_NUM_RANGE_STORE) max_number_of_chunks = OEM7600_MAX_SAT_NUM_RANGE_STORE;

  for (uint8_t sat_id = 0; sat_id < max_number_of_chunks; sat_id++)
  {
    if (offset_for_read_out > (uint32_t)(OEM7600_COMMON_HEADER_SIZE + tlm_length)) return DS_ERR_CODE_ERR;

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].prn_slot), &tlm_payload[offset_for_read_out], sizeof(uint16_t));
    offset_for_read_out += (uint32_t)(sizeof(uint16_t));

    // skip GLONASS related parameter
    offset_for_read_out += (uint32_t)(sizeof(uint16_t));

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].pseudo_range_m), &tlm_payload[offset_for_read_out], sizeof(double));
    offset_for_read_out += (uint32_t)(sizeof(double));

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].pseudo_range_std_deviation_m), &tlm_payload[offset_for_read_out], sizeof(float));
    offset_for_read_out += (uint32_t)(sizeof(float));

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].carrier_phase_cycle), &tlm_payload[offset_for_read_out], sizeof(double));
    offset_for_read_out += (uint32_t)(sizeof(double));

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].carrier_phase_std_deviation_cycle), &tlm_payload[offset_for_read_out], sizeof(float));
    offset_for_read_out += (uint32_t)(sizeof(float));

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].carrier_doppler_Hz), &tlm_payload[offset_for_read_out], sizeof(float));
    offset_for_read_out += (uint32_t)(sizeof(float));

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].carrier_to_noise_ratio_dB_Hz), &tlm_payload[offset_for_read_out], sizeof(float));
    offset_for_read_out += (uint32_t)(sizeof(float));

    memcpy(&(oem7600_driver->info.range_tlm[sat_id].continuous_tracking_time_sec), &tlm_payload[offset_for_read_out], sizeof(float));
    offset_for_read_out += (uint32_t)(sizeof(float));

    // 仕様書ではUlongとなっているが，出力は4Byte
    memcpy(&(oem7600_driver->info.range_tlm[sat_id].tracking_status), &tlm_payload[offset_for_read_out], sizeof(uint32_t));
    offset_for_read_out += (uint32_t)(sizeof(uint32_t));

    oem7600_driver->info.range_tlm[sat_id].is_updated = OEM7600_DATA_UPDATED;
  }

  // 今回更新しなかったバッファについて，更新状態を非更新で上書き
  for (uint8_t sat_id = max_number_of_chunks; sat_id < OEM7600_MAX_SAT_NUM_RANGE_STORE; sat_id++)
  {
    oem7600_driver->info.range_tlm[sat_id].is_updated = OEM7600_DATA_NOT_UPDATED;
  }

  return DS_ERR_CODE_OK;
}


static DS_ERR_CODE OEM7600_store_range_tlm_chunk_(OEM7600_Driver* oem7600_driver)
{
  DS_StreamConfig* p_stream_config = &(oem7600_driver->driver.super.stream_config[OEM7600_STREAM_TLM_CMD]);
  DriverSuper* p_super = &oem7600_driver->driver.super;
  OEM7600_Info* info = &oem7600_driver->info;
  uint16_t rx_buffer_read_pos = 0;
  int ret_if_rx;
  uint16_t if_rec_len;
  uint16_t unread_buffer_len;

  const uint16_t kMaxWaitCountToReset = 60000; // 返答テレメ待ちカウンタ最大値（この値を超えて受信待ちになっている場合、既にテレメを採りこぼしている）

  // 採りこぼし判定
  info->range_tlm_status.range_tlm_wait_counter++;
  if (info->range_tlm_status.range_tlm_wait_counter > kMaxWaitCountToReset)
  {
    OEM7600_end_rec_range_tlm(oem7600_driver);
    return DS_ERR_CODE_ERR;
  }

  // バッファに受信
  ret_if_rx = (*IF_RX[p_super->interface])(p_super->if_config,
                                           OEM7600_range_tlm_if_rx_buffer_,
                                           OEM7600_RANGE_TLM_IF_RX_BUFFER_SIZE);

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
  Printf("ret_if_rx %d \n", ret_if_rx);
#endif

  if (ret_if_rx <= 0) return DS_ERR_CODE_ERR;
  if (ret_if_rx == 0) return DS_ERR_CODE_OK;    // 受信なし

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
  Printf("rec data\n");
  for (int i = 0; i < ret_if_rx; ++i)
  {
    Printf("%02x ", OEM7600_range_tlm_if_rx_buffer_[i]);
  }
  Printf("\n");
#endif

  // 受信あり
  if_rec_len = (uint16_t)ret_if_rx;
  unread_buffer_len = if_rec_len - rx_buffer_read_pos;
  while (unread_buffer_len)
  {
#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
    Printf("unread_buffer_len %d \n", unread_buffer_len);
#endif

    DS_ERR_CODE ret_pickup = OEM7600_store_range_tlm_chunk_pickup_(oem7600_driver, if_rec_len, &rx_buffer_read_pos);

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
    Printf("ret_pickup %d \n", ret_pickup);
#endif

    if (ret_pickup != DS_ERR_CODE_OK)
    {
      // 不整合 → リセット
      info->range_tlm_status.received_tlm_len = 0;
    }

    unread_buffer_len = if_rec_len - rx_buffer_read_pos;
  }

  return DS_ERR_CODE_OK;
}


static DS_ERR_CODE OEM7600_store_range_tlm_chunk_pickup_(OEM7600_Driver* oem7600_driver, uint16_t if_rec_len, uint16_t* p_rx_buffer_read_pos)
{
  DS_StreamConfig* p_stream_config = &(oem7600_driver->driver.super.stream_config[OEM7600_STREAM_TLM_CMD]);
  OEM7600_Info* info = &oem7600_driver->info;
  uint16_t unread_buffer_len = if_rec_len - (*p_rx_buffer_read_pos);
  uint16_t received_tlm_id;
  uint32_t received_tlm_size;

  if (info->range_tlm_status.received_tlm_len == 0)
  {
    // ヘッダ未受信
    uint8_t* p_header = NULL;
    p_header = (uint8_t*)memchr(&OEM7600_range_tlm_if_rx_buffer_[(*p_rx_buffer_read_pos)],
                                (int)(OEM7600_rx_header_[0]),
                                (size_t)unread_buffer_len);

    if (p_header == NULL)
    {
      // ヘッダ候補なし
      unread_buffer_len = 0;
      (*p_rx_buffer_read_pos) = if_rec_len;
      return DS_ERR_CODE_OK;
    }

    (*p_rx_buffer_read_pos) = (uint16_t)(p_header - &OEM7600_range_tlm_if_rx_buffer_[(*p_rx_buffer_read_pos)]) + (*p_rx_buffer_read_pos);

    // ヘッダ 1 byte 確定
    OEM7600_range_tlm_buff_[info->range_tlm_status.received_tlm_len] = OEM7600_range_tlm_if_rx_buffer_[(*p_rx_buffer_read_pos)];
    info->range_tlm_status.received_tlm_len++;
    (*p_rx_buffer_read_pos)++;
    // unread_buffer_len--;
    unread_buffer_len = if_rec_len - (*p_rx_buffer_read_pos);

    if (unread_buffer_len == 0) return DS_ERR_CODE_OK;
  }


  if (info->range_tlm_status.received_tlm_len < sizeof(OEM7600_rx_header_))
  {
#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
    Printf("receiving rx header: %d, %d, \n", info->range_tlm_status.received_tlm_len, *p_rx_buffer_read_pos);
#endif
    // ヘッダ受信中
    uint16_t unread_header = sizeof(OEM7600_rx_header_) - info->range_tlm_status.received_tlm_len;
    uint16_t i;
    for (i = info->range_tlm_status.received_tlm_len; i < sizeof(OEM7600_rx_header_); ++i)
    {

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
      Printf("receiving rx  header for: %d, %d, \n", info->range_tlm_status.received_tlm_len, *p_rx_buffer_read_pos);
#endif

      if (OEM7600_range_tlm_if_rx_buffer_[(*p_rx_buffer_read_pos)] != OEM7600_rx_header_[info->range_tlm_status.received_tlm_len])
      {
        // 不整合
        return DS_ERR_CODE_ERR;
      }

      OEM7600_range_tlm_buff_[info->range_tlm_status.received_tlm_len] = OEM7600_range_tlm_if_rx_buffer_[(*p_rx_buffer_read_pos)];
      info->range_tlm_status.received_tlm_len++;
      (*p_rx_buffer_read_pos)++;
      unread_buffer_len--;

      if (unread_buffer_len == 0) return DS_ERR_CODE_OK;
    }
  }

  if (info->range_tlm_status.received_tlm_len < OEM7600_COMMON_HEADER_SIZE)
  {
#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
    Printf("receiving header: %d, %d, \n", info->range_tlm_status.received_tlm_len, *p_rx_buffer_read_pos);
#endif

    // フレーム長p_rx_buffer_read_posまで受信中
    uint16_t unread_header_len = OEM7600_COMMON_HEADER_SIZE - info->range_tlm_status.received_tlm_len;
    uint16_t copy_len = unread_header_len;
    if (copy_len > unread_buffer_len)
    {
      copy_len = unread_buffer_len;
    }

    memcpy(&OEM7600_range_tlm_buff_[info->range_tlm_status.received_tlm_len],
           &OEM7600_range_tlm_if_rx_buffer_[(*p_rx_buffer_read_pos)],
           copy_len);

    info->range_tlm_status.received_tlm_len += copy_len;
    (*p_rx_buffer_read_pos) += copy_len;
    unread_buffer_len -= copy_len;

    if (unread_buffer_len == 0) return DS_ERR_CODE_OK;
  }

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
  Printf("receiving body: %d, %d, \n", info->range_tlm_status.received_tlm_len, *p_rx_buffer_read_pos);
#endif

  // ここまできたらフレーム長などは受信できてる
  received_tlm_size = OEM7600_range_tlm_buff_[OEM7600_FRAME_LENGTH_POS] +
                      ((uint16_t)(OEM7600_range_tlm_buff_[OEM7600_FRAME_LENGTH_POS + 1]) << 8);
  info->range_tlm_status.expected_data_length = received_tlm_size + OEM7600_COMMON_HEADER_SIZE + OEM7600_RX_CRC_SIZE;
  received_tlm_id = OEM7600_analyze_common_header_tlm_(oem7600_driver, OEM7600_range_tlm_buff_);

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
  Printf("received_tlm_size %d \n", received_tlm_size);
  Printf("expected_data_length %d \n", info->range_tlm_status.expected_data_length);
#endif

  if (received_tlm_id != OEM7600_TLM_ID_RANGE)
  {
    // 不整合
    return DS_ERR_CODE_ERR;
  }
  if (info->range_tlm_status.expected_data_length > OEM7600_RANGE_TLM_FULL_SIZE)
  {
    // 不整合
    return DS_ERR_CODE_ERR;
  }

  // ここまで来たら，あとはデータをツモっていくだけ
  {
    uint16_t unreceived_len = info->range_tlm_status.expected_data_length - info->range_tlm_status.received_tlm_len;
    uint16_t copy_len = unreceived_len;
    if (copy_len > unread_buffer_len)
    {
      copy_len = unread_buffer_len;
    }

    memcpy(&OEM7600_range_tlm_buff_[info->range_tlm_status.received_tlm_len],
           &OEM7600_range_tlm_if_rx_buffer_[(*p_rx_buffer_read_pos)],
           copy_len);

    info->range_tlm_status.received_tlm_len += copy_len;
    (*p_rx_buffer_read_pos) += copy_len;
    unread_buffer_len -= copy_len;

    unreceived_len = info->range_tlm_status.expected_data_length - info->range_tlm_status.received_tlm_len;
    if (unreceived_len == 0)
    {
      // 受信完了
      OEM7600_end_rec_range_tlm(oem7600_driver);
      return OEM7600_analyze_range_tlm_(oem7600_driver, OEM7600_range_tlm_buff_, info->range_tlm_status.expected_data_length);
    }

    if (unread_buffer_len == 0) return DS_ERR_CODE_OK;
  }

  return DS_ERR_CODE_OK;
}


DS_CMD_ERR_CODE OEM7600_start_rec_range_tlm(OEM7600_Driver* oem7600_driver)
{
  DS_StreamConfig* p_stream_config = &(oem7600_driver->driver.super.stream_config[OEM7600_STREAM_TLM_CMD]);

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
  Printf("OEM7600_start_rec_range_tlm\n");
#endif

  oem7600_driver->info.range_tlm_status.is_receiving = 1;
  oem7600_driver->info.range_tlm_status.received_tlm_len = 0;
  oem7600_driver->info.range_tlm_status.expected_data_length = 0;
  oem7600_driver->info.range_tlm_status.range_tlm_wait_counter = 0;

  // AOBC宛てにrange tlm 1 shot 要求コマンドが来た際には，上の設定を行ったのち
  // ここで初めてコンポ側にrange tlm 1 shot 要求コマンドを送る
  DS_CMD_ERR_CODE cmd_return = OEM7600_set_tlm_contents(oem7600_driver, OEM7600_TLM_ID_RANGE, 0);

  return cmd_return;
}

DS_CMD_ERR_CODE OEM7600_end_rec_range_tlm(OEM7600_Driver* oem7600_driver)
{
  DS_StreamConfig* p_stream_config = &(oem7600_driver->driver.super.stream_config[OEM7600_STREAM_TLM_CMD]);

#ifdef DRIVER_OEM7600_DEBUG_SHOW_RANGE_DATA_BUFF
  Printf("OEM7600_end_rec_range_tlm\n");
#endif

  oem7600_driver->info.range_tlm_status.is_receiving = 0;

  // FIXME: rx_buffer が引数に必要だが, 意外と影響範囲が大きくて面倒くさい
  // そもそもここで DS_init しているのはなぜ？必要ないなら消したい
  // もしくは, rx_buffer を必要としないそれ以外の初期化を行う関数が欲しい
#if 0
  DS_init(&(oem7600_driver->driver.super),
          &(oem7600_driver->driver.uart_config),
          OEM7600_load_driver_super_init_settings_);
#endif

  return DS_CMD_OK;
}

#pragma section
