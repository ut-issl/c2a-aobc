#pragma section REPRO
/**
* @file   physical_constants.c
* @brief  物理で使われる定数群
*/

#include "physical_constants.h"
#include "math_constants.h"

// 単位変換
float PHYSICAL_CONST_degree_to_radian(float degree)
{
  return degree * MATH_CONST_PI / 180.0f;
}

float PHYSICAL_CONST_radian_to_degree(float radian)
{
  return radian / PHYSICAL_CONST_degree_to_radian(1.0f);
}

float PHYSICAL_CONST_hour_angle_to_rad(float hour_angle)
{
  return hour_angle * MATH_CONST_2PI / PHYSICAL_CONST_EARTH_SOLAR_DAY_s;
}

double PHYSICAL_CONST_hour_angle_to_rad_double(double hour_angle)
{
  return hour_angle * MATH_CONST_2PI_DOUBLE / (double)PHYSICAL_CONST_EARTH_SOLAR_DAY_s;
}

float PHYSICAL_CONST_rpm_to_rad_sec(float rpm)
{
  return rpm * MATH_CONST_PI / 30.0f;
}

float PHYSICAL_CONST_rad_sec_to_rpm(float rad_sec)
{
  return rad_sec / PHYSICAL_CONST_rpm_to_rad_sec(1.0f);
}

float PHYSICAL_CONST_rpd_to_rad_min(float rpd)
{
  return rpd * MATH_CONST_2PI / (24.0f * 60.0f);
}

float PHYSICAL_CONST_kelvin_to_degC(float kelvin)
{
  return kelvin + PHYSICAL_CONST_ABSOLUTE_ZERO_degC;
}

float PHYSICAL_CONST_degC_to_kelvin(float degC)
{
  return degC - PHYSICAL_CONST_ABSOLUTE_ZERO_degC;
}

float PHYSICAL_CONST_G_to_m_s2(float acc_G)
{
  return acc_G * PHYSICAL_CONST_STANDARD_GRAVITY;
}

float PHYSICAL_CONST_m_s2_to_G(float acc_m_s2)
{
  return acc_m_s2 / PHYSICAL_CONST_STANDARD_GRAVITY;
}

#pragma section
