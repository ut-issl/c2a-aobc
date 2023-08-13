#pragma section REPRO
/**
* @file   time_space.c
* @brief  各種時刻換算や座標変換用の関数群の定義
*/

#include "time_space.h"

#include <math.h>

#include "physical_constants.h"
#include "math_constants.h"
#include "c2a_math.h"
#include "matrix33.h"
#include "vector3.h"

/**
 * @brief      日周運動を表すR行列の計算
 * @param[in]  gast_rad       : Greenwich Apparent Sidereal Time [rad]
 * @param[out] axial_rotation : R
 * @note       歳差・章動を考慮しない場合，gast_radの代わりにgmst_radを用いることで，
               axial_rotationがECI/ECEF変換のDCMとなる。
 *             GMSTが0に近いとき，GASTは負の値になりうる。
 */
static void TIME_SPACE_axial_rotation_(const double gast_rad, double axial_rotation[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      歳差を表すP行列の計算
 * @param[in]  julian_century_terrestrial_time : Julian Century in Terrestrial Time [century]
                                                 (should be larger than zero)
 * @param[out] precession : P (set unit matrix when the input is out-of-range)
 * @return     C2A_MATH_ERROR_RANGE_OVER : Julian Century in Terrestrial Timeが0以下
 * @return     C2A_MATH_ERROR_OK
 */
static C2A_MATH_ERROR TIME_SPACE_precession_(const double julian_century_terrestrial_time[] ,
                                                   double precession[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      章動を表すN行列の計算
 * @param[in]  julian_century_terrestrial_time : Julian Century in Terrestrial Time [century]
                                                 (should be larger than zero)
 * @param[out] nutation : N (set unit matrix when the input is out-of-range)
 * @param[out] epsilon_rad
 * @param[out] depsilon_rad
 * @param[out] dpsi_rad
 * @return     C2A_MATH_ERROR_RANGE_OVER : Julian Century in Terrestrial Timeが0以下
 * @return     C2A_MATH_ERROR_OK
 * @note       ここでepsilon_rad,depsilon_rad,dpsi_radが更新される
 */
static C2A_MATH_ERROR TIME_SPACE_nutation_(const double julian_century_terrestrial_time[] ,
                                                 double nutation[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                 double* epsilon_rad, double* delta_epsilon_rad, double* delta_psi_rad);

double TIME_SPACE_convert_gpstime_to_julian_day(const uint16_t gps_time_week, const uint32_t gps_time_msec)
{
  const double kJulianDayAtGpsTimeEpoch_day = 2444244.5;  //!< Julian Day at Origin of GPSTime (1980/1/6 00:00)
  const double kMsecToSec     = 1.0e-3;                   //!< conversion from msec to sec
  const double kDayOfWeek_day = 7.0;                      //!< days of a week
  const double kSecOfWeek_sec = kDayOfWeek_day * (double)(PHYSICAL_CONST_EARTH_SOLAR_DAY_s);

  double gps_time_sec = (double)(gps_time_msec) * kMsecToSec;

  if (gps_time_sec > kSecOfWeek_sec) return kJulianDayAtGpsTimeEpoch_day;

  // Since both GPSTime and JulianDay have no leap seconds, they are directly converted to each other.
  double elapsed_julian_day = (double)(gps_time_week) * kDayOfWeek_day +
                               gps_time_sec / PHYSICAL_CONST_EARTH_SOLAR_DAY_s;

  return (kJulianDayAtGpsTimeEpoch_day + elapsed_julian_day);
}


double TIME_SPACE_convert_julian_day_to_century(const double julian_day)
{
  const double kJulianDayAtJ2000Epoch_day = 2451545.0;  //!< Julian Day at Epoch of J2000 (2000/1/1 12:00)

  if (julian_day < kJulianDayAtJ2000Epoch_day) return 0.0;

  return ((julian_day - kJulianDayAtJ2000Epoch_day) / (double)(PHYSICAL_CONST_DAY_OF_CENTURY_day));
}


float TIME_SPACE_convert_julian_day_to_decimal_year(const double julian_day, const double julian_day_base)
{
  if (julian_day < julian_day_base) return 0.0f;

  return (float)(((julian_day - julian_day_base) / (double)(PHYSICAL_CONST_DAY_OF_CENTURY_day * 1.0e-2)));
}


// TODO_L: 必要に応じて桁は削る
double TIME_SPACE_calc_gmst_rad(const double julian_day)
{
  if (julian_day <= 0) return 0.0;

  double julian_century = TIME_SPACE_convert_julian_day_to_century(julian_day);

  const double kOffsetNoonMidnight_day = 0.5; //!< offset day of noon from midnight [day]
  uint32_t jday_integer = (uint32_t)(julian_day);
  double elapsed_day = (julian_day - (double)(jday_integer)) + kOffsetNoonMidnight_day;
  if (elapsed_day >= 1.0)
  {
    elapsed_day = elapsed_day - 1.0;
  }
  double elapsed_time_of_day_sec = elapsed_day * PHYSICAL_CONST_EARTH_SOLAR_DAY_s;

  //!< Coefficients to compute Greenwich Mean Side Real Time (GMST)[hour angle/century]
  const double kCoeffTimeToGmst_hour_angle_century[] = { 24110.54841, 8640184.812866, 0.093104, -6.2e-6 };
  //!< ratio of the length of 1 seconds in Sidereal compare to Universal Time [-]
  const double kCoeffSecOfDayToGmst_hour_angle_sec = 1.002737909350975;

  double pow2_of_julian_century = julian_century * julian_century;
  double pow3_of_julian_century = pow2_of_julian_century * julian_century;

  double elapsed_day_ratio = (double)(elapsed_time_of_day_sec) / (double)(PHYSICAL_CONST_EARTH_SOLAR_DAY_s);
  double julian_century_at_midnight_day = julian_century -
                                          (elapsed_day_ratio / (double)(PHYSICAL_CONST_DAY_OF_CENTURY_day)); //!< JDUT1(0h)

  double gmst_hour_angle = kCoeffTimeToGmst_hour_angle_century[0] +
                           kCoeffTimeToGmst_hour_angle_century[1] * julian_century_at_midnight_day +
                           kCoeffTimeToGmst_hour_angle_century[2] * pow2_of_julian_century +
                           kCoeffTimeToGmst_hour_angle_century[3] * pow3_of_julian_century +
                           kCoeffSecOfDayToGmst_hour_angle_sec * (double)(elapsed_time_of_day_sec);
  double gmst_rad = PHYSICAL_CONST_hour_angle_to_rad_double(gmst_hour_angle);

  return C2A_MATH_normalize_zero_2pi_double(gmst_rad);
}


void TIME_SPACE_calc_sun_direction_eci(const double julian_century, float sun_direction_eci[3])
{
  if ((julian_century <= 0))
  {
    for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
    {
      sun_direction_eci[i] = 0.0f;
    }
    return;
  }

  //!< Mean obliquity of ecliptic [rad]
  const double kEclipticEpsilon_rad = 4.0909280E-01;
  //!< Coefficients to compute Mean Anomaly of Sun [rad/century]
  const double kCoeffsMeanAnomalySun_rad_century[] = { 6.2399989E+00, 6.2830186E+02 };
  //!< Coefficients to compute Ecliptic Longitude [rad/century]
  const double kCoeffsEclipticLongitude_rad_century[]
               = { 4.9382346E+00, 3.3413359E-02, 3.4906585E-04, 6.0606129E-03, -4.6286132E-05 };

  // TODO_L:別衛星では2piのclipをせずともそのままいけたが，今回大丈夫かは要確認
  double mean_anomaly_sun_rad  = (kCoeffsMeanAnomalySun_rad_century[0] +
                                  kCoeffsMeanAnomalySun_rad_century[1] * julian_century);
  double ecliptic_longitude_rad = kCoeffsEclipticLongitude_rad_century[0] +
                                  kCoeffsEclipticLongitude_rad_century[1] * sin(mean_anomaly_sun_rad) +
                                  kCoeffsEclipticLongitude_rad_century[2] * sin(2.0 * mean_anomaly_sun_rad) +
                                  kCoeffsEclipticLongitude_rad_century[3] * julian_century +
                                  kCoeffsEclipticLongitude_rad_century[4] + mean_anomaly_sun_rad;

  float sun_direction_eci_unnormalize[PHYSICAL_CONST_THREE_DIM];
  sun_direction_eci_unnormalize[0] = (float)(cos(ecliptic_longitude_rad));
  sun_direction_eci_unnormalize[1] = (float)(sin(ecliptic_longitude_rad) * cos(kEclipticEpsilon_rad));
  sun_direction_eci_unnormalize[2] = (float)(sin(ecliptic_longitude_rad) * sin(kEclipticEpsilon_rad));

  VECTOR3_normalize(sun_direction_eci, sun_direction_eci_unnormalize);
}


C2A_MATH_ERROR TIME_SPACE_convert_geodetic_to_geocentric(float* colat_rad, float* radius_m,
                                                   const double lla_rad_m[PHYSICAL_CONST_THREE_DIM])
{
  float lat_rad   = (float)(lla_rad_m[0]);
  float height_m  = (float)(lla_rad_m[2]);

  //!< Re*cos_lat
  float re_earth_cos_lat = PHYSICAL_CONST_EARTH_RADIUS_m * cosf(lat_rad);
  //!< Rp*sin_lat
  float rp_earth_sin_lat = PHYSICAL_CONST_EARTH_POLAR_RADIUS_m * sinf(lat_rad);
  //!< Rm^2 = (Re*cos_lat)^2 + (Rp*sin_lat)^2
  float pow2_radius_mean = re_earth_cos_lat * re_earth_cos_lat + rp_earth_sin_lat * rp_earth_sin_lat;
  float radius_mean      = C2A_MATH_sqrtf(pow2_radius_mean); //!< mean radius at the interest point

  //!< Re^2
  float pow2_earth_re = PHYSICAL_CONST_EARTH_RADIUS_m * PHYSICAL_CONST_EARTH_RADIUS_m;
  //!< (Re^2*cos_lat)^2
  float pow2_re2_cos  = pow2_earth_re * re_earth_cos_lat * re_earth_cos_lat;

  //!< Rp^2
  float pow2_earth_rp = PHYSICAL_CONST_EARTH_POLAR_RADIUS_m * PHYSICAL_CONST_EARTH_POLAR_RADIUS_m;
  //!< (Rp^2*sin_lat)^2
  float pow2_rp2_sin  = pow2_earth_rp * rp_earth_sin_lat * rp_earth_sin_lat;

  //!< 1st_term of r^2 = [(Re^2*cos_lat)^2 + (Rp^2*sin_lat)^2]/(Rm^2)
  float pow2_radius_m_1st_term = (pow2_re2_cos + pow2_rp2_sin) / pow2_radius_mean;

  //!< r^2 = 1st_term of r^2 + 2h*Rm + h^2
  float pow2_radius_m = pow2_radius_m_1st_term + 2.0f * height_m * radius_mean + height_m * height_m;
  *radius_m = C2A_MATH_sqrtf(pow2_radius_m);

  //!< cos(colat) = (sin_lat/r)*(h + (Rp^2)/Rm)
  float cos_colat = (sinf(lat_rad) / *radius_m) * (height_m + pow2_earth_rp / radius_mean);

  *colat_rad = C2A_MATH_acos_rad(cos_colat);

  // check validity and returns warning
  return C2A_MATH_check_abs_range_violation(cos_colat, 1.0f);
}


C2A_MATH_ERROR TIME_SPACE_convert_ecef_to_lla(double lla_rad_m[PHYSICAL_CONST_THREE_DIM],
                                        const double pos_ecef_m[PHYSICAL_CONST_THREE_DIM])
{
  double Re = (double)PHYSICAL_CONST_EARTH_RADIUS_m;
  double Re_p = (double)PHYSICAL_CONST_EARTH_POLAR_RADIUS_m;
  double ee1 = (double)PHYSICAL_CONST_EARTH_FIRST_ECCENTRICITY_SQUARED;
  double ee2 = (double)PHYSICAL_CONST_EARTH_SECOND_ECCENTRICITY_SQUARED;

  double distance_p_m = sqrt(pos_ecef_m[0] * pos_ecef_m[0] + pos_ecef_m[1] * pos_ecef_m[1]);

  double dot_z_earth_radius_a  = pos_ecef_m[2] * Re;
  double dot_xy_earth_radius_b = distance_p_m * Re_p;

  double theta_rad = atan(dot_z_earth_radius_a / dot_xy_earth_radius_b);

  double sin_theta = sin(theta_rad);
  double cos_theta = cos(theta_rad);

  double num_lat = pos_ecef_m[2] + ee2 * Re_p * sin_theta * sin_theta * sin_theta;
  double den_lat = distance_p_m - ee1 * Re * cos_theta * cos_theta * cos_theta;

  lla_rad_m[0] = atan(num_lat / den_lat);
  lla_rad_m[1] = atan2(pos_ecef_m[1], pos_ecef_m[0]);

  double radius_n = Re / sqrt(1.0 - ee1 * sin(lla_rad_m[0]) * sin(lla_rad_m[0]));
  lla_rad_m[2] = (distance_p_m / cos(lla_rad_m[0])) - radius_n;

  return C2A_MATH_ERROR_OK;
}


C2A_MATH_ERROR TIME_SPACE_convert_lla_to_ecef(double pos_ecef_m[PHYSICAL_CONST_THREE_DIM],
                                        const double lla_rad_m[PHYSICAL_CONST_THREE_DIM])
{
  double Re = (double)PHYSICAL_CONST_EARTH_RADIUS_m;
  double ee1 = (double)PHYSICAL_CONST_EARTH_FIRST_ECCENTRICITY_SQUARED;

  double radius_n = Re / sqrt(1.0 - ee1 * sin(lla_rad_m[0]) * sin(lla_rad_m[0]));
  pos_ecef_m[0] = (radius_n + lla_rad_m[2]) * cos(lla_rad_m[0]) * cos(lla_rad_m[1]);
  pos_ecef_m[1] = (radius_n + lla_rad_m[2]) * cos(lla_rad_m[0]) * sin(lla_rad_m[1]);
  pos_ecef_m[2] = (radius_n * (1.0 - ee1) + lla_rad_m[2]) * cos(lla_rad_m[0]) * sin(lla_rad_m[1]);

  return C2A_MATH_ERROR_OK;
}


void TIME_SPACE_trans_ned_to_ecef(float dcm_ned_to_ecef[][PHYSICAL_CONST_THREE_DIM],
                            const float lat_rad, const float lon_rad)
{
  dcm_ned_to_ecef[0][0] = - cosf(lon_rad) * sinf(lat_rad);
  dcm_ned_to_ecef[1][0] = - sinf(lat_rad) * sinf(lon_rad);
  dcm_ned_to_ecef[2][0] =   cosf(lat_rad);

  dcm_ned_to_ecef[0][1] = - sinf(lon_rad);
  dcm_ned_to_ecef[1][1] =   cosf(lon_rad);
  dcm_ned_to_ecef[2][1] =   0.0f;

  dcm_ned_to_ecef[0][2] = - cosf(lon_rad) * cosf(lat_rad);
  dcm_ned_to_ecef[1][2] = - cosf(lat_rad) * sinf(lon_rad);
  dcm_ned_to_ecef[2][2] = - sinf(lat_rad);
}


static void TIME_SPACE_axial_rotation_(const double gast_rad, double axial_rotation[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  MATRIX33_make_rotation_double(axial_rotation, gast_rad, MATRIX33_ROTATION_AXIS_Z);
}

static C2A_MATH_ERROR TIME_SPACE_precession_(const double julian_century_terrestrial_time[] ,
                                                   double precession[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  if (julian_century_terrestrial_time[0] <= 0.0)
  {
    MATRIX33_make_unit_double(precession);

    return C2A_MATH_ERROR_RANGE_OVER;
  }

  const double kCoeffsZeta_rad[3]
               = {   1.11808608650244E-02,   // [rad/century]
                     1.46355554053347E-06,   // [rad/century^2]
                     8.72567663260942E-08 }; // [rad/century^3]
  const double kCoeffsTheta_rad[3]
               = {   9.71717345516966E-03,   // [rad/century]
                   - 2.06845757045383E-06,   // [rad/century^2]
                   - 2.02812107218552E-07 }; // [rad/century^3]
  const double kCoeffsZ_rad[3]
               = {   1.11808608650244E-02,   // [rad/century]
                     5.30715840436986E-06,   // [rad/century^2]
                     8.82506343723687E-08 }; // [rad/century^3]

  // precession angles
  double zeta_rad = 0.0;
  double theta_rad = 0.0;
  double z_rad = 0.0;

  for (int i = 0; i < 3; i++)
  {
    zeta_rad += kCoeffsZeta_rad[i] * julian_century_terrestrial_time[i];
    theta_rad += kCoeffsTheta_rad[i] * julian_century_terrestrial_time[i];
    z_rad += kCoeffsZ_rad[i] * julian_century_terrestrial_time[i];
  }

  double rotation_zeta_z[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  double rotation_theta_y[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  double rotation_z_z[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_make_rotation_double(rotation_zeta_z, - zeta_rad, MATRIX33_ROTATION_AXIS_Z);
  MATRIX33_make_rotation_double(rotation_theta_y, theta_rad, MATRIX33_ROTATION_AXIS_Y);
  MATRIX33_make_rotation_double(rotation_z_z, - z_rad, MATRIX33_ROTATION_AXIS_Z);

  MATRIX33_multiply_matrix_matrix_double(precession, rotation_z_z, rotation_theta_y);
  MATRIX33_multiply_matrix_matrix_double(precession, precession, rotation_zeta_z);

  return C2A_MATH_ERROR_OK;
}

static C2A_MATH_ERROR TIME_SPACE_nutation_(const double julian_century_terrestrial_time[],
                                                 double nutation[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                 double* epsilon_rad, double* delta_epsilon_rad, double* delta_psi_rad)
{
  if (julian_century_terrestrial_time[0] <= 0.0)
  {
    MATRIX33_make_unit_double(nutation);

    return C2A_MATH_ERROR_RANGE_OVER;
  }

  const double kCoeffsEpsilon_rad[4]
    = {   4.09092804028403E-01,   // [rad]
        - 2.26965524811429E-04,   // [rad/century]
        - 2.86040071854626E-09,   // [rad/century^2]
          8.78967203851588E-09 }; // [rad/century^3]

  const double kCoeffsAnomalyMoon_rad[5]
    = {   2.35555574349388E+00,   // [rad]
          8.32869142571908E+03,   // [rad/century]
          1.54554723028271E-04,   // [rad/century^2]
          2.50333544240909E-07,   // [rad/century^3]
        - 1.18633907767503E-09 }; // [rad/century^4]
  const double kCoeffsAnomalySun_rad[5]
    = {   6.24006012691328E+00,   // [rad]
          6.28301955171396E+02,   // [rad/century]
        - 2.68198928389795E-06,   // [rad/century^2]
          6.59346606308968E-10,   // [rad/century^3]
        - 5.57050919594856E-11 }; // [rad/century^4]
  const double kCoeffsArgumentLatitudeMoon_rad[5]
    = {   1.62790508153752E+00,   // [rad]
          8.43346615691636E+03,   // [rad/century]
        - 6.18195621056391E-05,   // [rad/century^2]
        - 5.02751787310588E-09,   // [rad/century^3]
          2.02167305022676E-11 }; // [rad/century^4]
  const double kCoeffsElongationMoon_rad[5]
    = {   5.19846658866019E+00,   // [rad]
          7.77137714559371E+03,   // [rad/century]
        - 3.08855403687641E-05,   // [rad/century^2]
          3.19637659955517E-08,   // [rad/century^3]
        - 1.53637455543612E-10 }; // [rad/century^4]
  const double kCoeffsLongitudeAscendingNodeMoon_rad[5]
    = {   2.18243919661567E+00,   // [rad]
        - 3.37570459536308E+01,   // [rad/century]
          3.62262478798667E-05,   // [rad/century^2]
          3.73403497190564E-08,   // [rad/century^3]
        - 2.87930845210953E-10 }; // [rad/century^4]

  const double kCoeffsDeltaEpsilon_rad[9]
    = {
          4.46270993461327E-05,
          2.77798239275764E-06,
        - 4.36332312998582E-07,
          4.75117407487345E-07,
          3.39369576776675E-08,
        - 4.84813681109535E-09,
          1.06659009844098E-07,
          6.30257785442396E-08,
        - 4.84813681109535E-08
      };
  const double kCoeffsDeltaPsi_rad[9]
    = {
        - 8.34170419717067E-05,
        - 6.38499618021258E-06,
          1.00356431989674E-06,
        - 1.10537519292974E-06,
          7.17524248042112E-07,
          3.44217713587770E-07,
        - 2.52103114176958E-07,
        - 1.45444104332861E-07,
          1.06659009844098E-07
      };

  *epsilon_rad = kCoeffsEpsilon_rad[0];
  for (int i = 0; i < 3; i++)
  {
    *epsilon_rad += kCoeffsEpsilon_rad[i + 1] * julian_century_terrestrial_time[i];
  }

  // Delauney angles
  //!< l : mean anomaly of the moon [rad]
  double anomaly_moon_rad = kCoeffsAnomalyMoon_rad[0];
  //!< l' : mean anomaly of the sun [rad]
  double anomaly_sun_rad = kCoeffsAnomalySun_rad[0];
  //!< F : mean argument of latitude of the moon [rad]
  double argument_latitude_moon_rad = kCoeffsArgumentLatitudeMoon_rad[0];
  //!< D : mean elongation of the moon from the sun [rad]
  double elongation_moon_rad = kCoeffsElongationMoon_rad[0];
   //!< O : mean longitude of ascending node of the moon [rad]
  double longitude_ascending_node_moon_rad = kCoeffsLongitudeAscendingNodeMoon_rad[0];
  for (int i = 0; i < 4; i++)
  {
    anomaly_moon_rad += kCoeffsAnomalyMoon_rad[i + 1] * julian_century_terrestrial_time[i];
    anomaly_sun_rad += kCoeffsAnomalySun_rad[i + 1] * julian_century_terrestrial_time[i];
    argument_latitude_moon_rad += kCoeffsArgumentLatitudeMoon_rad[i + 1] * julian_century_terrestrial_time[i];
    elongation_moon_rad += kCoeffsElongationMoon_rad[i + 1] * julian_century_terrestrial_time[i];
    longitude_ascending_node_moon_rad += kCoeffsLongitudeAscendingNodeMoon_rad[i + 1] * julian_century_terrestrial_time[i];
  }

  double L_rad = argument_latitude_moon_rad + longitude_ascending_node_moon_rad;
  double Ld_rad = L_rad - elongation_moon_rad;

  *delta_epsilon_rad = kCoeffsDeltaEpsilon_rad[0] * cos(longitude_ascending_node_moon_rad) +
                       kCoeffsDeltaEpsilon_rad[1] * cos(2.0 * Ld_rad) +
                       kCoeffsDeltaEpsilon_rad[2] * cos(2.0 * longitude_ascending_node_moon_rad) +
                       kCoeffsDeltaEpsilon_rad[3] * cos(2.0 * L_rad) +
                       kCoeffsDeltaEpsilon_rad[4] * cos(anomaly_sun_rad) +
                       kCoeffsDeltaEpsilon_rad[5] * cos(anomaly_moon_rad) +
                       kCoeffsDeltaEpsilon_rad[6] * cos(2.0 * Ld_rad + anomaly_sun_rad) +
                       kCoeffsDeltaEpsilon_rad[7] * cos(2.0 * L_rad + anomaly_moon_rad) +
                       kCoeffsDeltaEpsilon_rad[8] * cos(2.0 * Ld_rad - anomaly_sun_rad);
  *delta_psi_rad = kCoeffsDeltaPsi_rad[0] * sin(longitude_ascending_node_moon_rad) +
                   kCoeffsDeltaPsi_rad[1] * sin(2.0 * Ld_rad) +
                   kCoeffsDeltaPsi_rad[2] * sin(2.0 * longitude_ascending_node_moon_rad) +
                   kCoeffsDeltaPsi_rad[3] * sin(2.0 * L_rad) +
                   kCoeffsDeltaPsi_rad[4] * sin(anomaly_sun_rad) +
                   kCoeffsDeltaPsi_rad[5] * sin(anomaly_moon_rad) +
                   kCoeffsDeltaPsi_rad[6] * sin(2.0 * Ld_rad + anomaly_sun_rad) +
                   kCoeffsDeltaPsi_rad[7] * sin(2.0 * L_rad + anomaly_moon_rad) +
                   kCoeffsDeltaPsi_rad[8] * sin(2.0 * Ld_rad - anomaly_sun_rad);
  double epsilon_mod_rad = *epsilon_rad + *delta_epsilon_rad;

  double rotation_epsilon_x[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  double rotation_delta_psi_z[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  double rotation_mod_epsilon_x[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_make_rotation_double(rotation_epsilon_x, *epsilon_rad, MATRIX33_ROTATION_AXIS_X);
  MATRIX33_make_rotation_double(rotation_delta_psi_z, - *delta_psi_rad, MATRIX33_ROTATION_AXIS_Z);
  MATRIX33_make_rotation_double(rotation_mod_epsilon_x, - epsilon_mod_rad, MATRIX33_ROTATION_AXIS_X);

  MATRIX33_multiply_matrix_matrix_double(nutation, rotation_mod_epsilon_x, rotation_delta_psi_z);
  MATRIX33_multiply_matrix_matrix_double(nutation, nutation, rotation_epsilon_x);

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR TIME_SPACE_trans_eci_to_ecef(const double julian_day, const double gmst_rad,
                                                  double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  if ((julian_day <= 0.0) || (gmst_rad <= 0.0))
  {
    MATRIX33_make_unit_double(dcm_eci_to_ecef);

    return C2A_MATH_ERROR_RANGE_OVER;
  }

  //!< conversion from sec to day
  const double kSecToDay = 1.0 / (double)(PHYSICAL_CONST_EARTH_SOLAR_DAY_s);
  //!< the gap between UT1 and UTC (UT1-UTC) [s]
  const double kDtUT1UTC_s = 32.184;
  //!< julian day for terrestrial time [day]
  double julian_day_terrestrial_time = julian_day + kDtUT1UTC_s * kSecToDay;
  //!< julian century for terrestrial time [century]
  double julian_century_terrestrial_time[4];

  //!< nth power of julian century for terrestrial time
  julian_century_terrestrial_time[0] = TIME_SPACE_convert_julian_day_to_century(julian_day_terrestrial_time);
  for (int i = 0; i < 3; i++)
  {
    julian_century_terrestrial_time[i + 1] = julian_century_terrestrial_time[i] * julian_century_terrestrial_time[0];
  }

  double dcm_precession[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  TIME_SPACE_precession_(julian_century_terrestrial_time, dcm_precession);

  double dcm_nutation[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  double epsilon_rad = 0.0;
  double delta_epsilon_rad = 0.0;
  double delta_psi_rad = 0.0;
  TIME_SPACE_nutation_(julian_century_terrestrial_time, dcm_nutation,
                      &epsilon_rad, &delta_epsilon_rad, &delta_psi_rad);

  //!< Eq : the equation of equinoxes [rad]
  double equation_of_equinoxes_rad = delta_psi_rad * cos(epsilon_rad + delta_epsilon_rad);
  //!< GAST : Greenwich Apparent Sidereal Time [rad]
  double gast_rad = gmst_rad + equation_of_equinoxes_rad;

  double dcm_axial_rotation[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  TIME_SPACE_axial_rotation_(gast_rad, dcm_axial_rotation);

  MATRIX33_multiply_matrix_matrix_double(dcm_eci_to_ecef, dcm_axial_rotation, dcm_nutation);
  MATRIX33_multiply_matrix_matrix_double(dcm_eci_to_ecef, dcm_eci_to_ecef, dcm_precession);

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR TIME_SPACE_trans_eci_to_ecef_rotation_only(const double gmst_rad,
                                                                double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  if (gmst_rad <= 0.0)
  {
    MATRIX33_make_unit_double(dcm_eci_to_ecef);

    return C2A_MATH_ERROR_RANGE_OVER;
  }

  TIME_SPACE_axial_rotation_(gmst_rad, dcm_eci_to_ecef);

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR TIME_SPACE_convert_posvel_ecef_to_eci(double pos_eci_m[PHYSICAL_CONST_THREE_DIM],
                                                     double vel_eci_m_s[PHYSICAL_CONST_THREE_DIM],
                                               const double dcm_ecef_to_eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                               const double pos_ecef_m[PHYSICAL_CONST_THREE_DIM],
                                               const double vel_ecef_m_s[PHYSICAL_CONST_THREE_DIM])
{
  MATRIX33_multiply_matrix_vector_double(pos_eci_m, dcm_ecef_to_eci, pos_ecef_m);

  // Ci2ecef*v_i = v_ecef+Wxr_ecef (※)に従ってECEFからECIへ変換
  //!< (※)のW (地球自転レートベクトル)
  double kEarthRotVector_rad_s[PHYSICAL_CONST_THREE_DIM] = { 0.0, 0.0, (double)(PHYSICAL_CONST_EARTH_ROTATION_SPEED_rad_s) };

  //!< (※)のWxr_ecef (ECEF系で見た速度のうち，地球自転分の見かけの速度)
  double earth_rot_velocity_m_s[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_outer_product_double(earth_rot_velocity_m_s, kEarthRotVector_rad_s, pos_ecef_m);

  //!< (※)の右辺
  double vel_wrt_inertial_ecef_m[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_add_double(vel_wrt_inertial_ecef_m, vel_ecef_m_s, earth_rot_velocity_m_s);

  MATRIX33_multiply_matrix_vector_double(vel_eci_m_s, dcm_ecef_to_eci, vel_wrt_inertial_ecef_m);

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR TIME_SPACE_convert_posvel_eci_to_ecef(double pos_ecef_m[PHYSICAL_CONST_THREE_DIM],
                                                     double vel_ecef_m_s[PHYSICAL_CONST_THREE_DIM],
                                               const double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                               const double pos_eci_m[PHYSICAL_CONST_THREE_DIM],
                                               const double vel_eci_m_s[PHYSICAL_CONST_THREE_DIM])
{
  MATRIX33_multiply_matrix_vector_double(pos_ecef_m, dcm_eci_to_ecef, pos_eci_m);

  //  v_ecef = Ci2ecef*v_i - Wxr_ecef (※)に従ってECEFからECIへ変換
  //!< (※)のW (地球自転レートベクトル)
  double kEarthRotVector_rad_s[PHYSICAL_CONST_THREE_DIM] = { 0.0, 0.0, (double)(PHYSICAL_CONST_EARTH_ROTATION_SPEED_rad_s) };

  //!< (※)のWxr_ecef (ECEF系で見た速度のうち，地球自転分の見かけの速度)
  double earth_rot_velocity_m_s[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_outer_product_double(earth_rot_velocity_m_s, kEarthRotVector_rad_s, pos_ecef_m);

  //!< (※)の右辺第1項
  double vel_wrt_inertial_ecef_m[PHYSICAL_CONST_THREE_DIM];
  MATRIX33_multiply_matrix_vector_double(vel_wrt_inertial_ecef_m, dcm_eci_to_ecef, vel_eci_m_s);

  //!< (※)の右辺全体
  VECTOR3_subtract_double(vel_ecef_m_s, vel_wrt_inertial_ecef_m, earth_rot_velocity_m_s);

  return C2A_MATH_ERROR_OK;
}

#pragma section
