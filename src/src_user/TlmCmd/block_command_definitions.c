#pragma section REPRO
#include "block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/TlmCmd/block_command_table.h>
#include <string.h> // for memcpy
#include "command_definitions.h"

#include <src_user/Settings/Modes/Transitions/sl_nop.h>
#include <src_user/Settings/Modes/Transitions/sl_initial.h>
#include <src_user/Settings/Modes/Transitions/sl_bdot.h>
#include <src_user/Settings/Modes/Transitions/sl_rough_sun_pointing.h>
#include <src_user/Settings/Modes/Transitions/sl_rough_three_axis.h>
#include <src_user/Settings/Modes/Transitions/sl_rough_three_axis_rw.h>
#include <src_user/Settings/Modes/Transitions/sl_fine_three_axis.h>

#include <src_user/Settings/Modes/TaskLists/tl_initial.h>
#include <src_user/Settings/Modes/TaskLists/tl_bdot.h>
#include <src_user/Settings/Modes/TaskLists/tl_rough_sun_pointing.h>
#include <src_user/Settings/Modes/TaskLists/tl_rough_three_axis.h>
#include <src_user/Settings/Modes/TaskLists/tl_rough_three_axis_rw.h>
#include <src_user/Settings/Modes/TaskLists/tl_fine_three_axis.h>

#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_cdh_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_drivers_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_inertial_ref_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_sun_vector_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_mtq_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_basic_sensor_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_rw_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_rm3100_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_stim210_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_oem7600_update.h>
#include <src_user/Settings/Modes/TaskLists/Elements/tl_elem_stt_update.h>

#include "./NormalBlockCommandDefinition/nbc_header.h"


/**
 * @brief
 * 2018/06/21 コメント追記
 * 引数にとったブロック番号のblock_command_tableをClearし，にコマンドを追加，activateに．
 */
void BC_load_defaults(void)
{
  // Block Cmds for Mode Transition
  BCL_load_bc(BC_SL_START_UP_TO_INITIAL,                     BCL_load_start_up_to_initial);
  BCL_load_bc(BC_SL_INITIAL_TO_BDOT,                         BCL_load_initial_to_bdot);
  BCL_load_bc(BC_SL_NOP,                                     BCL_load_nop_sl);
  BCL_load_bc(BC_SL_BDOT_TO_ROUGH_SUN_POINTING,              BCL_load_bdot_to_rough_sun_pointing);
  BCL_load_bc(BC_SL_ROUGH_SUN_POINTING_TO_ROUGH_THREE_AXIS,  BCL_load_rough_sun_pointing_to_rough_three_axis);
  BCL_load_bc(BC_SL_ROUGH_THREE_AXIS_TO_ROUGH_THREE_AXIS_RW, BCL_load_rough_three_axis_to_rough_three_axis_rw);
  BCL_load_bc(BC_SL_ROUGH_THREE_AXIS_RW_TO_FINE_THREE_AXIS,  BCL_load_rough_three_axis_rw_to_fine_three_axis);
  BCL_load_bc(BC_SL_ANY_TO_BDOT,                             BCL_load_any_to_bdot);
  BCL_load_bc(BC_SL_BDOT_TO_INITIAL,                         BCL_load_bdot_to_initial);

  // Block Cmds for TaskList
  // こいつは TDSP_initialize() で展開される
  BCL_load_bc(BC_TL_START_UP,             BCL_load_initial_mode);
  BCL_load_bc(BC_TL_INITIAL,              BCL_load_initial_mode);
  BCL_load_bc(BC_TL_BDOT,                 BCL_load_bdot_mode);
  BCL_load_bc(BC_TL_ROUGH_SUN_POINTING,   BCL_load_rough_sun_pointing_mode);
  BCL_load_bc(BC_TL_ROUGH_THREE_AXIS,     BCL_load_rough_three_axis_mode);
  BCL_load_bc(BC_TL_ROUGH_THREE_AXIS_RW,  BCL_load_rough_three_axis_rw_mode);
  BCL_load_bc(BC_TL_FINE_THREE_AXIS,      BCL_load_fine_three_axis_mode);

  // Block Cmds for Application Rotation / Combination
  // === CDH ===
  BCL_load_bc(BC_AC_CDH_UPDATE, BCL_load_cdh_update);

  // === AOCS ===
  BCL_load_bc(BC_AC_INERTIAL_REF_UPDATE, BCL_load_inertial_ref_update);
  BCL_load_bc(BC_AC_SUN_VECTOR_UPDATE,   BCL_load_sun_vector_update);
  BCL_load_bc(BC_AC_MTQ_UPDATE,          BCL_load_mtq_update);
  BCL_load_bc(BC_AC_BASIC_SENSOR_UPDATE, BCL_load_basic_sensor_update);
  BCL_load_bc(BC_AC_RW_UPDATE,           BCL_load_rw_update);
  BCL_load_bc(BC_AC_RM3100_UPDATE,       BCL_load_rm3100_update);
  BCL_load_bc(BC_AC_STIM210_UPDATE,      BCL_load_stim210_update);
  BCL_load_bc(BC_AC_OEM7600_UPDATE,      BCL_load_oem7600_update);
  BCL_load_bc(BC_AC_STT_UPDATE,          BCL_load_stt_update);

  // Normal Block Cmds
  // === AOCS ===
  BCL_load_bc(BC_HK_CYCLIC_TLM,          BCL_load_start_hk_tlm);
  BCL_load_bc(BC_SELECT_RM3100_ON_AOBC,  BCL_load_select_rm3100_on_aobc);
  BCL_load_bc(BC_SELECT_RM3100_EXTERNAL, BCL_load_select_rm3100_external);
  BCL_load_bc(BC_SET_SAGITTA_PARAMETER,  BCL_set_sagitta_parameter);
  BCL_load_bc(BC_READ_SAGITTA_PARAMETER, BCL_read_sagitta_parameter);

  // === POWER ===
  BCL_load_bc(BC_POWER_ON_MPU9250,       BCL_load_power_on_mpu9250);
  BCL_load_bc(BC_POWER_ON_RM3100,        BCL_load_power_on_rm3100);
  BCL_load_bc(BC_POWER_ON_NANOSSOC_D60,  BCL_load_power_on_nanossoc_d60);
  BCL_load_bc(BC_POWER_ON_STIM210,       BCL_load_power_on_stim210);
  BCL_load_bc(BC_POWER_ON_SAGITTA,       BCL_load_power_on_sagitta);
  BCL_load_bc(BC_POWER_ON_OEM7600,       BCL_load_power_on_oem7600);
  BCL_load_bc(BC_POWER_ON_MTQ,           BCL_load_power_on_mtq);
  BCL_load_bc(BC_POWER_ON_RWX,           BCL_load_power_on_rwx);
  BCL_load_bc(BC_POWER_ON_RWY,           BCL_load_power_on_rwy);
  BCL_load_bc(BC_POWER_ON_RWZ,           BCL_load_power_on_rwz);
  BCL_load_bc(BC_POWER_ON_INA260,        BCL_load_power_on_ina260);
  BCL_load_bc(BC_SET_INA260_OC_LIMIT,    BCL_load_set_ina260_oc_limit);
  BCL_load_bc(BC_POWER_OFF_MPU9250,      BCL_load_power_off_mpu9250);
  BCL_load_bc(BC_POWER_OFF_RM3100,       BCL_load_power_off_rm3100);
  BCL_load_bc(BC_POWER_OFF_NANOSSOC_D60, BCL_load_power_off_nanossoc_d60);
  BCL_load_bc(BC_POWER_OFF_STIM210,      BCL_load_power_off_stim210);
  BCL_load_bc(BC_POWER_OFF_SAGITTA,      BCL_load_power_off_sagitta);
  BCL_load_bc(BC_POWER_OFF_OEM7600,      BCL_load_power_off_oem7600);
  BCL_load_bc(BC_POWER_OFF_MTQ,          BCL_load_power_off_mtq);
  BCL_load_bc(BC_POWER_OFF_RWX,          BCL_load_power_off_rwx);
  BCL_load_bc(BC_POWER_OFF_RWY,          BCL_load_power_off_rwy);
  BCL_load_bc(BC_POWER_OFF_RWZ,          BCL_load_power_off_rwz);
  BCL_load_bc(BC_POWER_OFF_INA260,       BCL_load_power_off_ina260);
  BCL_load_bc(BC_RESET_MPU9250,          BCL_load_reset_mpu9250);
  BCL_load_bc(BC_RESET_RM3100,           BCL_load_reset_rm3100);
  BCL_load_bc(BC_RESET_NANOSSOC_D60,     BCL_load_reset_nanossoc_d60);
  BCL_load_bc(BC_RESET_STIM210,          BCL_load_reset_stim210);
  BCL_load_bc(BC_RESET_SAGITTA,          BCL_load_reset_sagitta);
  BCL_load_bc(BC_RESET_OEM7600,          BCL_load_reset_oem7600);
  BCL_load_bc(BC_RESET_MTQ,              BCL_load_reset_mtq);
  BCL_load_bc(BC_RESET_RWX,              BCL_load_reset_rwx);
  BCL_load_bc(BC_RESET_RWY,              BCL_load_reset_rwy);
  BCL_load_bc(BC_RESET_RWZ,              BCL_load_reset_rwz);
  BCL_load_bc(BC_RESET_INA260,           BCL_load_reset_ina260);

  // === Anomaly ===
  BCL_load_bc(BC_ACTIVATE_INA260_EH,         BCL_load_activate_ina260_eh);
  BCL_load_bc(BC_ACTIVATE_NANOSSOC_D60_EH,   BCL_load_activate_nanossoc_d60_eh);
  BCL_load_bc(BC_INACTIVATE_NANOSSOC_D60_EH, BCL_load_inactivate_nanossoc_d60_eh);
  BCL_load_bc(BC_ACTIVATE_STIM210_EL_EH,     BCL_load_activate_stim210_el_eh);
  BCL_load_bc(BC_INACTIVATE_STIM210_EL_EH,   BCL_load_inactivate_stim210_el_eh);
  BCL_load_bc(BC_ACTIVATE_SAGITTA_EL_EH,     BCL_load_activate_sagitta_el_eh);
  BCL_load_bc(BC_INACTIVATE_SAGITTA_EL_EH,   BCL_load_inactivate_sagitta_el_eh);
  BCL_load_bc(BC_ACTIVATE_OEM7600_EL_EH,     BCL_load_activate_oem7600_el_eh);
  BCL_load_bc(BC_INACTIVATE_OEM7600_EL_EH,   BCL_load_inactivate_oem7600_el_eh);

}

#pragma section
