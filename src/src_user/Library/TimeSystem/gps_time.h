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
 * @brief      GPS時刻積算秒計算
 * @param[in]  GPS_TIME 現在のGPS時刻
 * @param[in]  epoch_gps_week GPS時刻積算秒の元紀とするGPS時刻週番号
 * @return     GPS時刻元紀週番号先頭からの積算秒
 * @note       ロールオーバーまで，元紀から約7000週有効
 */
uint32_t GPS_TIME_calc_total_time_sec(const GpsTime gpstime, const uint16_t epoch_gps_week);

#endif
