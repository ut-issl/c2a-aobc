/**
 * @file
 * @brief  数学で使われる定数群
 * @note   基本はfloatで定義する．doubleが必要な場合は新たに定義してよいが，_DOUBLEを付ける
 */
#ifndef MATH_CONSTANTS_H_
#define MATH_CONSTANTS_H_

#define MATH_CONST_PI (3.141592f)                          //!< float型の円周率
#define MATH_CONST_PI_DOUBLE (3.141592653589793)           //!< double型の円周率
#define MATH_CONST_2PI (2.0f * MATH_CONST_PI)              //!< [rad]単位で表したfloat型の1周の角度,2pi
#define MATH_CONST_2PI_DOUBLE (2.0 * MATH_CONST_PI_DOUBLE) //!< [rad]単位で表したdouble型の1周の角度,2pi
#define MATH_CONST_PROHIBIT_DIVISION_VALUE (1.0e-9f)       //!< これより小さい値での割り算を許容しない
#define MATH_CONST_NORMAL_CHECK_THRESHOLD  (1.0e-3f)       //!< 正規化ベクトルの判定用
#define MATH_CONST_MACHINE_EPSILON (1.0e-9f)               //!< 桁落ち等の計算機演算誤差として無視して扱う数値

#endif
