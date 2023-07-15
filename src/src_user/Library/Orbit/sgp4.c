#pragma section REPRO
/**
 * @file
 * @brief  SGP4軌道伝搬に関するライブラリ
 * @note  全体的にfloatで計算している。元期からの経過時間が延びると桁落ちで精度が悪化する。
 * @note  1日程度なら数km精度
 */

#include "sgp4.h"

#include <math.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/math_constants.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/vector3.h>
#include <src_core/Library/print.h>

// SGP4内での使う定数
static const float SGP4_kKe_s = C2A_MATH_sqrtf(PHYSICAL_CONST_EARTH_GRAVITY_CONST_km3_s2 / powf(PHYSICAL_CONST_EARTH_RADIUS_km, 3.0f));
static const float SGP4_kDensityFunc = 1.012229f;  //!< 密度関数パラメータ s
static const float SGP4_kQMS4 = 1.880279e-9f;      //!< (q - s)^4

// TODO_L: 次の2つをコマンドにするか検討
static const float   SGP4_kKeplerAngleLimit_rad_ = 1e-6f; //!< ケプラー方程式を解くときの角度リミット
static const uint8_t SGP4_kKeplerIterationLimit_ = 10;    // ケプラー方程式を解く時のイタレーション回数制限

// static構造体
/**
 * @struct Sgp4SecularTerm
 * @brief  永年摂動項
 */
typedef struct
{
  float arg_peri_rad_;     //!< 近地点引数 [rad]
  float raan_rad_;         //!< 昇降点赤経 [rad]
  float eccentricity_;     //!< 離心率
  float semi_major_axis_;  //!< 軌道長半径 [-] (地球半径で無次元化)
  float capital_l_;        //!< 軌道位相に相当する一時変数
} Sgp4SecularTerm;

/**
 * @struct Sgp4LongPeriodTerm
 * @brief  長周期摂動項
 */
typedef struct
{
  float a_x_n_;        //!< 参考書の記号定義
  float a_y_n_;        //!< 参考書の記号定義
  float capital_l_t_;  //!< 軌道位相に相当する一時変数
} Sgp4LongPeriodTerm;

/**
 * @struct Sgp4DerivedElement
 * @brief  永年、長周期、短周期全て補正した後の軌道要素に相当する量
 */
typedef struct
{
  float position_;          //!< 位置関連情報
  float velocity_;          //!< 速度関連情報
  float velocity_f_;        //!< 速度補正値
  float true_anomaly_rad_;  //!< 真近点離角
  float raan_rad_;          //!< 昇降点赤経
  float inclination_rad_;   //!< 傾斜角
} Sgp4DerivedElement;

// Static関数プロトタイプ宣言
/**
 * @brief TLE定義時刻をユリウス日に変換する
 * @note  入力される年は2000年以降と仮定する
 * @param[in] tle_year : TLEで定義される年
 * @param[in] tle_day  : TLEで定義される年間通日
 * @return    ユリウス日 [jday]
 */
static double SGP4_convert_tle_time_to_julian_day_(uint8_t tle_year, double tle_day);
/**
 * @brief TLE入力値をSGP4入力値に単位変換する
 * @param[in]  tle_input  : TLE入力値
 * @return : SGP4単位系での入力値
 */
static Sgp4Input SGP4_convert_tle_to_sgp4_unit_(const TleInput tle_input);
/**
 * @brief TLEの古在平均運動からSGP4で使うSGP4を計算する
 * @param[out] original_mm : SGP4平均運動
 * @param[in]  sgp4_in : SGP4入力値
 * @return : C2A_MATH_ERRORに準拠
 */
static C2A_MATH_ERROR SGP4_calc_original_mean_motion_(Sgp4OriginalMeanMotion* original_mm, const Sgp4Input sgp4_in);
/**
 * @brief 永年摂動項に関する定数を求める
 * @param[out] secular_constants: 永年摂動項に関する定数
 * @param[in]  sgp4_const : SGP4定数
 * @return : C2A_MATH_ERRORに準拠
 */
static C2A_MATH_ERROR SGP4_calc_secular_constants_(Sgp4SecularConstants* secular_constants, const Sgp4Constants* sgp4_const);
/**
 * @brief 長周期摂動項に関する定数を求める
 * @param[in] sgp4_const : SGP4定数
 * @return : 長周期摂動項に関する定数
 */
static Sgp4LongPeriodConstants SGP4_calc_long_period_constants_(const Sgp4Constants* sgp4_const);

/**
 * @brief 永年摂動項を求める
 * @param[out] secular_term : 永年摂動項
 * @param[in]  sgp4_const : SGP4定数
 * @param[in]  time_jday  : 軌道情報を求めたい時刻 [ユリウス日]
 * @return : C2A_MATH_ERRORに準拠
 */
static C2A_MATH_ERROR SGP4_calc_secular_term_(Sgp4SecularTerm* secular_term,
                                              const Sgp4Constants* sgp4_const,
                                              const double time_jday);
/**
 * @brief 長周期摂動項を求める
 * @param[out] long_term   : 長周期摂動項
 * @param[in]  sgp4_const   : SGP4入力値
 * @param[in]  secular_term : 永年摂動項
 * @return : C2A_MATH_ERRORに準拠
 */
static C2A_MATH_ERROR SGP4_calc_long_period_term_(Sgp4LongPeriodTerm* long_term,
                                                  const Sgp4Constants* sgp4_const,
                                                  const Sgp4SecularTerm secular_term);
/**
 * @brief Kepler方程式を解く
 * @param[in] long_term : 長周期摂動項
 * @param[in] u_rad : 真近点離角に相当する量の初期値
 * @return : 真近点離角に相当する量
 */
static float SGP4_solve_kepler_equation_(const Sgp4LongPeriodTerm long_term, const float u_rad);
/**
 * @brief 短周期摂動項を計算し、SGP4での軌道要素に相当する量を求める
 * @param[out] derived_element : 短周期摂動項を考慮した軌道要素
 * @param[in]  long_term : 長周期摂動項
 * @param[in]  semi_major_axis : 永年項で求めた無次元化された軌道長半径
 * @param[in]  raan_rad : 永年項で求めた昇降点赤経 [rad]
 * @param[in]  eo_rad : 長周期項における真近点離角に相当する量 [rad]
 * @param[in]  inclination_rad : TLEの傾斜角 [rad]
 * @return : C2A_MATH_ERRORに準拠
 */
static C2A_MATH_ERROR SGP4_calc_short_period_term_(Sgp4DerivedElement* derived_element,
                                                   const Sgp4LongPeriodTerm long_term,
                                                   const float semi_major_axis,
                                                   const float raan_rad,
                                                   const float eo, const float inclination_rad);
/**
 * @brief 方向ベクトルを計算する
 * @param[out] position_vector : 位置に関する方向情報
 * @param[out] velocity_vector : 速度に関する方向情報
 * @param[in]  short_term : 補正後の軌道要素情報
 */
static void SGP4_calc_orientation_vector_(float position_vector[PHYSICAL_CONST_THREE_DIM],
                                          float velocity_vector[PHYSICAL_CONST_THREE_DIM],
                                          const Sgp4DerivedElement short_term);


// 公開関数
C2A_MATH_ERROR SGP4_initialize(Sgp4Constants* sgp4_constants, const TleInput tle_input)
{
  // アサーション
  if (tle_input.eccentricity < 0.0f) return C2A_MATH_ERROR_RANGE_OVER;
  if (tle_input.eccentricity >= 1.0f) return C2A_MATH_ERROR_RANGE_OVER;
  if (tle_input.epoch_day < 1.0f) return C2A_MATH_ERROR_RANGE_OVER;
  if (tle_input.epoch_day > 367.0f) return C2A_MATH_ERROR_RANGE_OVER;

  C2A_MATH_ERROR ret;

  sgp4_constants->input_ = SGP4_convert_tle_to_sgp4_unit_(tle_input);

  ret = SGP4_calc_original_mean_motion_(&(sgp4_constants->original_mm_), sgp4_constants->input_);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  // 低高度での空気密度関連パラメータ補正がここに入るが、通常の衛星では高度156km以下になったときは運用しないので、実装しない

  ret = SGP4_calc_secular_constants_(&(sgp4_constants->secular_), sgp4_constants);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  sgp4_constants->long_period_ = SGP4_calc_long_period_constants_(sgp4_constants);

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR SGP4_calc_orbit(float pos_eci_km[PHYSICAL_CONST_THREE_DIM],
                               float vel_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                               const Sgp4Constants* constants,
                               double time_jday)
{
  C2A_MATH_ERROR ret;
  Sgp4SecularTerm secular_term;
  ret = SGP4_calc_secular_term_(&secular_term, constants, time_jday);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  Sgp4LongPeriodTerm long_term;
  ret = SGP4_calc_long_period_term_(&long_term, constants, secular_term);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  float true_anomaly_rad = long_term.capital_l_t_ - secular_term.raan_rad_;
  true_anomaly_rad = SGP4_solve_kepler_equation_(long_term, true_anomaly_rad);

  Sgp4DerivedElement derived_element;
  ret = SGP4_calc_short_period_term_(&derived_element, long_term,
                                     secular_term.semi_major_axis_,
                                     secular_term.raan_rad_,
                                     true_anomaly_rad,
                                     constants->input_.inclination_rad_);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  // 最終的な位置速度計算
  float position_vector[PHYSICAL_CONST_THREE_DIM];
  float velocity_vector[PHYSICAL_CONST_THREE_DIM];
  SGP4_calc_orientation_vector_(position_vector, velocity_vector, derived_element);

  VECTOR3_scalar_product(pos_eci_km, derived_element.position_, position_vector);
  VECTOR3_scalar_product(pos_eci_km, PHYSICAL_CONST_EARTH_RADIUS_km, pos_eci_km);

  float first_term[PHYSICAL_CONST_THREE_DIM];
  float second_term[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_scalar_product(first_term, derived_element.velocity_, position_vector);
  VECTOR3_scalar_product(second_term, derived_element.velocity_f_, velocity_vector);
  VECTOR3_add(vel_eci_km_s, first_term, second_term);
  VECTOR3_scalar_product(vel_eci_km_s, PHYSICAL_CONST_EARTH_RADIUS_km * SGP4_kKe_s, vel_eci_km_s);

  return C2A_MATH_ERROR_OK;
}

// Static関数
static double SGP4_convert_tle_time_to_julian_day_(uint8_t tle_year, double tle_day)
{
  const double kJday2000 = 2451544.50; //!< 2000/1/1 0:0:0 でのユリウス日

  // うるう年の処理
  double leap_day = 1.0; // 2000年自体がうるう年なので1から始める
  uint8_t num_leap_year = tle_year / 4;
  uint8_t mod_year_4 = tle_year % 4;
  if (mod_year_4 == 0)
  {
    leap_day += ((double)(num_leap_year) - 1.0);  // その年が閏年の場合は閏日はtle_dayに含まれる
  }
  else
  {
    leap_day += (double)(num_leap_year);
  }
  return kJday2000 + (double)tle_year * 365.0 + leap_day + tle_day - 1.0; // DOYのため-1する
}

static Sgp4Input SGP4_convert_tle_to_sgp4_unit_(const TleInput tle_input)
{
  Sgp4Input sgp4_input;
  // first line
  sgp4_input.epoch_jday_ = SGP4_convert_tle_time_to_julian_day_(tle_input.epoch_year, tle_input.epoch_day);
  sgp4_input.b_star_ = tle_input.b_star;
  // second line
  sgp4_input.inclination_rad_     = PHYSICAL_CONST_degree_to_radian(tle_input.inclination_deg);
  sgp4_input.raan_rad_            = PHYSICAL_CONST_degree_to_radian(tle_input.raan_deg);
  sgp4_input.arg_perigee_rad_     = PHYSICAL_CONST_degree_to_radian(tle_input.arg_perigee_deg);
  sgp4_input.mean_anomaly_rad_    = PHYSICAL_CONST_degree_to_radian(tle_input.mean_anomaly_deg);
  sgp4_input.mean_motion_rad_min_ = PHYSICAL_CONST_rpd_to_rad_min(tle_input.mean_motion_rpd);
  sgp4_input.eccentricity_        = tle_input.eccentricity;

  sgp4_input.inclination_rad_  = C2A_MATH_normalize_zero_2pi(sgp4_input.inclination_rad_);
  sgp4_input.raan_rad_         = C2A_MATH_normalize_zero_2pi(sgp4_input.raan_rad_);
  sgp4_input.arg_perigee_rad_  = C2A_MATH_normalize_zero_2pi(sgp4_input.arg_perigee_rad_);
  sgp4_input.mean_anomaly_rad_ = C2A_MATH_normalize_zero_2pi(sgp4_input.mean_anomaly_rad_);

  return sgp4_input;
}

static C2A_MATH_ERROR SGP4_calc_original_mean_motion_(Sgp4OriginalMeanMotion* original_mm, const Sgp4Input sgp4_in)
{
  float mean_motion_rad_s = sgp4_in.mean_motion_rad_min_ / 60.0f;
  float a1 = powf(SGP4_kKe_s / mean_motion_rad_s, 2.0f / 3.0f);

  float temp_numerator = 3.0f * powf(cosf(sgp4_in.inclination_rad_), 2.0f) - 1.0f;
  float temp_denominator = powf((1.0f - powf(sgp4_in.eccentricity_, 2.0f)), 1.5f);
  if (fabsf(temp_denominator) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;
  float temp_c = temp_numerator / temp_denominator;

  float delta_1 = 0.75f * PHYSICAL_CONST_EARTH_J2 / powf(a1, 2.0f) * temp_c;
  float ao = a1 * (1.0f - delta_1 / 3.0f -
                          powf(delta_1, 2.0f) -
                          (134.0f / 81.0f) * powf(delta_1, 3.0f));
  float delta_o = 0.75f * PHYSICAL_CONST_EARTH_J2 / powf(ao, 2.0f) * temp_c;

  if (fabsf(1.0f + delta_o) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;
  original_mm->mean_motion_rad_min_ = sgp4_in.mean_motion_rad_min_ / (1.0f + delta_o);

  if (fabsf(1.0f - delta_o) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;
  original_mm->semi_major_axis_ = ao / (1.0f - delta_o);

  return C2A_MATH_ERROR_OK;
}

static C2A_MATH_ERROR SGP4_calc_secular_constants_(Sgp4SecularConstants* secular_constants, const Sgp4Constants* sgp4_const)
{
  // 短い変数名に置き換える
  const float i_rad = sgp4_const->input_.inclination_rad_;
  const float e = sgp4_const->input_.eccentricity_;
  const float arg_peri_rad = sgp4_const->input_.arg_perigee_rad_;
  const float b_star = sgp4_const->input_.b_star_;

  const float ao = sgp4_const->original_mm_.semi_major_axis_;
  const float no_rad_min = sgp4_const->original_mm_.mean_motion_rad_min_;
  if (fabsf(ao) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;
  if (fabsf(no_rad_min) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;

  // 共通定数の計算
  float cos_i = cosf(i_rad);
  if (fabsf(ao - SGP4_kDensityFunc) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;
  float xi = 1.0f / (ao - SGP4_kDensityFunc);

  float beta = C2A_MATH_sqrtf(1.0f - e * e);
  if (fabsf(beta) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;

  float eta = ao * e * xi;
  if (fabsf(1.0f - powf(eta, 2.0f)) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;

  // C定数
  float c[SGP4_NUM_COEFF_C];

  float coeff_c = SGP4_kQMS4 * powf(xi, 4.0f) * no_rad_min;
  float coeff_eta = powf((1.0f - powf(eta, 2.0f)), -3.5f);
  float term_c2_1 = ao * (1.0f + 4.0f * e * eta +
                                 1.5f * powf(eta, 2.0f) +
                                 e * powf(eta, 3.0f));
  float term_c2_2 = 0.75 * (PHYSICAL_CONST_EARTH_J2 * xi)/(1.0f - powf(eta, 2.0f)) *
                    (-0.5f + 1.5f * powf(cos_i, 2.0f)) *
                    (8.0f + 24.0f * powf(eta, 2.0f) + 3.0f * powf(eta, 4.0f));
  c[1] = coeff_c * coeff_eta * (term_c2_1 + term_c2_2);
  c[0] = b_star * c[1];
  if (e < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    c[2] = 0.0f;  // e = 0のときは係数は0
  }
  else
  {
    float sin_i = sinf(i_rad);
    c[2] = (coeff_c * xi * -1.0f * PHYSICAL_CONST_EARTH_J3 * sin_i) / (0.5f * PHYSICAL_CONST_EARTH_J2 * e);
  }
  float term_c4_1 = 2.0f * eta * (1.0f + e * eta) + 0.5f * e + 0.5f * powf(eta, 3.0f);
  float term_c4_2 = (PHYSICAL_CONST_EARTH_J2 * xi) / (ao * (1.0f - powf(eta, 2.0f)));
  float term_c4_3 = 3.0f * (1.0f - 3.0f * powf(cos_i, 2.0f)) *
                    (1.0f + 1.5f * powf(eta, 2.0f) - 2.0f * e * eta - 0.5f * e * powf(eta, 3.0f));
  float term_c4_4 = 0.75f * (1.0f - powf(cos_i, 2.0f)) *
                     (2.0f * eta - e * eta - e * powf(eta, 3.0f)) * cos(2.0f * arg_peri_rad);
  c[3] = 2.0f * ao * powf(beta, 2.0f) * coeff_c * coeff_eta *
         (term_c4_1 - term_c4_2 * (term_c4_3 + term_c4_4));
  c[4] = 2.0f * ao * powf(beta, 2.0f) * coeff_c / no_rad_min * coeff_eta *
         (1.0f + 11.0f / 4.0f * eta * (eta + e) + e * powf(eta, 3.0f));

  // D定数
  float d[SGP4_NUM_COEFF_D];
  d[0] = 0.0f;
  d[1] = 4.0f * ao * xi * powf(c[0], 2.0f);
  d[2] = 4.0f / 3.0f * ao * powf(xi, 2.0f) * (17.0f * ao + SGP4_kDensityFunc) * powf(c[0], 3.0f);
  d[3] = 2.0f / 3.0f * ao * powf(xi, 3.0f) * (221.0f * ao + 31.0f * SGP4_kDensityFunc) * powf(c[0], 4.0f);

  // DF項の計算
  float term_d_ma_df_2 = 0.75f * PHYSICAL_CONST_EARTH_J2 *
                      (-1.0f + 3.0f * powf(cos_i, 2.0f)) /
                      (powf(ao, 2.0f) * powf(beta, 3.0f));
  float term_d_ma_df_3 = 0.046875f * powf(PHYSICAL_CONST_EARTH_J2, 2.0f) *
                      (13.0f - 78.0f * powf(cos_i, 2.0f) + 137.0f * powf(cos_i, 4.0f)) /
                      (powf(ao, 4.0f) * powf(beta, 7.0f));
  float d_mean_anomaly_df_rad_min = (1.0f + term_d_ma_df_2 + term_d_ma_df_3) * no_rad_min;

  float term_d_ap_df_1 = -0.75f * PHYSICAL_CONST_EARTH_J2 *
                      (1.0f - 5.0f * powf(cos_i, 2.0f)) /
                      (powf(ao, 2.0f) * powf(beta, 4.0f));
  float term_d_ap_df_2 = 0.046875f * powf(PHYSICAL_CONST_EARTH_J2, 2.0f) *
                      (7.0f - 114.0f * powf(cos_i, 2.0f) + 395.0f * powf(cos_i, 4.0f)) /
                      (powf(ao, 4.0f) * powf(beta, 8.0f));
  float term_d_ap_df_3 = -0.046875f * PHYSICAL_CONST_EARTH_J4 *
                      (3.0f - 36.0f * powf(cos_i, 2.0f) + 49.0f * powf(cos_i, 4.0f)) /
                      (powf(ao, 4.0f) * powf(beta, 8.0f));
  float d_arg_peri_df_rad_min = (term_d_ap_df_1 + term_d_ap_df_2 + term_d_ap_df_3) * no_rad_min;

  float term_d_raan_df_1 = -1.5f * PHYSICAL_CONST_EARTH_J2 * cos_i /
                        (powf(ao, 2.0f) * powf(beta, 4.0f));
  float term_d_raan_df_2 = 0.375f * powf(PHYSICAL_CONST_EARTH_J2, 2.0f) *
                        (4.0f * cos_i - 19.0f * powf(cos_i, 3.0f)) /
                        (powf(ao, 4.0f) * powf(beta, 8.0f));
  float term_d_raan_df_3 = 0.9375f * PHYSICAL_CONST_EARTH_J4 *  cos_i *
                        (3.0f - 7.0f * powf(cos_i, 2.0f)) /
                        (powf(ao, 4.0f) * powf(beta, 8.0f));
  float d_raan_df_rad_min = (term_d_raan_df_1 + term_d_raan_df_2 + term_d_raan_df_3) * no_rad_min;

  // 変分項
  float d_arg_peri_rad_min = b_star * c[2] * cosf(arg_peri_rad);
  float d_raan_rad_min2 = -5.25f * no_rad_min * PHYSICAL_CONST_EARTH_J2 * cos_i * c[0] / (ao * powf(beta, 2.0f));
  float coeff_d_mean_anomaly;
  if (fabsf(e * eta) < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    coeff_d_mean_anomaly = 0.0f;
  }
  else
  {
    coeff_d_mean_anomaly = -2.0f / 3.0f * SGP4_kQMS4 * b_star * powf(xi, 4.0f) / (e * eta);
  }

  // Lの各項係数
  float coeff_l_1 = 1.5f * c[0];
  float coeff_l_2 = d[1] + 2.0f * powf(c[0], 2.0f);
  float coeff_l_3 = 0.25f * (3.0f * d[2] + 12.0f * c[0] * d[1] + 10.0f * powf(c[0], 3.0f));
  float coeff_l_4 = 0.2f  * (3.0f * d[3] + 12.0f * c[0] * d[2] + 6.0f * powf(d[1], 2.0f) +
                             30.0f * powf(c[0], 2.0f) * d[1] + 15.0f * powf(c[0], 4.0f));

  // 代入
  secular_constants->eta_ = eta;
  for (int idx = 0; idx < SGP4_NUM_COEFF_C; idx++)
  {
    secular_constants->coeff_c_[idx] = c[idx];
  }
  for (int idx = 0; idx < SGP4_NUM_COEFF_D; idx++)
  {
    secular_constants->coeff_d_[idx] = d[idx];
  }

  secular_constants->d_mean_anomaly_df_rad_min_ = d_mean_anomaly_df_rad_min;
  secular_constants->d_arg_peri_df_rad_min_ = d_arg_peri_df_rad_min;
  secular_constants->d_raan_df_rad_min_ = d_raan_df_rad_min;
  secular_constants->d_arg_peri_rad_min_ = d_arg_peri_rad_min;
  secular_constants->coeff_d_mean_anomaly_ = coeff_d_mean_anomaly;
  secular_constants->d_raan_rad_min2_ = d_raan_rad_min2;

  secular_constants->coeff_l_1_ = coeff_l_1;
  secular_constants->coeff_l_2_ = coeff_l_2;
  secular_constants->coeff_l_3_ = coeff_l_3;
  secular_constants->coeff_l_4_ = coeff_l_4;

  return C2A_MATH_ERROR_OK;
}

static Sgp4LongPeriodConstants SGP4_calc_long_period_constants_(const Sgp4Constants* sgp4_const)
{
  // 短い変数名に置き換える
  const float i_rad = sgp4_const->input_.inclination_rad_;

  // 共通変数
  float sin_i = sinf(i_rad);
  float cos_i = cosf(i_rad);

  // 長周期項定数
  Sgp4LongPeriodConstants long_period_const;
  long_period_const.coeff_ay_nl_ = -1.0f * PHYSICAL_CONST_EARTH_J3 * sin_i / (2.0f * PHYSICAL_CONST_EARTH_J2);

  if (fabs(1.0f + cos_i) < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    long_period_const.coeff_ll_ = 0.5f * long_period_const.coeff_ay_nl_ *
                                  (3.0f + 5.0f * cos_i) / (2.0f * MATH_CONST_PROHIBIT_DIVISION_VALUE);
  }
  else
  {
    long_period_const.coeff_ll_ = 0.5f * long_period_const.coeff_ay_nl_ *
                                  (3.0f + 5.0f * cos_i) / (1.0f + cos_i);
  }
  return long_period_const;
}

static C2A_MATH_ERROR SGP4_calc_secular_term_(Sgp4SecularTerm* secular_term,
                                       const Sgp4Constants* sgp4_const,
                                       const double time_jday)
{
  // 短い変数名に置き換える
  const float mo_rad = sgp4_const->input_.mean_anomaly_rad_;
  const float arg_peri_o_rad = sgp4_const->input_.arg_perigee_rad_;
  const float raan_o_rad = sgp4_const->input_.raan_rad_;
  const float e = sgp4_const->input_.eccentricity_;
  const float b_star = sgp4_const->input_.b_star_;

  const float ao = sgp4_const->original_mm_.semi_major_axis_;
  const float no_rad_min = sgp4_const->original_mm_.mean_motion_rad_min_;

  const float eta = sgp4_const->secular_.eta_;
  const float c1 = sgp4_const->secular_.coeff_c_[0];
  const float c4 = sgp4_const->secular_.coeff_c_[3];
  const float c5 = sgp4_const->secular_.coeff_c_[4];
  const float d2 = sgp4_const->secular_.coeff_d_[1];
  const float d3 = sgp4_const->secular_.coeff_d_[2];
  const float d4 = sgp4_const->secular_.coeff_d_[3];

  const float d_m_df_rad_min = sgp4_const->secular_.d_mean_anomaly_df_rad_min_;
  const float d_arg_peri_df_rad_min = sgp4_const->secular_.d_arg_peri_df_rad_min_;
  const float d_raan_df_rad_min = sgp4_const->secular_.d_raan_df_rad_min_;
  const float d_arg_peri_rad_min = sgp4_const->secular_.d_arg_peri_rad_min_;
  const float d_raan_rad_min2 = sgp4_const->secular_.d_raan_rad_min2_;
  const float coeff_d_mean_anomaly = sgp4_const->secular_.coeff_d_mean_anomaly_;

  const float coeff_l_1 = sgp4_const->secular_.coeff_l_1_;
  const float coeff_l_2 = sgp4_const->secular_.coeff_l_2_;
  const float coeff_l_3 = sgp4_const->secular_.coeff_l_3_;
  const float coeff_l_4 = sgp4_const->secular_.coeff_l_4_;

  // 時間差分
  // TODO_L ここだけdoubleにして他はfloatに明示的にキャストせずにかけている。
  // 精度上はあまり問題ないが気になるなら修正しても良い
  double dt_min = (time_jday - sgp4_const->input_.epoch_jday_) * 24.0 * 60.0;
  double dt_min2 = dt_min * dt_min;
  double dt_min3 = dt_min2 * dt_min;
  double dt_min4 = dt_min3 * dt_min;

  // DF
  float m_df_rad = mo_rad + d_m_df_rad_min * dt_min;
  float arg_peri_df_rad = arg_peri_o_rad + d_arg_peri_df_rad_min * dt_min;
  float raan_df_rad = raan_o_rad + d_raan_df_rad_min * dt_min;

  // 変分
  float d_arg_peri_rad = d_arg_peri_rad_min * dt_min;
  float d_mean_anomaly_rad = coeff_d_mean_anomaly * (powf(1.0f + eta * cosf(m_df_rad), 3.0f) -
                                                     powf(1.0f + eta * cosf(mo_rad),   3.0f));
  float d_raan_rad = d_raan_rad_min2 * dt_min2;

  // 永年摂動項
  float mp_rad = m_df_rad + d_arg_peri_rad + d_mean_anomaly_rad;
  secular_term->arg_peri_rad_ = arg_peri_df_rad - d_arg_peri_rad - d_mean_anomaly_rad;
  secular_term->raan_rad_ = raan_df_rad - d_raan_rad;
  secular_term->eccentricity_ = e - b_star * c4 * dt_min - b_star * c5 * (sinf(mp_rad) - sinf(mo_rad));
  float a_coeff = 1.0f - c1 * dt_min - d2 * dt_min2 - d3 * dt_min3 - d4 * dt_min4;
  secular_term->semi_major_axis_ = ao * powf(a_coeff, 2.0f);
  secular_term->capital_l_ = mp_rad + secular_term->arg_peri_rad_ + secular_term->raan_rad_ +
                             no_rad_min * dt_min2 * (coeff_l_1 +
                                                     coeff_l_2 * dt_min  +
                                                     coeff_l_3 * dt_min2 +
                                                     coeff_l_4 * dt_min3);

  // 角度は0 to 2piにする
  secular_term->arg_peri_rad_ = C2A_MATH_normalize_zero_2pi(secular_term->arg_peri_rad_);
  secular_term->raan_rad_ = C2A_MATH_normalize_zero_2pi(secular_term->raan_rad_);
  secular_term->capital_l_ = C2A_MATH_normalize_zero_2pi(secular_term->capital_l_);

  // 計算後のエラーチェック
  if (secular_term->eccentricity_ > 1.0f) return C2A_MATH_ERROR_RANGE_OVER;
  if (secular_term->eccentricity_ < 0.0f) return C2A_MATH_ERROR_RANGE_OVER;
  if (secular_term->semi_major_axis_ < 0.95f) return C2A_MATH_ERROR_RANGE_OVER;

  return C2A_MATH_ERROR_OK;
}

static C2A_MATH_ERROR SGP4_calc_long_period_term_(Sgp4LongPeriodTerm* long_term,
                                                  const Sgp4Constants* sgp4_const,
                                                  const Sgp4SecularTerm secular_term)
{
  // 短い変数名に置き換える
  const float e_s = secular_term.eccentricity_;
  const float arg_peri_s_rad = secular_term.arg_peri_rad_;
  float a_s = secular_term.semi_major_axis_;
  if (fabs(a_s) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;

  float beta_s2 = 1.0f - e_s * e_s;
  if (fabs(beta_s2) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;

  long_term->a_x_n_ = e_s * cosf(arg_peri_s_rad);

  float ll = sgp4_const->long_period_.coeff_ll_ * long_term->a_x_n_ / (a_s * beta_s2);
  long_term->capital_l_t_ = secular_term.capital_l_ + ll;

  long_term->a_y_n_ = e_s * sinf(arg_peri_s_rad) + sgp4_const->long_period_.coeff_ay_nl_ / (a_s * beta_s2);

  return C2A_MATH_ERROR_OK;
}

static float SGP4_solve_kepler_equation_(const Sgp4LongPeriodTerm long_term, const float u_rad)
{
  float eo = C2A_MATH_normalize_zero_2pi(u_rad);
  for (int itr = 0; itr < SGP4_kKeplerIterationLimit_; itr++)
  {
    float sin_eo = sinf(eo);
    float cos_eo = cosf(eo);
    float denominator = - long_term.a_y_n_ * sin_eo - long_term.a_x_n_ * cos_eo + 1.0f;
    float numerator = u_rad - long_term.a_y_n_ * cos_eo + long_term.a_x_n_ * sin_eo - eo;
    float d_eo = numerator / denominator;
    eo += d_eo;
    if (fabs(d_eo) < SGP4_kKeplerAngleLimit_rad_) break;
  }

  eo = C2A_MATH_normalize_zero_2pi(eo);
  return eo;
}

static C2A_MATH_ERROR SGP4_calc_short_period_term_(Sgp4DerivedElement* derived_element,
                                                   const Sgp4LongPeriodTerm long_term,
                                                   const float semi_major_axis,
                                                   const float raan_rad,
                                                   const float eo, const float inclination_rad)
{
  // 短い変数名に置き換える
  const float a_x_n = long_term.a_x_n_;
  const float a_y_n = long_term.a_y_n_;
  const float a = semi_major_axis;
  const float j2 = PHYSICAL_CONST_EARTH_J2;

  float cos_i = cosf(inclination_rad);
  float sin_i = sinf(inclination_rad);

  // 演算
  float e_cos = a_x_n * cosf(eo) + a_y_n * sinf(eo);
  float e_sin = a_x_n * sinf(eo) - a_y_n * cosf(eo);
  float e_l2 = a_x_n * a_x_n + a_y_n * a_y_n;
  float p_l = a * (1.0f - e_l2);
  if (fabs(p_l) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;

  float position = a * (1.0f - e_cos);
  if (fabs(position) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;
  float velocity = C2A_MATH_sqrtf(a) / position * e_sin;
  float velocity_f = C2A_MATH_sqrtf(p_l) / position;

  float coeff_temp = e_sin / (1.0f + C2A_MATH_sqrtf(1.0f - e_l2));
  float cos_u = cosf(eo) - a_x_n + a_y_n * coeff_temp;  // sin_uと割り算取るのでどちらもa/rはかけない
  float sin_u = sinf(eo) - a_y_n - a_x_n * coeff_temp;
  float u = atan2f(sin_u, cos_u);
  float cos_2u = cosf(2.0f * u);
  float sin_2u = sinf(2.0f * u);

  float coeff_common = j2 / (4.0f * p_l);
  float cos_i2 = powf(cos_i, 2.0f);
  float n = 1.0f / powf(a, 1.5f);

  float d_position = coeff_common * (1.0f - cos_i2) * cos_2u;
  float d_true_anomaly_rad = - coeff_common / (2.0f * p_l) * (7.0f * cos_i2 - 1.0f) * sin_2u;
  float d_raan_rad = 3.0f * coeff_common / p_l * cos_i * sin_2u;
  float d_inclination_rad = 3.0f * coeff_common / p_l * cos_i * sin_i * cos_2u;
  float d_velocity = -2.0f * coeff_common * n * (1.0f - cos_i2) * sin_2u;
  float d_velocity_f = 2.0f * coeff_common * n * ((1.0f - cos_i2) * cos_2u -
                                                  1.5f * (1.0f - 3.0f * cos_i2));

  // 補正後の値
  derived_element->position_ = d_position + position * (1.0f - 0.75f * j2 * (3.0f * cos_i2 - 1.0f) *
                                                       C2A_MATH_sqrtf(1.0f - e_l2) / (p_l * p_l));
  derived_element->true_anomaly_rad_ = u - d_true_anomaly_rad;
  derived_element->raan_rad_ = raan_rad + d_raan_rad;
  derived_element->inclination_rad_ = inclination_rad + d_inclination_rad;
  derived_element->velocity_ = velocity + d_velocity;
  derived_element->velocity_f_ = velocity_f + d_velocity_f;

  // 地球に衝突している
  if (derived_element->position_ < 0.95f) return C2A_MATH_ERROR_RANGE_OVER;

  return C2A_MATH_ERROR_OK;
}

static void SGP4_calc_orientation_vector_(float position_vector[PHYSICAL_CONST_THREE_DIM],
                                          float velocity_vector[PHYSICAL_CONST_THREE_DIM],
                                          const Sgp4DerivedElement short_term)
{
  float m[PHYSICAL_CONST_THREE_DIM];
  m[0] = - sinf(short_term.raan_rad_) * cosf(short_term.inclination_rad_);
  m[1] =   cosf(short_term.raan_rad_) * cosf(short_term.inclination_rad_);
  m[2] =   sinf(short_term.inclination_rad_);

  float n[PHYSICAL_CONST_THREE_DIM];
  n[0] = cosf(short_term.raan_rad_);
  n[1] = sinf(short_term.raan_rad_);
  n[2] = 0.0f;

  float sin_u = sinf(short_term.true_anomaly_rad_);
  float cos_u = cosf(short_term.true_anomaly_rad_);

  float first_term[PHYSICAL_CONST_THREE_DIM];
  float second_term[PHYSICAL_CONST_THREE_DIM];

  VECTOR3_scalar_product(first_term, sin_u, m);
  VECTOR3_scalar_product(second_term, cos_u, n);
  VECTOR3_add(position_vector, first_term, second_term);

  VECTOR3_scalar_product(first_term, cos_u, m);
  VECTOR3_scalar_product(second_term, -1.0f * sin_u, n);
  VECTOR3_add(velocity_vector, first_term, second_term);

  return;
}

#pragma section
