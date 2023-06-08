/**
 * @file   gps_time.h
 * @brief  GPS時刻構造体と定数の定義
 */
#ifndef GPS_TIME_H_
#define GPS_TIME_H_

#include "stdint.h"

 /**
  * @struct   GPS_TIME_OF_WEEK
  * @brief    GPS時刻構造体
  * @note     うるう秒はUTCやJdayへの換算時に，GPS時刻に対して加算する
  *           うるう秒は2035年に廃止予定だが，それまでは今後も更新がありえるため，gpsr_selector宛てCMDで書き換え可能とする
  */
typedef struct
{
  uint16_t week_number;   //!< 週番号
  uint32_t msec_of_week;  //!< 週内の経過秒 [ミリ秒]
  uint8_t  leap_seconds;  //!< うるう秒 [秒]（UTCやJdayへの換算時にはGPS時刻に対して加算する)
} GPS_TIME_OF_WEEK;


/**
 * @brief      uint変数からのGPS時刻構造体生成
 * @param[in]  week_number    : 週番号
 * @param[in]  msec_of_week   : 週内の経過秒 [ミリ秒]
 * @param[in]  leap_seconds   : うるう秒 [秒]
 * @return     GPS_TIME_OF_WEEK
 */
GPS_TIME_OF_WEEK GPS_TIME_OF_WEEK_create_gps_time(const uint16_t week_number, const uint32_t msec_of_week, const uint8_t leap_seconds);

#endif
