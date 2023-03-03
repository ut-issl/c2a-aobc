/**
* @file   physical_constants.h
* @brief  物理で使われる定数群
* @note   基本はfloatで定義する．doubleが必要な場合は新たに定義してよいが，定数には_DOUBLE・関数には_doubleを付ける
*/
#ifndef PHYSICAL_CONSTANTS_H_
#define PHYSICAL_CONSTANTS_H_

// 要素数
#define PHYSICAL_CONST_THREE_DIM      (3)  //!< 三次元ベクトルなどの要素数
#define PHYSICAL_CONST_QUATERNION_DIM (4)  //!< Quaternionの要素数を表す


// 物理定数
#define PHYSICAL_CONST_SPEED_OF_LIGHT_m_s (2.997925e8f)   //!< 光速 [m/s]
#define PHYSICAL_CONST_ABSOLUTE_ZERO_degC (-273.15f)      //!< 絶対零度 [degC]
#define PHYSICAL_CONST_STANDARD_GRAVITY   (9.80665f)      //!< 標準重力加速度 [m/s2]

// 天文関連
#define PHYSICAL_CONST_AU_m                       (1.495979e11f)   //!< 天文単位 [m]
#define PHYSICAL_CONST_SOLAR_CONST_W_m2           (1366.0f)        //!< 太陽定数 [W/m2]
#define PHYSICAL_CONST_DAY_OF_CENTURY_day         (36525.0f)       //!< 1世紀の長さを [day] で表したもの [day]
#define PHYSICAL_CONST_EARTH_SIDEREAL_DAY_s       (86164.0f)       //!< 恒星日(23h56m4s) [sec]
#define PHYSICAL_CONST_EARTH_SOLAR_DAY_s          (86400.0f)       //!< 太陽日(24h) [sec]
#define PHYSICAL_CONST_EARTH_RADIUS_m             (6.378137e6f)    //!< 地球平均半径 [m]
#define PHYSICAL_CONST_EARTH_POLAR_RADIUS_m       (6.35675231e6f)  //!< 地球極半径 [m] (WGS84)
#define PHYSICAL_CONST_EARTH_GRAVITY_CONST_m3_s2  (3.986005e14f)   //!< 地心重力定数 [m3/s2] (WGS84)
#define PHYSICAL_CONST_EARTH_J2                   (1.082630e-3f)   //!< 地球J2項 (WGS84)
#define PHYSICAL_CONST_EARTH_J3                   (-2.53215e-6f)   //!< 地球J3項 (WGS84)
#define PHYSICAL_CONST_EARTH_J4                   (-1.61099e-6f)   //!< 地球J4項 (WGS84)
#define PHYSICAL_CONST_EARTH_GRAVITY_CONST_km3_s2 (PHYSICAL_CONST_EARTH_GRAVITY_CONST_m3_s2 / 1.0e9f) //!< 地心重力定数 [km3/s2]
#define PHYSICAL_CONST_EARTH_RADIUS_km (PHYSICAL_CONST_EARTH_RADIUS_m / 1.0e3f) //!< 地球平均半径 [km]
#define PHYSICAL_CONST_EARTH_ROTATION_SPEED_rad_s (MATH_CONST_2PI / PHYSICAL_CONST_EARTH_SIDEREAL_DAY_s)  //!< 恒星日基準の地球自転速度 [rad/s]
#define PHYSICAL_CONST_EARTH_FIRST_OBLATENESS ((PHYSICAL_CONST_EARTH_RADIUS_m - PHYSICAL_CONST_EARTH_POLAR_RADIUS_m) / PHYSICAL_CONST_EARTH_RADIUS_m) //!< 地球第一扁平率 (WGS84)
#define PHYSICAL_CONST_EARTH_SECOND_OBLATENESS ((PHYSICAL_CONST_EARTH_RADIUS_m - PHYSICAL_CONST_EARTH_POLAR_RADIUS_m) / PHYSICAL_CONST_EARTH_POLAR_RADIUS_m) //!< 地球第二扁平率 (WGS84)
#define PHYSICAL_CONST_EARTH_FIRST_ECCENTRICITY_SQUARED (2 * PHYSICAL_CONST_EARTH_FIRST_OBLATENESS - PHYSICAL_CONST_EARTH_FIRST_OBLATENESS * PHYSICAL_CONST_EARTH_FIRST_OBLATENESS) //!< 地球第一離心率の二乗 (WGS84)
#define PHYSICAL_CONST_EARTH_SECOND_ECCENTRICITY_SQUARED (2 * PHYSICAL_CONST_EARTH_SECOND_OBLATENESS - PHYSICAL_CONST_EARTH_SECOND_OBLATENESS * PHYSICAL_CONST_EARTH_SECOND_OBLATENESS) //!< 地球第二離心率の二乗 (WGS84)

// 単位変換関数
/**
 * @brief  DegreeからRadianへの角度単位変換
 * @param  degree : 角度[deg]
 * @return 角度[rad]
 */
float PHYSICAL_CONST_degree_to_radian(float degree);

/**
 * @brief  RadianからDegreeへの角度単位変換
 * @param  radian : 角度[rad]
 * @return 角度[deg]
 */
float PHYSICAL_CONST_radian_to_degree(float radian);

/**
 * @brief  Hour angleからRadianへのfloat型の角度単位変換
 * @param  hour_angle : 角度[hour angle] (24 [hour angle] が360 [deg] に対応)
 * @return 角度[rad]
 */
float PHYSICAL_CONST_hour_angle_to_rad(float hour_angle);

/**
 * @brief  Hour angleからRadianへのdouble型の角度単位変換
 * @param  hour_angle : 角度[hour angle] (24 [hour angle] が360 [deg] に対応)
 * @return 角度[rad]
 */
double PHYSICAL_CONST_hour_angle_to_rad_double(double hour_angle);

/**
 * @brief  RPMからrad/sへの角速度単位変換
 * @param  rpm : 角速度[rpm]
 * @return 角速度[rad/s]
 */
float PHYSICAL_CONST_rpm_to_rad_sec(float rpm);

/**
 * @brief  rad/sからRPMへの角速度単位変換
 * @param  rad_s : 角速度[rad/s]
 * @return 角速度[rpm]
 */
float PHYSICAL_CONST_rad_sec_to_rpm(float rad_sec);

/**
 * @brief  RPDからrad/minへの角速度単位変換
 * @param  rpd : 角速度[rpd]
 * @return 角速度[rad/min]
 */
float PHYSICAL_CONST_rpd_to_rad_min(float rpd);

/**
 * @brief  KelvinからdegCへの温度単位変換
 * @param  kelvin : 温度[K]
 * @return 温度[degC]
 */
float PHYSICAL_CONST_kelvin_to_degC(float kelvin);

/**
 * @brief  degCからKelvinへの温度単位変換
 * @param  degreeC : 温度[degC]
 * @return 温度[K]
 */
float PHYSICAL_CONST_degC_to_kelvin(float degC);

/**
 * @brief  Gからm/s2への加速度単位変換
 * @param  acc_G : 加速度[G]
 * @return 加速度[m/s2]
 */
float PHYSICAL_CONST_G_to_m_s2(float acc_G);

/**
 * @brief  m/s2からGへの加速度単位変換
 * @param  acc_m_s2 : 加速度[m/s2]
 * @return 加速度[G]
 */
float PHYSICAL_CONST_m_s2_to_G(float acc_m_s2);

#endif
