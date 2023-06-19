/**
 * @file  nanossoc_d60_parameters.c
 * @brief nanossoc-D60に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/nanossoc_d60_parameters.h>

// Frame conversion
const Quaternion NANOSSOC_D60_PARAMETERS_py_quaternion_c2b = { 0.70710665f, 0.0f, 0.0f, 0.70710665f };
const Quaternion NANOSSOC_D60_PARAMETERS_my_quaternion_c2b = { 0.0f, -0.70710665f, 0.70710665f, 0.0f };
const Quaternion NANOSSOC_D60_PARAMETERS_pz_quaternion_c2b = { 0.0f, 0.0f, 0.70710665f, 0.70710665f };
const Quaternion NANOSSOC_D60_PARAMETERS_mz_quaternion_c2b = { -0.707106471f, 0.707106471f, 0.0f, 0.0f };
