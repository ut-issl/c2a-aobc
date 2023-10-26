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

  // CDH: 50-69 通信・不揮発メモリエラーなど
  EL_GROUP_TLM_ERROR_INA260 = 50,  // TLMエラー:INA260
  EL_GROUP_TLM_ERROR_MPU9250,      // TLMエラー:MPU9250
  EL_GROUP_TLM_ERROR_RM3100,       // TLMエラー:RM3100
  EL_GROUP_TLM_ERROR_NANOSSOC,     // TLMエラー:NANOSSOC-D60
  EL_GROUP_TLM_ERROR_STIM210,      // TLMエラー:STIM210
  EL_GROUP_TLM_ERROR_SAGITTA,      // TLMエラー:SAGITTA
  EL_GROUP_TLM_ERROR_OEM7600,      // TLMエラー:OEM7600
  EL_GROUP_TLM_ERROR_RW0003X,      // TLMエラー:RW0003 x axis
  EL_GROUP_TLM_ERROR_RW0003Y,      // TLMエラー:RW0003 y axis
  EL_GROUP_TLM_ERROR_RW0003Z,      // TLMエラー:RW0003 z axis
  // EL_GROUP_OTHER_ERROR_INA260,     // その他エラー:INA260 [TODO_L] 使用時はコンポON/OFF時にenable/disableする
  EL_GROUP_ERROR_MPU9250,    // その他エラー:MPU9250
  EL_GROUP_ERROR_RM3100,     // その他エラー:RM3100
  EL_GROUP_ERROR_NANOSSOC,   // その他エラー:NANOSSOC-D60
  EL_GROUP_ERROR_STIM210,    // その他エラー:STIM210
  EL_GROUP_ERROR_SAGITTA,    // その他エラー:SAGITTA
  EL_GROUP_ERROR_OEM7600,    // その他エラー:OEM7600
  EL_GROUP_ERROR_RW0003X,    // その他エラー:RW0003 x axis
  EL_GROUP_ERROR_RW0003Y,    // その他エラー:RW0003 y axis
  EL_GROUP_ERROR_RW0003Z,    // その他エラー:RW0003 z axis

  // POWER: 70-79  過電流など
  EL_GROUP_SW_OC = 70,
  EL_GROUP_HW_OC,

  // THERMAL: 80-89  温度異常など
  EL_GROUP_TEMPERATURE_HIGH = 80, // 高温異常
  EL_GROUP_TEMPERATURE_LOW,       // 低温異常

  // AOCS: 90-119 制御異常など
  EL_GROUP_AUTO_MODE_TRANSITION = 90, //!< 自動モード遷移
  EL_GROUP_CALCULATION_ERROR,    //!< アルゴリズム計算内での計算異常

  // その他: 120-127
  EL_GROUP_DR_WRITE = 120,       //!< DR書き込みエラー
  EL_GROUP_DR_READ,              //!< DR読み出しエラー
  EL_GROUP_NV_BC_MGR,            //!< 不揮発BC操作時のエラー
  EL_GROUP_NVM_TRIPLE_REDUNDANT, //!< 不揮発メモリ三重冗長エラー

  // とりあえず最大値は0x7f(127)に！
  EL_GROUP_MAX = 0x80       // これはいじらない！
} EL_GROUP;

#endif
