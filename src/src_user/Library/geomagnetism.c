#pragma section REPRO
/**
* @file   geomagnetism.c
* @brief  地磁場計算関連の関数群の定義
* @note   IGRF 13th generation at 2020
* @note   アルゴリズムは右記URL先の産総研資料(igrf5.pdf)を参考に改変した. (https://www.gsj.jp/data/openfile/no0614/indexJ.html)
*/

#include "geomagnetism.h"

#include <math.h>

#include "string.h" // for memcpy
#include "math_constants.h"
#include "vector3.h"
#include "matrix33.h"
#include "time_space.h"

#define GEOMAGNETISM_POLAR_REGION_SIN_COLAT_LIMIT 0.20f


//!< GEOMAGNETISM_Coeffsのデフォルト値の定義
static GEOMAGNETISM_Coeffs geomag_coeff_ =
{
  //!< Epoch time of the Table [julian day] (for IGRF 13th generation at 2020/1/1 midnight)
  2458849.5,

  //!< igrf_gnm_nTのデフォルト値 [nT] (IGRF 13th generation at 2020)
  {
    { - 29404.8f, - 1450.9f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f,
            0.0f,      0.0f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[1][m]

    {  - 2499.6f,   2982.0f, 1677.0f,     0.0f,     0.0f,    0.0f,    0.0f,
            0.0f,      0.0f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[2][m]

    {    1363.2f, - 2381.2f, 1236.2f,   525.7f,     0.0f,    0.0f,    0.0f,
            0.0f,      0.0f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[3][m]

    {     903.0f,    809.5f,   86.3f, - 309.4f,    48.0f,    0.0f,    0.0f,
            0.0f,      0.0f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[4][m]

    {   - 234.3f,    363.2f,  187.8f, - 140.7f, - 151.2f,   13.5f,    0.0f,
            0.0f,      0.0f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[5][m]

    {      66.0f,     65.5f,   72.9f, - 121.5f,  - 36.2f,   13.5f, - 64.7f,
            0.0f,      0.0f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[6][m]

    {      80.6f,   - 76.7f,  - 8.2f,    56.5f,    15.8f,    6.4f,  - 7.2f,
            9.8f,      0.0f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[7][m]

    {      23.7f,      9.7f, - 17.6f,   - 0.5f,  - 21.1f,   15.3f,   13.7f,
         - 16.5f,    - 0.3f,    0.0f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[8][m]

    {       5.0f,      8.4f,    2.9f,   - 1.5f,   - 1.1f, - 13.2f,    1.1f,
            8.8f,    - 9.3f, - 11.9f,     0.0f,     0.0f,    0.0f,    0.0f }, // g[9][m]

    {     - 1.9f,    - 6.2f,  - 0.1f,     1.7f,   - 0.9f,    0.7f,  - 0.9f,
            1.9f,      1.4f,  - 2.4f,   - 3.8f,     0.0f,    0.0f,    0.0f }, // g[10][m]

    {       3.0f,    - 1.4f,  - 2.5f,     2.3f,   - 0.9f,    0.3f,  - 0.7f,
          - 0.1f,      1.4f,  - 0.6f,     0.2f,     3.1f,    0.0f,    0.0f }, // g[11][m]

    {     - 2.0f,    - 0.1f,    0.5f,     1.3f,   - 1.2f,    0.7f,    0.3f,
            0.5f,    - 0.3f,  - 0.5f,     0.1f,   - 1.1f,  - 0.3f,    0.0f }, // g[12][m]

    {       0.1f,    - 0.9f,    0.5f,     0.7f,   - 0.3f,    0.8f,    0.0f,
            0.8f,      0.0f,    0.4f,     0.1f,     0.5f,  - 0.5f,  - 0.4f }  // g[13][m]
  },

  //!< igrf_hnm_nTのデフォルト値 [nT] (IGRF 13th generation at 2020)
  {
    {   0.0f,   4652.5f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f,
        0.0f,      0.0f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[1][m]

    {   0.0f, - 2991.6f, - 734.6f,     0.0f,     0.0f,   0.0f,    0.0f,
        0.0f,      0.0f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[2][m]

    {   0.0f,  -  82.1f,   241.9f, - 543.4f,     0.0f,   0.0f,    0.0f,
        0.0f,      0.0f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[3][m]

    {   0.0f,    281.9f, - 158.4f,   199.7f, - 349.7f,   0.0f,    0.0f,
        0.0f,      0.0f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[4][m]

    {   0.0f,     47.7f,   208.3f, - 121.2f,    32.3f,  98.9f,    0.0f,
        0.0f,      0.0f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[5][m]

    {   0.0f,   - 19.1f,    25.1f,    52.8f,  - 64.5f,   8.9f,   68.1f,
        0.0f,      0.0f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[6][m]

    {   0.0f,   - 51.5f,  - 16.9f,     2.2f,    23.5f, - 2.2f, - 27.2f,
      - 1.8f,      0.0f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[7][m]

    {   0.0f,      8.4f,  - 15.3f,    12.8f,  - 11.7f,  14.9f,    3.6f,
      - 6.9f,      2.8f,     0.0f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[8][m]

    {   0.0f,   - 23.4f,    11.0f,     9.8f,   - 5.1f, - 6.3f,    7.8f,
        0.4f,    - 1.4f,     9.6f,     0.0f,     0.0f,   0.0f,    0.0f }, // h[9][m]

    {   0.0f,      3.4f,   - 0.2f,     3.6f,     4.8f, - 8.6f,  - 0.1f,
      - 4.3f,    - 3.4f,   - 0.1f,    -8.8f,     0.0f,   0.0f,    0.0f }, // h[10][m]

    {   0.0f,      0.0f,     2.5f,   - 0.6f,   - 0.4f,   0.6f,  - 0.2f,
      - 1.7f,    - 1.6f,   - 3.0f,   - 2.0f,   - 2.6f,   0.0f,    0.0f }, // h[11][m]

    {   0.0f,    - 1.2f,     0.5f,     1.4f,   - 1.8f,   0.1f,    0.8f,
      - 0.2f,      0.6f,     0.2f,   - 0.9f,     0.0f,   0.5f,    0.0f }, // h[12][m]

    {   0.0f,    - 0.9f,     0.6f,     1.4f,   - 0.4f, - 1.3f,  - 0.1f,
        0.3f,    - 0.1f,     0.5f,     0.5f,   - 0.4f, - 0.4f,  - 0.6f }  // h[13][m]
  },

  //!< igrf_gtnm_nTのデフォルト値 [nT] (IGRF 13th generation at 2020)
  {
    {    5.7f,    7.4f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[1][m]

    { - 11.0f,  - 7.0f,  - 2.1f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[2][m]

    {    2.2f,  - 5.9f,    3.1f, - 12.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[3][m]

    {  - 1.2f,  - 1.6f,  - 5.9f,    5.2f,  - 5.1f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[4][m]

    {  - 0.3f,    0.5f,  - 0.6f,    0.2f,    1.3f,    0.9f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[5][m]

    {  - 0.5f,  - 0.3f,    0.4f,    1.3f,  - 1.4f,    0.0f,    0.9f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[6][m]

    {  - 0.1f,  - 0.2f,    0.0f,    0.7f,    0.1f,  - 0.5f,  - 0.8f,
         0.8f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[7][m]

    {    0.0f,    0.1f,  - 0.1f,    0.4f,  - 0.1f,    0.4f,    0.3f,
       - 0.1f,    0.4f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[8][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[9][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[10][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[11][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // gt[12][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }  // gt[13][m]
  },

  //!< igrf_htnm_nTのデフォルト値 [nT] (IGRF 13th generation at 2020)
  {
    {    0.0f, - 25.9f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[1][m]

    {    0.0f, - 30.2f, - 22.4f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[2][m]

    {    0.0f,    6.0f,  - 1.1f,    0.5f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[3][m]

    {    0.0f,  - 0.1f,    6.5f,    3.6f,  - 5.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[4][m]

    {    0.0f,    0.0f,    2.5f,  - 0.6f,    3.0f,    0.3f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[5][m]

    {    0.0f,    0.0f,  - 1.6f,  - 1.3f,    0.8f,    0.0f,    1.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[6][m]

    {    0.0f,    0.6f,    0.6f,  - 0.8f,  - 0.2f,  - 1.1f,    0.1f,
         0.3f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[7][m]

    {    0.0f,  - 0.2f,    0.6f,  - 0.2f,    0.5f,  - 0.3f,  - 0.4f,
         0.5f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[8][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[9][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[10][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[11][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }, // ht[12][m]

    {    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,
         0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f,    0.0f }  // ht[13][m]
  }

};


// localの作業用関数の宣言

//!< ルジャンドル陪関数の基底に用いる三角関数のn倍の計算
static void GEOMAGNETISM_calc_trigonometrics_(float sinn[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                              float cosn[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                              const float base_angle_rad,
                                              const uint8_t order);

//!< ルジャンドル陪関数の各係数の計算
static void GEOMAGNETISM_calc_legendre_coeffs_(float pn[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float dpn[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float pn_1[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float pn_2[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float* dpn_1n_1,
                                               const float sinl, const float cosl,
                                               const uint8_t order);


//!< ガウス係数の計算
static void GEOMAGNETISM_calc_gauss_coeffs_(float gnm[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                            float hnm[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                            const float decimal_year, const uint8_t order);



C2A_MATH_ERROR GEOMAGNETISM_calc_igrf(float* mag_i_nT, const uint8_t clac_order,
                                      const double jday,
                                      const double dcm_ecef2eci[][PHYSICAL_CONST_THREE_DIM],
                                      const double lla_rad_m[PHYSICAL_CONST_THREE_DIM])
{
  //!< spherical radius of ideal earth sphere [m]
  const float kEarthBaseRadiusGeomag_m = 6.3712e6f;
  float lat_rad = (float)(lla_rad_m[0]);
  float lon_rad = (float)(lla_rad_m[1]);
  float float_dcm_ecef2eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    for (int j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      float_dcm_ecef2eci[i][j] = (float)dcm_ecef2eci[i][j];
    }
  }

  float decimal_year = TIME_SPACE_convert_julian_day_to_decimal_year(jday, geomag_coeff_.igrf_base_julian_day);
  if (decimal_year <= 0.0f)
  {
    VECTOR3_initialize(mag_i_nT, 0.0f);
    return C2A_MATH_ERROR_RANGE_OVER;
  }

  // cast is necessary since C2A_MATH_clip_value handles only float variables
  uint8_t order = (uint8_t)C2A_MATH_clip_value((float)(clac_order), (float)(GEOMAGNETISM_IGRF_ORDER_MAX), 1.0f);

  //!< geocentric co-lattitude and mean radius at the interest point
  float colat_rad, radius_m;
  C2A_MATH_ERROR warning = TIME_SPACE_convert_geodetic_to_geocentric(&colat_rad, &radius_m, lla_rad_m);

  //!< buffer to avoid repitational computation of cos, sin harmonics
  float sin_m[GEOMAGNETISM_IGRF_ORDER_MAX + 1];
  float cos_m[GEOMAGNETISM_IGRF_ORDER_MAX + 1];
  GEOMAGNETISM_calc_trigonometrics_(sin_m, cos_m, lon_rad, order);

  //!< buffer to solve the recurrence equation
  float legendre_p_n[GEOMAGNETISM_IGRF_ORDER_MAX + 1];    //!< P[n][m]
  float legendre_dp_n[GEOMAGNETISM_IGRF_ORDER_MAX + 1];   //!< dP[n][m]
  float legendre_p_n_2[GEOMAGNETISM_IGRF_ORDER_MAX + 1];  //!< P[n-2][m]
  float legendre_p_n_1[GEOMAGNETISM_IGRF_ORDER_MAX + 1];  //!< P[n-1][m]
  float legendre_dp_n_1_n_1  = 0.0f;                      //!< dP[n-1][n-1]
  float sin_colat = sinf(colat_rad);
  float cos_colat = cosf(colat_rad);

  //!< geomagnetic coeffsitients
  float gnm[GEOMAGNETISM_IGRF_ORDER_MAX + 1];
  float hnm[GEOMAGNETISM_IGRF_ORDER_MAX + 1];

  float geo_aspect       = kEarthBaseRadiusGeomag_m / radius_m;    //!< (a/r) in the reference
  float pow_k_geo_aspect = geo_aspect * geo_aspect;                //!< (a/r)^(n+2) | n=0

  //!< geomagnetism in NED coordinate [nT]
  // mag_ned[0] =  ΣnΣm [(a/r)^(n+2)*(G(n,m)*cos_m_lon + H(n,m)*sin_m_lon)*dP(n,m)]
  // mag_ned[1] = (ΣnΣm [m*(a/r)^(n+2)*(G(n,m)*sin_m_lon - H(n,m)*cos_m_lon)*P(n,m)])/sin_colat
  // mag_ned[2] = -ΣnΣm [(n+1)*(a/r)^(n+2)*(G(n,m)*cos_m_lon + H(n,m)*sin_m_lon)P*(n,m)]
  float mag_ned_nT[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };
  for (uint8_t n = 1; n <= order; n++)
  {
    pow_k_geo_aspect = pow_k_geo_aspect * geo_aspect;
    GEOMAGNETISM_calc_gauss_coeffs_(gnm, hnm, decimal_year, n);
    GEOMAGNETISM_calc_legendre_coeffs_(legendre_p_n, legendre_dp_n, legendre_p_n_1, legendre_p_n_2,
                                       &legendre_dp_n_1_n_1, sin_colat, cos_colat, n);

    //!< buffer for summation (its unit is no meaning)
    float mag_ned_temp[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 0.0f };
    for (uint8_t m = 0; m <= n; m++)
    {
      mag_ned_temp[0] += (gnm[m] * cos_m[m] + hnm[m] * sin_m[m]) * legendre_dp_n[m];
      mag_ned_temp[1] += (float)(m) * (gnm[m] * sin_m[m] - hnm[m] * cos_m[m]) * legendre_p_n[m];
      mag_ned_temp[2] += - (gnm[m] * cos_m[m] + hnm[m] * sin_m[m]) * legendre_p_n[m];
    }

    mag_ned_nT[0] += pow_k_geo_aspect * mag_ned_temp[0];
    mag_ned_nT[1] += pow_k_geo_aspect * mag_ned_temp[1];
    mag_ned_nT[2] += (float)(n + 1) * pow_k_geo_aspect * mag_ned_temp[2];
  }

  // zero-devide constantly occurs at north/south pole
  // 軌道傾斜角97deg位であれば，sin_colat > 0.1くらいはあるため，ゼロ割には至らない
  if (fabs(sin_colat) > MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    mag_ned_nT[1] = mag_ned_nT[1] / sin_colat;
  }
  else
  {
    mag_ned_nT[1] = mag_ned_nT[1] / MATH_CONST_PROHIBIT_DIVISION_VALUE;

    // ここか，上のTIME_SPACE_convert_geodetic_to_geocentricのどちらかでwarningを出せば，呼び出し元で検知できるので上書きで良い
    warning = C2A_MATH_ERROR_RANGE_OVER_LOW;
  }

  // convert NED to ECI
  float dcm_ned2ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_ned2eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  TIME_SPACE_trans_ned_to_ecef(dcm_ned2ecef, lat_rad, lon_rad);
  MATRIX33_multiply_matrix_matrix(dcm_ned2eci, float_dcm_ecef2eci, dcm_ned2ecef);
  MATRIX33_multiply_matrix_vector(mag_i_nT, dcm_ned2eci, mag_ned_nT);

  return warning;
}



// localの作業用関数

static void GEOMAGNETISM_calc_trigonometrics_(float sinn[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                              float cosn[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                              const float base_angle_rad,
                                              const uint8_t order)
{
  sinn[0] = 0.0f;
  cosn[0] = 1.0f;

  sinn[1] = sinf(base_angle_rad);
  cosn[1] = cosf(base_angle_rad);

  for (uint8_t n = 2; n <= order; n++)
  {
    sinn[n] = sinn[n - 1] * cosn[1] + cosn[n - 1] * sinn[1];
    cosn[n] = cosn[n - 1] * cosn[1] - sinn[n - 1] * sinn[1];
  }
}


static void GEOMAGNETISM_calc_legendre_coeffs_(float p_n[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float dp_n[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float p_n_1[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float p_n_2[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                               float* dp_n_1_n_1,
                                               const float sinl, const float cosl,
                                               const uint8_t order)
{
  uint8_t n = order;

  if (n == 1)
  {
    p_n[0]  =   cosl; // P10
    p_n[n]  =   sinl; // P11
    dp_n[0] = - sinl; // dP10
    dp_n[n] =   cosl; // dP11

    // initialize buffers for n = 2
    p_n_2[0] = 1.0f;       // P(n-2,n-2)  = P(n-2,0) = P00
    p_n_2[1] = 0.0f;       // P(n-2,n-1)  = P01
    p_n_1[0] = p_n[0];     // P(n-1,n-2)  = P(n-1,0) = P10
    p_n_1[1] = p_n[n];     // P(n-1,n-1)  = P11
    *dp_n_1_n_1 = dp_n[n]; // dP(n-1,n-1) = dP11
    return;
  }

  // P(n,n) = (2n-1)*(sin_l)*P(n-1,n-1)
  p_n[n]  = (float)(2 * n - 1) * sinl * p_n_1[n - 1];

  // P(n,0) = (1/n)*[(2n-1)*(cos_l)*P(n-1,0) - (n-1)*P(n-2,0)]
  p_n[0]  = (1.0f / (float)(n)) * ((float)(2 * n - 1) * cosl * p_n_1[0] - (float)(n - 1) * p_n_2[0]);

  for (uint8_t m = 1; m < n; m++)
  {
    if (fabsf(sinl) > GEOMAGNETISM_POLAR_REGION_SIN_COLAT_LIMIT)
    {
      // P(n,m) = [(n+m-1)*P(n-1,m-1) - (n-m+1)*cos_l*P(n,m-1)]/sin_l
      p_n[m] = ((float)(n + m - 1) * p_n_1[m - 1] - (float)(n - m + 1) * cosl * p_n[m - 1]) / sinl;
    }
    else
    {
      // P(n,m) = [P(n-1,m) + (n-m+1)*sin_l*P(n,m-1)]/cos_l
      p_n[m] = (p_n_1[m] + (float)(n - m + 1) * sinl * p_n[m - 1]) / cosl;
    }
  }

  // dP(n,0) = -P(n,1)
  dp_n[0] = -p_n[1];

  // dP(n,n) = (2n-1)*[(cos_l)*P(n-1,n-1) + (sin_l)*dP(n-1,n-1)]
  // (equation in the reference document is incorrect...)
  dp_n[n] = (float)(2 * n - 1) * (cosl * p_n_1[n - 1] + sinl * (*dp_n_1_n_1));

  for (uint8_t m = 1; m < n; m++)
  {
    // dP(n,m) = [(n+m)*(n-m+1)*P(n,m-1) - P(n,m+1)]/2
    dp_n[m] = 0.5f * ((float)((n + m) * (n - m + 1)) * p_n[m - 1] - p_n[m + 1]);
  }

  // swap buffer for the next recurrence equation
  memcpy(p_n_2, p_n_1, (size_t)(sizeof(float) * (GEOMAGNETISM_IGRF_ORDER_MAX + 1)));
  memcpy(p_n_1, p_n, (size_t)(sizeof(float) * (GEOMAGNETISM_IGRF_ORDER_MAX + 1)));
  *dp_n_1_n_1 = dp_n[n];
}


static void GEOMAGNETISM_calc_gauss_coeffs_(float gnm[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                            float hnm[GEOMAGNETISM_IGRF_ORDER_MAX + 1],
                                            const float decimal_year, const uint8_t order)
{
  uint8_t n = order;

  // solve equation for m = 0
  gnm[0] = geomag_coeff_.igrf_gnm_nT[n - 1][0] + decimal_year * geomag_coeff_.igrf_gtnm_nT[n - 1][0];
  hnm[0] = geomag_coeff_.igrf_hnm_nT[n - 1][0] + decimal_year * geomag_coeff_.igrf_htnm_nT[n - 1][0];

  float factorial_coeff = C2A_MATH_sqrtf(2.0f); //!< factorial_coeff(n,m) = [2(n-m)!/(n+m)!]^(1/2) (for m >0)
  float devide_coeff    = 0.0;
  for (uint8_t m = 1; m <= n; m++)
  {
    // factorial_coeff(m) = factorial_coeff(m-1)/[(n+m)(n+m-1)]^(1/2) (for m < n)
    // factorial_coeff(m) = factorial_coeff(m-1)/[(n+m)]^(1/2) (for m = n)
    if (m == n)
    {
      devide_coeff = C2A_MATH_sqrtf((float)((n + m)));
    }
    else
    {
      devide_coeff = C2A_MATH_sqrtf((float)((n + m)*(n - m + 1)));
    }
    factorial_coeff = factorial_coeff / devide_coeff;

    // g(n,m,t) = [g0(n,m) + dt*gt(n,m)]*factorial_coeff
    gnm[m] = geomag_coeff_.igrf_gnm_nT[n - 1][m] + decimal_year * geomag_coeff_.igrf_gtnm_nT[n - 1][m];
    gnm[m] = gnm[m] * factorial_coeff;

    // h(n,m,t) = [h0(n,m) + dt*ht(n,m)]*factorial_coeff
    hnm[m] = geomag_coeff_.igrf_hnm_nT[n - 1][m] + decimal_year * geomag_coeff_.igrf_htnm_nT[n - 1][m];
    hnm[m] = hnm[m] * factorial_coeff;
  }
}

#pragma section
