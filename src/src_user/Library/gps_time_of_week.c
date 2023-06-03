/**
 * @file   gps_time_of_week.c
 * @brief  GPS時刻構造体と定数の定義
 */

#include "gps_time_of_week.h"


GPS_TIME_OF_WEEK GPS_TIME_OF_WEEK_create_gps_time(const uint16_t week_number, const uint32_t msec_of_week, const uint8_t leap_seconds)
{
  GPS_TIME_OF_WEEK gps_time;
  gps_time.week_number  = week_number;
  gps_time.msec_of_week = msec_of_week;
  gps_time.leap_seconds = leap_seconds;

  return gps_time;
}


GPS_TIME_OF_WEEK GPS_TIME_OF_WEEK_count_up(const GPS_TIME_OF_WEEK current_gps_time)
{
  GPS_TIME_OF_WEEK updated_gps_time = current_gps_time;

  return updated_gps_time;
}

