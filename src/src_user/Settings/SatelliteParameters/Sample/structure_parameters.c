/**
 * @file  structure_parameters.c
 * @brief 衛星構造に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/structure_parameters.h>

const float STRUCTURE_PARAMETERS_mass_sc_kg = 10.0;

const float STRUCTURE_PARAMETERS_rmm_sc_body_Am2[PHYSICAL_CONST_THREE_DIM] = {0.3f, 0.2f, 0.1f};

const float STRUCTURE_PARAMETERS_inertia_tensor_sc_body_kgm2[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM] = {{0.1f, 0.0f, 0.0f},
                                                                                                                    {0.0f, 0.2f, 0.0f},
                                                                                                                    {0.0f, 0.0f, 0.3f}};
