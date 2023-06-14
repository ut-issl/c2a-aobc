/**
 * @file  structure_parameters.h
 * @brief 衛星構造に関する衛星固有パラメータを管理する
 */

#ifndef STRUCTURE_PARAMETERS_H_
#define STRUCTURE_PARAMETERS_H_

#include <src_user/Library/physical_constants.h>

extern const float STRUCTURE_PARAMETERS_mass_sc_kg; //!< Spacecraft mass [kg]

extern const float STRUCTURE_PARAMETERS_rmm_sc_body_Am2[PHYSICAL_CONST_THREE_DIM]; //!< Spacecraft Residual Magnetic Moment at body-fixed frame [Am2]

extern const float STRUCTURE_PARAMETERS_inertia_tensor_sc_body_kgm2[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< Spacecraft inertia tensor @ body-fixed frame [kg m2]

#endif // STRUCTURE_PARAMETERS_H_
