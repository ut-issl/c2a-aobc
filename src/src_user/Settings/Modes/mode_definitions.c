#pragma section REPRO
/**
 * @file
 * @brief モード定義とモード遷移定義
 */
#include "mode_definitions.h"

#include <src_core/System/ModeManager/mode_manager.h>

#include <src_user/TlmCmd/block_command_definitions.h>

// モードごとのTLを指定
void MD_load_mode_list(void)
{
  MM_set_mode_list(MD_MODEID_START_UP,            BC_TL_START_UP);
  MM_set_mode_list(MD_MODEID_INITIAL,             BC_TL_INITIAL);
  MM_set_mode_list(MD_MODEID_BDOT,                BC_TL_BDOT);
  MM_set_mode_list(MD_MODEID_ROUGH_SUN_POINTING,  BC_TL_ROUGH_SUN_POINTING);
  MM_set_mode_list(MD_MODEID_ROUGH_THREE_AXIS,    BC_TL_ROUGH_THREE_AXIS);
  MM_set_mode_list(MD_MODEID_ROUGH_THREE_AXIS_RW, BC_TL_ROUGH_THREE_AXIS_RW);
  MM_set_mode_list(MD_MODEID_FINE_THREE_AXIS,     BC_TL_FINE_THREE_AXIS);
}

void MD_load_transition_table(void)
{
  // 起動時のみ実行
  MM_set_transition_table(MD_MODEID_START_UP, MD_MODEID_INITIAL, BC_SL_START_UP_TO_INITIAL);

  // from INITIAL
  MM_set_transition_table(MD_MODEID_INITIAL, MD_MODEID_BDOT, BC_SL_INITIAL_TO_BDOT);

  // from Bdot
  MM_set_transition_table(MD_MODEID_BDOT, MD_MODEID_INITIAL, BC_SL_BDOT_TO_INITIAL);
  MM_set_transition_table(MD_MODEID_BDOT, MD_MODEID_BDOT, BC_SL_NOP);
  MM_set_transition_table(MD_MODEID_BDOT, MD_MODEID_ROUGH_SUN_POINTING, BC_SL_BDOT_TO_ROUGH_SUN_POINTING);

  // from Rough Sun Pointing
  MM_set_transition_table(MD_MODEID_ROUGH_SUN_POINTING, MD_MODEID_BDOT, BC_SL_ANY_TO_BDOT);
  MM_set_transition_table(MD_MODEID_ROUGH_SUN_POINTING, MD_MODEID_ROUGH_SUN_POINTING, BC_SL_NOP);
  MM_set_transition_table(MD_MODEID_ROUGH_SUN_POINTING, MD_MODEID_ROUGH_THREE_AXIS, BC_SL_ROUGH_SUN_POINTING_TO_ROUGH_THREE_AXIS);

  // from Rough Three Axis
  MM_set_transition_table(MD_MODEID_ROUGH_THREE_AXIS, MD_MODEID_BDOT, BC_SL_ANY_TO_BDOT);
  MM_set_transition_table(MD_MODEID_ROUGH_THREE_AXIS, MD_MODEID_ROUGH_THREE_AXIS, BC_SL_NOP);
  MM_set_transition_table(MD_MODEID_ROUGH_THREE_AXIS, MD_MODEID_ROUGH_THREE_AXIS_RW, BC_SL_ROUGH_THREE_AXIS_TO_ROUGH_THREE_AXIS_RW);

  // from Rough Three Axis RW
  MM_set_transition_table(MD_MODEID_ROUGH_THREE_AXIS_RW, MD_MODEID_BDOT, BC_SL_ANY_TO_BDOT);
  MM_set_transition_table(MD_MODEID_ROUGH_THREE_AXIS_RW, MD_MODEID_ROUGH_THREE_AXIS_RW, BC_SL_NOP);
  MM_set_transition_table(MD_MODEID_ROUGH_THREE_AXIS_RW, MD_MODEID_FINE_THREE_AXIS, BC_SL_ROUGH_THREE_AXIS_RW_TO_FINE_THREE_AXIS);

  // from Fine Three Axis
  MM_set_transition_table(MD_MODEID_FINE_THREE_AXIS, MD_MODEID_BDOT, BC_SL_ANY_TO_BDOT);
  MM_set_transition_table(MD_MODEID_FINE_THREE_AXIS, MD_MODEID_FINE_THREE_AXIS, BC_SL_NOP);
}

#pragma section
