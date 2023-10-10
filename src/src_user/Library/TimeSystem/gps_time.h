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
  */
typedef struct
{
  uint16_t week_number;   //!< 週番号
  uint32_t msec_of_week;  //!< 週内の経過秒 [ミリ秒]
} GpsTime;


/**
 * @brief      uint変数からのGPS時刻構造体生成
 * @param[in]  week_number    : 週番号
 * @param[in]  msec_of_week   : 週内の経過秒 [ミリ秒]
 * @return     GPS_TIME
 */
GpsTime GPS_TIME_create_gps_time(const uint16_t week_number, const uint32_t msec_of_week);


/**
 * @brief      GPS時刻積算ミリ秒計算
 * @param[in]  GPS_TIME
 * @return     GPS時刻元紀(UTC 1980/1/6 midnight = GPS時刻 0week, 0sec)からの積算ミリ秒
 */
float GPS_TIME_calc_total_time_msec(const GpsTime gpstime);

#endif
