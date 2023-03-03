cmake_minimum_required(VERSION 3.13)

project(C2A_USER_CMD_TLM)

set(C2A_SRCS
  block_command_definitions.c
  block_command_user_settings.c
  command_definitions.c
  telemetry_definitions.c
  user_packet_handler.c
  common_cmd_packet.c
  common_tlm_cmd_packet.c
  common_tlm_packet.c
  NormalBlockCommandDefinition/nbc_start_hk_tlm.c
  NormalBlockCommandDefinition/nbc_mpu9250.c
  NormalBlockCommandDefinition/nbc_rm3100.c
  NormalBlockCommandDefinition/nbc_nanossoc_d60.c
  NormalBlockCommandDefinition/nbc_stim210.c
  NormalBlockCommandDefinition/nbc_sagitta.c
  NormalBlockCommandDefinition/nbc_oem7600.c
  NormalBlockCommandDefinition/nbc_mtq.c
  NormalBlockCommandDefinition/nbc_rw0003.c
  NormalBlockCommandDefinition/nbc_ina260.c
)

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
