#pragma section REPRO
/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#include "block_command_definitions.h"
#include <src_core/tlm_cmd/block_command_loader.h>
#include <src_core/tlm_cmd/block_command_table.h>
#include <src_core/system/watchdog_timer/watchdog_timer.h>
#include <string.h>
#include "command_definitions.h"
#include "./normal_block_command_definition/nbc_header.h"

/**
 * @brief
 * 各ブロックコマンドIDに中身の初期値をロードしていく
 */
void BC_load_defaults(void)
{
  BCL_load_bc(BC_AR_DRIVERS_UPDATE_INI, BCL_load_ar_drivers_update_ini);
  BCL_load_bc(BC_AR_DEBUG_DISPLAY_INI, BCL_load_ar_debug_display_ini);
  BCL_load_bc(BC_AR_GS_RELATED_PROCESS, BCL_load_ar_gs_related_process);
  BCL_load_bc(BC_AC_TLM_CMD_HIRATE, BCL_load_ac_tlm_cmd_hirate);
  BCL_load_bc(BC_HK_CYCLIC_TLM, BCL_load_hk_cyclic_tlm);
  BCL_load_bc(BC_TL_START_UP, BCL_load_tl_start_up);    // STARTUPモード
  WDT_clear_wdt();
  BCL_load_bc(BC_TL_INITIAL, BCL_load_tl_initial);    // INITIALモード
  BCL_load_bc(BC_SL_START_UP_TO_INITIAL, BCL_load_sl_start_up_to_initial);
  BCL_load_bc(BC_SL_NOP, BCL_load_sl_nop);
}

#pragma section
