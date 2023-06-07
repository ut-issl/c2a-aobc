cmake_minimum_required(VERSION 3.13)

project(C2A_USER_SETTINGS)

set(C2A_SRCS
  # Settings
  Applications/NvmParams/non_volatile_memory_normal_parameters.c
  Applications/NvmParams/non_volatile_memory_redundant_parameters.c
  Modes/mode_definitions.c
  # TaskList
  Modes/TaskLists/tl_bdot.c
  Modes/TaskLists/tl_initial.c
  Modes/TaskLists/tl_rough_sun_pointing.c
  Modes/TaskLists/tl_rough_three_axis.c
  Modes/TaskLists/tl_rough_three_axis_rw.c
  Modes/TaskLists/tl_fine_three_axis.c
  Modes/TaskLists/tl_gpsr_range_observe.c
  Modes/TaskLists/Elements/tl_elem_debug_display.c
  Modes/TaskLists/Elements/tl_elem_drivers_update.c
  Modes/TaskLists/Elements/tl_elem_gs_related_process.c
  Modes/TaskLists/Elements/tl_elem_cdh_update.c
  Modes/TaskLists/Elements/tl_elem_inertial_ref_update.c
  Modes/TaskLists/Elements/tl_elem_sun_vector_update.c
  Modes/TaskLists/Elements/tl_elem_mtq_update.c
  Modes/TaskLists/Elements/tl_elem_basic_sensor_update.c
  Modes/TaskLists/Elements/tl_elem_rw_update.c
  Modes/TaskLists/Elements/tl_elem_rm3100_update.c
  Modes/TaskLists/Elements/tl_elem_stim210_update.c
  Modes/TaskLists/Elements/tl_elem_oem7600_update.c
  Modes/TaskLists/Elements/tl_elem_stt_update.c
  # System
  System/event_logger_settings.c
  System/EventHandlerRules/event_handler_rules.c
  System/EventHandlerRules/event_handler_rule_tlmcmd.c
  System/EventHandlerRules/event_handler_rule_hw_oc.c
  System/EventHandlerRules/event_handler_rule_sw_oc.c
  # Transition
  Modes/Transitions/sl_nop.c
  Modes/Transitions/sl_initial.c
  Modes/Transitions/sl_bdot.c
  Modes/Transitions/sl_rough_sun_pointing.c
  Modes/Transitions/sl_rough_three_axis.c
  Modes/Transitions/sl_rough_three_axis_rw.c
  Modes/Transitions/sl_fine_three_axis.c
  # TlmCmd
  TlmCmd/common_cmd_packet_define.c
  TlmCmd/Ccsds/apid_define.c
  # Satellite Parameters
  ${C2A_SATELLITE_PARAMETERS_DIR}/orbit_parameters.c
  ${C2A_SATELLITE_PARAMETERS_DIR}/structure_parameters.c
)

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
