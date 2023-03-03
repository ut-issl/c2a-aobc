/**
* @file   geomagnetism.h
* @brief  地磁場計算関連の関数群の定義
* @note   IGRF 13th generation at 2020
* @note   アルゴリズムは右記URL先の産総研資料(igrf5.pdf)を参考に改変した. (https://www.gsj.jp/data/openfile/no0614/indexJ.html)
*/

#ifndef GEOMAGNETISM_H_
#define GEOMAGNETISM_H_

#include "stdint.h"
#include "physical_constants.h"
#include "c2a_math.h"

#define GEOMAGNETISM_IGRF_ORDER_MAX 13  //!< IGRF計算次数

/**
 * @struct GEOMAGNETISM_Coeffs
 * @brief  地磁場計算に用いる各種係数の定義
 * @note   Library内でデフォルト値は定義するが，Library外から書換することを想定
 */
typedef struct
{
  //!< epoch time of coefficients' table [julian day]
  double igrf_base_julian_day;

  //!< igrf計算用の係数g(n,m), h(n,m) [nT]
  float igrf_gnm_nT[GEOMAGNETISM_IGRF_ORDER_MAX][GEOMAGNETISM_IGRF_ORDER_MAX + 1];
  float igrf_hnm_nT[GEOMAGNETISM_IGRF_ORDER_MAX][GEOMAGNETISM_IGRF_ORDER_MAX + 1];

  //!< igrf計算用の係数のうち時間依存項gt(n,m), ht(n,m) [nT]
  float igrf_gtnm_nT[GEOMAGNETISM_IGRF_ORDER_MAX][GEOMAGNETISM_IGRF_ORDER_MAX + 1];
  float igrf_htnm_nT[GEOMAGNETISM_IGRF_ORDER_MAX][GEOMAGNETISM_IGRF_ORDER_MAX + 1];

} GEOMAGNETISM_Coeffs;


// TODO_L: 係数表書換用IFの作成

/**
 * @brief        IGRFに基づく簡易地磁場計算
 * @param[out]   mag_i_nT    : J2000系での地磁場ベクトル [nT]
 * @param[in]    clac_order  : 計算次数
 * @param[in]    jday        : JulianDay [day]
 * @param[in]    dcm_ecef2eci: ECEFからECIへの座標変換行列
 * @param[in]    lla_rad_m   : 測地系経緯度で表した衛星位置 [rad, m]
 * @return       C2A_MATH_ERROR
 * @note         計算失敗時はmag_i_nTをゼロにする．利用側で前回値保存等の対処をしてほしい．
 */
C2A_MATH_ERROR GEOMAGNETISM_calc_igrf(float* mag_i_nT, const uint8_t clac_order,
                                      const double jday,
                                      const double dcm_ecef2eci[][PHYSICAL_CONST_THREE_DIM],
                                      const double lla_rad_m[PHYSICAL_CONST_THREE_DIM]);

#endif
