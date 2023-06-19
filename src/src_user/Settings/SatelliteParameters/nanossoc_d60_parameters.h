/**
 * @file  nanossoc_d60_parameters.h
 * @brief nanossoc-D60に関する衛星固有パラメータを管理する
 */

#ifndef NANOSSOC_D60_PARAMETERS_H_
#define NANOSSOC_D60_PARAMETERS_H_

#include <src_user/Library/quaternion.h>

// Frame conversion
extern const Quaternion NANOSSOC_D60_PARAMETERS_py_quaternion_c2b;  //!< Frame conversion quaternion for PY sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_my_quaternion_c2b;  //!< Frame conversion quaternion for MY sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_pz_quaternion_c2b;  //!< Frame conversion quaternion for PZ sun sensor
extern const Quaternion NANOSSOC_D60_PARAMETERS_mz_quaternion_c2b;  //!< Frame conversion quaternion for MZ sun sensor


#endif // NANOSSOC_D60_PARAMETERS_H_
