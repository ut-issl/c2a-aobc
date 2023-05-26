/**
 * @file
 * @brief SGP4軌道伝搬に関するライブラリ
 * @note  全体的にfloatで計算している。元期からの経過時間が延びると桁落ちで精度が悪化する。
 * @note  1日程度なら数km精度。
 */
#ifndef SGP4_H_
#define SGP4_H_

#include <stdint.h>
#include "../physical_constants.h"
#include "../c2a_math.h"

#define SGP4_NUM_COEFF_C (5)  //!< C係数の要素数
#define SGP4_NUM_COEFF_D (4)  //!< D係数の要素数

/**
 * @struct TleInput
 * @brief  入力値として使うTLE内パラメータの構造体
 * @note   平均運動の1階微分と2階微分はSGP4では使わない
 */
typedef struct
{
  // 第一行
  uint8_t epoch_year;     //!< 元期年の下二桁、2000年以降と想定している
  double  epoch_day;      //!< 元期の年間通日+少数項 [day] [1.0 ~ 367.0]
  float   b_star;         //!< 抗力係数 [-]
  // 第二行
  float inclination_deg;  //!< 軌道傾斜角 [deg]
  float raan_deg;         //!< 昇降点赤経 [deg]
  float eccentricity;     //!< 離心率 [-] [0.0, 1.0)
  float arg_perigee_deg;  //!< 近地点引数 [deg]
  float mean_anomaly_deg; //!< 平均近点離角 [deg]
  float mean_motion_rpd;  //!< 平均運動(古在の平均運動) [rotation/day]
  // TODO_L 平均運動はdoubleにするべき
} TleInput;

/**
 * @struct Sgp4Input
 * @brief  SGP4入力用にTLEを単位変換したもの
 */
typedef struct
{
  double epoch_jday_;             //!< 元期 [ユリウス日]
  float  b_star_;                 //!< 抗力係数 [-]
  float  inclination_rad_;        //!< 軌道傾斜角 [rad]
  float  raan_rad_;               //!< 昇降点赤経 [rad]
  float  eccentricity_;           //!< 離心率 [-]
  float  arg_perigee_rad_;        //!< 近地点引数 [rad]
  float  mean_anomaly_rad_;       //!< 平均近点離角 [rad]
  float  mean_motion_rad_min_;    //!< 平均運動(古在の平均運動) [rad/min]
} Sgp4Input;

/**
 * @struct Sgp4OriginalMeanMotion
 * @brief  古在の平均運動ではない、SGP4て使われている平均運動
 */
typedef struct
{
  float mean_motion_rad_min_; //!< 平均運動 [rad/min]
  float semi_major_axis_;     //!< 地球半径で正規化された軌道長半径 [-]
} Sgp4OriginalMeanMotion;

/**
 * @struct Sgp4SecularConstants
 * @brief  永年摂動項に関する定数
 * @note   各変数に明確な物理的な意味があるわけではないので、変数説明コメントは付けない(つけれない)
 * @note   変数名は参考文献に対応する形で付けている(仕様書で説明する)
 */
typedef struct
{
  float eta_;
  float coeff_c_[SGP4_NUM_COEFF_C];
  float coeff_d_[SGP4_NUM_COEFF_D];
  float coeff_l_1_;
  float coeff_l_2_;
  float coeff_l_3_;
  float coeff_l_4_;
  float d_mean_anomaly_df_rad_min_;
  float d_arg_peri_df_rad_min_;
  float d_raan_df_rad_min_;
  float d_arg_peri_rad_min_;
  float coeff_d_mean_anomaly_;
  float d_raan_rad_min2_;
} Sgp4SecularConstants;

/**
 * @struct Sgp4LongPeriod
 * @brief  長周期摂動項に関する定数
 * @note   各変数に明確な物理的な意味があるわけではないので、変数説明コメントは付けない(つけれない)
 * @note   変数名は参考文献に対応する形で付けている(仕様書で説明する)
 */
typedef struct
{
  float coeff_ay_nl_;
  float coeff_ll_;
} Sgp4LongPeriodConstants;

/**
 * @struct Sgp4Constants
 * @brief  SGP4におけるTLE依存の定数値
 * @note   TLEが更新されると再計算が必要だが、それまではこの定数値を使い軌道計算を行う
 */
typedef struct
{
  Sgp4Input input_;                     //!< TLEをSGP4単位系に変換した後の値
  Sgp4OriginalMeanMotion original_mm_;  //!< SGP4で使われる平均運動定義
  Sgp4SecularConstants secular_;        //!< 永年摂動項定数
  Sgp4LongPeriodConstants long_period_; //!< 長周期摂動項定数
} Sgp4Constants;

/**
 * @brief TLEを入力値としてSGP4関連定数を更新する
 * @param[in]  tle_input       : TLE入力値
 * @param[out] sgp4_constants  : SGP4定数出力
 * @return : C2A_MATH_ERRORに準拠
 */
C2A_MATH_ERROR SGP4_initialize(Sgp4Constants* sgp4_constants, const TleInput tle_input);

/**
 * @brief SGP4による軌道計算を行う
 * @param[out] pos_eci_km   : 衛星位置 @ ECI [km]
 * @param[out] vel_eci_km_s : 衛星速度 @ ECI [km/s]
 * @param[in]  constants    : SGP4定数
 * @param[in]  time_jday    : 位置速度を求めたい時刻 [ユリウス日]
 * @return : C2A_MATH_ERRORに準拠
 */
C2A_MATH_ERROR SGP4_calc_orbit(float pos_eci_km[PHYSICAL_CONST_THREE_DIM],
                               float vel_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                               const Sgp4Constants* constants,
                               double time_jday);

#endif
