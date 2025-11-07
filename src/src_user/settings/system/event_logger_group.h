/**
 * @file
 * @brief event_logger の イベント識別IDを定義
 * @note  前半は C2A core で使われるため，IDのかぶりに注意すること
 */
#ifndef EVENT_LOGGER_GROUP_H_
#define EVENT_LOGGER_GROUP_H_

/**
 * @enum  EL_GROUP
 * @brief event_logger の Event Group の user 定義部分
 * @note  uint32_t を想定
 * @note  前半の id は C2A core 側で EL_CORE_GROUP として定義されていることに注意する
 */
typedef enum
{
  // C2A CORE: 0-49
  // src_core/System/EventManager/event_logger.h の EL_CORE_GROUP にて定義

  // CDH: 50-79 通信・不揮発メモリエラーなど
  EL_GROUP_TLM_ERROR_INA260   = 50,  // TLMエラー:INA260
  EL_GROUP_TLM_ERROR_MPU9250  = 51,  // TLMエラー:MPU9250
  EL_GROUP_TLM_ERROR_RM3100   = 52,  // TLMエラー:RM3100
  EL_GROUP_TLM_ERROR_NANOSSOC = 53,  // TLMエラー:NANOSSOC-D60
  EL_GROUP_TLM_ERROR_STIM210  = 54,  // TLMエラー:STIM210
  EL_GROUP_TLM_ERROR_SAGITTA  = 55,  // TLMエラー:SAGITTA
  EL_GROUP_TLM_ERROR_OEM7600  = 56,  // TLMエラー:OEM7600
  EL_GROUP_TLM_ERROR_RW0003X  = 57,  // TLMエラー:RW0003 x axis
  EL_GROUP_TLM_ERROR_RW0003Y  = 58,  // TLMエラー:RW0003 y axis
  EL_GROUP_TLM_ERROR_RW0003Z  = 59,  // TLMエラー:RW0003 z axis

  // 一の位をコンポごとに統一するため，以下のように数字を割り当てる
  EL_GROUP_CHECKSUM_ERROR_NANOSSOC = 63,  // CHECKSUMエラー:NANOSSOC-D60
  EL_GROUP_CRC_ERROR_STIM210       = 64,  // CRCエラー:STIM210
  EL_GROUP_XXHASH_ERROR_SAGITTA    = 65,  // XXHASHエラー:SAGITTA
  EL_GROUP_CRC_ERROR_OEM7600       = 66,  // CRCエラー:OEM7600
  EL_GROUP_CRC_ERROR_RW0003X       = 67,  // CRCエラー:RW0003 x axis
  EL_GROUP_CRC_ERROR_RW0003Y       = 68,  // CRCエラー:RW0003 y axis
  EL_GROUP_CRC_ERROR_RW0003Z       = 69,  // CRCエラー:RW0003 z axis

  // EL_GROUP_OTHER_ERROR_INA260 = 70,  // その他エラー:INA260 [TODO_L] 使用時はコンポON/OFF時にenable/disableする
  EL_GROUP_ERROR_MPU9250      = 71,  // その他エラー:MPU9250
  EL_GROUP_ERROR_RM3100       = 72,  // その他エラー:RM3100
  EL_GROUP_ERROR_NANOSSOC     = 73,  // その他エラー:NANOSSOC-D60
  EL_GROUP_ERROR_STIM210      = 74,  // その他エラー:STIM210
  EL_GROUP_ERROR_SAGITTA      = 75,  // その他エラー:SAGITTA
  EL_GROUP_ERROR_OEM7600      = 76,  // その他エラー:OEM7600
  EL_GROUP_ERROR_RW0003X      = 77,  // その他エラー:RW0003 x axis
  EL_GROUP_ERROR_RW0003Y      = 78,  // その他エラー:RW0003 y axis
  EL_GROUP_ERROR_RW0003Z      = 79,  // その他エラー:RW0003 z axis

  // POWER: 80-89  過電流など
  EL_GROUP_SW_OC = 80,
  EL_GROUP_HW_OC = 81,

  // THERMAL: 90-99  温度異常など
  EL_GROUP_TEMPERATURE_HIGH = 90,  // 高温異常
  EL_GROUP_TEMPERATURE_LOW  = 91,  // 低温異常

  // AOCS: 100-119 制御異常など
  EL_GROUP_AUTO_MODE_TRANSITION = 100,  //!< 自動モード遷移
  EL_GROUP_CALCULATION_ERROR    = 101,  //!< アルゴリズム計算内での計算異常

  // その他: 120-127
  EL_GROUP_DR_WRITE             = 120,  //!< DR書き込みエラー
  EL_GROUP_DR_READ              = 121,  //!< DR読み出しエラー
  EL_GROUP_NVM_TRIPLE_REDUNDANT = 122,  //!< 不揮発メモリ三重冗長エラー
  EL_GROUP_NVM_BC               = 123,  //!< 不揮発BC操作時のエラー

  // とりあえず最大値は0x7f(127)に！
  EL_GROUP_MAX = 0x80       // これはいじらない！
} EL_GROUP;

#endif
