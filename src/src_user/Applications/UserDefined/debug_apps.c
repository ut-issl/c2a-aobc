#pragma section REPRO
#include "debug_apps.h"

#include <stddef.h> // for NULL

#include <src_core/Library/print.h>
#include "../../Library/vt100.h"
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_core/System/TaskManager/task_dispatcher.h>
#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include <src_core/TlmCmd/packet_handler.h>
#include "../../TlmCmd/telemetry_definitions.h"
#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/Applications/memory_dump.h>
#include <src_core/Applications/gs_command_dispatcher.h>
#include <src_core/Applications/realtime_command_dispatcher.h>

void flush_screen_(void);
void print_time_stamp_(void);
// void print_anomaly_status_(void);
void print_cmd_status_(void);

AppInfo flush_screen(void)
{
  return AI_create_app_info("fscr", NULL, flush_screen_);
}

AppInfo print_time_stamp(void)
{
  return AI_create_app_info("tstm", NULL, print_time_stamp_);
}

// AppInfo print_anomaly_status(void)
// {
//   return AI_create_app_info("anmly", NULL, print_anomaly_status_);
// }

AppInfo print_cmd_status(void)
{
  return AI_create_app_info("cmds", NULL, print_cmd_status_);
}

void flush_screen_(void)
{
  VT100_erase_down();
  VT100_reset_cursor();
  VT100_erase_line();
  Printf("-- EQUULEUS Flight S/W (H-ON, F-ON) --\n");
  VT100_erase_line();
  // Printf("BUILD: %s %s (Rev. %d)\n", __DATE__, __TIME__, SVN_REVISION);
  Printf("BUILD: %s %s\n", __DATE__, __TIME__);
}

void print_time_stamp_(void)
{
  VT100_erase_line();
  Printf("CYCLE: TOTAL %04d, MODE %04d\n",
         TMGR_get_master_total_cycle(), TMGR_get_master_mode_cycle());
  VT100_erase_line();
  Printf("MODE: STAT %d, PREV %d, CURR %d\n",
         mode_manager->stat, mode_manager->previous_id, mode_manager->current_id);
}

// void print_anomaly_status_(void)
// {
//   const AL_AnomalyRecord* ar = AL_get_latest_record();
//   VT100_erase_line();
//   Printf("ANOMALY: CTR %d, HEAD %d, TIME: <%d, %d, %d>, CODE: <%d, %d>, RL %d\n",
//          anomaly_logger->counter, anomaly_logger->header,
//          ar->time.total_cycle, ar->time.mode_cycle, ar->time.step,
//          ar->code.group, ar->code.local,
//          ar->run_length);
// }

void print_cmd_status_(void)
{
  VT100_erase_line();
  // Printf("CMD: GS %3d, RT %3d, Ack %3d, Code 0x%02x, Sts %3d\n",
  //       (PL_count_executed_nodes(&PH_gs_cmd_list) & 0xff),
  //       (PL_count_executed_nodes(&PH_rt_cmd_list) & 0xff),
  //       gph->cmd_ack, gs_command_dispatcher->prev.code, gs_command_dispatcher->prev.sts);
  // Printf("BUF: %d\n", cmi->ms_buffer_in_use);
}
#pragma section
