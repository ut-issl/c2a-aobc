/**
* @file   inertial_geomag_calculator.h
* @brief  各種慣性系リファレンス計算のうち，IGRF計算を行う
*/
#ifndef INERTIAL_GEOMAG_CALCULATOR_H_
#define INERTIAL_GEOMAG_CALCULATOR_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @struct InertialGeomagCalculator
 * @brief  計算時刻オフセットの設定値など
 */
typedef struct
{
  uint8_t calc_order; //!< IGRFの計算次数(1~13)
} InertialGeomagCalculator;

AppInfo APP_GEOMAG_ECI_CALC_create_app(void);

#endif
