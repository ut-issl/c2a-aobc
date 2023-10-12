/**
 * @file   gps_time.c
 * @brief  GPS時刻構造体と定数の定義
 */

#include "gps_time.h"
#include <src_user/Library/physical_constants.h>


GpsTime GPS_TIME_create_gps_time(const uint16_t week_number, const uint32_t msec_of_week)
{
  GpsTime gps_time;
  gps_time.week_number  = week_number;
  gps_time.msec_of_week = msec_of_week;

  return gps_time;
}


uint32_t GPS_TIME_calc_total_time_s(const GpsTime gpstime, const uint16_t epoch_gps_week)
{
  // 元紀以前なら計算せずにゼロを返す
  if (gpstime.week_number < epoch_gps_week) return 0;

  // 通常，受信機出力はGPS時刻正秒と同期してタグ付けされるため，ミリ秒以下は切る
  uint32_t diff_gps_week = (uint32_t)(gpstime.week_number) - (uint32_t)(epoch_gps_week);
  uint32_t elapsed_sec = (uint32_t)(PHYSICAL_CONST_EARTH_SOLAR_DAY_s) * diff_gps_week * (uint32_t)(PHYSICAL_CONST_DAY_OF_WEEK) +
                         gpstime.msec_of_week / (uint32_t)(PHYSICAL_CONST_SEC_TO_MSEC);

  return elapsed_sec;
}
