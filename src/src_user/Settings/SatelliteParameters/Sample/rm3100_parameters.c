/**
 * @file  rm3100_parameters.c
 * @brief RM3100に関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/rm3100_parameters.h>

// AOBC RM3100
// Magnetometer bias
// The following parameter should be tuned with magnetic experiment
const float RM3100_PARAMETERS_mag_aobc_bias_compo_nT[PHYSICAL_CONST_THREE_DIM] = { 32808.59f, -79748.68f, 22059.96f };

// Magnetometer filter
// 1st order Low Pass Filter
const float RM3100_PARAMETERS_mag_aobc_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM] = { 0.5f, 0.5f, 0.5f };
// Spike filter
const uint8_t RM3100_PARAMETERS_mag_aobc_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = { 3, 3, 3 };
const uint8_t RM3100_PARAMETERS_mag_aobc_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = { 60, 60, 60 };
const float RM3100_PARAMETERS_mag_aobc_spike_reject_threshold_nT[PHYSICAL_CONST_THREE_DIM] = { 5000.0f, 5000.0f, 5000.0f };
const float RM3100_PARAMETERS_mag_aobc_spike_amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM] = { 1500.0f, 1500.0f, 1500.0f };


// External RM3100
// Frame conversion
const Quaternion RM3100_PARAMETERS_mag_ext_quaternion_c2b = {-0.707106471, 0.707107127, 0.0f, 0.0f};

// Magnetometer bias
// The following parameter should be tuned with magnetic experiment
const float RM3100_PARAMETERS_mag_ext_bias_compo_nT[PHYSICAL_CONST_THREE_DIM] = { 36824.97f, -4596.48f,  -1133.40f };

// Magnetometer filter
// 1st order Low Pass Filter
const float RM3100_PARAMETERS_mag_ext_cut_off_freq_lpf_1st_Hz[PHYSICAL_CONST_THREE_DIM] = { 0.5f, 0.5f, 0.5f };
// Spike filter
const uint8_t RM3100_PARAMETERS_mag_ext_spike_count_limit_to_accept[PHYSICAL_CONST_THREE_DIM] = { 3, 3, 3 };
const uint8_t RM3100_PARAMETERS_mag_ext_spike_count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM] = { 60, 60, 60 };
const float RM3100_PARAMETERS_mag_ext_spike_reject_threshold_nT[PHYSICAL_CONST_THREE_DIM] = { 5000.0f, 5000.0f, 5000.0f };
const float RM3100_PARAMETERS_mag_ext_spike_amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM] = { 1500.0f, 1500.0f, 1500.0f };
