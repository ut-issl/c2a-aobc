#pragma section REPRO
/**
 * @file
 * @brief  テレメトリ定義
 * @note   このコードは自動生成されています！
 */
#include <src_core/TlmCmd/telemetry_frame.h>
#include "telemetry_definitions.h"
#include "telemetry_source.h"

static TF_TLM_FUNC_ACK Tlm_AOBC_OBC_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_TL_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_BL_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_CA_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_TF_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_DCU_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_MM_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_AM_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_APP_TIME_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_EL_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_EL_TLOG_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_EL_CLOG_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_EH_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_EH_RULE_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_EH_LOG_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_EH_INDEX_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_GIT_REV_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_HK_GEN_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_HK_COMPO_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_HK_ALGO_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_COMPONENTS_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_POWER_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_FRAME_TRANSFORMATION_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_CONTROL_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_BIAS_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_TEMP_CALIB_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_INTERPOLATION_TARGET1_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_INTERPOLATION_TARGET2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_NON_VOLATILE_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_ORBIT_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_AOCS_MANAGER_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SUN_POINTING_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_THREE_AXIS_MTQ_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_THREE_AXIS_RW_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_FILTERS_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_ANOMALY_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_APP_TIME_2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_SET1_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_SET2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_READ1_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_READ2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_STT_GYRO_EKF1_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_STT_GYRO_EKF2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_GPSR_RANGE_P1_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_GPSR_RANGE_P2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_ORBIT2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_FILTERS_2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_DR_ALGORITHM_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_RW_DETAILED_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA1_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA2_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA3_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA4_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA5_(uint8_t* packet, uint16_t* len, uint16_t max_len);

void TF_load_tlm_table(TF_TlmInfo tlm_table[TF_MAX_TLMS])
{
  tlm_table[Tlm_CODE_AOBC_OBC].tlm_func = Tlm_AOBC_OBC_;
  tlm_table[Tlm_CODE_AOBC_TL].tlm_func = Tlm_AOBC_TL_;
  tlm_table[Tlm_CODE_AOBC_BL].tlm_func = Tlm_AOBC_BL_;
  tlm_table[Tlm_CODE_AOBC_CA].tlm_func = Tlm_AOBC_CA_;
  tlm_table[Tlm_CODE_AOBC_TF].tlm_func = Tlm_AOBC_TF_;
  tlm_table[Tlm_CODE_AOBC_DCU].tlm_func = Tlm_AOBC_DCU_;
  tlm_table[Tlm_CODE_AOBC_MM].tlm_func = Tlm_AOBC_MM_;
  tlm_table[Tlm_CODE_AOBC_AM].tlm_func = Tlm_AOBC_AM_;
  tlm_table[Tlm_CODE_AOBC_APP_TIME].tlm_func = Tlm_AOBC_APP_TIME_;
  tlm_table[Tlm_CODE_AOBC_EL].tlm_func = Tlm_AOBC_EL_;
  tlm_table[Tlm_CODE_AOBC_EL_TLOG].tlm_func = Tlm_AOBC_EL_TLOG_;
  tlm_table[Tlm_CODE_AOBC_EL_CLOG].tlm_func = Tlm_AOBC_EL_CLOG_;
  tlm_table[Tlm_CODE_AOBC_EH].tlm_func = Tlm_AOBC_EH_;
  tlm_table[Tlm_CODE_AOBC_EH_RULE].tlm_func = Tlm_AOBC_EH_RULE_;
  tlm_table[Tlm_CODE_AOBC_EH_LOG].tlm_func = Tlm_AOBC_EH_LOG_;
  tlm_table[Tlm_CODE_AOBC_EH_INDEX].tlm_func = Tlm_AOBC_EH_INDEX_;
  tlm_table[Tlm_CODE_AOBC_GIT_REV].tlm_func = Tlm_AOBC_GIT_REV_;
  tlm_table[Tlm_CODE_AOBC_HK_GEN].tlm_func = Tlm_AOBC_HK_GEN_;
  tlm_table[Tlm_CODE_AOBC_HK_COMPO].tlm_func = Tlm_AOBC_HK_COMPO_;
  tlm_table[Tlm_CODE_AOBC_HK_ALGO].tlm_func = Tlm_AOBC_HK_ALGO_;
  tlm_table[Tlm_CODE_AOBC_COMPONENTS].tlm_func = Tlm_AOBC_COMPONENTS_;
  tlm_table[Tlm_CODE_AOBC_POWER].tlm_func = Tlm_AOBC_POWER_;
  tlm_table[Tlm_CODE_AOBC_FRAME_TRANSFORMATION].tlm_func = Tlm_AOBC_FRAME_TRANSFORMATION_;
  tlm_table[Tlm_CODE_AOBC_CONTROL].tlm_func = Tlm_AOBC_CONTROL_;
  tlm_table[Tlm_CODE_AOBC_BIAS].tlm_func = Tlm_AOBC_BIAS_;
  tlm_table[Tlm_CODE_AOBC_TEMP_CALIB].tlm_func = Tlm_AOBC_TEMP_CALIB_;
  tlm_table[Tlm_CODE_AOBC_INTERPOLATION_TARGET1].tlm_func = Tlm_AOBC_INTERPOLATION_TARGET1_;
  tlm_table[Tlm_CODE_AOBC_INTERPOLATION_TARGET2].tlm_func = Tlm_AOBC_INTERPOLATION_TARGET2_;
  tlm_table[Tlm_CODE_AOBC_NON_VOLATILE].tlm_func = Tlm_AOBC_NON_VOLATILE_;
  tlm_table[Tlm_CODE_AOBC_ORBIT].tlm_func = Tlm_AOBC_ORBIT_;
  tlm_table[Tlm_CODE_AOBC_AOCS_MANAGER].tlm_func = Tlm_AOBC_AOCS_MANAGER_;
  tlm_table[Tlm_CODE_AOBC_SUN_POINTING].tlm_func = Tlm_AOBC_SUN_POINTING_;
  tlm_table[Tlm_CODE_AOBC_THREE_AXIS_MTQ].tlm_func = Tlm_AOBC_THREE_AXIS_MTQ_;
  tlm_table[Tlm_CODE_AOBC_THREE_AXIS_RW].tlm_func = Tlm_AOBC_THREE_AXIS_RW_;
  tlm_table[Tlm_CODE_AOBC_FILTERS].tlm_func = Tlm_AOBC_FILTERS_;
  tlm_table[Tlm_CODE_AOBC_ANOMALY].tlm_func = Tlm_AOBC_ANOMALY_;
  tlm_table[Tlm_CODE_AOBC_APP_TIME_2].tlm_func = Tlm_AOBC_APP_TIME_2_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA_SET1].tlm_func = Tlm_AOBC_SAGITTA_SET1_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA_SET2].tlm_func = Tlm_AOBC_SAGITTA_SET2_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA_READ1].tlm_func = Tlm_AOBC_SAGITTA_READ1_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA_READ2].tlm_func = Tlm_AOBC_SAGITTA_READ2_;
  tlm_table[Tlm_CODE_AOBC_STT_GYRO_EKF1].tlm_func = Tlm_AOBC_STT_GYRO_EKF1_;
  tlm_table[Tlm_CODE_AOBC_STT_GYRO_EKF2].tlm_func = Tlm_AOBC_STT_GYRO_EKF2_;
  tlm_table[Tlm_CODE_AOBC_GPSR_RANGE_P1].tlm_func = Tlm_AOBC_GPSR_RANGE_P1_;
  tlm_table[Tlm_CODE_AOBC_GPSR_RANGE_P2].tlm_func = Tlm_AOBC_GPSR_RANGE_P2_;
  tlm_table[Tlm_CODE_AOBC_ORBIT2].tlm_func = Tlm_AOBC_ORBIT2_;
  tlm_table[Tlm_CODE_AOBC_FILTERS_2].tlm_func = Tlm_AOBC_FILTERS_2_;
  tlm_table[Tlm_CODE_AOBC_DR_ALGORITHM].tlm_func = Tlm_AOBC_DR_ALGORITHM_;
  tlm_table[Tlm_CODE_AOBC_RW_DETAILED].tlm_func = Tlm_AOBC_RW_DETAILED_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA1].tlm_func = Tlm_AOBC_SAGITTA1_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA2].tlm_func = Tlm_AOBC_SAGITTA2_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA3].tlm_func = Tlm_AOBC_SAGITTA3_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA4].tlm_func = Tlm_AOBC_SAGITTA4_;
  tlm_table[Tlm_CODE_AOBC_SAGITTA5].tlm_func = Tlm_AOBC_SAGITTA5_;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_OBC_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (220 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(TMGR_get_master_clock().mode_cycle));
  TF_copy_u8(&packet[30], (uint8_t)(mode_manager->stat));
  TF_copy_u8(&packet[31], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[32], (uint8_t)(mode_manager->previous_id));
  TF_copy_u8(&packet[33], (uint8_t)(task_dispatcher->task_list_id));
  TF_copy_u32(&packet[34], (uint32_t)(task_dispatcher->tskd.prev_err.time.total_cycle));
  TF_copy_u8(&packet[38], (uint8_t)(task_dispatcher->tskd.prev_err.time.step));
  TF_copy_u16(&packet[39], (uint16_t)(task_dispatcher->tskd.prev_err.code));
  TF_copy_u8(&packet[41], (uint8_t)(task_dispatcher->tskd.prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[42], (uint32_t)(task_dispatcher->tskd.prev_err.cmd_ret.err_code));
  TF_copy_u32(&packet[46], PH_gs_cmd_list.executed_nodes_);
  TF_copy_u32(&packet[50], (uint32_t)(gs_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[54], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_u8(&packet[56], (uint8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[57], (uint32_t)(gs_command_dispatcher->prev.cmd_ret.err_code));
  TF_copy_u32(&packet[61], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[65], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_u8(&packet[67], (uint8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[68], (uint32_t)(gs_command_dispatcher->prev_err.cmd_ret.err_code));
  TF_copy_u32(&packet[72], (uint32_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[76], PH_rt_cmd_list.executed_nodes_);
  TF_copy_u32(&packet[80], (uint32_t)(realtime_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[84], (uint16_t)(realtime_command_dispatcher->prev.code));
  TF_copy_u8(&packet[86], (uint8_t)(realtime_command_dispatcher->prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[87], (uint32_t)(realtime_command_dispatcher->prev.cmd_ret.err_code));
  TF_copy_u32(&packet[91], (uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[95], (uint16_t)(realtime_command_dispatcher->prev_err.code));
  TF_copy_u8(&packet[97], (uint8_t)(realtime_command_dispatcher->prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[98], (uint32_t)(realtime_command_dispatcher->prev_err.cmd_ret.err_code));
  TF_copy_u32(&packet[102], (uint32_t)(realtime_command_dispatcher->error_counter));
  TF_copy_u32(&packet[106], PH_tl_cmd_list[TLCD_ID_FROM_GS].executed_nodes_);
  TF_copy_u8(&packet[110], (uint8_t)(PH_tl_cmd_list[TLCD_ID_FROM_GS].active_nodes_));
  TF_copy_u32(&packet[111], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle));
  TF_copy_u16(&packet[115], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_u8(&packet[117], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[118], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.cmd_ret.err_code));
  TF_copy_u32(&packet[122], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle));
  TF_copy_u16(&packet[126], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_u8(&packet[128], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[129], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.err_code));
  TF_copy_u32(&packet[133], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].error_counter));
  TF_copy_u8(&packet[137], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error));
  TF_copy_u8(&packet[138], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout));
  TF_copy_u32(&packet[139], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[143], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u32(&packet[145], PH_tl_cmd_list[TLCD_ID_DEPLOY_BC].executed_nodes_);
  TF_copy_u8(&packet[149], (uint8_t)(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC].active_nodes_));
  TF_copy_u32(&packet[150], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.time.total_cycle));
  TF_copy_u16(&packet[154], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.code));
  TF_copy_i8(&packet[156], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[157], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.time.total_cycle));
  TF_copy_u16(&packet[161], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.code));
  TF_copy_i8(&packet[163], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[164], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].error_counter));
  TF_copy_u8(&packet[168], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error));
  TF_copy_u8(&packet[169], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout));
  TF_copy_u32(&packet[170], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u16(&packet[174], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u32(&packet[176], PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM].executed_nodes_);
  TF_copy_u8(&packet[180], (uint8_t)(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM].active_nodes_));
  TF_copy_u32(&packet[181], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.time.total_cycle));
  TF_copy_u16(&packet[185], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.code));
  TF_copy_i8(&packet[187], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[188], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.time.total_cycle));
  TF_copy_u16(&packet[192], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.code));
  TF_copy_i8(&packet[194], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[195], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].error_counter));
  TF_copy_u8(&packet[199], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error));
  TF_copy_u8(&packet[200], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout));
  TF_copy_u32(&packet[201], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u16(&packet[205], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u8(&packet[207], (uint8_t)(block_command_table->pos.block));
  TF_copy_u8(&packet[208], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[209], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[213], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[215], PH_rt_tlm_list.executed_nodes_);
  TF_copy_u8(&packet[219], (uint8_t)(PH_rt_tlm_list.active_nodes_));
#endif

  *len = 220;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_TL_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  int offset = TL_TLM_PAGE_SIZE*(timeline_command_dispatcher->tlm_info_.page_no);

  if (224 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], TLCD_update_tl_list_for_tlm(timeline_command_dispatcher->tlm_info_.id));
  TF_copy_u8(&packet[27], timeline_command_dispatcher->tlm_info_.page_no);
  TF_copy_u32(&packet[28], timeline_command_dispatcher->tlm_info_.updated_at);
  TF_copy_u16(&packet[32], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+0]));
  TF_copy_u32(&packet[34], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+0]));
  TF_copy_u8(&packet[38], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+0])[0]);
  TF_copy_u8(&packet[39], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+0])[1]);
  TF_copy_u8(&packet[40], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+0])[2]);
  TF_copy_u8(&packet[41], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+0])[3]);
  TF_copy_u8(&packet[42], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+0])[4]);
  TF_copy_u8(&packet[43], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+0])[5]);
  TF_copy_u16(&packet[44], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+1]));
  TF_copy_u32(&packet[46], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+1]));
  TF_copy_u8(&packet[50], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+1])[0]);
  TF_copy_u8(&packet[51], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+1])[1]);
  TF_copy_u8(&packet[52], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+1])[2]);
  TF_copy_u8(&packet[53], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+1])[3]);
  TF_copy_u8(&packet[54], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+1])[4]);
  TF_copy_u8(&packet[55], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+1])[5]);
  TF_copy_u16(&packet[56], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+2]));
  TF_copy_u32(&packet[58], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+2]));
  TF_copy_u8(&packet[62], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+2])[0]);
  TF_copy_u8(&packet[63], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+2])[1]);
  TF_copy_u8(&packet[64], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+2])[2]);
  TF_copy_u8(&packet[65], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+2])[3]);
  TF_copy_u8(&packet[66], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+2])[4]);
  TF_copy_u8(&packet[67], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+2])[5]);
  TF_copy_u16(&packet[68], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+3]));
  TF_copy_u32(&packet[70], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+3]));
  TF_copy_u8(&packet[74], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+3])[0]);
  TF_copy_u8(&packet[75], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+3])[1]);
  TF_copy_u8(&packet[76], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+3])[2]);
  TF_copy_u8(&packet[77], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+3])[3]);
  TF_copy_u8(&packet[78], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+3])[4]);
  TF_copy_u8(&packet[79], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+3])[5]);
  TF_copy_u16(&packet[80], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+4]));
  TF_copy_u32(&packet[82], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+4]));
  TF_copy_u8(&packet[86], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+4])[0]);
  TF_copy_u8(&packet[87], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+4])[1]);
  TF_copy_u8(&packet[88], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+4])[2]);
  TF_copy_u8(&packet[89], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+4])[3]);
  TF_copy_u8(&packet[90], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+4])[4]);
  TF_copy_u8(&packet[91], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+4])[5]);
  TF_copy_u16(&packet[92], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+5]));
  TF_copy_u32(&packet[94], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+5]));
  TF_copy_u8(&packet[98], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+5])[0]);
  TF_copy_u8(&packet[99], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+5])[1]);
  TF_copy_u8(&packet[100], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+5])[2]);
  TF_copy_u8(&packet[101], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+5])[3]);
  TF_copy_u8(&packet[102], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+5])[4]);
  TF_copy_u8(&packet[103], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+5])[5]);
  TF_copy_u16(&packet[104], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+6]));
  TF_copy_u32(&packet[106], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+6]));
  TF_copy_u8(&packet[110], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+6])[0]);
  TF_copy_u8(&packet[111], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+6])[1]);
  TF_copy_u8(&packet[112], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+6])[2]);
  TF_copy_u8(&packet[113], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+6])[3]);
  TF_copy_u8(&packet[114], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+6])[4]);
  TF_copy_u8(&packet[115], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+6])[5]);
  TF_copy_u16(&packet[116], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+7]));
  TF_copy_u32(&packet[118], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+7]));
  TF_copy_u8(&packet[122], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+7])[0]);
  TF_copy_u8(&packet[123], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+7])[1]);
  TF_copy_u8(&packet[124], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+7])[2]);
  TF_copy_u8(&packet[125], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+7])[3]);
  TF_copy_u8(&packet[126], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+7])[4]);
  TF_copy_u8(&packet[127], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+7])[5]);
  TF_copy_u16(&packet[128], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+8]));
  TF_copy_u32(&packet[130], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+8]));
  TF_copy_u8(&packet[134], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+8])[0]);
  TF_copy_u8(&packet[135], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+8])[1]);
  TF_copy_u8(&packet[136], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+8])[2]);
  TF_copy_u8(&packet[137], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+8])[3]);
  TF_copy_u8(&packet[138], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+8])[4]);
  TF_copy_u8(&packet[139], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+8])[5]);
  TF_copy_u16(&packet[140], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+9]));
  TF_copy_u32(&packet[142], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+9]));
  TF_copy_u8(&packet[146], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+9])[0]);
  TF_copy_u8(&packet[147], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+9])[1]);
  TF_copy_u8(&packet[148], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+9])[2]);
  TF_copy_u8(&packet[149], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+9])[3]);
  TF_copy_u8(&packet[150], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+9])[4]);
  TF_copy_u8(&packet[151], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+9])[5]);
  TF_copy_u16(&packet[152], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+10]));
  TF_copy_u32(&packet[154], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+10]));
  TF_copy_u8(&packet[158], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+10])[0]);
  TF_copy_u8(&packet[159], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+10])[1]);
  TF_copy_u8(&packet[160], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+10])[2]);
  TF_copy_u8(&packet[161], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+10])[3]);
  TF_copy_u8(&packet[162], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+10])[4]);
  TF_copy_u8(&packet[163], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+10])[5]);
  TF_copy_u16(&packet[164], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+11]));
  TF_copy_u32(&packet[166], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+11]));
  TF_copy_u8(&packet[170], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+11])[0]);
  TF_copy_u8(&packet[171], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+11])[1]);
  TF_copy_u8(&packet[172], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+11])[2]);
  TF_copy_u8(&packet[173], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+11])[3]);
  TF_copy_u8(&packet[174], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+11])[4]);
  TF_copy_u8(&packet[175], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+11])[5]);
  TF_copy_u16(&packet[176], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+12]));
  TF_copy_u32(&packet[178], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+12]));
  TF_copy_u8(&packet[182], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+12])[0]);
  TF_copy_u8(&packet[183], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+12])[1]);
  TF_copy_u8(&packet[184], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+12])[2]);
  TF_copy_u8(&packet[185], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+12])[3]);
  TF_copy_u8(&packet[186], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+12])[4]);
  TF_copy_u8(&packet[187], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+12])[5]);
  TF_copy_u16(&packet[188], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+13]));
  TF_copy_u32(&packet[190], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+13]));
  TF_copy_u8(&packet[194], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+13])[0]);
  TF_copy_u8(&packet[195], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+13])[1]);
  TF_copy_u8(&packet[196], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+13])[2]);
  TF_copy_u8(&packet[197], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+13])[3]);
  TF_copy_u8(&packet[198], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+13])[4]);
  TF_copy_u8(&packet[199], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+13])[5]);
  TF_copy_u16(&packet[200], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+14]));
  TF_copy_u32(&packet[202], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+14]));
  TF_copy_u8(&packet[206], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+14])[0]);
  TF_copy_u8(&packet[207], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+14])[1]);
  TF_copy_u8(&packet[208], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+14])[2]);
  TF_copy_u8(&packet[209], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+14])[3]);
  TF_copy_u8(&packet[210], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+14])[4]);
  TF_copy_u8(&packet[211], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+14])[5]);
  TF_copy_u16(&packet[212], (uint16_t)CCP_get_id(timeline_command_dispatcher->tlm_info_.tl_list[offset+15]));
  TF_copy_u32(&packet[214], CCP_get_ti(timeline_command_dispatcher->tlm_info_.tl_list[offset+15]));
  TF_copy_u8(&packet[218], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+15])[0]);
  TF_copy_u8(&packet[219], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+15])[1]);
  TF_copy_u8(&packet[220], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+15])[2]);
  TF_copy_u8(&packet[221], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+15])[3]);
  TF_copy_u8(&packet[222], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+15])[4]);
  TF_copy_u8(&packet[223], CCP_get_param_head(timeline_command_dispatcher->tlm_info_.tl_list[offset+15])[5]);
#endif

  *len = 224;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_BL_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (234 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u16(&packet[26], block_command_table->pos.block);
  TF_copy_u8(&packet[28], block_command_table->pos.cmd);
  TF_copy_u8(&packet[29], BCT_get_bc_length(block_command_table->pos.block));
  TF_copy_u8(&packet[30], BCE_get_bc_exe_params(block_command_table->pos.block)->rotate.next_cmd);
  TF_copy_u16(&packet[31], BCE_get_bc_exe_params(block_command_table->pos.block)->rotate.counter);
  TF_copy_u16(&packet[33], BCE_get_bc_exe_params(block_command_table->pos.block)->rotate.interval);
  TF_copy_u16(&packet[35], BCE_get_bc_exe_params(block_command_table->pos.block)->timelimit_combine.call_num);
  TF_copy_u16(&packet[37], BCE_get_bc_exe_params(block_command_table->pos.block)->timelimit_combine.timeover_num);
  TF_copy_u8(&packet[39], BCE_get_bc_exe_params(block_command_table->pos.block)->timelimit_combine.last_timeover_cmd_pos);
  TF_copy_u8(&packet[40], BCE_get_bc_exe_params(block_command_table->pos.block)->timelimit_combine.worst_cmd_pos);
  TF_copy_u8(&packet[41], BCE_get_bc_exe_params(block_command_table->pos.block)->is_active);
  TF_copy_u16(&packet[42], (uint16_t)BCT_get_id(block_command_table->pos.block, 0));
  TF_copy_u32(&packet[44], (uint32_t)BCT_get_ti(block_command_table->pos.block, 0));
  TF_copy_u8(&packet[48], BCT_get_param_head(block_command_table->pos.block, 0)[0]);
  TF_copy_u8(&packet[49], BCT_get_param_head(block_command_table->pos.block, 0)[1]);
  TF_copy_u8(&packet[50], BCT_get_param_head(block_command_table->pos.block, 0)[2]);
  TF_copy_u8(&packet[51], BCT_get_param_head(block_command_table->pos.block, 0)[3]);
  TF_copy_u8(&packet[52], BCT_get_param_head(block_command_table->pos.block, 0)[4]);
  TF_copy_u8(&packet[53], BCT_get_param_head(block_command_table->pos.block, 0)[5]);
  TF_copy_u16(&packet[54], (uint16_t)BCT_get_id(block_command_table->pos.block, 1));
  TF_copy_u32(&packet[56], (uint32_t)BCT_get_ti(block_command_table->pos.block, 1));
  TF_copy_u8(&packet[60], BCT_get_param_head(block_command_table->pos.block, 1)[0]);
  TF_copy_u8(&packet[61], BCT_get_param_head(block_command_table->pos.block, 1)[1]);
  TF_copy_u8(&packet[62], BCT_get_param_head(block_command_table->pos.block, 1)[2]);
  TF_copy_u8(&packet[63], BCT_get_param_head(block_command_table->pos.block, 1)[3]);
  TF_copy_u8(&packet[64], BCT_get_param_head(block_command_table->pos.block, 1)[4]);
  TF_copy_u8(&packet[65], BCT_get_param_head(block_command_table->pos.block, 1)[5]);
  TF_copy_u16(&packet[66], (uint16_t)BCT_get_id(block_command_table->pos.block, 2));
  TF_copy_u32(&packet[68], (uint32_t)BCT_get_ti(block_command_table->pos.block, 2));
  TF_copy_u8(&packet[72], BCT_get_param_head(block_command_table->pos.block, 2)[0]);
  TF_copy_u8(&packet[73], BCT_get_param_head(block_command_table->pos.block, 2)[1]);
  TF_copy_u8(&packet[74], BCT_get_param_head(block_command_table->pos.block, 2)[2]);
  TF_copy_u8(&packet[75], BCT_get_param_head(block_command_table->pos.block, 2)[3]);
  TF_copy_u8(&packet[76], BCT_get_param_head(block_command_table->pos.block, 2)[4]);
  TF_copy_u8(&packet[77], BCT_get_param_head(block_command_table->pos.block, 2)[5]);
  TF_copy_u16(&packet[78], (uint16_t)BCT_get_id(block_command_table->pos.block, 3));
  TF_copy_u32(&packet[80], (uint32_t)BCT_get_ti(block_command_table->pos.block, 3));
  TF_copy_u8(&packet[84], BCT_get_param_head(block_command_table->pos.block, 3)[0]);
  TF_copy_u8(&packet[85], BCT_get_param_head(block_command_table->pos.block, 3)[1]);
  TF_copy_u8(&packet[86], BCT_get_param_head(block_command_table->pos.block, 3)[2]);
  TF_copy_u8(&packet[87], BCT_get_param_head(block_command_table->pos.block, 3)[3]);
  TF_copy_u8(&packet[88], BCT_get_param_head(block_command_table->pos.block, 3)[4]);
  TF_copy_u8(&packet[89], BCT_get_param_head(block_command_table->pos.block, 3)[5]);
  TF_copy_u16(&packet[90], (uint16_t)BCT_get_id(block_command_table->pos.block, 4));
  TF_copy_u32(&packet[92], (uint32_t)BCT_get_ti(block_command_table->pos.block, 4));
  TF_copy_u8(&packet[96], BCT_get_param_head(block_command_table->pos.block, 4)[0]);
  TF_copy_u8(&packet[97], BCT_get_param_head(block_command_table->pos.block, 4)[1]);
  TF_copy_u8(&packet[98], BCT_get_param_head(block_command_table->pos.block, 4)[2]);
  TF_copy_u8(&packet[99], BCT_get_param_head(block_command_table->pos.block, 4)[3]);
  TF_copy_u8(&packet[100], BCT_get_param_head(block_command_table->pos.block, 4)[4]);
  TF_copy_u8(&packet[101], BCT_get_param_head(block_command_table->pos.block, 4)[5]);
  TF_copy_u16(&packet[102], (uint16_t)BCT_get_id(block_command_table->pos.block, 5));
  TF_copy_u32(&packet[104], (uint32_t)BCT_get_ti(block_command_table->pos.block, 5));
  TF_copy_u8(&packet[108], BCT_get_param_head(block_command_table->pos.block, 5)[0]);
  TF_copy_u8(&packet[109], BCT_get_param_head(block_command_table->pos.block, 5)[1]);
  TF_copy_u8(&packet[110], BCT_get_param_head(block_command_table->pos.block, 5)[2]);
  TF_copy_u8(&packet[111], BCT_get_param_head(block_command_table->pos.block, 5)[3]);
  TF_copy_u8(&packet[112], BCT_get_param_head(block_command_table->pos.block, 5)[4]);
  TF_copy_u8(&packet[113], BCT_get_param_head(block_command_table->pos.block, 5)[5]);
  TF_copy_u16(&packet[114], (uint16_t)BCT_get_id(block_command_table->pos.block, 6));
  TF_copy_u32(&packet[116], (uint32_t)BCT_get_ti(block_command_table->pos.block, 6));
  TF_copy_u8(&packet[120], BCT_get_param_head(block_command_table->pos.block, 6)[0]);
  TF_copy_u8(&packet[121], BCT_get_param_head(block_command_table->pos.block, 6)[1]);
  TF_copy_u8(&packet[122], BCT_get_param_head(block_command_table->pos.block, 6)[2]);
  TF_copy_u8(&packet[123], BCT_get_param_head(block_command_table->pos.block, 6)[3]);
  TF_copy_u8(&packet[124], BCT_get_param_head(block_command_table->pos.block, 6)[4]);
  TF_copy_u8(&packet[125], BCT_get_param_head(block_command_table->pos.block, 6)[5]);
  TF_copy_u16(&packet[126], (uint16_t)BCT_get_id(block_command_table->pos.block, 7));
  TF_copy_u32(&packet[128], (uint32_t)BCT_get_ti(block_command_table->pos.block, 7));
  TF_copy_u8(&packet[132], BCT_get_param_head(block_command_table->pos.block, 7)[0]);
  TF_copy_u8(&packet[133], BCT_get_param_head(block_command_table->pos.block, 7)[1]);
  TF_copy_u8(&packet[134], BCT_get_param_head(block_command_table->pos.block, 7)[2]);
  TF_copy_u8(&packet[135], BCT_get_param_head(block_command_table->pos.block, 7)[3]);
  TF_copy_u8(&packet[136], BCT_get_param_head(block_command_table->pos.block, 7)[4]);
  TF_copy_u8(&packet[137], BCT_get_param_head(block_command_table->pos.block, 7)[5]);
  TF_copy_u16(&packet[138], (uint16_t)BCT_get_id(block_command_table->pos.block, 8));
  TF_copy_u32(&packet[140], (uint32_t)BCT_get_ti(block_command_table->pos.block, 8));
  TF_copy_u8(&packet[144], BCT_get_param_head(block_command_table->pos.block, 8)[0]);
  TF_copy_u8(&packet[145], BCT_get_param_head(block_command_table->pos.block, 8)[1]);
  TF_copy_u8(&packet[146], BCT_get_param_head(block_command_table->pos.block, 8)[2]);
  TF_copy_u8(&packet[147], BCT_get_param_head(block_command_table->pos.block, 8)[3]);
  TF_copy_u8(&packet[148], BCT_get_param_head(block_command_table->pos.block, 8)[4]);
  TF_copy_u8(&packet[149], BCT_get_param_head(block_command_table->pos.block, 8)[5]);
  TF_copy_u16(&packet[150], (uint16_t)BCT_get_id(block_command_table->pos.block, 9));
  TF_copy_u32(&packet[152], (uint32_t)BCT_get_ti(block_command_table->pos.block, 9));
  TF_copy_u8(&packet[156], BCT_get_param_head(block_command_table->pos.block, 9)[0]);
  TF_copy_u8(&packet[157], BCT_get_param_head(block_command_table->pos.block, 9)[1]);
  TF_copy_u8(&packet[158], BCT_get_param_head(block_command_table->pos.block, 9)[2]);
  TF_copy_u8(&packet[159], BCT_get_param_head(block_command_table->pos.block, 9)[3]);
  TF_copy_u8(&packet[160], BCT_get_param_head(block_command_table->pos.block, 9)[4]);
  TF_copy_u8(&packet[161], BCT_get_param_head(block_command_table->pos.block, 9)[5]);
  TF_copy_u16(&packet[162], (uint16_t)BCT_get_id(block_command_table->pos.block, 10));
  TF_copy_u32(&packet[164], (uint32_t)BCT_get_ti(block_command_table->pos.block, 10));
  TF_copy_u8(&packet[168], BCT_get_param_head(block_command_table->pos.block, 10)[0]);
  TF_copy_u8(&packet[169], BCT_get_param_head(block_command_table->pos.block, 10)[1]);
  TF_copy_u8(&packet[170], BCT_get_param_head(block_command_table->pos.block, 10)[2]);
  TF_copy_u8(&packet[171], BCT_get_param_head(block_command_table->pos.block, 10)[3]);
  TF_copy_u8(&packet[172], BCT_get_param_head(block_command_table->pos.block, 10)[4]);
  TF_copy_u8(&packet[173], BCT_get_param_head(block_command_table->pos.block, 10)[5]);
  TF_copy_u16(&packet[174], (uint16_t)BCT_get_id(block_command_table->pos.block, 11));
  TF_copy_u32(&packet[176], (uint32_t)BCT_get_ti(block_command_table->pos.block, 11));
  TF_copy_u8(&packet[180], BCT_get_param_head(block_command_table->pos.block, 11)[0]);
  TF_copy_u8(&packet[181], BCT_get_param_head(block_command_table->pos.block, 11)[1]);
  TF_copy_u8(&packet[182], BCT_get_param_head(block_command_table->pos.block, 11)[2]);
  TF_copy_u8(&packet[183], BCT_get_param_head(block_command_table->pos.block, 11)[3]);
  TF_copy_u8(&packet[184], BCT_get_param_head(block_command_table->pos.block, 11)[4]);
  TF_copy_u8(&packet[185], BCT_get_param_head(block_command_table->pos.block, 11)[5]);
  TF_copy_u16(&packet[186], (uint16_t)BCT_get_id(block_command_table->pos.block, 12));
  TF_copy_u32(&packet[188], (uint32_t)BCT_get_ti(block_command_table->pos.block, 12));
  TF_copy_u8(&packet[192], BCT_get_param_head(block_command_table->pos.block, 12)[0]);
  TF_copy_u8(&packet[193], BCT_get_param_head(block_command_table->pos.block, 12)[1]);
  TF_copy_u8(&packet[194], BCT_get_param_head(block_command_table->pos.block, 12)[2]);
  TF_copy_u8(&packet[195], BCT_get_param_head(block_command_table->pos.block, 12)[3]);
  TF_copy_u8(&packet[196], BCT_get_param_head(block_command_table->pos.block, 12)[4]);
  TF_copy_u8(&packet[197], BCT_get_param_head(block_command_table->pos.block, 12)[5]);
  TF_copy_u16(&packet[198], (uint16_t)BCT_get_id(block_command_table->pos.block, 13));
  TF_copy_u32(&packet[200], (uint32_t)BCT_get_ti(block_command_table->pos.block, 13));
  TF_copy_u8(&packet[204], BCT_get_param_head(block_command_table->pos.block, 13)[0]);
  TF_copy_u8(&packet[205], BCT_get_param_head(block_command_table->pos.block, 13)[1]);
  TF_copy_u8(&packet[206], BCT_get_param_head(block_command_table->pos.block, 13)[2]);
  TF_copy_u8(&packet[207], BCT_get_param_head(block_command_table->pos.block, 13)[3]);
  TF_copy_u8(&packet[208], BCT_get_param_head(block_command_table->pos.block, 13)[4]);
  TF_copy_u8(&packet[209], BCT_get_param_head(block_command_table->pos.block, 13)[5]);
  TF_copy_u16(&packet[210], (uint16_t)BCT_get_id(block_command_table->pos.block, 14));
  TF_copy_u32(&packet[212], (uint32_t)BCT_get_ti(block_command_table->pos.block, 14));
  TF_copy_u8(&packet[216], BCT_get_param_head(block_command_table->pos.block, 14)[0]);
  TF_copy_u8(&packet[217], BCT_get_param_head(block_command_table->pos.block, 14)[1]);
  TF_copy_u8(&packet[218], BCT_get_param_head(block_command_table->pos.block, 14)[2]);
  TF_copy_u8(&packet[219], BCT_get_param_head(block_command_table->pos.block, 14)[3]);
  TF_copy_u8(&packet[220], BCT_get_param_head(block_command_table->pos.block, 14)[4]);
  TF_copy_u8(&packet[221], BCT_get_param_head(block_command_table->pos.block, 14)[5]);
  TF_copy_u16(&packet[222], (uint16_t)BCT_get_id(block_command_table->pos.block, 15));
  TF_copy_u32(&packet[224], (uint32_t)BCT_get_ti(block_command_table->pos.block, 15));
  TF_copy_u8(&packet[228], BCT_get_param_head(block_command_table->pos.block, 15)[0]);
  TF_copy_u8(&packet[229], BCT_get_param_head(block_command_table->pos.block, 15)[1]);
  TF_copy_u8(&packet[230], BCT_get_param_head(block_command_table->pos.block, 15)[2]);
  TF_copy_u8(&packet[231], BCT_get_param_head(block_command_table->pos.block, 15)[3]);
  TF_copy_u8(&packet[232], BCT_get_param_head(block_command_table->pos.block, 15)[4]);
  TF_copy_u8(&packet[233], BCT_get_param_head(block_command_table->pos.block, 15)[5]);
#endif

  *len = 234;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_CA_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (139 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(command_analyze->tlm_page_no));
  TF_copy_u32(&packet[27], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+0].cmd_func));
  TF_copy_u8(&packet[31], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+0].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+0].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[32], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+0].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+0].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[33], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+0].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+0].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[34], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+1].cmd_func));
  TF_copy_u8(&packet[38], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+1].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+1].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[39], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+1].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+1].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[40], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+1].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+1].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[41], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+2].cmd_func));
  TF_copy_u8(&packet[45], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+2].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+2].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[46], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+2].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+2].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[47], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+2].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+2].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[48], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+3].cmd_func));
  TF_copy_u8(&packet[52], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+3].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+3].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[53], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+3].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+3].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[54], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+3].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+3].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[55], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+4].cmd_func));
  TF_copy_u8(&packet[59], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+4].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+4].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[60], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+4].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+4].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[61], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+4].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+4].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[62], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+5].cmd_func));
  TF_copy_u8(&packet[66], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+5].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+5].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[67], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+5].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+5].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[68], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+5].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+5].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[69], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+6].cmd_func));
  TF_copy_u8(&packet[73], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+6].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+6].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[74], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+6].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+6].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[75], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+6].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+6].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[76], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+7].cmd_func));
  TF_copy_u8(&packet[80], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+7].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+7].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[81], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+7].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+7].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[82], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+7].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+7].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[83], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+8].cmd_func));
  TF_copy_u8(&packet[87], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+8].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+8].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[88], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+8].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+8].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[89], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+8].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+8].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[90], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+9].cmd_func));
  TF_copy_u8(&packet[94], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+9].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+9].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[95], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+9].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+9].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[96], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+9].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+9].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[97], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+10].cmd_func));
  TF_copy_u8(&packet[101], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+10].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+10].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[102], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+10].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+10].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[103], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+10].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+10].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[104], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+11].cmd_func));
  TF_copy_u8(&packet[108], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+11].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+11].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[109], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+11].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+11].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[110], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+11].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+11].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[111], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+12].cmd_func));
  TF_copy_u8(&packet[115], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+12].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+12].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[116], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+12].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+12].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[117], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+12].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+12].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[118], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+13].cmd_func));
  TF_copy_u8(&packet[122], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+13].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+13].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[123], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+13].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+13].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[124], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+13].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+13].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[125], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+14].cmd_func));
  TF_copy_u8(&packet[129], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+14].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+14].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[130], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+14].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+14].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[131], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+14].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+14].param_size_infos[2].packed_info.bit.second & 0x0f)));
  TF_copy_u32(&packet[132], (uint32_t)(command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+15].cmd_func));
  TF_copy_u8(&packet[136], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+15].param_size_infos[0].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+15].param_size_infos[0].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[137], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+15].param_size_infos[1].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+15].param_size_infos[1].packed_info.bit.second & 0x0f)));
  TF_copy_u8(&packet[138], (uint8_t)(((command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+15].param_size_infos[2].packed_info.bit.first & 0x0f) << 4) | (command_analyze->cmd_table[CA_TLM_PAGE_SIZE*(command_analyze->tlm_page_no)+15].param_size_infos[2].packed_info.bit.second & 0x0f)));
#endif

  *len = 139;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_TF_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  uint8_t page = telemetry_frame->tlm_page_no;
  int offset = TF_TLM_PAGE_SIZE * (telemetry_frame->tlm_page_no);

  if (155 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], page);
  TF_copy_u32(&packet[27], (uint32_t)(telemetry_frame->tlm_table[offset+0].tlm_func));
  TF_copy_u32(&packet[31], (uint32_t)(telemetry_frame->tlm_table[offset+1].tlm_func));
  TF_copy_u32(&packet[35], (uint32_t)(telemetry_frame->tlm_table[offset+2].tlm_func));
  TF_copy_u32(&packet[39], (uint32_t)(telemetry_frame->tlm_table[offset+3].tlm_func));
  TF_copy_u32(&packet[43], (uint32_t)(telemetry_frame->tlm_table[offset+4].tlm_func));
  TF_copy_u32(&packet[47], (uint32_t)(telemetry_frame->tlm_table[offset+5].tlm_func));
  TF_copy_u32(&packet[51], (uint32_t)(telemetry_frame->tlm_table[offset+6].tlm_func));
  TF_copy_u32(&packet[55], (uint32_t)(telemetry_frame->tlm_table[offset+7].tlm_func));
  TF_copy_u32(&packet[59], (uint32_t)(telemetry_frame->tlm_table[offset+8].tlm_func));
  TF_copy_u32(&packet[63], (uint32_t)(telemetry_frame->tlm_table[offset+9].tlm_func));
  TF_copy_u32(&packet[67], (uint32_t)(telemetry_frame->tlm_table[offset+10].tlm_func));
  TF_copy_u32(&packet[71], (uint32_t)(telemetry_frame->tlm_table[offset+11].tlm_func));
  TF_copy_u32(&packet[75], (uint32_t)(telemetry_frame->tlm_table[offset+12].tlm_func));
  TF_copy_u32(&packet[79], (uint32_t)(telemetry_frame->tlm_table[offset+13].tlm_func));
  TF_copy_u32(&packet[83], (uint32_t)(telemetry_frame->tlm_table[offset+14].tlm_func));
  TF_copy_u32(&packet[87], (uint32_t)(telemetry_frame->tlm_table[offset+15].tlm_func));
  TF_copy_u32(&packet[91], (uint32_t)(telemetry_frame->tlm_table[offset+16].tlm_func));
  TF_copy_u32(&packet[95], (uint32_t)(telemetry_frame->tlm_table[offset+17].tlm_func));
  TF_copy_u32(&packet[99], (uint32_t)(telemetry_frame->tlm_table[offset+18].tlm_func));
  TF_copy_u32(&packet[103], (uint32_t)(telemetry_frame->tlm_table[offset+19].tlm_func));
  TF_copy_u32(&packet[107], (uint32_t)(telemetry_frame->tlm_table[offset+20].tlm_func));
  TF_copy_u32(&packet[111], (uint32_t)(telemetry_frame->tlm_table[offset+21].tlm_func));
  TF_copy_u32(&packet[115], (uint32_t)(telemetry_frame->tlm_table[offset+22].tlm_func));
  TF_copy_u32(&packet[119], (uint32_t)(telemetry_frame->tlm_table[offset+23].tlm_func));
  TF_copy_u32(&packet[123], (uint32_t)(telemetry_frame->tlm_table[offset+24].tlm_func));
  TF_copy_u32(&packet[127], (uint32_t)(telemetry_frame->tlm_table[offset+25].tlm_func));
  TF_copy_u32(&packet[131], (uint32_t)(telemetry_frame->tlm_table[offset+26].tlm_func));
  TF_copy_u32(&packet[135], (uint32_t)(telemetry_frame->tlm_table[offset+27].tlm_func));
  TF_copy_u32(&packet[139], (uint32_t)(telemetry_frame->tlm_table[offset+28].tlm_func));
  TF_copy_u32(&packet[143], (uint32_t)(telemetry_frame->tlm_table[offset+29].tlm_func));
  TF_copy_u32(&packet[147], (uint32_t)(telemetry_frame->tlm_table[offset+30].tlm_func));
  TF_copy_u32(&packet[151], (uint32_t)(telemetry_frame->tlm_table[offset+31].tlm_func));
#endif

  *len = 155;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_DCU_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (114 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], divided_cmd_utility->exec_log_order[0]);
  TF_copy_u8(&packet[27], divided_cmd_utility->exec_log_order[1]);
  TF_copy_u8(&packet[28], divided_cmd_utility->exec_log_order[2]);
  TF_copy_u8(&packet[29], divided_cmd_utility->exec_log_order[3]);
  TF_copy_u8(&packet[30], divided_cmd_utility->exec_log_order[4]);
  TF_copy_u8(&packet[31], divided_cmd_utility->exec_log_order[5]);
  TF_copy_u8(&packet[32], divided_cmd_utility->exec_log_order[6]);
  TF_copy_u8(&packet[33], divided_cmd_utility->exec_log_order[7]);
  TF_copy_u16(&packet[34], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].cmd_code));
  TF_copy_u8(&packet[36], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].status));
  TF_copy_u16(&packet[37], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].exec_counter));
  TF_copy_i8(&packet[39], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].last_exec_sts));
  TF_copy_u32(&packet[40], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].last_exec_time.total_cycle));
  TF_copy_u16(&packet[44], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].cmd_code));
  TF_copy_u8(&packet[46], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].status));
  TF_copy_u16(&packet[47], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].exec_counter));
  TF_copy_i8(&packet[49], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].last_exec_sts));
  TF_copy_u32(&packet[50], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].last_exec_time.total_cycle));
  TF_copy_u16(&packet[54], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[2]].cmd_code));
  TF_copy_u8(&packet[56], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[2]].status));
  TF_copy_u16(&packet[57], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[2]].exec_counter));
  TF_copy_i8(&packet[59], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[2]].last_exec_sts));
  TF_copy_u32(&packet[60], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[2]].last_exec_time.total_cycle));
  TF_copy_u16(&packet[64], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[3]].cmd_code));
  TF_copy_u8(&packet[66], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[3]].status));
  TF_copy_u16(&packet[67], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[3]].exec_counter));
  TF_copy_i8(&packet[69], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[3]].last_exec_sts));
  TF_copy_u32(&packet[70], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[3]].last_exec_time.total_cycle));
  TF_copy_u16(&packet[74], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[4]].cmd_code));
  TF_copy_u8(&packet[76], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[4]].status));
  TF_copy_u16(&packet[77], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[4]].exec_counter));
  TF_copy_i8(&packet[79], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[4]].last_exec_sts));
  TF_copy_u32(&packet[80], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[4]].last_exec_time.total_cycle));
  TF_copy_u16(&packet[84], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[5]].cmd_code));
  TF_copy_u8(&packet[86], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[5]].status));
  TF_copy_u16(&packet[87], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[5]].exec_counter));
  TF_copy_i8(&packet[89], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[5]].last_exec_sts));
  TF_copy_u32(&packet[90], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[5]].last_exec_time.total_cycle));
  TF_copy_u16(&packet[94], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[6]].cmd_code));
  TF_copy_u8(&packet[96], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[6]].status));
  TF_copy_u16(&packet[97], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[6]].exec_counter));
  TF_copy_i8(&packet[99], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[6]].last_exec_sts));
  TF_copy_u32(&packet[100], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[6]].last_exec_time.total_cycle));
  TF_copy_u16(&packet[104], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[7]].cmd_code));
  TF_copy_u8(&packet[106], (uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[7]].status));
  TF_copy_u16(&packet[107], (uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[7]].exec_counter));
  TF_copy_i8(&packet[109], (int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[7]].last_exec_sts));
  TF_copy_u32(&packet[110], (uint32_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[7]].last_exec_time.total_cycle));
#endif

  *len = 114;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_MM_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (101 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[27], (uint8_t)(mode_manager->previous_id));
  TF_copy_u8(&packet[28], (uint8_t)(mode_manager->stat));
  TF_copy_u8(&packet[29], (uint8_t)(mode_manager->mode_list[0]));
  TF_copy_u8(&packet[30], (uint8_t)(mode_manager->mode_list[1]));
  TF_copy_u8(&packet[31], (uint8_t)(mode_manager->mode_list[2]));
  TF_copy_u8(&packet[32], (uint8_t)(mode_manager->mode_list[3]));
  TF_copy_u8(&packet[33], (uint8_t)(mode_manager->mode_list[4]));
  TF_copy_u8(&packet[34], (uint8_t)(mode_manager->mode_list[5]));
  TF_copy_u8(&packet[35], (uint8_t)(mode_manager->mode_list[6]));
  TF_copy_u8(&packet[36], (uint8_t)(mode_manager->mode_list[7]));
  TF_copy_u8(&packet[37], (uint8_t)(mode_manager->transition_table[0][0]));
  TF_copy_u8(&packet[38], (uint8_t)(mode_manager->transition_table[0][1]));
  TF_copy_u8(&packet[39], (uint8_t)(mode_manager->transition_table[0][2]));
  TF_copy_u8(&packet[40], (uint8_t)(mode_manager->transition_table[0][3]));
  TF_copy_u8(&packet[41], (uint8_t)(mode_manager->transition_table[0][4]));
  TF_copy_u8(&packet[42], (uint8_t)(mode_manager->transition_table[0][5]));
  TF_copy_u8(&packet[43], (uint8_t)(mode_manager->transition_table[0][6]));
  TF_copy_u8(&packet[44], (uint8_t)(mode_manager->transition_table[0][7]));
  TF_copy_u8(&packet[45], (uint8_t)(mode_manager->transition_table[1][0]));
  TF_copy_u8(&packet[46], (uint8_t)(mode_manager->transition_table[1][1]));
  TF_copy_u8(&packet[47], (uint8_t)(mode_manager->transition_table[1][2]));
  TF_copy_u8(&packet[48], (uint8_t)(mode_manager->transition_table[1][3]));
  TF_copy_u8(&packet[49], (uint8_t)(mode_manager->transition_table[1][4]));
  TF_copy_u8(&packet[50], (uint8_t)(mode_manager->transition_table[1][5]));
  TF_copy_u8(&packet[51], (uint8_t)(mode_manager->transition_table[1][6]));
  TF_copy_u8(&packet[52], (uint8_t)(mode_manager->transition_table[1][7]));
  TF_copy_u8(&packet[53], (uint8_t)(mode_manager->transition_table[2][0]));
  TF_copy_u8(&packet[54], (uint8_t)(mode_manager->transition_table[2][1]));
  TF_copy_u8(&packet[55], (uint8_t)(mode_manager->transition_table[2][2]));
  TF_copy_u8(&packet[56], (uint8_t)(mode_manager->transition_table[2][3]));
  TF_copy_u8(&packet[57], (uint8_t)(mode_manager->transition_table[2][4]));
  TF_copy_u8(&packet[58], (uint8_t)(mode_manager->transition_table[2][5]));
  TF_copy_u8(&packet[59], (uint8_t)(mode_manager->transition_table[2][6]));
  TF_copy_u8(&packet[60], (uint8_t)(mode_manager->transition_table[2][7]));
  TF_copy_u8(&packet[61], (uint8_t)(mode_manager->transition_table[3][0]));
  TF_copy_u8(&packet[62], (uint8_t)(mode_manager->transition_table[3][1]));
  TF_copy_u8(&packet[63], (uint8_t)(mode_manager->transition_table[3][2]));
  TF_copy_u8(&packet[64], (uint8_t)(mode_manager->transition_table[3][3]));
  TF_copy_u8(&packet[65], (uint8_t)(mode_manager->transition_table[3][4]));
  TF_copy_u8(&packet[66], (uint8_t)(mode_manager->transition_table[3][5]));
  TF_copy_u8(&packet[67], (uint8_t)(mode_manager->transition_table[3][6]));
  TF_copy_u8(&packet[68], (uint8_t)(mode_manager->transition_table[3][7]));
  TF_copy_u8(&packet[69], (uint8_t)(mode_manager->transition_table[4][0]));
  TF_copy_u8(&packet[70], (uint8_t)(mode_manager->transition_table[4][1]));
  TF_copy_u8(&packet[71], (uint8_t)(mode_manager->transition_table[4][2]));
  TF_copy_u8(&packet[72], (uint8_t)(mode_manager->transition_table[4][3]));
  TF_copy_u8(&packet[73], (uint8_t)(mode_manager->transition_table[4][4]));
  TF_copy_u8(&packet[74], (uint8_t)(mode_manager->transition_table[4][5]));
  TF_copy_u8(&packet[75], (uint8_t)(mode_manager->transition_table[4][6]));
  TF_copy_u8(&packet[76], (uint8_t)(mode_manager->transition_table[4][7]));
  TF_copy_u8(&packet[77], (uint8_t)(mode_manager->transition_table[5][0]));
  TF_copy_u8(&packet[78], (uint8_t)(mode_manager->transition_table[5][1]));
  TF_copy_u8(&packet[79], (uint8_t)(mode_manager->transition_table[5][2]));
  TF_copy_u8(&packet[80], (uint8_t)(mode_manager->transition_table[5][3]));
  TF_copy_u8(&packet[81], (uint8_t)(mode_manager->transition_table[5][4]));
  TF_copy_u8(&packet[82], (uint8_t)(mode_manager->transition_table[5][5]));
  TF_copy_u8(&packet[83], (uint8_t)(mode_manager->transition_table[5][6]));
  TF_copy_u8(&packet[84], (uint8_t)(mode_manager->transition_table[5][7]));
  TF_copy_u8(&packet[85], (uint8_t)(mode_manager->transition_table[6][0]));
  TF_copy_u8(&packet[86], (uint8_t)(mode_manager->transition_table[6][1]));
  TF_copy_u8(&packet[87], (uint8_t)(mode_manager->transition_table[6][2]));
  TF_copy_u8(&packet[88], (uint8_t)(mode_manager->transition_table[6][3]));
  TF_copy_u8(&packet[89], (uint8_t)(mode_manager->transition_table[6][4]));
  TF_copy_u8(&packet[90], (uint8_t)(mode_manager->transition_table[6][5]));
  TF_copy_u8(&packet[91], (uint8_t)(mode_manager->transition_table[6][6]));
  TF_copy_u8(&packet[92], (uint8_t)(mode_manager->transition_table[6][7]));
  TF_copy_u8(&packet[93], (uint8_t)(mode_manager->transition_table[7][0]));
  TF_copy_u8(&packet[94], (uint8_t)(mode_manager->transition_table[7][1]));
  TF_copy_u8(&packet[95], (uint8_t)(mode_manager->transition_table[7][2]));
  TF_copy_u8(&packet[96], (uint8_t)(mode_manager->transition_table[7][3]));
  TF_copy_u8(&packet[97], (uint8_t)(mode_manager->transition_table[7][4]));
  TF_copy_u8(&packet[98], (uint8_t)(mode_manager->transition_table[7][5]));
  TF_copy_u8(&packet[99], (uint8_t)(mode_manager->transition_table[7][6]));
  TF_copy_u8(&packet[100], (uint8_t)(mode_manager->transition_table[7][7]));
#endif

  *len = 101;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_AM_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (219 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(app_manager->page_no));
  TF_copy_u32(&packet[27], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+0].initializer));
  TF_copy_u32(&packet[31], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+0].entry_point));
  TF_copy_u8(&packet[35], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+0].init_duration));
  TF_copy_u8(&packet[36], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+0].prev));
  TF_copy_u8(&packet[37], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+0].min));
  TF_copy_u8(&packet[38], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+0].max));
  TF_copy_u32(&packet[39], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+1].initializer));
  TF_copy_u32(&packet[43], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+1].entry_point));
  TF_copy_u8(&packet[47], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+1].init_duration));
  TF_copy_u8(&packet[48], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+1].prev));
  TF_copy_u8(&packet[49], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+1].min));
  TF_copy_u8(&packet[50], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+1].max));
  TF_copy_u32(&packet[51], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+2].initializer));
  TF_copy_u32(&packet[55], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+2].entry_point));
  TF_copy_u8(&packet[59], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+2].init_duration));
  TF_copy_u8(&packet[60], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+2].prev));
  TF_copy_u8(&packet[61], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+2].min));
  TF_copy_u8(&packet[62], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+2].max));
  TF_copy_u32(&packet[63], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+3].initializer));
  TF_copy_u32(&packet[67], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+3].entry_point));
  TF_copy_u8(&packet[71], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+3].init_duration));
  TF_copy_u8(&packet[72], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+3].prev));
  TF_copy_u8(&packet[73], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+3].min));
  TF_copy_u8(&packet[74], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+3].max));
  TF_copy_u32(&packet[75], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+4].initializer));
  TF_copy_u32(&packet[79], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+4].entry_point));
  TF_copy_u8(&packet[83], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+4].init_duration));
  TF_copy_u8(&packet[84], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+4].prev));
  TF_copy_u8(&packet[85], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+4].min));
  TF_copy_u8(&packet[86], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+4].max));
  TF_copy_u32(&packet[87], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+5].initializer));
  TF_copy_u32(&packet[91], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+5].entry_point));
  TF_copy_u8(&packet[95], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+5].init_duration));
  TF_copy_u8(&packet[96], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+5].prev));
  TF_copy_u8(&packet[97], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+5].min));
  TF_copy_u8(&packet[98], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+5].max));
  TF_copy_u32(&packet[99], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+6].initializer));
  TF_copy_u32(&packet[103], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+6].entry_point));
  TF_copy_u8(&packet[107], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+6].init_duration));
  TF_copy_u8(&packet[108], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+6].prev));
  TF_copy_u8(&packet[109], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+6].min));
  TF_copy_u8(&packet[110], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+6].max));
  TF_copy_u32(&packet[111], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+7].initializer));
  TF_copy_u32(&packet[115], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+7].entry_point));
  TF_copy_u8(&packet[119], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+7].init_duration));
  TF_copy_u8(&packet[120], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+7].prev));
  TF_copy_u8(&packet[121], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+7].min));
  TF_copy_u8(&packet[122], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+7].max));
  TF_copy_u32(&packet[123], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+8].initializer));
  TF_copy_u32(&packet[127], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+8].entry_point));
  TF_copy_u8(&packet[131], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+8].init_duration));
  TF_copy_u8(&packet[132], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+8].prev));
  TF_copy_u8(&packet[133], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+8].min));
  TF_copy_u8(&packet[134], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+8].max));
  TF_copy_u32(&packet[135], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+9].initializer));
  TF_copy_u32(&packet[139], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+9].entry_point));
  TF_copy_u8(&packet[143], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+9].init_duration));
  TF_copy_u8(&packet[144], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+9].prev));
  TF_copy_u8(&packet[145], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+9].min));
  TF_copy_u8(&packet[146], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+9].max));
  TF_copy_u32(&packet[147], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+10].initializer));
  TF_copy_u32(&packet[151], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+10].entry_point));
  TF_copy_u8(&packet[155], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+10].init_duration));
  TF_copy_u8(&packet[156], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+10].prev));
  TF_copy_u8(&packet[157], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+10].min));
  TF_copy_u8(&packet[158], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+10].max));
  TF_copy_u32(&packet[159], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+11].initializer));
  TF_copy_u32(&packet[163], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+11].entry_point));
  TF_copy_u8(&packet[167], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+11].init_duration));
  TF_copy_u8(&packet[168], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+11].prev));
  TF_copy_u8(&packet[169], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+11].min));
  TF_copy_u8(&packet[170], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+11].max));
  TF_copy_u32(&packet[171], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+12].initializer));
  TF_copy_u32(&packet[175], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+12].entry_point));
  TF_copy_u8(&packet[179], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+12].init_duration));
  TF_copy_u8(&packet[180], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+12].prev));
  TF_copy_u8(&packet[181], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+12].min));
  TF_copy_u8(&packet[182], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+12].max));
  TF_copy_u32(&packet[183], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+13].initializer));
  TF_copy_u32(&packet[187], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+13].entry_point));
  TF_copy_u8(&packet[191], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+13].init_duration));
  TF_copy_u8(&packet[192], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+13].prev));
  TF_copy_u8(&packet[193], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+13].min));
  TF_copy_u8(&packet[194], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+13].max));
  TF_copy_u32(&packet[195], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+14].initializer));
  TF_copy_u32(&packet[199], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+14].entry_point));
  TF_copy_u8(&packet[203], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+14].init_duration));
  TF_copy_u8(&packet[204], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+14].prev));
  TF_copy_u8(&packet[205], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+14].min));
  TF_copy_u8(&packet[206], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+14].max));
  TF_copy_u32(&packet[207], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+15].initializer));
  TF_copy_u32(&packet[211], (uint32_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+15].entry_point));
  TF_copy_u8(&packet[215], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+15].init_duration));
  TF_copy_u8(&packet[216], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+15].prev));
  TF_copy_u8(&packet[217], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+15].min));
  TF_copy_u8(&packet[218], (uint8_t)(app_manager->ais[(AM_TLM_PAGE_SIZE*app_manager->page_no)+15].max));
#endif

  *len = 219;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_APP_TIME_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (226 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(app_manager->ais[0].init_duration));
  TF_copy_u8(&packet[27], (uint8_t)(app_manager->ais[0].prev));
  TF_copy_u8(&packet[28], (uint8_t)(app_manager->ais[0].min));
  TF_copy_u8(&packet[29], (uint8_t)(app_manager->ais[0].max));
  TF_copy_u8(&packet[30], (uint8_t)(app_manager->ais[1].init_duration));
  TF_copy_u8(&packet[31], (uint8_t)(app_manager->ais[1].prev));
  TF_copy_u8(&packet[32], (uint8_t)(app_manager->ais[1].min));
  TF_copy_u8(&packet[33], (uint8_t)(app_manager->ais[1].max));
  TF_copy_u8(&packet[34], (uint8_t)(app_manager->ais[2].init_duration));
  TF_copy_u8(&packet[35], (uint8_t)(app_manager->ais[2].prev));
  TF_copy_u8(&packet[36], (uint8_t)(app_manager->ais[2].min));
  TF_copy_u8(&packet[37], (uint8_t)(app_manager->ais[2].max));
  TF_copy_u8(&packet[38], (uint8_t)(app_manager->ais[3].init_duration));
  TF_copy_u8(&packet[39], (uint8_t)(app_manager->ais[3].prev));
  TF_copy_u8(&packet[40], (uint8_t)(app_manager->ais[3].min));
  TF_copy_u8(&packet[41], (uint8_t)(app_manager->ais[3].max));
  TF_copy_u8(&packet[42], (uint8_t)(app_manager->ais[4].init_duration));
  TF_copy_u8(&packet[43], (uint8_t)(app_manager->ais[4].prev));
  TF_copy_u8(&packet[44], (uint8_t)(app_manager->ais[4].min));
  TF_copy_u8(&packet[45], (uint8_t)(app_manager->ais[4].max));
  TF_copy_u8(&packet[46], (uint8_t)(app_manager->ais[5].init_duration));
  TF_copy_u8(&packet[47], (uint8_t)(app_manager->ais[5].prev));
  TF_copy_u8(&packet[48], (uint8_t)(app_manager->ais[5].min));
  TF_copy_u8(&packet[49], (uint8_t)(app_manager->ais[5].max));
  TF_copy_u8(&packet[50], (uint8_t)(app_manager->ais[6].init_duration));
  TF_copy_u8(&packet[51], (uint8_t)(app_manager->ais[6].prev));
  TF_copy_u8(&packet[52], (uint8_t)(app_manager->ais[6].min));
  TF_copy_u8(&packet[53], (uint8_t)(app_manager->ais[6].max));
  TF_copy_u8(&packet[54], (uint8_t)(app_manager->ais[7].init_duration));
  TF_copy_u8(&packet[55], (uint8_t)(app_manager->ais[7].prev));
  TF_copy_u8(&packet[56], (uint8_t)(app_manager->ais[7].min));
  TF_copy_u8(&packet[57], (uint8_t)(app_manager->ais[7].max));
  TF_copy_u8(&packet[58], (uint8_t)(app_manager->ais[8].init_duration));
  TF_copy_u8(&packet[59], (uint8_t)(app_manager->ais[8].prev));
  TF_copy_u8(&packet[60], (uint8_t)(app_manager->ais[8].min));
  TF_copy_u8(&packet[61], (uint8_t)(app_manager->ais[8].max));
  TF_copy_u8(&packet[62], (uint8_t)(app_manager->ais[9].init_duration));
  TF_copy_u8(&packet[63], (uint8_t)(app_manager->ais[9].prev));
  TF_copy_u8(&packet[64], (uint8_t)(app_manager->ais[9].min));
  TF_copy_u8(&packet[65], (uint8_t)(app_manager->ais[9].max));
  TF_copy_u8(&packet[66], (uint8_t)(app_manager->ais[10].init_duration));
  TF_copy_u8(&packet[67], (uint8_t)(app_manager->ais[10].prev));
  TF_copy_u8(&packet[68], (uint8_t)(app_manager->ais[10].min));
  TF_copy_u8(&packet[69], (uint8_t)(app_manager->ais[10].max));
  TF_copy_u8(&packet[70], (uint8_t)(app_manager->ais[11].init_duration));
  TF_copy_u8(&packet[71], (uint8_t)(app_manager->ais[11].prev));
  TF_copy_u8(&packet[72], (uint8_t)(app_manager->ais[11].min));
  TF_copy_u8(&packet[73], (uint8_t)(app_manager->ais[11].max));
  TF_copy_u8(&packet[74], (uint8_t)(app_manager->ais[12].init_duration));
  TF_copy_u8(&packet[75], (uint8_t)(app_manager->ais[12].prev));
  TF_copy_u8(&packet[76], (uint8_t)(app_manager->ais[12].min));
  TF_copy_u8(&packet[77], (uint8_t)(app_manager->ais[12].max));
  TF_copy_u8(&packet[78], (uint8_t)(app_manager->ais[13].init_duration));
  TF_copy_u8(&packet[79], (uint8_t)(app_manager->ais[13].prev));
  TF_copy_u8(&packet[80], (uint8_t)(app_manager->ais[13].min));
  TF_copy_u8(&packet[81], (uint8_t)(app_manager->ais[13].max));
  TF_copy_u8(&packet[82], (uint8_t)(app_manager->ais[14].init_duration));
  TF_copy_u8(&packet[83], (uint8_t)(app_manager->ais[14].prev));
  TF_copy_u8(&packet[84], (uint8_t)(app_manager->ais[14].min));
  TF_copy_u8(&packet[85], (uint8_t)(app_manager->ais[14].max));
  TF_copy_u8(&packet[86], (uint8_t)(app_manager->ais[15].init_duration));
  TF_copy_u8(&packet[87], (uint8_t)(app_manager->ais[15].prev));
  TF_copy_u8(&packet[88], (uint8_t)(app_manager->ais[15].min));
  TF_copy_u8(&packet[89], (uint8_t)(app_manager->ais[15].max));
  TF_copy_u8(&packet[90], (uint8_t)(app_manager->ais[16].init_duration));
  TF_copy_u8(&packet[91], (uint8_t)(app_manager->ais[16].prev));
  TF_copy_u8(&packet[92], (uint8_t)(app_manager->ais[16].min));
  TF_copy_u8(&packet[93], (uint8_t)(app_manager->ais[16].max));
  TF_copy_u8(&packet[94], (uint8_t)(app_manager->ais[17].init_duration));
  TF_copy_u8(&packet[95], (uint8_t)(app_manager->ais[17].prev));
  TF_copy_u8(&packet[96], (uint8_t)(app_manager->ais[17].min));
  TF_copy_u8(&packet[97], (uint8_t)(app_manager->ais[17].max));
  TF_copy_u8(&packet[98], (uint8_t)(app_manager->ais[18].init_duration));
  TF_copy_u8(&packet[99], (uint8_t)(app_manager->ais[18].prev));
  TF_copy_u8(&packet[100], (uint8_t)(app_manager->ais[18].min));
  TF_copy_u8(&packet[101], (uint8_t)(app_manager->ais[18].max));
  TF_copy_u8(&packet[102], (uint8_t)(app_manager->ais[19].init_duration));
  TF_copy_u8(&packet[103], (uint8_t)(app_manager->ais[19].prev));
  TF_copy_u8(&packet[104], (uint8_t)(app_manager->ais[19].min));
  TF_copy_u8(&packet[105], (uint8_t)(app_manager->ais[19].max));
  TF_copy_u8(&packet[106], (uint8_t)(app_manager->ais[20].init_duration));
  TF_copy_u8(&packet[107], (uint8_t)(app_manager->ais[20].prev));
  TF_copy_u8(&packet[108], (uint8_t)(app_manager->ais[20].min));
  TF_copy_u8(&packet[109], (uint8_t)(app_manager->ais[20].max));
  TF_copy_u8(&packet[110], (uint8_t)(app_manager->ais[21].init_duration));
  TF_copy_u8(&packet[111], (uint8_t)(app_manager->ais[21].prev));
  TF_copy_u8(&packet[112], (uint8_t)(app_manager->ais[21].min));
  TF_copy_u8(&packet[113], (uint8_t)(app_manager->ais[21].max));
  TF_copy_u8(&packet[114], (uint8_t)(app_manager->ais[22].init_duration));
  TF_copy_u8(&packet[115], (uint8_t)(app_manager->ais[22].prev));
  TF_copy_u8(&packet[116], (uint8_t)(app_manager->ais[22].min));
  TF_copy_u8(&packet[117], (uint8_t)(app_manager->ais[22].max));
  TF_copy_u8(&packet[118], (uint8_t)(app_manager->ais[23].init_duration));
  TF_copy_u8(&packet[119], (uint8_t)(app_manager->ais[23].prev));
  TF_copy_u8(&packet[120], (uint8_t)(app_manager->ais[23].min));
  TF_copy_u8(&packet[121], (uint8_t)(app_manager->ais[23].max));
  TF_copy_u8(&packet[122], (uint8_t)(app_manager->ais[24].init_duration));
  TF_copy_u8(&packet[123], (uint8_t)(app_manager->ais[24].prev));
  TF_copy_u8(&packet[124], (uint8_t)(app_manager->ais[24].min));
  TF_copy_u8(&packet[125], (uint8_t)(app_manager->ais[24].max));
  TF_copy_u8(&packet[126], (uint8_t)(app_manager->ais[25].init_duration));
  TF_copy_u8(&packet[127], (uint8_t)(app_manager->ais[25].prev));
  TF_copy_u8(&packet[128], (uint8_t)(app_manager->ais[25].min));
  TF_copy_u8(&packet[129], (uint8_t)(app_manager->ais[25].max));
  TF_copy_u8(&packet[130], (uint8_t)(app_manager->ais[26].init_duration));
  TF_copy_u8(&packet[131], (uint8_t)(app_manager->ais[26].prev));
  TF_copy_u8(&packet[132], (uint8_t)(app_manager->ais[26].min));
  TF_copy_u8(&packet[133], (uint8_t)(app_manager->ais[26].max));
  TF_copy_u8(&packet[134], (uint8_t)(app_manager->ais[27].init_duration));
  TF_copy_u8(&packet[135], (uint8_t)(app_manager->ais[27].prev));
  TF_copy_u8(&packet[136], (uint8_t)(app_manager->ais[27].min));
  TF_copy_u8(&packet[137], (uint8_t)(app_manager->ais[27].max));
  TF_copy_u8(&packet[138], (uint8_t)(app_manager->ais[28].init_duration));
  TF_copy_u8(&packet[139], (uint8_t)(app_manager->ais[28].prev));
  TF_copy_u8(&packet[140], (uint8_t)(app_manager->ais[28].min));
  TF_copy_u8(&packet[141], (uint8_t)(app_manager->ais[28].max));
  TF_copy_u8(&packet[142], (uint8_t)(app_manager->ais[29].init_duration));
  TF_copy_u8(&packet[143], (uint8_t)(app_manager->ais[29].prev));
  TF_copy_u8(&packet[144], (uint8_t)(app_manager->ais[29].min));
  TF_copy_u8(&packet[145], (uint8_t)(app_manager->ais[29].max));
  TF_copy_u8(&packet[146], (uint8_t)(app_manager->ais[30].init_duration));
  TF_copy_u8(&packet[147], (uint8_t)(app_manager->ais[30].prev));
  TF_copy_u8(&packet[148], (uint8_t)(app_manager->ais[30].min));
  TF_copy_u8(&packet[149], (uint8_t)(app_manager->ais[30].max));
  TF_copy_u8(&packet[150], (uint8_t)(app_manager->ais[31].init_duration));
  TF_copy_u8(&packet[151], (uint8_t)(app_manager->ais[31].prev));
  TF_copy_u8(&packet[152], (uint8_t)(app_manager->ais[31].min));
  TF_copy_u8(&packet[153], (uint8_t)(app_manager->ais[31].max));
  TF_copy_u8(&packet[154], (uint8_t)(app_manager->ais[32].init_duration));
  TF_copy_u8(&packet[155], (uint8_t)(app_manager->ais[32].prev));
  TF_copy_u8(&packet[156], (uint8_t)(app_manager->ais[32].min));
  TF_copy_u8(&packet[157], (uint8_t)(app_manager->ais[32].max));
  TF_copy_u8(&packet[158], (uint8_t)(app_manager->ais[33].init_duration));
  TF_copy_u8(&packet[159], (uint8_t)(app_manager->ais[33].prev));
  TF_copy_u8(&packet[160], (uint8_t)(app_manager->ais[33].min));
  TF_copy_u8(&packet[161], (uint8_t)(app_manager->ais[33].max));
  TF_copy_u8(&packet[162], (uint8_t)(app_manager->ais[34].init_duration));
  TF_copy_u8(&packet[163], (uint8_t)(app_manager->ais[34].prev));
  TF_copy_u8(&packet[164], (uint8_t)(app_manager->ais[34].min));
  TF_copy_u8(&packet[165], (uint8_t)(app_manager->ais[34].max));
  TF_copy_u8(&packet[166], (uint8_t)(app_manager->ais[35].init_duration));
  TF_copy_u8(&packet[167], (uint8_t)(app_manager->ais[35].prev));
  TF_copy_u8(&packet[168], (uint8_t)(app_manager->ais[35].min));
  TF_copy_u8(&packet[169], (uint8_t)(app_manager->ais[35].max));
  TF_copy_u8(&packet[170], (uint8_t)(app_manager->ais[36].init_duration));
  TF_copy_u8(&packet[171], (uint8_t)(app_manager->ais[36].prev));
  TF_copy_u8(&packet[172], (uint8_t)(app_manager->ais[36].min));
  TF_copy_u8(&packet[173], (uint8_t)(app_manager->ais[36].max));
  TF_copy_u8(&packet[174], (uint8_t)(app_manager->ais[37].init_duration));
  TF_copy_u8(&packet[175], (uint8_t)(app_manager->ais[37].prev));
  TF_copy_u8(&packet[176], (uint8_t)(app_manager->ais[37].min));
  TF_copy_u8(&packet[177], (uint8_t)(app_manager->ais[37].max));
  TF_copy_u8(&packet[178], (uint8_t)(app_manager->ais[38].init_duration));
  TF_copy_u8(&packet[179], (uint8_t)(app_manager->ais[38].prev));
  TF_copy_u8(&packet[180], (uint8_t)(app_manager->ais[38].min));
  TF_copy_u8(&packet[181], (uint8_t)(app_manager->ais[38].max));
  TF_copy_u8(&packet[182], (uint8_t)(app_manager->ais[39].init_duration));
  TF_copy_u8(&packet[183], (uint8_t)(app_manager->ais[39].prev));
  TF_copy_u8(&packet[184], (uint8_t)(app_manager->ais[39].min));
  TF_copy_u8(&packet[185], (uint8_t)(app_manager->ais[39].max));
  TF_copy_u8(&packet[186], (uint8_t)(app_manager->ais[40].init_duration));
  TF_copy_u8(&packet[187], (uint8_t)(app_manager->ais[40].prev));
  TF_copy_u8(&packet[188], (uint8_t)(app_manager->ais[40].min));
  TF_copy_u8(&packet[189], (uint8_t)(app_manager->ais[40].max));
  TF_copy_u8(&packet[190], (uint8_t)(app_manager->ais[41].init_duration));
  TF_copy_u8(&packet[191], (uint8_t)(app_manager->ais[41].prev));
  TF_copy_u8(&packet[192], (uint8_t)(app_manager->ais[41].min));
  TF_copy_u8(&packet[193], (uint8_t)(app_manager->ais[41].max));
  TF_copy_u8(&packet[194], (uint8_t)(app_manager->ais[42].init_duration));
  TF_copy_u8(&packet[195], (uint8_t)(app_manager->ais[42].prev));
  TF_copy_u8(&packet[196], (uint8_t)(app_manager->ais[42].min));
  TF_copy_u8(&packet[197], (uint8_t)(app_manager->ais[42].max));
  TF_copy_u8(&packet[198], (uint8_t)(app_manager->ais[43].init_duration));
  TF_copy_u8(&packet[199], (uint8_t)(app_manager->ais[43].prev));
  TF_copy_u8(&packet[200], (uint8_t)(app_manager->ais[43].min));
  TF_copy_u8(&packet[201], (uint8_t)(app_manager->ais[43].max));
  TF_copy_u8(&packet[202], (uint8_t)(app_manager->ais[44].init_duration));
  TF_copy_u8(&packet[203], (uint8_t)(app_manager->ais[44].prev));
  TF_copy_u8(&packet[204], (uint8_t)(app_manager->ais[44].min));
  TF_copy_u8(&packet[205], (uint8_t)(app_manager->ais[44].max));
  TF_copy_u8(&packet[206], (uint8_t)(app_manager->ais[45].init_duration));
  TF_copy_u8(&packet[207], (uint8_t)(app_manager->ais[45].prev));
  TF_copy_u8(&packet[208], (uint8_t)(app_manager->ais[45].min));
  TF_copy_u8(&packet[209], (uint8_t)(app_manager->ais[45].max));
  TF_copy_u8(&packet[210], (uint8_t)(app_manager->ais[46].init_duration));
  TF_copy_u8(&packet[211], (uint8_t)(app_manager->ais[46].prev));
  TF_copy_u8(&packet[212], (uint8_t)(app_manager->ais[46].min));
  TF_copy_u8(&packet[213], (uint8_t)(app_manager->ais[46].max));
  TF_copy_u8(&packet[214], (uint8_t)(app_manager->ais[47].init_duration));
  TF_copy_u8(&packet[215], (uint8_t)(app_manager->ais[47].prev));
  TF_copy_u8(&packet[216], (uint8_t)(app_manager->ais[47].min));
  TF_copy_u8(&packet[217], (uint8_t)(app_manager->ais[47].max));
  TF_copy_u8(&packet[218], (uint8_t)(app_manager->ais[48].init_duration));
  TF_copy_u8(&packet[219], (uint8_t)(app_manager->ais[48].prev));
  TF_copy_u8(&packet[220], (uint8_t)(app_manager->ais[48].min));
  TF_copy_u8(&packet[221], (uint8_t)(app_manager->ais[48].max));
  TF_copy_u8(&packet[222], (uint8_t)(app_manager->ais[49].init_duration));
  TF_copy_u8(&packet[223], (uint8_t)(app_manager->ais[49].prev));
  TF_copy_u8(&packet[224], (uint8_t)(app_manager->ais[49].min));
  TF_copy_u8(&packet[225], (uint8_t)(app_manager->ais[49].max));
#endif

  *len = 226;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_EL_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (214 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], event_logger->statistics.record_counter_total);
  TF_copy_u32(&packet[30], event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]);
  TF_copy_u32(&packet[38], event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]);
  TF_copy_u32(&packet[42], event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]);
  TF_copy_u32(&packet[46], event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]);
  TF_copy_u8(&packet[50], event_logger->tlm_info.tlog.page_no);
  TF_copy_u8(&packet[51], (uint8_t)event_logger->tlm_info.tlog.err_level);
  TF_copy_u8(&packet[52], event_logger->tlm_info.clog.page_no);
  TF_copy_u8(&packet[53], (uint8_t)event_logger->tlm_info.clog.err_level);
  TF_copy_u32(&packet[54], (uint32_t)event_logger->latest_event.group);
  TF_copy_u32(&packet[58], event_logger->latest_event.local);
  TF_copy_u8(&packet[62], (uint8_t)event_logger->latest_event.err_level);
  TF_copy_u32(&packet[63], event_logger->latest_event.time.total_cycle);
  TF_copy_u8(&packet[67], (uint8_t)event_logger->latest_event.time.step);
  TF_copy_u32(&packet[68], event_logger->latest_event.note);
  TF_copy_u8(&packet[72], event_logger->is_logging_enable[0]);
  TF_copy_u8(&packet[73], event_logger->is_logging_enable[1]);
  TF_copy_u8(&packet[74], event_logger->is_logging_enable[2]);
  TF_copy_u8(&packet[75], event_logger->is_logging_enable[3]);
  TF_copy_u8(&packet[76], event_logger->is_logging_enable[4]);
  TF_copy_u8(&packet[77], event_logger->is_logging_enable[5]);
  TF_copy_u8(&packet[78], event_logger->is_logging_enable[6]);
  TF_copy_u8(&packet[79], event_logger->is_logging_enable[7]);
  TF_copy_u8(&packet[80], event_logger->is_logging_enable[8]);
  TF_copy_u8(&packet[81], event_logger->is_logging_enable[9]);
  TF_copy_u8(&packet[82], event_logger->is_logging_enable[10]);
  TF_copy_u8(&packet[83], event_logger->is_logging_enable[11]);
  TF_copy_u8(&packet[84], event_logger->is_logging_enable[12]);
  TF_copy_u8(&packet[85], event_logger->is_logging_enable[13]);
  TF_copy_u8(&packet[86], event_logger->is_logging_enable[14]);
  TF_copy_u8(&packet[87], event_logger->is_logging_enable[15]);
  TF_copy_u8(&packet[88], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group);
  TF_copy_u32(&packet[89], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local);
  TF_copy_u32(&packet[93], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle);
  TF_copy_u8(&packet[97], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step);
  TF_copy_u32(&packet[98], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note);
  TF_copy_u8(&packet[102], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->group);
  TF_copy_u32(&packet[103], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->local);
  TF_copy_u32(&packet[107], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->time.total_cycle);
  TF_copy_u8(&packet[111], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->time.step);
  TF_copy_u32(&packet[112], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 1)->note);
  TF_copy_u8(&packet[116], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->group);
  TF_copy_u32(&packet[117], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->local);
  TF_copy_u32(&packet[121], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->time.total_cycle);
  TF_copy_u8(&packet[125], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->time.step);
  TF_copy_u32(&packet[126], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 2)->note);
  TF_copy_u8(&packet[130], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->group);
  TF_copy_u32(&packet[131], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->local);
  TF_copy_u32(&packet[135], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->time.total_cycle);
  TF_copy_u8(&packet[139], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->time.step);
  TF_copy_u32(&packet[140], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->note);
  TF_copy_u8(&packet[144], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->group);
  TF_copy_u32(&packet[145], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->local);
  TF_copy_u32(&packet[149], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->time.total_cycle);
  TF_copy_u8(&packet[153], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->time.step);
  TF_copy_u32(&packet[154], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 1)->note);
  TF_copy_u8(&packet[158], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->group);
  TF_copy_u32(&packet[159], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->local);
  TF_copy_u32(&packet[163], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->time.total_cycle);
  TF_copy_u8(&packet[167], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->time.step);
  TF_copy_u32(&packet[168], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 2)->note);
  TF_copy_u8(&packet[172], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->group);
  TF_copy_u32(&packet[173], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->local);
  TF_copy_u32(&packet[177], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->time.total_cycle);
  TF_copy_u8(&packet[181], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->time.step);
  TF_copy_u32(&packet[182], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 0)->note);
  TF_copy_u8(&packet[186], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->group);
  TF_copy_u32(&packet[187], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->local);
  TF_copy_u32(&packet[191], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->time.total_cycle);
  TF_copy_u8(&packet[195], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->time.step);
  TF_copy_u32(&packet[196], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EL, 1)->note);
  TF_copy_u8(&packet[200], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->group);
  TF_copy_u32(&packet[201], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->local);
  TF_copy_u32(&packet[205], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->time.total_cycle);
  TF_copy_u8(&packet[209], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->time.step);
  TF_copy_u32(&packet[210], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_EH, 0)->note);
#endif

  *len = 214;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_EL_TLOG_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  EL_ERROR_LEVEL err_level = event_logger->tlm_info.tlog.err_level;
  uint16_t offset = EL_TLOG_TLM_PAGE_SIZE * event_logger->tlm_info.tlog.page_no;
  const EL_Event* events = event_logger->tlogs[err_level].events;

  if (216 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], event_logger->tlm_info.tlog.page_no);
  TF_copy_u8(&packet[27], (uint8_t)((err_level << 4 & 0xf0) | (event_logger->tlogs[err_level].is_enable_overwrite << 3 & 0x08) | ( event_logger->tlogs[err_level].is_table_overflow << 2 & 0x04)));
  TF_copy_u32(&packet[28], event_logger->statistics.record_counter_total);
  TF_copy_u32(&packet[32], event_logger->statistics.record_counters[err_level]);
  TF_copy_u16(&packet[36], event_logger->tlogs[err_level].log_capacity);
  TF_copy_u16(&packet[38], event_logger->tlogs[err_level].log_wp);
  TF_copy_u8(&packet[40], (uint8_t)events[offset+0].group);
  TF_copy_u32(&packet[41], events[offset+0].local);
  TF_copy_u32(&packet[45], events[offset+0].time.total_cycle);
  TF_copy_u8(&packet[49], (uint8_t)events[offset+0].time.step);
  TF_copy_u8(&packet[50], (uint8_t)events[offset+0].note);
  TF_copy_u8(&packet[51], (uint8_t)events[offset+1].group);
  TF_copy_u32(&packet[52], events[offset+1].local);
  TF_copy_u32(&packet[56], events[offset+1].time.total_cycle);
  TF_copy_u8(&packet[60], (uint8_t)events[offset+1].time.step);
  TF_copy_u8(&packet[61], (uint8_t)events[offset+1].note);
  TF_copy_u8(&packet[62], (uint8_t)events[offset+2].group);
  TF_copy_u32(&packet[63], events[offset+2].local);
  TF_copy_u32(&packet[67], events[offset+2].time.total_cycle);
  TF_copy_u8(&packet[71], (uint8_t)events[offset+2].time.step);
  TF_copy_u8(&packet[72], (uint8_t)events[offset+2].note);
  TF_copy_u8(&packet[73], (uint8_t)events[offset+3].group);
  TF_copy_u32(&packet[74], events[offset+3].local);
  TF_copy_u32(&packet[78], events[offset+3].time.total_cycle);
  TF_copy_u8(&packet[82], (uint8_t)events[offset+3].time.step);
  TF_copy_u8(&packet[83], (uint8_t)events[offset+3].note);
  TF_copy_u8(&packet[84], (uint8_t)events[offset+4].group);
  TF_copy_u32(&packet[85], events[offset+4].local);
  TF_copy_u32(&packet[89], events[offset+4].time.total_cycle);
  TF_copy_u8(&packet[93], (uint8_t)events[offset+4].time.step);
  TF_copy_u8(&packet[94], (uint8_t)events[offset+4].note);
  TF_copy_u8(&packet[95], (uint8_t)events[offset+5].group);
  TF_copy_u32(&packet[96], events[offset+5].local);
  TF_copy_u32(&packet[100], events[offset+5].time.total_cycle);
  TF_copy_u8(&packet[104], (uint8_t)events[offset+5].time.step);
  TF_copy_u8(&packet[105], (uint8_t)events[offset+5].note);
  TF_copy_u8(&packet[106], (uint8_t)events[offset+6].group);
  TF_copy_u32(&packet[107], events[offset+6].local);
  TF_copy_u32(&packet[111], events[offset+6].time.total_cycle);
  TF_copy_u8(&packet[115], (uint8_t)events[offset+6].time.step);
  TF_copy_u8(&packet[116], (uint8_t)events[offset+6].note);
  TF_copy_u8(&packet[117], (uint8_t)events[offset+7].group);
  TF_copy_u32(&packet[118], events[offset+7].local);
  TF_copy_u32(&packet[122], events[offset+7].time.total_cycle);
  TF_copy_u8(&packet[126], (uint8_t)events[offset+7].time.step);
  TF_copy_u8(&packet[127], (uint8_t)events[offset+7].note);
  TF_copy_u8(&packet[128], (uint8_t)events[offset+8].group);
  TF_copy_u32(&packet[129], events[offset+8].local);
  TF_copy_u32(&packet[133], events[offset+8].time.total_cycle);
  TF_copy_u8(&packet[137], (uint8_t)events[offset+8].time.step);
  TF_copy_u8(&packet[138], (uint8_t)events[offset+8].note);
  TF_copy_u8(&packet[139], (uint8_t)events[offset+9].group);
  TF_copy_u32(&packet[140], events[offset+9].local);
  TF_copy_u32(&packet[144], events[offset+9].time.total_cycle);
  TF_copy_u8(&packet[148], (uint8_t)events[offset+9].time.step);
  TF_copy_u8(&packet[149], (uint8_t)events[offset+9].note);
  TF_copy_u8(&packet[150], (uint8_t)events[offset+10].group);
  TF_copy_u32(&packet[151], events[offset+10].local);
  TF_copy_u32(&packet[155], events[offset+10].time.total_cycle);
  TF_copy_u8(&packet[159], (uint8_t)events[offset+10].time.step);
  TF_copy_u8(&packet[160], (uint8_t)events[offset+10].note);
  TF_copy_u8(&packet[161], (uint8_t)events[offset+11].group);
  TF_copy_u32(&packet[162], events[offset+11].local);
  TF_copy_u32(&packet[166], events[offset+11].time.total_cycle);
  TF_copy_u8(&packet[170], (uint8_t)events[offset+11].time.step);
  TF_copy_u8(&packet[171], (uint8_t)events[offset+11].note);
  TF_copy_u8(&packet[172], (uint8_t)events[offset+12].group);
  TF_copy_u32(&packet[173], events[offset+12].local);
  TF_copy_u32(&packet[177], events[offset+12].time.total_cycle);
  TF_copy_u8(&packet[181], (uint8_t)events[offset+12].time.step);
  TF_copy_u8(&packet[182], (uint8_t)events[offset+12].note);
  TF_copy_u8(&packet[183], (uint8_t)events[offset+13].group);
  TF_copy_u32(&packet[184], events[offset+13].local);
  TF_copy_u32(&packet[188], events[offset+13].time.total_cycle);
  TF_copy_u8(&packet[192], (uint8_t)events[offset+13].time.step);
  TF_copy_u8(&packet[193], (uint8_t)events[offset+13].note);
  TF_copy_u8(&packet[194], (uint8_t)events[offset+14].group);
  TF_copy_u32(&packet[195], events[offset+14].local);
  TF_copy_u32(&packet[199], events[offset+14].time.total_cycle);
  TF_copy_u8(&packet[203], (uint8_t)events[offset+14].time.step);
  TF_copy_u8(&packet[204], (uint8_t)events[offset+14].note);
  TF_copy_u8(&packet[205], (uint8_t)events[offset+15].group);
  TF_copy_u32(&packet[206], events[offset+15].local);
  TF_copy_u32(&packet[210], events[offset+15].time.total_cycle);
  TF_copy_u8(&packet[214], (uint8_t)events[offset+15].time.step);
  TF_copy_u8(&packet[215], (uint8_t)events[offset+15].note);
#endif

  *len = 216;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_EL_CLOG_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  EL_ERROR_LEVEL err_level = event_logger->tlm_info.clog.err_level;
  uint16_t offset = EL_CLOG_TLM_PAGE_SIZE * event_logger->tlm_info.clog.page_no;
  const uint16_t* log_orders = event_logger->clogs[err_level].log_orders;
  const EL_CLogElement* logs = event_logger->clogs[err_level].logs;

  if (218 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], event_logger->tlm_info.clog.page_no);
  TF_copy_u8(&packet[27], (uint8_t)err_level);
  TF_copy_u32(&packet[28], event_logger->statistics.record_counter_total);
  TF_copy_u32(&packet[32], event_logger->statistics.record_counters[err_level]);
  TF_copy_u16(&packet[36], event_logger->clogs[err_level].log_capacity);
  TF_copy_u8(&packet[38], (uint8_t)logs[log_orders[offset+0]].event.group);
  TF_copy_u32(&packet[39], logs[log_orders[offset+0]].event.local);
  TF_copy_u32(&packet[43], logs[log_orders[offset+0]].event.time.total_cycle);
  TF_copy_u8(&packet[47], (uint8_t)logs[log_orders[offset+0]].event.time.step);
  TF_copy_u8(&packet[48], (uint8_t)logs[log_orders[offset+0]].event.note);
  TF_copy_u16(&packet[49], logs[log_orders[offset+0]].count);
  TF_copy_u32(&packet[51], logs[log_orders[offset+0]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[55], (uint8_t)logs[log_orders[offset+0]].delta_record_time.step);
  TF_copy_u8(&packet[56], (uint8_t)logs[log_orders[offset+1]].event.group);
  TF_copy_u32(&packet[57], logs[log_orders[offset+1]].event.local);
  TF_copy_u32(&packet[61], logs[log_orders[offset+1]].event.time.total_cycle);
  TF_copy_u8(&packet[65], (uint8_t)logs[log_orders[offset+1]].event.time.step);
  TF_copy_u8(&packet[66], (uint8_t)logs[log_orders[offset+1]].event.note);
  TF_copy_u16(&packet[67], logs[log_orders[offset+1]].count);
  TF_copy_u32(&packet[69], logs[log_orders[offset+1]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[73], (uint8_t)logs[log_orders[offset+1]].delta_record_time.step);
  TF_copy_u8(&packet[74], (uint8_t)logs[log_orders[offset+2]].event.group);
  TF_copy_u32(&packet[75], logs[log_orders[offset+2]].event.local);
  TF_copy_u32(&packet[79], logs[log_orders[offset+2]].event.time.total_cycle);
  TF_copy_u8(&packet[83], (uint8_t)logs[log_orders[offset+2]].event.time.step);
  TF_copy_u8(&packet[84], (uint8_t)logs[log_orders[offset+2]].event.note);
  TF_copy_u16(&packet[85], logs[log_orders[offset+2]].count);
  TF_copy_u32(&packet[87], logs[log_orders[offset+2]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[91], (uint8_t)logs[log_orders[offset+2]].delta_record_time.step);
  TF_copy_u8(&packet[92], (uint8_t)logs[log_orders[offset+3]].event.group);
  TF_copy_u32(&packet[93], logs[log_orders[offset+3]].event.local);
  TF_copy_u32(&packet[97], logs[log_orders[offset+3]].event.time.total_cycle);
  TF_copy_u8(&packet[101], (uint8_t)logs[log_orders[offset+3]].event.time.step);
  TF_copy_u8(&packet[102], (uint8_t)logs[log_orders[offset+3]].event.note);
  TF_copy_u16(&packet[103], logs[log_orders[offset+3]].count);
  TF_copy_u32(&packet[105], logs[log_orders[offset+3]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[109], (uint8_t)logs[log_orders[offset+3]].delta_record_time.step);
  TF_copy_u8(&packet[110], (uint8_t)logs[log_orders[offset+4]].event.group);
  TF_copy_u32(&packet[111], logs[log_orders[offset+4]].event.local);
  TF_copy_u32(&packet[115], logs[log_orders[offset+4]].event.time.total_cycle);
  TF_copy_u8(&packet[119], (uint8_t)logs[log_orders[offset+4]].event.time.step);
  TF_copy_u8(&packet[120], (uint8_t)logs[log_orders[offset+4]].event.note);
  TF_copy_u16(&packet[121], logs[log_orders[offset+4]].count);
  TF_copy_u32(&packet[123], logs[log_orders[offset+4]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[127], (uint8_t)logs[log_orders[offset+4]].delta_record_time.step);
  TF_copy_u8(&packet[128], (uint8_t)logs[log_orders[offset+5]].event.group);
  TF_copy_u32(&packet[129], logs[log_orders[offset+5]].event.local);
  TF_copy_u32(&packet[133], logs[log_orders[offset+5]].event.time.total_cycle);
  TF_copy_u8(&packet[137], (uint8_t)logs[log_orders[offset+5]].event.time.step);
  TF_copy_u8(&packet[138], (uint8_t)logs[log_orders[offset+5]].event.note);
  TF_copy_u16(&packet[139], logs[log_orders[offset+5]].count);
  TF_copy_u32(&packet[141], logs[log_orders[offset+5]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[145], (uint8_t)logs[log_orders[offset+5]].delta_record_time.step);
  TF_copy_u8(&packet[146], (uint8_t)logs[log_orders[offset+6]].event.group);
  TF_copy_u32(&packet[147], logs[log_orders[offset+6]].event.local);
  TF_copy_u32(&packet[151], logs[log_orders[offset+6]].event.time.total_cycle);
  TF_copy_u8(&packet[155], (uint8_t)logs[log_orders[offset+6]].event.time.step);
  TF_copy_u8(&packet[156], (uint8_t)logs[log_orders[offset+6]].event.note);
  TF_copy_u16(&packet[157], logs[log_orders[offset+6]].count);
  TF_copy_u32(&packet[159], logs[log_orders[offset+6]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[163], (uint8_t)logs[log_orders[offset+6]].delta_record_time.step);
  TF_copy_u8(&packet[164], (uint8_t)logs[log_orders[offset+7]].event.group);
  TF_copy_u32(&packet[165], logs[log_orders[offset+7]].event.local);
  TF_copy_u32(&packet[169], logs[log_orders[offset+7]].event.time.total_cycle);
  TF_copy_u8(&packet[173], (uint8_t)logs[log_orders[offset+7]].event.time.step);
  TF_copy_u8(&packet[174], (uint8_t)logs[log_orders[offset+7]].event.note);
  TF_copy_u16(&packet[175], logs[log_orders[offset+7]].count);
  TF_copy_u32(&packet[177], logs[log_orders[offset+7]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[181], (uint8_t)logs[log_orders[offset+7]].delta_record_time.step);
  TF_copy_u8(&packet[182], (uint8_t)logs[log_orders[offset+8]].event.group);
  TF_copy_u32(&packet[183], logs[log_orders[offset+8]].event.local);
  TF_copy_u32(&packet[187], logs[log_orders[offset+8]].event.time.total_cycle);
  TF_copy_u8(&packet[191], (uint8_t)logs[log_orders[offset+8]].event.time.step);
  TF_copy_u8(&packet[192], (uint8_t)logs[log_orders[offset+8]].event.note);
  TF_copy_u16(&packet[193], logs[log_orders[offset+8]].count);
  TF_copy_u32(&packet[195], logs[log_orders[offset+8]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[199], (uint8_t)logs[log_orders[offset+8]].delta_record_time.step);
  TF_copy_u8(&packet[200], (uint8_t)logs[log_orders[offset+9]].event.group);
  TF_copy_u32(&packet[201], logs[log_orders[offset+9]].event.local);
  TF_copy_u32(&packet[205], logs[log_orders[offset+9]].event.time.total_cycle);
  TF_copy_u8(&packet[209], (uint8_t)logs[log_orders[offset+9]].event.time.step);
  TF_copy_u8(&packet[210], (uint8_t)logs[log_orders[offset+9]].event.note);
  TF_copy_u16(&packet[211], logs[log_orders[offset+9]].count);
  TF_copy_u32(&packet[213], logs[log_orders[offset+9]].delta_record_time.total_cycle);
  TF_copy_u8(&packet[217], (uint8_t)logs[log_orders[offset+9]].delta_record_time.step);
#endif

  *len = 218;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_EH_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (101 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], event_utility->is_enabled_eh_execution);
  TF_copy_u32(&packet[27], event_handler->el_event_counter.counter_total);
  TF_copy_u32(&packet[31], event_handler->el_event_counter.counters[EL_ERROR_LEVEL_HIGH]);
  TF_copy_u32(&packet[39], event_handler->el_event_counter.counters[EL_ERROR_LEVEL_LOW]);
  TF_copy_u32(&packet[43], event_handler->el_event_counter.counters[EL_ERROR_LEVEL_EL]);
  TF_copy_u32(&packet[47], event_handler->el_event_counter.counters[EL_ERROR_LEVEL_EH]);
  TF_copy_u32(&packet[51], event_handler->log_table.respond_counter);
  TF_copy_u16(&packet[55], event_handler->log_table.log_wp);
  TF_copy_u8(&packet[57], event_handler->exec_settings.max_response_num);
  TF_copy_u16(&packet[58], event_handler->exec_settings.max_check_event_num);
  TF_copy_u8(&packet[60], event_handler->exec_settings.max_multi_level_num);
  TF_copy_u8(&packet[61], (uint8_t)event_handler->reg_from_cmd.rule_id);
  TF_copy_u8(&packet[62], (uint8_t)event_handler->reg_from_cmd.settings.event.group);
  TF_copy_u32(&packet[63], event_handler->reg_from_cmd.settings.event.local);
  TF_copy_u8(&packet[67], (uint8_t)((((uint8_t)event_handler->reg_from_cmd.settings.event.err_level) << 5 & 0xe0) | (event_handler->reg_from_cmd.settings.should_match_err_level << 4 & 0x10) | (event_handler->reg_from_cmd.settings.is_active << 3 & 0x08) | (((uint8_t)event_handler->reg_from_cmd.settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[68], event_handler->reg_from_cmd.settings.condition.count_threshold);
  TF_copy_u32(&packet[70], event_handler->reg_from_cmd.settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[74], (uint16_t)event_handler->reg_from_cmd.settings.deploy_bct_id);
  TF_copy_u8(&packet[76], (uint8_t)event_handler->reg_from_cmd.register_ack);
  TF_copy_u8(&packet[77], event_handler->tlm_info.rule.page_no);
  TF_copy_u8(&packet[78], (uint8_t)event_handler->tlm_info.rule.target_rule_id);
  TF_copy_u8(&packet[79], event_handler->tlm_info.rule_sorted_index.page_no);
  TF_copy_u8(&packet[80], event_handler->tlm_info.log.page_no);
  TF_copy_u8(&packet[81], (uint8_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.event.group);
  TF_copy_u32(&packet[82], event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.event.local);
  TF_copy_u8(&packet[86], (uint8_t)((((uint8_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.event.err_level) << 5 & 0xe0) | (event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.should_match_err_level << 4 & 0x10) | (event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.is_active << 3 & 0x08) | (((uint8_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[87], event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.condition.count_threshold);
  TF_copy_u32(&packet[89], event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[93], (uint16_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.deploy_bct_id);
  TF_copy_u16(&packet[95], event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].counter);
  TF_copy_u32(&packet[97], (uint32_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].last_event_time.total_cycle);
#endif

  *len = 101;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_EH_RULE_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  uint16_t offset = event_handler->tlm_info.rule.page_no * EH_RULE_TLM_PAGE_SIZE;
  const EH_Rule* rules = event_handler->rule_table.rules;

  if (189 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], event_handler->tlm_info.rule.page_no);
  TF_copy_u16(&packet[27], event_handler->rule_table.registered_rule_num);
  TF_copy_u8(&packet[29], (uint8_t)rules[0 + offset].settings.event.group);
  TF_copy_u32(&packet[30], rules[0 + offset].settings.event.local);
  TF_copy_u8(&packet[34], (uint8_t)((((uint8_t)rules[0 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[0 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[0 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[0 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[35], rules[0 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[37], rules[0 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[41], (uint16_t)rules[0 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[43], rules[0 + offset].counter);
  TF_copy_u32(&packet[45], (uint32_t)rules[0 + offset].last_event_time.total_cycle);
  TF_copy_u8(&packet[49], (uint8_t)rules[1 + offset].settings.event.group);
  TF_copy_u32(&packet[50], rules[1 + offset].settings.event.local);
  TF_copy_u8(&packet[54], (uint8_t)((((uint8_t)rules[1 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[1 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[1 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[1 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[55], rules[1 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[57], rules[1 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[61], (uint16_t)rules[1 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[63], rules[1 + offset].counter);
  TF_copy_u32(&packet[65], (uint32_t)rules[1 + offset].last_event_time.total_cycle);
  TF_copy_u8(&packet[69], (uint8_t)rules[2 + offset].settings.event.group);
  TF_copy_u32(&packet[70], rules[2 + offset].settings.event.local);
  TF_copy_u8(&packet[74], (uint8_t)((((uint8_t)rules[2 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[2 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[2 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[2 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[75], rules[2 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[77], rules[2 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[81], (uint16_t)rules[2 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[83], rules[2 + offset].counter);
  TF_copy_u32(&packet[85], (uint32_t)rules[2 + offset].last_event_time.total_cycle);
  TF_copy_u8(&packet[89], (uint8_t)rules[3 + offset].settings.event.group);
  TF_copy_u32(&packet[90], rules[3 + offset].settings.event.local);
  TF_copy_u8(&packet[94], (uint8_t)((((uint8_t)rules[3 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[3 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[3 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[3 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[95], rules[3 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[97], rules[3 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[101], (uint16_t)rules[3 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[103], rules[3 + offset].counter);
  TF_copy_u32(&packet[105], (uint32_t)rules[3 + offset].last_event_time.total_cycle);
  TF_copy_u8(&packet[109], (uint8_t)rules[4 + offset].settings.event.group);
  TF_copy_u32(&packet[110], rules[4 + offset].settings.event.local);
  TF_copy_u8(&packet[114], (uint8_t)((((uint8_t)rules[4 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[4 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[4 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[4 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[115], rules[4 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[117], rules[4 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[121], (uint16_t)rules[4 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[123], rules[4 + offset].counter);
  TF_copy_u32(&packet[125], (uint32_t)rules[4 + offset].last_event_time.total_cycle);
  TF_copy_u8(&packet[129], (uint8_t)rules[5 + offset].settings.event.group);
  TF_copy_u32(&packet[130], rules[5 + offset].settings.event.local);
  TF_copy_u8(&packet[134], (uint8_t)((((uint8_t)rules[5 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[5 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[5 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[5 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[135], rules[5 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[137], rules[5 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[141], (uint16_t)rules[5 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[143], rules[5 + offset].counter);
  TF_copy_u32(&packet[145], (uint32_t)rules[5 + offset].last_event_time.total_cycle);
  TF_copy_u8(&packet[149], (uint8_t)rules[6 + offset].settings.event.group);
  TF_copy_u32(&packet[150], rules[6 + offset].settings.event.local);
  TF_copy_u8(&packet[154], (uint8_t)((((uint8_t)rules[6 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[6 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[6 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[6 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[155], rules[6 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[157], rules[6 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[161], (uint16_t)rules[6 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[163], rules[6 + offset].counter);
  TF_copy_u32(&packet[165], (uint32_t)rules[6 + offset].last_event_time.total_cycle);
  TF_copy_u8(&packet[169], (uint8_t)rules[7 + offset].settings.event.group);
  TF_copy_u32(&packet[170], rules[7 + offset].settings.event.local);
  TF_copy_u8(&packet[174], (uint8_t)((((uint8_t)rules[7 + offset].settings.event.err_level) << 5 & 0xe0) | (rules[7 + offset].settings.should_match_err_level << 4 & 0x10) | (rules[7 + offset].settings.is_active << 3 & 0x08) | (((uint8_t)rules[7 + offset].settings.condition.type) & 0x07)));
  TF_copy_u16(&packet[175], rules[7 + offset].settings.condition.count_threshold);
  TF_copy_u32(&packet[177], rules[7 + offset].settings.condition.time_threshold_ms);
  TF_copy_u16(&packet[181], (uint16_t)rules[7 + offset].settings.deploy_bct_id);
  TF_copy_u16(&packet[183], rules[7 + offset].counter);
  TF_copy_u32(&packet[185], (uint32_t)rules[7 + offset].last_event_time.total_cycle);
#endif

  *len = 189;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_EH_LOG_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  uint16_t offset = event_handler->tlm_info.log.page_no * EH_LOG_TLM_PAGE_SIZE;

  if (153 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], event_handler->tlm_info.log.page_no);
  TF_copy_u32(&packet[27], event_handler->log_table.respond_counter);
  TF_copy_u16(&packet[31], event_handler->log_table.log_wp);
  TF_copy_u8(&packet[33], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(0 + offset))->rule_id);
  TF_copy_u32(&packet[34], EH_get_the_nth_log_from_the_latest((uint16_t)(0 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[38], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(0 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[39], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(1 + offset))->rule_id);
  TF_copy_u32(&packet[40], EH_get_the_nth_log_from_the_latest((uint16_t)(1 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[44], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(1 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[45], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(2 + offset))->rule_id);
  TF_copy_u32(&packet[46], EH_get_the_nth_log_from_the_latest((uint16_t)(2 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[50], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(2 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[51], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(3 + offset))->rule_id);
  TF_copy_u32(&packet[52], EH_get_the_nth_log_from_the_latest((uint16_t)(3 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[56], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(3 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[57], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(4 + offset))->rule_id);
  TF_copy_u32(&packet[58], EH_get_the_nth_log_from_the_latest((uint16_t)(4 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[62], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(4 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[63], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(5 + offset))->rule_id);
  TF_copy_u32(&packet[64], EH_get_the_nth_log_from_the_latest((uint16_t)(5 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[68], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(5 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[69], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(6 + offset))->rule_id);
  TF_copy_u32(&packet[70], EH_get_the_nth_log_from_the_latest((uint16_t)(6 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[74], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(6 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[75], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(7 + offset))->rule_id);
  TF_copy_u32(&packet[76], EH_get_the_nth_log_from_the_latest((uint16_t)(7 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[80], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(7 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[81], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(8 + offset))->rule_id);
  TF_copy_u32(&packet[82], EH_get_the_nth_log_from_the_latest((uint16_t)(8 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[86], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(8 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[87], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(9 + offset))->rule_id);
  TF_copy_u32(&packet[88], EH_get_the_nth_log_from_the_latest((uint16_t)(9 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[92], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(9 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[93], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(10 + offset))->rule_id);
  TF_copy_u32(&packet[94], EH_get_the_nth_log_from_the_latest((uint16_t)(10 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[98], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(10 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[99], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(11 + offset))->rule_id);
  TF_copy_u32(&packet[100], EH_get_the_nth_log_from_the_latest((uint16_t)(11 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[104], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(11 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[105], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(12 + offset))->rule_id);
  TF_copy_u32(&packet[106], EH_get_the_nth_log_from_the_latest((uint16_t)(12 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[110], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(12 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[111], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(13 + offset))->rule_id);
  TF_copy_u32(&packet[112], EH_get_the_nth_log_from_the_latest((uint16_t)(13 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[116], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(13 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[117], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(14 + offset))->rule_id);
  TF_copy_u32(&packet[118], EH_get_the_nth_log_from_the_latest((uint16_t)(14 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[122], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(14 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[123], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(15 + offset))->rule_id);
  TF_copy_u32(&packet[124], EH_get_the_nth_log_from_the_latest((uint16_t)(15 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[128], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(15 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[129], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(16 + offset))->rule_id);
  TF_copy_u32(&packet[130], EH_get_the_nth_log_from_the_latest((uint16_t)(16 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[134], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(16 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[135], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(17 + offset))->rule_id);
  TF_copy_u32(&packet[136], EH_get_the_nth_log_from_the_latest((uint16_t)(17 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[140], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(17 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[141], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(18 + offset))->rule_id);
  TF_copy_u32(&packet[142], EH_get_the_nth_log_from_the_latest((uint16_t)(18 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[146], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(18 + offset))->deploy_cmd_ack);
  TF_copy_u8(&packet[147], (uint8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(19 + offset))->rule_id);
  TF_copy_u32(&packet[148], EH_get_the_nth_log_from_the_latest((uint16_t)(19 + offset))->respond_time_in_master_cycle);
  TF_copy_i8(&packet[152], (int8_t)EH_get_the_nth_log_from_the_latest((uint16_t)(19 + offset))->deploy_cmd_ack);
#endif

  *len = 153;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_EH_INDEX_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  uint16_t offset = event_handler->tlm_info.rule_sorted_index.page_no * EH_RULE_TLM_PAGE_SIZE;

  if (83 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], event_handler->tlm_info.rule_sorted_index.page_no);
  TF_copy_u8(&packet[27], (uint8_t)event_handler->sorted_idxes[0 + offset].group);
  TF_copy_u32(&packet[28], event_handler->sorted_idxes[0 + offset].local);
  TF_copy_u8(&packet[32], event_handler->sorted_idxes[0 + offset].duplicate_id);
  TF_copy_u8(&packet[33], (uint8_t)event_handler->sorted_idxes[0 + offset].rule_id);
  TF_copy_u8(&packet[34], (uint8_t)event_handler->sorted_idxes[1 + offset].group);
  TF_copy_u32(&packet[35], event_handler->sorted_idxes[1 + offset].local);
  TF_copy_u8(&packet[39], event_handler->sorted_idxes[1 + offset].duplicate_id);
  TF_copy_u8(&packet[40], (uint8_t)event_handler->sorted_idxes[1 + offset].rule_id);
  TF_copy_u8(&packet[41], (uint8_t)event_handler->sorted_idxes[2 + offset].group);
  TF_copy_u32(&packet[42], event_handler->sorted_idxes[2 + offset].local);
  TF_copy_u8(&packet[46], event_handler->sorted_idxes[2 + offset].duplicate_id);
  TF_copy_u8(&packet[47], (uint8_t)event_handler->sorted_idxes[2 + offset].rule_id);
  TF_copy_u8(&packet[48], (uint8_t)event_handler->sorted_idxes[3 + offset].group);
  TF_copy_u32(&packet[49], event_handler->sorted_idxes[3 + offset].local);
  TF_copy_u8(&packet[53], event_handler->sorted_idxes[3 + offset].duplicate_id);
  TF_copy_u8(&packet[54], (uint8_t)event_handler->sorted_idxes[3 + offset].rule_id);
  TF_copy_u8(&packet[55], (uint8_t)event_handler->sorted_idxes[4 + offset].group);
  TF_copy_u32(&packet[56], event_handler->sorted_idxes[4 + offset].local);
  TF_copy_u8(&packet[60], event_handler->sorted_idxes[4 + offset].duplicate_id);
  TF_copy_u8(&packet[61], (uint8_t)event_handler->sorted_idxes[4 + offset].rule_id);
  TF_copy_u8(&packet[62], (uint8_t)event_handler->sorted_idxes[5 + offset].group);
  TF_copy_u32(&packet[63], event_handler->sorted_idxes[5 + offset].local);
  TF_copy_u8(&packet[67], event_handler->sorted_idxes[5 + offset].duplicate_id);
  TF_copy_u8(&packet[68], (uint8_t)event_handler->sorted_idxes[5 + offset].rule_id);
  TF_copy_u8(&packet[69], (uint8_t)event_handler->sorted_idxes[6 + offset].group);
  TF_copy_u32(&packet[70], event_handler->sorted_idxes[6 + offset].local);
  TF_copy_u8(&packet[74], event_handler->sorted_idxes[6 + offset].duplicate_id);
  TF_copy_u8(&packet[75], (uint8_t)event_handler->sorted_idxes[6 + offset].rule_id);
  TF_copy_u8(&packet[76], (uint8_t)event_handler->sorted_idxes[7 + offset].group);
  TF_copy_u32(&packet[77], event_handler->sorted_idxes[7 + offset].local);
  TF_copy_u8(&packet[81], event_handler->sorted_idxes[7 + offset].duplicate_id);
  TF_copy_u8(&packet[82], (uint8_t)event_handler->sorted_idxes[7 + offset].rule_id);
#endif

  *len = 83;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_GIT_REV_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (108 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)GIT_REV_CORE[0]);
  TF_copy_u8(&packet[27], (uint8_t)GIT_REV_CORE[1]);
  TF_copy_u8(&packet[28], (uint8_t)GIT_REV_CORE[2]);
  TF_copy_u8(&packet[29], (uint8_t)GIT_REV_CORE[3]);
  TF_copy_u8(&packet[30], (uint8_t)GIT_REV_CORE[4]);
  TF_copy_u8(&packet[31], (uint8_t)GIT_REV_CORE[5]);
  TF_copy_u8(&packet[32], (uint8_t)GIT_REV_CORE[6]);
  TF_copy_u8(&packet[33], (uint8_t)GIT_REV_CORE[7]);
  TF_copy_u8(&packet[34], (uint8_t)GIT_REV_CORE[8]);
  TF_copy_u8(&packet[35], (uint8_t)GIT_REV_CORE[9]);
  TF_copy_u8(&packet[36], (uint8_t)GIT_REV_CORE[10]);
  TF_copy_u8(&packet[37], (uint8_t)GIT_REV_CORE[11]);
  TF_copy_u8(&packet[38], (uint8_t)GIT_REV_CORE[12]);
  TF_copy_u8(&packet[39], (uint8_t)GIT_REV_CORE[13]);
  TF_copy_u8(&packet[40], (uint8_t)GIT_REV_CORE[14]);
  TF_copy_u8(&packet[41], (uint8_t)GIT_REV_CORE[15]);
  TF_copy_u8(&packet[42], (uint8_t)GIT_REV_CORE[16]);
  TF_copy_u8(&packet[43], (uint8_t)GIT_REV_CORE[17]);
  TF_copy_u8(&packet[44], (uint8_t)GIT_REV_CORE[18]);
  TF_copy_u8(&packet[45], (uint8_t)GIT_REV_CORE[19]);
  TF_copy_u8(&packet[46], (uint8_t)GIT_REV_CORE[20]);
  TF_copy_u8(&packet[47], (uint8_t)GIT_REV_CORE[21]);
  TF_copy_u8(&packet[48], (uint8_t)GIT_REV_CORE[22]);
  TF_copy_u8(&packet[49], (uint8_t)GIT_REV_CORE[23]);
  TF_copy_u8(&packet[50], (uint8_t)GIT_REV_CORE[24]);
  TF_copy_u8(&packet[51], (uint8_t)GIT_REV_CORE[25]);
  TF_copy_u8(&packet[52], (uint8_t)GIT_REV_CORE[26]);
  TF_copy_u8(&packet[53], (uint8_t)GIT_REV_CORE[27]);
  TF_copy_u8(&packet[54], (uint8_t)GIT_REV_CORE[28]);
  TF_copy_u8(&packet[55], (uint8_t)GIT_REV_CORE[29]);
  TF_copy_u8(&packet[56], (uint8_t)GIT_REV_CORE[30]);
  TF_copy_u8(&packet[57], (uint8_t)GIT_REV_CORE[31]);
  TF_copy_u8(&packet[58], (uint8_t)GIT_REV_CORE[32]);
  TF_copy_u8(&packet[59], (uint8_t)GIT_REV_CORE[33]);
  TF_copy_u8(&packet[60], (uint8_t)GIT_REV_CORE[34]);
  TF_copy_u8(&packet[61], (uint8_t)GIT_REV_CORE[35]);
  TF_copy_u8(&packet[62], (uint8_t)GIT_REV_CORE[36]);
  TF_copy_u8(&packet[63], (uint8_t)GIT_REV_CORE[37]);
  TF_copy_u8(&packet[64], (uint8_t)GIT_REV_CORE[38]);
  TF_copy_u8(&packet[65], (uint8_t)GIT_REV_CORE[39]);
  TF_copy_u8(&packet[66], (uint8_t)GIT_REV_CORE[40]);
  TF_copy_u8(&packet[67], (uint8_t)GIT_REV_USER[0]);
  TF_copy_u8(&packet[68], (uint8_t)GIT_REV_USER[1]);
  TF_copy_u8(&packet[69], (uint8_t)GIT_REV_USER[2]);
  TF_copy_u8(&packet[70], (uint8_t)GIT_REV_USER[3]);
  TF_copy_u8(&packet[71], (uint8_t)GIT_REV_USER[4]);
  TF_copy_u8(&packet[72], (uint8_t)GIT_REV_USER[5]);
  TF_copy_u8(&packet[73], (uint8_t)GIT_REV_USER[6]);
  TF_copy_u8(&packet[74], (uint8_t)GIT_REV_USER[7]);
  TF_copy_u8(&packet[75], (uint8_t)GIT_REV_USER[8]);
  TF_copy_u8(&packet[76], (uint8_t)GIT_REV_USER[9]);
  TF_copy_u8(&packet[77], (uint8_t)GIT_REV_USER[10]);
  TF_copy_u8(&packet[78], (uint8_t)GIT_REV_USER[11]);
  TF_copy_u8(&packet[79], (uint8_t)GIT_REV_USER[12]);
  TF_copy_u8(&packet[80], (uint8_t)GIT_REV_USER[13]);
  TF_copy_u8(&packet[81], (uint8_t)GIT_REV_USER[14]);
  TF_copy_u8(&packet[82], (uint8_t)GIT_REV_USER[15]);
  TF_copy_u8(&packet[83], (uint8_t)GIT_REV_USER[16]);
  TF_copy_u8(&packet[84], (uint8_t)GIT_REV_USER[17]);
  TF_copy_u8(&packet[85], (uint8_t)GIT_REV_USER[18]);
  TF_copy_u8(&packet[86], (uint8_t)GIT_REV_USER[19]);
  TF_copy_u8(&packet[87], (uint8_t)GIT_REV_USER[20]);
  TF_copy_u8(&packet[88], (uint8_t)GIT_REV_USER[21]);
  TF_copy_u8(&packet[89], (uint8_t)GIT_REV_USER[22]);
  TF_copy_u8(&packet[90], (uint8_t)GIT_REV_USER[23]);
  TF_copy_u8(&packet[91], (uint8_t)GIT_REV_USER[24]);
  TF_copy_u8(&packet[92], (uint8_t)GIT_REV_USER[25]);
  TF_copy_u8(&packet[93], (uint8_t)GIT_REV_USER[26]);
  TF_copy_u8(&packet[94], (uint8_t)GIT_REV_USER[27]);
  TF_copy_u8(&packet[95], (uint8_t)GIT_REV_USER[28]);
  TF_copy_u8(&packet[96], (uint8_t)GIT_REV_USER[29]);
  TF_copy_u8(&packet[97], (uint8_t)GIT_REV_USER[30]);
  TF_copy_u8(&packet[98], (uint8_t)GIT_REV_USER[31]);
  TF_copy_u8(&packet[99], (uint8_t)GIT_REV_USER[32]);
  TF_copy_u8(&packet[100], (uint8_t)GIT_REV_USER[33]);
  TF_copy_u8(&packet[101], (uint8_t)GIT_REV_USER[34]);
  TF_copy_u8(&packet[102], (uint8_t)GIT_REV_USER[35]);
  TF_copy_u8(&packet[103], (uint8_t)GIT_REV_USER[36]);
  TF_copy_u8(&packet[104], (uint8_t)GIT_REV_USER[37]);
  TF_copy_u8(&packet[105], (uint8_t)GIT_REV_USER[38]);
  TF_copy_u8(&packet[106], (uint8_t)GIT_REV_USER[39]);
  TF_copy_u8(&packet[107], (uint8_t)GIT_REV_USER[40]);
#endif

  *len = 108;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_HK_GEN_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (226 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(TMGR_get_master_clock().mode_cycle));
  TF_copy_u8(&packet[30], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[31], (uint8_t)( ((uint8_t)(mode_manager->stat) << 7 & 0x80) | ((uint8_t)(mode_manager->previous_id) & 0x7F) ));
  TF_copy_u16(&packet[32], task_dispatcher->task_list_id);
  TF_copy_u8(&packet[34], (uint8_t)(PH_gs_cmd_list.executed_nodes_));
  TF_copy_u32(&packet[35], (uint32_t)gs_command_dispatcher->prev.time.total_cycle);
  TF_copy_u16(&packet[39], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_u8(&packet[41], (uint8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
  TF_copy_u8(&packet[42], (uint8_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[43], (uint32_t)gs_command_dispatcher->prev_err.time.total_cycle);
  TF_copy_u16(&packet[47], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_u8(&packet[49], (uint8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
  TF_copy_u16(&packet[50], (uint16_t)(gs_command_dispatcher->prev_err.cmd_ret.err_code));
  TF_copy_u8(&packet[52], (uint8_t)(PH_tl_cmd_list[TLCD_ID_FROM_GS].executed_nodes_));
  TF_copy_u8(&packet[53], (uint8_t)(PH_tl_cmd_list[TLCD_ID_FROM_GS].active_nodes_));
  TF_copy_u32(&packet[54], (uint32_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle);
  TF_copy_u16(&packet[58], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_u8(&packet[60], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[61], (uint32_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle);
  TF_copy_u16(&packet[65], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_u8(&packet[67], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.exec_sts));
  TF_copy_u16(&packet[68], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.err_code));
  TF_copy_u32(&packet[70], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[74], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u8(&packet[76], (uint8_t)(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC].executed_nodes_));
  TF_copy_u8(&packet[77], (uint8_t)(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC].active_nodes_));
  TF_copy_u8(&packet[78], (uint8_t)(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM].executed_nodes_));
  TF_copy_u8(&packet[79], (uint8_t)(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM].active_nodes_));
  TF_copy_u8(&packet[80], (uint8_t)(((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error << 7 & 0x80) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout << 6 & 0x40) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error << 5 & 0x20) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout << 4 & 0x10) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error << 3 & 0x08) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout << 2 & 0x04) | (0 << 2 & 0x02) | (0 << 2 & 0x01) ));
  TF_copy_u32(&packet[81], event_logger->statistics.record_counter_total);
  TF_copy_u16(&packet[85], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]);
  TF_copy_u16(&packet[87], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]);
  TF_copy_u16(&packet[89], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]);
  TF_copy_u16(&packet[91], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]);
  TF_copy_u8(&packet[93], (uint8_t)event_logger->latest_event.err_level);
  TF_copy_u8(&packet[94], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->group);
  TF_copy_u32(&packet[95], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->local);
  TF_copy_u32(&packet[99], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->time.total_cycle);
  TF_copy_u8(&packet[103], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->time.step);
  TF_copy_u32(&packet[104], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0)->note);
  TF_copy_u8(&packet[108], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group);
  TF_copy_u32(&packet[109], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local);
  TF_copy_u32(&packet[113], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle);
  TF_copy_u8(&packet[117], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step);
  TF_copy_u32(&packet[118], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note);
  TF_copy_u32(&packet[122], event_handler->log_table.respond_counter);
  TF_copy_u16(&packet[126], (uint16_t)EH_get_the_nth_log_from_the_latest(0)->rule_id);
  TF_copy_u32(&packet[128], (uint32_t)EH_get_the_nth_log_from_the_latest(0)->respond_time_in_master_cycle);
  TF_copy_u8(&packet[132], (uint8_t)(power_switch_control->switch_state_5v_tlm.byte));
  TF_copy_u8(&packet[133], (uint8_t)(power_switch_control->switch_state_unreg_tlm.byte));
  TF_copy_i16(&packet[134], (int16_t)(ina260_driver[INA260_IDX_PIC]->info.current_raw));
  TF_copy_u16(&packet[136], (uint16_t)(ina260_driver[INA260_IDX_PIC]->info.voltage_raw));
  TF_copy_i16(&packet[138], (int16_t)(ina260_driver[INA260_IDX_STIM210]->info.current_raw));
  TF_copy_u16(&packet[140], (uint16_t)(ina260_driver[INA260_IDX_STIM210]->info.voltage_raw));
  TF_copy_i16(&packet[142], (int16_t)(ina260_driver[INA260_IDX_SAGITTA]->info.current_raw));
  TF_copy_u16(&packet[144], (uint16_t)(ina260_driver[INA260_IDX_SAGITTA]->info.voltage_raw));
  TF_copy_i16(&packet[146], (int16_t)(ina260_driver[INA260_IDX_OEM7600]->info.current_raw));
  TF_copy_u16(&packet[148], (uint16_t)(ina260_driver[INA260_IDX_OEM7600]->info.voltage_raw));
  TF_copy_i16(&packet[150], (int16_t)(ina260_driver[INA260_IDX_RM3100]->info.current_raw));
  TF_copy_u16(&packet[152], (uint16_t)(ina260_driver[INA260_IDX_RM3100]->info.voltage_raw));
  TF_copy_i16(&packet[154], (int16_t)(ina260_driver[INA260_IDX_NANOSSOC_D60]->info.current_raw));
  TF_copy_u16(&packet[156], (uint16_t)(ina260_driver[INA260_IDX_NANOSSOC_D60]->info.voltage_raw));
  TF_copy_i16(&packet[158], (int16_t)(ina260_driver[INA260_IDX_MTQ]->info.current_raw));
  TF_copy_u16(&packet[160], (uint16_t)(ina260_driver[INA260_IDX_MTQ]->info.voltage_raw));
  TF_copy_i16(&packet[162], (int16_t)(ina260_driver[INA260_IDX_RW0003_X]->info.current_raw));
  TF_copy_u16(&packet[164], (uint16_t)(ina260_driver[INA260_IDX_RW0003_X]->info.voltage_raw));
  TF_copy_i16(&packet[166], (int16_t)(ina260_driver[INA260_IDX_RW0003_Y]->info.current_raw));
  TF_copy_u16(&packet[168], (uint16_t)(ina260_driver[INA260_IDX_RW0003_Y]->info.voltage_raw));
  TF_copy_i16(&packet[170], (int16_t)(ina260_driver[INA260_IDX_RW0003_Z]->info.current_raw));
  TF_copy_u16(&packet[172], (uint16_t)(ina260_driver[INA260_IDX_RW0003_Z]->info.voltage_raw));
  TF_copy_float(&packet[174], (float)(thermo_sensor->temperature_degC[APP_TSNS_IDX_PIC]));
  TF_copy_float(&packet[178], (float)(thermo_sensor->temperature_degC[APP_TSNS_IDX_RW_DCDC]));
  TF_copy_float(&packet[182], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.temperature_degC));
  TF_copy_float(&packet[186], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.temperature_compo_degC[0]));
  TF_copy_float(&packet[190], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.temperature_compo_degC[1]));
  TF_copy_float(&packet[194], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.temperature_compo_degC[2]));
  TF_copy_float(&packet[198], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.temperature.mcu_degC));
  TF_copy_float(&packet[202], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.temperature.fpga_degC));
  TF_copy_float(&packet[206], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.temperature.cmos_degC));
  TF_copy_float(&packet[210], (float)(rw0003_driver[RW0003_IDX_ON_X]->info.temperature_degC));
  TF_copy_float(&packet[214], (float)(rw0003_driver[RW0003_IDX_ON_Y]->info.temperature_degC));
  TF_copy_float(&packet[218], (float)(rw0003_driver[RW0003_IDX_ON_Z]->info.temperature_degC));
  TF_copy_float(&packet[222], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.hardware_status.temperature_degC));
#endif

  *len = 226;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_HK_COMPO_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (218 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_body_rad_s[0]));
  TF_copy_float(&packet[30], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_body_rad_s[1]));
  TF_copy_float(&packet[34], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_body_rad_s[2]));
  TF_copy_float(&packet[38], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_body_nT[0]));
  TF_copy_float(&packet[42], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_body_nT[1]));
  TF_copy_float(&packet[46], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_body_nT[2]));
  TF_copy_float(&packet[50], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_body_nT[0]));
  TF_copy_float(&packet[54], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_body_nT[1]));
  TF_copy_float(&packet[58], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_body_nT[2]));
  TF_copy_float(&packet[62], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_body_nT[0]));
  TF_copy_float(&packet[66], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_body_nT[1]));
  TF_copy_float(&packet[70], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_body_nT[2]));
  TF_copy_u8(&packet[74], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.operation_mode));
  TF_copy_float(&packet[75], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_body_rad_sec[0]));
  TF_copy_float(&packet[79], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_body_rad_sec[1]));
  TF_copy_float(&packet[83], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_body_rad_sec[2]));
  TF_copy_float(&packet[87], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.sun_intensity_percent));
  TF_copy_u8(&packet[91], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.err_code));
  TF_copy_float(&packet[92], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.sun_intensity_percent));
  TF_copy_u8(&packet[96], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.err_code));
  TF_copy_float(&packet[97], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.sun_intensity_percent));
  TF_copy_u8(&packet[101], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.err_code));
  TF_copy_float(&packet[102], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.sun_intensity_percent));
  TF_copy_u8(&packet[106], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.err_code));
  TF_copy_float(&packet[107], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.sun_intensity_percent));
  TF_copy_u8(&packet[111], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.err_code));
  TF_copy_float(&packet[112], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.sun_intensity_percent));
  TF_copy_u8(&packet[116], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.err_code));
  TF_copy_u32(&packet[117], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.unix_time_ms));
  TF_copy_u8(&packet[121], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.is_valid_quaternion));
  TF_copy_u8(&packet[122], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.num_stars_centroided));
  TF_copy_u8(&packet[123], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.num_stars_matched));
  TF_copy_u32(&packet[124], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.stable_count));
  TF_copy_float(&packet[128], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.quaternion_i2b.vector_part[0]));
  TF_copy_float(&packet[132], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.quaternion_i2b.vector_part[1]));
  TF_copy_float(&packet[136], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.quaternion_i2b.vector_part[2]));
  TF_copy_float(&packet[140], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.quaternion_i2b.scalar_part));
  TF_copy_u16(&packet[144], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.times.gps_time_week));
  TF_copy_u32(&packet[146], (uint32_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.times.gps_time_ms));
  TF_copy_u32(&packet[150], (uint32_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.times.tlm_received_aobc_time));
  TF_copy_u8(&packet[154], (uint8_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.num_of_visible_sats));
  TF_copy_double(&packet[155], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.pos_antenna_ecef_m[0]));
  TF_copy_double(&packet[163], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.pos_antenna_ecef_m[1]));
  TF_copy_double(&packet[171], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.pos_antenna_ecef_m[2]));
  TF_copy_double(&packet[179], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.vel_antenna_ecef_m_s[0]));
  TF_copy_double(&packet[187], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.vel_antenna_ecef_m_s[1]));
  TF_copy_double(&packet[195], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.vel_antenna_ecef_m_s[2]));
  TF_copy_float(&packet[203], (float)(rw0003_driver[RW0003_IDX_ON_X]->info.speed_rad_s));
  TF_copy_float(&packet[207], (float)(rw0003_driver[RW0003_IDX_ON_Y]->info.speed_rad_s));
  TF_copy_float(&packet[211], (float)(rw0003_driver[RW0003_IDX_ON_Z]->info.speed_rad_s));
  TF_copy_i8(&packet[215], (int8_t)(mtq_seiren_controller->mtq_output_duty[MTQ_SEIREN_IDX_X]));
  TF_copy_i8(&packet[216], (int8_t)(mtq_seiren_controller->mtq_output_duty[MTQ_SEIREN_IDX_Y]));
  TF_copy_i8(&packet[217], (int8_t)(mtq_seiren_controller->mtq_output_duty[MTQ_SEIREN_IDX_Z]));
#endif

  *len = 218;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_HK_ALGO_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (225 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(aocs_manager->ang_vel_est_body_rad_s[0]));
  TF_copy_float(&packet[30], (float)(aocs_manager->ang_vel_est_body_rad_s[1]));
  TF_copy_float(&packet[34], (float)(aocs_manager->ang_vel_est_body_rad_s[2]));
  TF_copy_float(&packet[38], (float)(aocs_manager->ang_vel_target_body_rad_s[0]));
  TF_copy_float(&packet[42], (float)(aocs_manager->ang_vel_target_body_rad_s[1]));
  TF_copy_float(&packet[46], (float)(aocs_manager->ang_vel_target_body_rad_s[2]));
  TF_copy_float(&packet[50], (float)(aocs_manager->quaternion_est_i2b.vector_part[0]));
  TF_copy_float(&packet[54], (float)(aocs_manager->quaternion_est_i2b.vector_part[1]));
  TF_copy_float(&packet[58], (float)(aocs_manager->quaternion_est_i2b.vector_part[2]));
  TF_copy_float(&packet[62], (float)(aocs_manager->quaternion_est_i2b.scalar_part));
  TF_copy_u8(&packet[66], (uint8_t)(quaternion_interpolator->index));
  TF_copy_float(&packet[67], (float)(aocs_manager->quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[71], (float)(aocs_manager->quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[75], (float)(aocs_manager->quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[79], (float)(aocs_manager->quaternion_target_i2t.scalar_part));
  TF_copy_float(&packet[83], (float)(aocs_manager->sun_vec_est_eci[0]));
  TF_copy_float(&packet[87], (float)(aocs_manager->sun_vec_est_eci[1]));
  TF_copy_float(&packet[91], (float)(aocs_manager->sun_vec_est_eci[2]));
  TF_copy_float(&packet[95], (float)(aocs_manager->sun_vec_est_body[0]));
  TF_copy_float(&packet[99], (float)(aocs_manager->sun_vec_est_body[1]));
  TF_copy_float(&packet[103], (float)(aocs_manager->sun_vec_est_body[2]));
  TF_copy_float(&packet[107], (float)(aocs_manager->sun_vec_target_body[0]));
  TF_copy_float(&packet[111], (float)(aocs_manager->sun_vec_target_body[1]));
  TF_copy_float(&packet[115], (float)(aocs_manager->sun_vec_target_body[2]));
  TF_copy_float(&packet[119], (float)(aocs_manager->mag_vec_est_eci_nT[0]));
  TF_copy_float(&packet[123], (float)(aocs_manager->mag_vec_est_eci_nT[1]));
  TF_copy_float(&packet[127], (float)(aocs_manager->mag_vec_est_eci_nT[2]));
  TF_copy_float(&packet[131], (float)(aocs_manager->mag_vec_est_body_nT[0]));
  TF_copy_float(&packet[135], (float)(aocs_manager->mag_vec_est_body_nT[1]));
  TF_copy_float(&packet[139], (float)(aocs_manager->mag_vec_est_body_nT[2]));
  TF_copy_float(&packet[143], (float)(aocs_manager->internal_torque_target_body_Nm[0]));
  TF_copy_float(&packet[147], (float)(aocs_manager->internal_torque_target_body_Nm[1]));
  TF_copy_float(&packet[151], (float)(aocs_manager->internal_torque_target_body_Nm[2]));
  TF_copy_float(&packet[155], (float)(aocs_manager->external_torque_target_body_Nm[0]));
  TF_copy_float(&packet[159], (float)(aocs_manager->external_torque_target_body_Nm[1]));
  TF_copy_float(&packet[163], (float)(aocs_manager->external_torque_target_body_Nm[2]));
  TF_copy_double(&packet[167], (double)(aocs_manager->reference_jday));
  TF_copy_double(&packet[175], (double)(aocs_manager->sat_pos_est_ecef_m[0]));
  TF_copy_double(&packet[183], (double)(aocs_manager->sat_pos_est_ecef_m[1]));
  TF_copy_double(&packet[191], (double)(aocs_manager->sat_pos_est_ecef_m[2]));
  TF_copy_double(&packet[199], (double)(aocs_manager->sat_vel_est_ecef_m_s[0]));
  TF_copy_double(&packet[207], (double)(aocs_manager->sat_vel_est_ecef_m_s[1]));
  TF_copy_double(&packet[215], (double)(aocs_manager->sat_vel_est_ecef_m_s[2]));
  TF_copy_u8(&packet[223], (uint8_t)(aocs_mode_manager->bdot_retry_count));
  TF_copy_u8(&packet[224], (uint8_t)(aocs_manager->flags_for_tlm));
#endif

  *len = 225;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_COMPONENTS_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (200 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.accel_compo_m_s2[0]));
  TF_copy_float(&packet[30], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.accel_compo_m_s2[1]));
  TF_copy_float(&packet[34], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.accel_compo_m_s2[2]));
  TF_copy_float(&packet[38], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.sun_angle_compo_deg[0]));
  TF_copy_float(&packet[42], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.sun_angle_compo_deg[1]));
  TF_copy_float(&packet[46], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.sun_vec_body[0]));
  TF_copy_float(&packet[50], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.sun_vec_body[1]));
  TF_copy_float(&packet[54], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.sun_vec_body[2]));
  TF_copy_float(&packet[58], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.sun_angle_compo_deg[0]));
  TF_copy_float(&packet[62], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.sun_angle_compo_deg[1]));
  TF_copy_float(&packet[66], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.sun_vec_body[0]));
  TF_copy_float(&packet[70], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.sun_vec_body[1]));
  TF_copy_float(&packet[74], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.sun_vec_body[2]));
  TF_copy_float(&packet[78], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.sun_angle_compo_deg[0]));
  TF_copy_float(&packet[82], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.sun_angle_compo_deg[1]));
  TF_copy_float(&packet[86], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.sun_vec_body[0]));
  TF_copy_float(&packet[90], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.sun_vec_body[1]));
  TF_copy_float(&packet[94], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.sun_vec_body[2]));
  TF_copy_float(&packet[98], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.sun_angle_compo_deg[0]));
  TF_copy_float(&packet[102], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.sun_angle_compo_deg[1]));
  TF_copy_float(&packet[106], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.sun_vec_body[0]));
  TF_copy_float(&packet[110], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.sun_vec_body[1]));
  TF_copy_float(&packet[114], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.sun_vec_body[2]));
  TF_copy_float(&packet[118], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.sun_angle_compo_deg[0]));
  TF_copy_float(&packet[122], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.sun_angle_compo_deg[1]));
  TF_copy_float(&packet[126], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.sun_vec_body[0]));
  TF_copy_float(&packet[130], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.sun_vec_body[1]));
  TF_copy_float(&packet[134], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.sun_vec_body[2]));
  TF_copy_float(&packet[138], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.sun_angle_compo_deg[0]));
  TF_copy_float(&packet[142], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.sun_angle_compo_deg[1]));
  TF_copy_float(&packet[146], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.sun_vec_body[0]));
  TF_copy_float(&packet[150], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.sun_vec_body[1]));
  TF_copy_float(&packet[154], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.sun_vec_body[2]));
  TF_copy_u8(&packet[158], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.counter));
  TF_copy_u16(&packet[159], (uint16_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.latency_sec));
  TF_copy_u8(&packet[161], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.normal_mode_format));
  TF_copy_u8(&packet[162], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.gyro_output_mode));
  TF_copy_u8(&packet[163], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.termination_mode));
  TF_copy_u8(&packet[164], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.low_pass_filter_frequency));
  TF_copy_u8(&packet[165], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.sample_rate));
  TF_copy_u8(&packet[166], (uint8_t)(stim210_driver[STIM210_IDX_IN_UNIT]->info.status));
  TF_copy_u32(&packet[167], (uint32_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.uart_baudrate));
  TF_copy_float(&packet[171], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.vector_part[0]));
  TF_copy_float(&packet[175], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.vector_part[1]));
  TF_copy_float(&packet[179], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.vector_part[2]));
  TF_copy_float(&packet[183], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.scalar_part));
  TF_copy_u16(&packet[187], (uint16_t)(magnetic_exclusive_control_timer->config.observe_duration_ms));
  TF_copy_u16(&packet[189], (uint16_t)(magnetic_exclusive_control_timer->config.control_duration_ms));
  TF_copy_u16(&packet[191], (uint16_t)(magnetic_exclusive_control_timer->config.standby_duration_ms));
  TF_copy_u16(&packet[193], (uint16_t)(magnetic_exclusive_control_timer->buffered_config.observe_duration_ms));
  TF_copy_u16(&packet[195], (uint16_t)(magnetic_exclusive_control_timer->buffered_config.control_duration_ms));
  TF_copy_u16(&packet[197], (uint16_t)(magnetic_exclusive_control_timer->buffered_config.standby_duration_ms));
  TF_copy_u8(&packet[199], (uint8_t)(magnetic_exclusive_control_timer->is_enable));
#endif

  *len = 200;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_POWER_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (137 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(power_switch_control->switch_state_5v[APP_PSC_5V_IDX_INA260]));
  TF_copy_u8(&packet[27], (uint8_t)(power_switch_control->switch_state_5v[APP_PSC_5V_IDX_MPU9250]));
  TF_copy_u8(&packet[28], (uint8_t)(power_switch_control->switch_state_5v[APP_PSC_5V_IDX_RM3100]));
  TF_copy_u8(&packet[29], (uint8_t)(power_switch_control->switch_state_5v[APP_PSC_5V_IDX_NANOSSOC_D60]));
  TF_copy_u8(&packet[30], (uint8_t)(power_switch_control->switch_state_5v[APP_PSC_5V_IDX_MTQ_SEIREN]));
  TF_copy_u8(&packet[31], (uint8_t)(power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_STIM210]));
  TF_copy_u8(&packet[32], (uint8_t)(power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_SAGITTA]));
  TF_copy_u8(&packet[33], (uint8_t)(power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_OEM7600]));
  TF_copy_u8(&packet[34], (uint8_t)(power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWX]));
  TF_copy_u8(&packet[35], (uint8_t)(power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWY]));
  TF_copy_u8(&packet[36], (uint8_t)(power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWZ]));
  TF_copy_i16(&packet[37], (int16_t)(ina260_driver[INA260_IDX_PIC]->info.current_raw));
  TF_copy_u16(&packet[39], (uint16_t)(ina260_driver[INA260_IDX_PIC]->info.voltage_raw));
  TF_copy_u16(&packet[41], (uint16_t)(ina260_driver[INA260_IDX_PIC]->info.oc_threshold_raw));
  TF_copy_u16(&packet[43], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_PIC]));
  TF_copy_u16(&packet[45], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_PIC]));
  TF_copy_i16(&packet[47], (int16_t)(ina260_driver[INA260_IDX_STIM210]->info.current_raw));
  TF_copy_u16(&packet[49], (uint16_t)(ina260_driver[INA260_IDX_STIM210]->info.voltage_raw));
  TF_copy_u16(&packet[51], (uint16_t)(ina260_driver[INA260_IDX_STIM210]->info.oc_threshold_raw));
  TF_copy_u16(&packet[53], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_STIM210]));
  TF_copy_u16(&packet[55], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_STIM210]));
  TF_copy_i16(&packet[57], (int16_t)(ina260_driver[INA260_IDX_SAGITTA]->info.current_raw));
  TF_copy_u16(&packet[59], (uint16_t)(ina260_driver[INA260_IDX_SAGITTA]->info.voltage_raw));
  TF_copy_u16(&packet[61], (uint16_t)(ina260_driver[INA260_IDX_SAGITTA]->info.oc_threshold_raw));
  TF_copy_u16(&packet[63], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_SAGITTA]));
  TF_copy_u16(&packet[65], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_SAGITTA]));
  TF_copy_i16(&packet[67], (int16_t)(ina260_driver[INA260_IDX_OEM7600]->info.current_raw));
  TF_copy_u16(&packet[69], (uint16_t)(ina260_driver[INA260_IDX_OEM7600]->info.voltage_raw));
  TF_copy_u16(&packet[71], (uint16_t)(ina260_driver[INA260_IDX_OEM7600]->info.oc_threshold_raw));
  TF_copy_u16(&packet[73], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_OEM7600]));
  TF_copy_u16(&packet[75], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_OEM7600]));
  TF_copy_i16(&packet[77], (int16_t)(ina260_driver[INA260_IDX_RM3100]->info.current_raw));
  TF_copy_u16(&packet[79], (uint16_t)(ina260_driver[INA260_IDX_RM3100]->info.voltage_raw));
  TF_copy_u16(&packet[81], (uint16_t)(ina260_driver[INA260_IDX_RM3100]->info.oc_threshold_raw));
  TF_copy_u16(&packet[83], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_RM3100]));
  TF_copy_u16(&packet[85], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_RM3100]));
  TF_copy_i16(&packet[87], (int16_t)(ina260_driver[INA260_IDX_NANOSSOC_D60]->info.current_raw));
  TF_copy_u16(&packet[89], (uint16_t)(ina260_driver[INA260_IDX_NANOSSOC_D60]->info.voltage_raw));
  TF_copy_u16(&packet[91], (uint16_t)(ina260_driver[INA260_IDX_NANOSSOC_D60]->info.oc_threshold_raw));
  TF_copy_u16(&packet[93], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_NANOSSOC_D60]));
  TF_copy_u16(&packet[95], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_NANOSSOC_D60]));
  TF_copy_i16(&packet[97], (int16_t)(ina260_driver[INA260_IDX_MTQ]->info.current_raw));
  TF_copy_u16(&packet[99], (uint16_t)(ina260_driver[INA260_IDX_MTQ]->info.voltage_raw));
  TF_copy_u16(&packet[101], (uint16_t)(ina260_driver[INA260_IDX_MTQ]->info.oc_threshold_raw));
  TF_copy_u16(&packet[103], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_MTQ]));
  TF_copy_u16(&packet[105], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_MTQ]));
  TF_copy_i16(&packet[107], (int16_t)(ina260_driver[INA260_IDX_RW0003_X]->info.current_raw));
  TF_copy_u16(&packet[109], (uint16_t)(ina260_driver[INA260_IDX_RW0003_X]->info.voltage_raw));
  TF_copy_u16(&packet[111], (uint16_t)(ina260_driver[INA260_IDX_RW0003_X]->info.oc_threshold_raw));
  TF_copy_u16(&packet[113], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_RW0003_X]));
  TF_copy_u16(&packet[115], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_RW0003_X]));
  TF_copy_i16(&packet[117], (int16_t)(ina260_driver[INA260_IDX_RW0003_Y]->info.current_raw));
  TF_copy_u16(&packet[119], (uint16_t)(ina260_driver[INA260_IDX_RW0003_Y]->info.voltage_raw));
  TF_copy_u16(&packet[121], (uint16_t)(ina260_driver[INA260_IDX_RW0003_Y]->info.oc_threshold_raw));
  TF_copy_u16(&packet[123], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_RW0003_Y]));
  TF_copy_u16(&packet[125], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_RW0003_Y]));
  TF_copy_i16(&packet[127], (int16_t)(ina260_driver[INA260_IDX_RW0003_Z]->info.current_raw));
  TF_copy_u16(&packet[129], (uint16_t)(ina260_driver[INA260_IDX_RW0003_Z]->info.voltage_raw));
  TF_copy_u16(&packet[131], (uint16_t)(ina260_driver[INA260_IDX_RW0003_Z]->info.oc_threshold_raw));
  TF_copy_u16(&packet[133], (uint16_t)(current_anomaly->hw_oc_event_logger_threshold_V[INA260_IDX_RW0003_Z]));
  TF_copy_u16(&packet[135], (uint16_t)(current_anomaly->sw_oc_threshold_mA[INA260_IDX_RW0003_Z]));
#endif

  *len = 137;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_FRAME_TRANSFORMATION_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (202 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[30], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[34], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[38], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[42], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[46], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[50], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[54], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[58], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[62], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[66], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[70], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[74], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[78], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[82], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[86], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[90], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[94], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[98], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[102], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[106], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[110], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[114], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[118], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_0]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[122], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[126], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[130], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[134], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_1]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[138], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[142], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[146], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[150], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_2]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[154], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[158], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[162], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[166], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_3]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[170], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[174], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[178], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[182], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_4]->info.frame_transform_c2b.scalar_part));
  TF_copy_float(&packet[186], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.frame_transform_c2b.vector_part[0]));
  TF_copy_float(&packet[190], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.frame_transform_c2b.vector_part[1]));
  TF_copy_float(&packet[194], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.frame_transform_c2b.vector_part[2]));
  TF_copy_float(&packet[198], (float)(nanossoc_d60_driver[NANOSSOC_D60_IDX_5]->info.frame_transform_c2b.scalar_part));
#endif

  *len = 202;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_CONTROL_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (241 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(bdot->control_gain[0]));
  TF_copy_float(&packet[30], (float)(bdot->control_gain[1]));
  TF_copy_float(&packet[34], (float)(bdot->control_gain[2]));
  TF_copy_u32(&packet[38], (uint32_t)(bdot->minimum_time_derivative_step_ms));
  TF_copy_u8(&packet[42], (uint8_t)(magnetometer_selector->state));
  TF_copy_u8(&packet[43], (uint8_t)(magnetometer_selector->auto_flag));
  TF_copy_u8(&packet[44], (uint8_t)(gyro_selector->state));
  TF_copy_u8(&packet[45], (uint8_t)(gyro_selector->auto_flag));
  TF_copy_float(&packet[46], (float)(sun_sensor_selector->sun_intensity_upper_threshold_percent));
  TF_copy_float(&packet[50], (float)(sun_sensor_selector->sun_intensity_lower_threshold_percent));
  TF_copy_u8(&packet[54], (uint8_t)(rough_three_axis_determination->method));
  TF_copy_u8(&packet[55], (uint8_t)(rough_three_axis_determination->sun_invisible_propagation));
  TF_copy_float(&packet[56], (float)(rough_three_axis_determination->q_method_info.sun_vec_weight));
  TF_copy_float(&packet[60], (float)(rough_three_axis_determination->q_method_info.mag_vec_weight));
  TF_copy_u8(&packet[64], (uint8_t)(fine_three_axis_determination->method));
  TF_copy_float(&packet[65], (float)(aocs_manager->ang_vel_error_body_rad_s[0]));
  TF_copy_float(&packet[69], (float)(aocs_manager->ang_vel_error_body_rad_s[1]));
  TF_copy_float(&packet[73], (float)(aocs_manager->ang_vel_error_body_rad_s[2]));
  TF_copy_float(&packet[77], (float)(aocs_manager->quaternion_error_b2t.vector_part[0]));
  TF_copy_float(&packet[81], (float)(aocs_manager->quaternion_error_b2t.vector_part[1]));
  TF_copy_float(&packet[85], (float)(aocs_manager->quaternion_error_b2t.vector_part[2]));
  TF_copy_float(&packet[89], (float)(aocs_manager->quaternion_error_b2t.scalar_part));
  TF_copy_float(&packet[93], (float)(aocs_manager->sun_vec_error_rad));
  TF_copy_float(&packet[97], (float)(unloading->control_gain));
  TF_copy_u8(&packet[101], (uint8_t)(unloading->exec_is_enable));
  TF_copy_float(&packet[102], (float)(unloading->angular_velocity_upper_threshold_rad_s[0]));
  TF_copy_float(&packet[106], (float)(unloading->angular_velocity_target_rad_s[0]));
  TF_copy_float(&packet[110], (float)(unloading->angular_velocity_lower_threshold_rad_s[0]));
  TF_copy_float(&packet[114], (float)(unloading->angular_velocity_upper_threshold_rad_s[1]));
  TF_copy_float(&packet[118], (float)(unloading->angular_velocity_target_rad_s[1]));
  TF_copy_float(&packet[122], (float)(unloading->angular_velocity_lower_threshold_rad_s[1]));
  TF_copy_float(&packet[126], (float)(unloading->angular_velocity_upper_threshold_rad_s[2]));
  TF_copy_float(&packet[130], (float)(unloading->angular_velocity_target_rad_s[2]));
  TF_copy_float(&packet[134], (float)(unloading->angular_velocity_lower_threshold_rad_s[2]));
  TF_copy_u8(&packet[138], (uint8_t)(target_attitude_calculator->mode));
  TF_copy_u8(&packet[139], (uint8_t)(target_attitude_calculator->is_enabled));
  TF_copy_float(&packet[140], (float)(target_attitude_calculator->quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[144], (float)(target_attitude_calculator->quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[148], (float)(target_attitude_calculator->quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[152], (float)(target_attitude_calculator->quaternion_target_i2t.scalar_part));
  TF_copy_float(&packet[156], (float)(target_attitude_calculator->ang_vel_target_body_rad_s[0]));
  TF_copy_float(&packet[160], (float)(target_attitude_calculator->ang_vel_target_body_rad_s[1]));
  TF_copy_float(&packet[164], (float)(target_attitude_calculator->ang_vel_target_body_rad_s[2]));
  TF_copy_u8(&packet[168], (uint8_t)(target_attitude_from_orbit->main_target_direction));
  TF_copy_u8(&packet[169], (uint8_t)(target_attitude_from_orbit->sub_target_direction));
  TF_copy_float(&packet[170], (float)(target_attitude_from_orbit->vec_to_main_target_body[0]));
  TF_copy_float(&packet[174], (float)(target_attitude_from_orbit->vec_to_main_target_body[1]));
  TF_copy_float(&packet[178], (float)(target_attitude_from_orbit->vec_to_main_target_body[2]));
  TF_copy_float(&packet[182], (float)(target_attitude_from_orbit->vec_to_sub_target_body[0]));
  TF_copy_float(&packet[186], (float)(target_attitude_from_orbit->vec_to_sub_target_body[1]));
  TF_copy_float(&packet[190], (float)(target_attitude_from_orbit->vec_to_sub_target_body[2]));
  TF_copy_float(&packet[194], (float)(target_attitude_from_orbit->target_lla_rad_m[0]));
  TF_copy_float(&packet[198], (float)(target_attitude_from_orbit->target_lla_rad_m[1]));
  TF_copy_float(&packet[202], (float)(target_attitude_from_orbit->target_lla_rad_m[2]));
  TF_copy_u8(&packet[206], (uint8_t)(target_attitude_from_orbit->offset_angle_axis));
  TF_copy_float(&packet[207], (float)(target_attitude_from_orbit->offset_angle_rad));
  TF_copy_u8(&packet[211], (uint8_t)(quaternion_interpolator->current_target_num));
  TF_copy_u8(&packet[212], (uint8_t)(quaternion_interpolator->index));
  TF_copy_u32(&packet[213], (uint32_t)(quaternion_interpolator->previous_attitude_changed_ti));
  TF_copy_float(&packet[217], (float)(quaternion_interpolator->previous_quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[221], (float)(quaternion_interpolator->previous_quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[225], (float)(quaternion_interpolator->previous_quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[229], (float)(quaternion_interpolator->previous_quaternion_target_i2t.scalar_part));
  TF_copy_float(&packet[233], (float)(target_attitude_calculator->sampling_freq_Hz));
  TF_copy_float(&packet[237], (float)(target_attitude_calculator->cut_off_freq_lpf_1st_Hz));
#endif

  *len = 241;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_BIAS_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (206 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_bias_compo_rad_s[0]));
  TF_copy_float(&packet[30], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_bias_compo_rad_s[1]));
  TF_copy_float(&packet[34], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_bias_compo_rad_s[2]));
  TF_copy_float(&packet[38], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_bias_compo_nT[0]));
  TF_copy_float(&packet[42], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_bias_compo_nT[1]));
  TF_copy_float(&packet[46], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_bias_compo_nT[2]));
  TF_copy_float(&packet[50], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_bias_compo_nT[0]));
  TF_copy_float(&packet[54], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_bias_compo_nT[1]));
  TF_copy_float(&packet[58], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_bias_compo_nT[2]));
  TF_copy_float(&packet[62], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_bias_compo_nT[0]));
  TF_copy_float(&packet[66], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_bias_compo_nT[1]));
  TF_copy_float(&packet[70], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_bias_compo_nT[2]));
  TF_copy_float(&packet[74], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_bias_compo_rad_sec[0]));
  TF_copy_float(&packet[78], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_bias_compo_rad_sec[1]));
  TF_copy_float(&packet[82], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_bias_compo_rad_sec[2]));
  TF_copy_float(&packet[86], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_raw_compo_rad_s[0]));
  TF_copy_float(&packet[90], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_raw_compo_rad_s[1]));
  TF_copy_float(&packet[94], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_raw_compo_rad_s[2]));
  TF_copy_float(&packet[98], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_raw_compo_nT[0]));
  TF_copy_float(&packet[102], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_raw_compo_nT[1]));
  TF_copy_float(&packet[106], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_raw_compo_nT[2]));
  TF_copy_float(&packet[110], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_raw_compo_nT[0]));
  TF_copy_float(&packet[114], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_raw_compo_nT[1]));
  TF_copy_float(&packet[118], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_raw_compo_nT[2]));
  TF_copy_float(&packet[122], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_raw_compo_nT[0]));
  TF_copy_float(&packet[126], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_raw_compo_nT[1]));
  TF_copy_float(&packet[130], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_raw_compo_nT[2]));
  TF_copy_float(&packet[134], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_raw_compo_rad_sec[0]));
  TF_copy_float(&packet[138], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_raw_compo_rad_sec[1]));
  TF_copy_float(&packet[142], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_raw_compo_rad_sec[2]));
  TF_copy_float(&packet[146], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_compo_rad_s[0]));
  TF_copy_float(&packet[150], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_compo_rad_s[1]));
  TF_copy_float(&packet[154], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_compo_rad_s[2]));
  TF_copy_float(&packet[158], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_compo_nT[0]));
  TF_copy_float(&packet[162], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_compo_nT[1]));
  TF_copy_float(&packet[166], (float)(mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_compo_nT[2]));
  TF_copy_float(&packet[170], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_compo_nT[0]));
  TF_copy_float(&packet[174], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_compo_nT[1]));
  TF_copy_float(&packet[178], (float)(rm3100_driver[RM3100_IDX_ON_AOBC]->info.mag_compo_nT[2]));
  TF_copy_float(&packet[182], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_compo_nT[0]));
  TF_copy_float(&packet[186], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_compo_nT[1]));
  TF_copy_float(&packet[190], (float)(rm3100_driver[RM3100_IDX_EXTERNAL]->info.mag_compo_nT[2]));
  TF_copy_float(&packet[194], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_compo_rad_sec[0]));
  TF_copy_float(&packet[198], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_compo_rad_sec[1]));
  TF_copy_float(&packet[202], (float)(stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_compo_rad_sec[2]));
#endif

  *len = 206;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_TEMP_CALIB_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (218 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[0].range_low_));
  TF_copy_float(&packet[30], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[0].range_high_));
  TF_copy_float(&packet[34], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[0].coefficients_[0]));
  TF_copy_float(&packet[38], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[0].coefficients_[1]));
  TF_copy_float(&packet[42], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[1].range_low_));
  TF_copy_float(&packet[46], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[1].range_high_));
  TF_copy_float(&packet[50], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[1].coefficients_[0]));
  TF_copy_float(&packet[54], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[1].coefficients_[1]));
  TF_copy_float(&packet[58], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[2].range_low_));
  TF_copy_float(&packet[62], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[2].range_high_));
  TF_copy_float(&packet[66], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[2].coefficients_[0]));
  TF_copy_float(&packet[70], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_bias_compo_rad_s[2].coefficients_[1]));
  TF_copy_float(&packet[74], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[0].range_low_));
  TF_copy_float(&packet[78], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[0].range_high_));
  TF_copy_float(&packet[82], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[0].coefficients_[0]));
  TF_copy_float(&packet[86], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[0].coefficients_[1]));
  TF_copy_float(&packet[90], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[1].range_low_));
  TF_copy_float(&packet[94], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[1].range_high_));
  TF_copy_float(&packet[98], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[1].coefficients_[0]));
  TF_copy_float(&packet[102], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[1].coefficients_[1]));
  TF_copy_float(&packet[106], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[2].range_low_));
  TF_copy_float(&packet[110], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[2].range_high_));
  TF_copy_float(&packet[114], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[2].coefficients_[0]));
  TF_copy_float(&packet[118], (float)(di_mpu9250[MPU9250_IDX_ON_AOBC]->gyro_scale_factor_compo[2].coefficients_[1]));
  TF_copy_float(&packet[122], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[0].range_low_));
  TF_copy_float(&packet[126], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[0].range_high_));
  TF_copy_float(&packet[130], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[0].coefficients_[0]));
  TF_copy_float(&packet[134], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[0].coefficients_[1]));
  TF_copy_float(&packet[138], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[1].range_low_));
  TF_copy_float(&packet[142], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[1].range_high_));
  TF_copy_float(&packet[146], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[1].coefficients_[0]));
  TF_copy_float(&packet[150], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[1].coefficients_[1]));
  TF_copy_float(&packet[154], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[2].range_low_));
  TF_copy_float(&packet[158], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[2].range_high_));
  TF_copy_float(&packet[162], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[2].coefficients_[0]));
  TF_copy_float(&packet[166], (float)(di_stim210[STIM210_IDX_IN_UNIT]->bias_compo_rad_s[2].coefficients_[1]));
  TF_copy_float(&packet[170], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[0].range_low_));
  TF_copy_float(&packet[174], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[0].range_high_));
  TF_copy_float(&packet[178], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[0].coefficients_[0]));
  TF_copy_float(&packet[182], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[0].coefficients_[1]));
  TF_copy_float(&packet[186], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[1].range_low_));
  TF_copy_float(&packet[190], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[1].range_high_));
  TF_copy_float(&packet[194], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[1].coefficients_[0]));
  TF_copy_float(&packet[198], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[1].coefficients_[1]));
  TF_copy_float(&packet[202], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[2].range_low_));
  TF_copy_float(&packet[206], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[2].range_high_));
  TF_copy_float(&packet[210], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[2].coefficients_[0]));
  TF_copy_float(&packet[214], (float)(di_stim210[STIM210_IDX_IN_UNIT]->scale_factor_compo[2].coefficients_[1]));
#endif

  *len = 218;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_INTERPOLATION_TARGET1_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (226 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(quaternion_interpolator->target_list[0].attitude_changed_ti));
  TF_copy_float(&packet[30], (float)(quaternion_interpolator->target_list[0].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[34], (float)(quaternion_interpolator->target_list[0].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[38], (float)(quaternion_interpolator->target_list[0].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[42], (float)(quaternion_interpolator->target_list[0].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[46], (uint32_t)(quaternion_interpolator->target_list[1].attitude_changed_ti));
  TF_copy_float(&packet[50], (float)(quaternion_interpolator->target_list[1].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[54], (float)(quaternion_interpolator->target_list[1].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[58], (float)(quaternion_interpolator->target_list[1].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[62], (float)(quaternion_interpolator->target_list[1].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[66], (uint32_t)(quaternion_interpolator->target_list[2].attitude_changed_ti));
  TF_copy_float(&packet[70], (float)(quaternion_interpolator->target_list[2].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[74], (float)(quaternion_interpolator->target_list[2].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[78], (float)(quaternion_interpolator->target_list[2].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[82], (float)(quaternion_interpolator->target_list[2].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[86], (uint32_t)(quaternion_interpolator->target_list[3].attitude_changed_ti));
  TF_copy_float(&packet[90], (float)(quaternion_interpolator->target_list[3].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[94], (float)(quaternion_interpolator->target_list[3].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[98], (float)(quaternion_interpolator->target_list[3].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[102], (float)(quaternion_interpolator->target_list[3].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[106], (uint32_t)(quaternion_interpolator->target_list[4].attitude_changed_ti));
  TF_copy_float(&packet[110], (float)(quaternion_interpolator->target_list[4].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[114], (float)(quaternion_interpolator->target_list[4].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[118], (float)(quaternion_interpolator->target_list[4].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[122], (float)(quaternion_interpolator->target_list[4].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[126], (uint32_t)(quaternion_interpolator->target_list[5].attitude_changed_ti));
  TF_copy_float(&packet[130], (float)(quaternion_interpolator->target_list[5].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[134], (float)(quaternion_interpolator->target_list[5].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[138], (float)(quaternion_interpolator->target_list[5].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[142], (float)(quaternion_interpolator->target_list[5].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[146], (uint32_t)(quaternion_interpolator->target_list[6].attitude_changed_ti));
  TF_copy_float(&packet[150], (float)(quaternion_interpolator->target_list[6].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[154], (float)(quaternion_interpolator->target_list[6].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[158], (float)(quaternion_interpolator->target_list[6].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[162], (float)(quaternion_interpolator->target_list[6].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[166], (uint32_t)(quaternion_interpolator->target_list[7].attitude_changed_ti));
  TF_copy_float(&packet[170], (float)(quaternion_interpolator->target_list[7].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[174], (float)(quaternion_interpolator->target_list[7].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[178], (float)(quaternion_interpolator->target_list[7].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[182], (float)(quaternion_interpolator->target_list[7].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[186], (uint32_t)(quaternion_interpolator->target_list[8].attitude_changed_ti));
  TF_copy_float(&packet[190], (float)(quaternion_interpolator->target_list[8].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[194], (float)(quaternion_interpolator->target_list[8].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[198], (float)(quaternion_interpolator->target_list[8].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[202], (float)(quaternion_interpolator->target_list[8].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[206], (uint32_t)(quaternion_interpolator->target_list[9].attitude_changed_ti));
  TF_copy_float(&packet[210], (float)(quaternion_interpolator->target_list[9].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[214], (float)(quaternion_interpolator->target_list[9].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[218], (float)(quaternion_interpolator->target_list[9].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[222], (float)(quaternion_interpolator->target_list[9].quaternion_target_i2t.scalar_part));
#endif

  *len = 226;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_INTERPOLATION_TARGET2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (226 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(quaternion_interpolator->target_list[10].attitude_changed_ti));
  TF_copy_float(&packet[30], (float)(quaternion_interpolator->target_list[10].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[34], (float)(quaternion_interpolator->target_list[10].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[38], (float)(quaternion_interpolator->target_list[10].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[42], (float)(quaternion_interpolator->target_list[10].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[46], (uint32_t)(quaternion_interpolator->target_list[11].attitude_changed_ti));
  TF_copy_float(&packet[50], (float)(quaternion_interpolator->target_list[11].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[54], (float)(quaternion_interpolator->target_list[11].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[58], (float)(quaternion_interpolator->target_list[11].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[62], (float)(quaternion_interpolator->target_list[11].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[66], (uint32_t)(quaternion_interpolator->target_list[12].attitude_changed_ti));
  TF_copy_float(&packet[70], (float)(quaternion_interpolator->target_list[12].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[74], (float)(quaternion_interpolator->target_list[12].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[78], (float)(quaternion_interpolator->target_list[12].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[82], (float)(quaternion_interpolator->target_list[12].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[86], (uint32_t)(quaternion_interpolator->target_list[13].attitude_changed_ti));
  TF_copy_float(&packet[90], (float)(quaternion_interpolator->target_list[13].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[94], (float)(quaternion_interpolator->target_list[13].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[98], (float)(quaternion_interpolator->target_list[13].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[102], (float)(quaternion_interpolator->target_list[13].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[106], (uint32_t)(quaternion_interpolator->target_list[14].attitude_changed_ti));
  TF_copy_float(&packet[110], (float)(quaternion_interpolator->target_list[14].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[114], (float)(quaternion_interpolator->target_list[14].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[118], (float)(quaternion_interpolator->target_list[14].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[122], (float)(quaternion_interpolator->target_list[14].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[126], (uint32_t)(quaternion_interpolator->target_list[15].attitude_changed_ti));
  TF_copy_float(&packet[130], (float)(quaternion_interpolator->target_list[15].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[134], (float)(quaternion_interpolator->target_list[15].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[138], (float)(quaternion_interpolator->target_list[15].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[142], (float)(quaternion_interpolator->target_list[15].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[146], (uint32_t)(quaternion_interpolator->target_list[16].attitude_changed_ti));
  TF_copy_float(&packet[150], (float)(quaternion_interpolator->target_list[16].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[154], (float)(quaternion_interpolator->target_list[16].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[158], (float)(quaternion_interpolator->target_list[16].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[162], (float)(quaternion_interpolator->target_list[16].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[166], (uint32_t)(quaternion_interpolator->target_list[17].attitude_changed_ti));
  TF_copy_float(&packet[170], (float)(quaternion_interpolator->target_list[17].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[174], (float)(quaternion_interpolator->target_list[17].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[178], (float)(quaternion_interpolator->target_list[7].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[182], (float)(quaternion_interpolator->target_list[17].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[186], (uint32_t)(quaternion_interpolator->target_list[18].attitude_changed_ti));
  TF_copy_float(&packet[190], (float)(quaternion_interpolator->target_list[18].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[194], (float)(quaternion_interpolator->target_list[18].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[198], (float)(quaternion_interpolator->target_list[18].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[202], (float)(quaternion_interpolator->target_list[18].quaternion_target_i2t.scalar_part));
  TF_copy_u32(&packet[206], (uint32_t)(quaternion_interpolator->target_list[19].attitude_changed_ti));
  TF_copy_float(&packet[210], (float)(quaternion_interpolator->target_list[19].quaternion_target_i2t.vector_part[0]));
  TF_copy_float(&packet[214], (float)(quaternion_interpolator->target_list[19].quaternion_target_i2t.vector_part[1]));
  TF_copy_float(&packet[218], (float)(quaternion_interpolator->target_list[19].quaternion_target_i2t.vector_part[2]));
  TF_copy_float(&packet[222], (float)(quaternion_interpolator->target_list[19].quaternion_target_i2t.scalar_part));
#endif

  *len = 226;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_NON_VOLATILE_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (145 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(non_volatile_memory_manager->error_status));
  TF_copy_u32(&packet[27], (uint32_t)(non_volatile_memory_manager->write_info.start_address));
  TF_copy_u8(&packet[31], (uint8_t)(non_volatile_memory_manager->write_info.data_size_byte));
  TF_copy_u8(&packet[32], (uint8_t)(non_volatile_memory_manager->write_info.data[0]));
  TF_copy_u8(&packet[33], (uint8_t)(non_volatile_memory_manager->write_info.data[1]));
  TF_copy_u8(&packet[34], (uint8_t)(non_volatile_memory_manager->write_info.data[2]));
  TF_copy_u8(&packet[35], (uint8_t)(non_volatile_memory_manager->write_info.data[3]));
  TF_copy_u8(&packet[36], (uint8_t)(non_volatile_memory_manager->write_info.data[4]));
  TF_copy_u8(&packet[37], (uint8_t)(non_volatile_memory_manager->write_info.data[5]));
  TF_copy_u8(&packet[38], (uint8_t)(non_volatile_memory_manager->write_info.data[6]));
  TF_copy_u8(&packet[39], (uint8_t)(non_volatile_memory_manager->write_info.data[7]));
  TF_copy_u32(&packet[40], (uint32_t)(non_volatile_memory_manager->read_info.start_address));
  TF_copy_u8(&packet[44], (uint8_t)(non_volatile_memory_manager->read_info.data_size_byte));
  TF_copy_u8(&packet[45], (uint8_t)(non_volatile_memory_manager->read_info.data[0]));
  TF_copy_u8(&packet[46], (uint8_t)(non_volatile_memory_manager->read_info.data[1]));
  TF_copy_u8(&packet[47], (uint8_t)(non_volatile_memory_manager->read_info.data[2]));
  TF_copy_u8(&packet[48], (uint8_t)(non_volatile_memory_manager->read_info.data[3]));
  TF_copy_u8(&packet[49], (uint8_t)(non_volatile_memory_manager->read_info.data[4]));
  TF_copy_u8(&packet[50], (uint8_t)(non_volatile_memory_manager->read_info.data[5]));
  TF_copy_u8(&packet[51], (uint8_t)(non_volatile_memory_manager->read_info.data[6]));
  TF_copy_u8(&packet[52], (uint8_t)(non_volatile_memory_manager->read_info.data[7]));
  TF_copy_u8(&packet[53], (uint8_t)(non_volatile_memory_partition->error_status));
  TF_copy_u8(&packet[54], (uint8_t)(non_volatile_memory_partition->write_info.partition_id));
  TF_copy_u32(&packet[55], (uint32_t)(non_volatile_memory_partition->write_info.start_address));
  TF_copy_u8(&packet[59], (uint8_t)(non_volatile_memory_partition->write_info.data_size_byte));
  TF_copy_u8(&packet[60], (uint8_t)(non_volatile_memory_partition->write_info.data[0]));
  TF_copy_u8(&packet[61], (uint8_t)(non_volatile_memory_partition->write_info.data[1]));
  TF_copy_u8(&packet[62], (uint8_t)(non_volatile_memory_partition->write_info.data[2]));
  TF_copy_u8(&packet[63], (uint8_t)(non_volatile_memory_partition->write_info.data[3]));
  TF_copy_u8(&packet[64], (uint8_t)(non_volatile_memory_partition->write_info.data[4]));
  TF_copy_u8(&packet[65], (uint8_t)(non_volatile_memory_partition->write_info.data[5]));
  TF_copy_u8(&packet[66], (uint8_t)(non_volatile_memory_partition->write_info.data[6]));
  TF_copy_u8(&packet[67], (uint8_t)(non_volatile_memory_partition->write_info.data[7]));
  TF_copy_u8(&packet[68], (uint8_t)(non_volatile_memory_partition->read_info.partition_id));
  TF_copy_u32(&packet[69], (uint32_t)(non_volatile_memory_partition->read_info.start_address));
  TF_copy_u8(&packet[73], (uint8_t)(non_volatile_memory_partition->read_info.data_size_byte));
  TF_copy_u8(&packet[74], (uint8_t)(non_volatile_memory_partition->read_info.data[0]));
  TF_copy_u8(&packet[75], (uint8_t)(non_volatile_memory_partition->read_info.data[1]));
  TF_copy_u8(&packet[76], (uint8_t)(non_volatile_memory_partition->read_info.data[2]));
  TF_copy_u8(&packet[77], (uint8_t)(non_volatile_memory_partition->read_info.data[3]));
  TF_copy_u8(&packet[78], (uint8_t)(non_volatile_memory_partition->read_info.data[4]));
  TF_copy_u8(&packet[79], (uint8_t)(non_volatile_memory_partition->read_info.data[5]));
  TF_copy_u8(&packet[80], (uint8_t)(non_volatile_memory_partition->read_info.data[6]));
  TF_copy_u8(&packet[81], (uint8_t)(non_volatile_memory_partition->read_info.data[7]));
  TF_copy_u8(&packet[82], (uint8_t)(aocs_data_recorder->record_permission));
  TF_copy_u8(&packet[83], (uint8_t)(aocs_data_recorder->read_mode));
  TF_copy_u8(&packet[84], (uint8_t)(aocs_data_recorder->overwrite_mode));
  TF_copy_u8(&packet[85], (uint8_t)(aocs_data_recorder->write_partition_id));
  TF_copy_u32(&packet[86], (uint32_t)(aocs_data_recorder->write_pointer));
  TF_copy_u8(&packet[90], (uint8_t)(aocs_data_recorder->read_partition_id));
  TF_copy_u32(&packet[91], (uint32_t)(aocs_data_recorder->read_pointer));
  TF_copy_u8(&packet[95], (uint8_t)(fm25v10_driver[FM25V10_IDX_1]->info.status));
  TF_copy_u32(&packet[96], (uint32_t)(fm25v10_driver[FM25V10_IDX_1]->info.last_start_address));
  TF_copy_u8(&packet[100], (uint8_t)(fm25v10_driver[FM25V10_IDX_1]->info.last_read_data_byte));
  TF_copy_u8(&packet[101], (uint8_t)(fm25v10_driver[FM25V10_IDX_1]->info.last_write_data_byte));
  TF_copy_u8(&packet[102], (uint8_t)(fm25v10_driver[FM25V10_IDX_2]->info.status));
  TF_copy_u32(&packet[103], (uint32_t)(fm25v10_driver[FM25V10_IDX_2]->info.last_start_address));
  TF_copy_u8(&packet[107], (uint8_t)(fm25v10_driver[FM25V10_IDX_2]->info.last_read_data_byte));
  TF_copy_u8(&packet[108], (uint8_t)(fm25v10_driver[FM25V10_IDX_2]->info.last_write_data_byte));
  TF_copy_u8(&packet[109], (uint8_t)(fm25v10_driver[FM25V10_IDX_3]->info.status));
  TF_copy_u32(&packet[110], (uint32_t)(fm25v10_driver[FM25V10_IDX_3]->info.last_start_address));
  TF_copy_u8(&packet[114], (uint8_t)(fm25v10_driver[FM25V10_IDX_3]->info.last_read_data_byte));
  TF_copy_u8(&packet[115], (uint8_t)(fm25v10_driver[FM25V10_IDX_3]->info.last_write_data_byte));
  TF_copy_u8(&packet[116], (uint8_t)(fm25v10_driver[FM25V10_IDX_4]->info.status));
  TF_copy_u32(&packet[117], (uint32_t)(fm25v10_driver[FM25V10_IDX_4]->info.last_start_address));
  TF_copy_u8(&packet[121], (uint8_t)(fm25v10_driver[FM25V10_IDX_4]->info.last_read_data_byte));
  TF_copy_u8(&packet[122], (uint8_t)(fm25v10_driver[FM25V10_IDX_4]->info.last_write_data_byte));
  TF_copy_u8(&packet[123], nvm_bc->is_active);
  TF_copy_u16(&packet[124], (uint16_t)nvm_bc->bc_id_to_copy);
  TF_copy_u8(&packet[126], nvm_bc->bc_num_to_copy);
  TF_copy_u32(&packet[127], nvm_bc->address_for_ready_flags);
  TF_copy_u32(&packet[131], nvm_bc->address_for_bc);
  TF_copy_u8(&packet[135], (uint8_t)((nvm_bc->is_ready_to_restore[0] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[1] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[2] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[3] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[4] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[5] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[6] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[7] & 0x01) ));
  TF_copy_u8(&packet[136], (uint8_t)((nvm_bc->is_ready_to_restore[8] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[9] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[10] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[11] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[12] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[13] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[14] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[15] & 0x01) ));
  TF_copy_u8(&packet[137], (uint8_t)((nvm_bc->is_ready_to_restore[16] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[17] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[18] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[19] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[20] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[21] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[22] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[23] & 0x01) ));
  TF_copy_u8(&packet[138], (uint8_t)((nvm_bc->is_ready_to_restore[24] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[25] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[26] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[27] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[28] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[29] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[30] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[31] & 0x01) ));
  TF_copy_u8(&packet[139], (uint8_t)((nvm_bc->is_ready_to_restore[32] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[33] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[34] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[35] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[36] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[37] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[38] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[39] & 0x01) ));
  TF_copy_u8(&packet[140], (uint8_t)((nvm_bc->is_ready_to_restore[40] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[41] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[42] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[43] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[44] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[45] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[46] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[47] & 0x01) ));
  TF_copy_u8(&packet[141], (uint8_t)((nvm_bc->is_ready_to_restore[48] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[49] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[50] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[51] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[52] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[53] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[54] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[55] & 0x01) ));
  TF_copy_u8(&packet[142], (uint8_t)((nvm_bc->is_ready_to_restore[56] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[57] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[58] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[59] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[60] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[61] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[62] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[63] & 0x01) ));
  TF_copy_u8(&packet[143], (uint8_t)((nvm_bc->is_ready_to_restore[64] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[65] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[66] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[67] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[68] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[69] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[70] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[71] & 0x01) ));
  TF_copy_u8(&packet[144], (uint8_t)((nvm_bc->is_ready_to_restore[72] << 7 & 0x80) | (nvm_bc->is_ready_to_restore[73] << 6 & 0x40) | (nvm_bc->is_ready_to_restore[74] << 5 & 0x20) | (nvm_bc->is_ready_to_restore[75] << 4 & 0x10) | (nvm_bc->is_ready_to_restore[76] << 3 & 0x08) | (nvm_bc->is_ready_to_restore[77] << 2 & 0x04) | (nvm_bc->is_ready_to_restore[78] << 1 & 0x02) | (nvm_bc->is_ready_to_restore[79] & 0x01) ));
#endif

  *len = 145;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_ORBIT_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (157 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(orbit_calculator->method));
  TF_copy_float(&packet[27], (float)(kepler_orbit_propagator->orbital_elements.semi_major_axis_km));
  TF_copy_float(&packet[31], (float)(kepler_orbit_propagator->orbital_elements.eccentricity));
  TF_copy_float(&packet[35], (float)(kepler_orbit_propagator->orbital_elements.inclination_rad));
  TF_copy_float(&packet[39], (float)(kepler_orbit_propagator->orbital_elements.raan_rad));
  TF_copy_float(&packet[43], (float)(kepler_orbit_propagator->orbital_elements.arg_perigee_rad));
  TF_copy_double(&packet[47], (double)(kepler_orbit_propagator->orbital_elements.epoch_jday));
  TF_copy_float(&packet[55], (float)(kepler_orbit_propagator->orbital_elements_cmd_tmp.semi_major_axis_km));
  TF_copy_float(&packet[59], (float)(kepler_orbit_propagator->orbital_elements_cmd_tmp.eccentricity));
  TF_copy_float(&packet[63], (float)(kepler_orbit_propagator->orbital_elements_cmd_tmp.inclination_rad));
  TF_copy_float(&packet[67], (float)(kepler_orbit_propagator->orbital_elements_cmd_tmp.raan_rad));
  TF_copy_float(&packet[71], (float)(kepler_orbit_propagator->orbital_elements_cmd_tmp.arg_perigee_rad));
  TF_copy_double(&packet[75], (double)(kepler_orbit_propagator->orbital_elements_cmd_tmp.epoch_jday));
  TF_copy_u8(&packet[83], (uint8_t)(sgp4_orbit_propagator->tle_input.epoch_year));
  TF_copy_double(&packet[84], (double)(sgp4_orbit_propagator->tle_input.epoch_day));
  TF_copy_float(&packet[92], (float)(sgp4_orbit_propagator->tle_input.b_star));
  TF_copy_float(&packet[96], (float)(sgp4_orbit_propagator->tle_input.inclination_deg));
  TF_copy_float(&packet[100], (float)(sgp4_orbit_propagator->tle_input.raan_deg));
  TF_copy_float(&packet[104], (float)(sgp4_orbit_propagator->tle_input.eccentricity));
  TF_copy_float(&packet[108], (float)(sgp4_orbit_propagator->tle_input.arg_perigee_deg));
  TF_copy_float(&packet[112], (float)(sgp4_orbit_propagator->tle_input.mean_anomaly_deg));
  TF_copy_float(&packet[116], (float)(sgp4_orbit_propagator->tle_input.mean_motion_rpd));
  TF_copy_u8(&packet[120], (uint8_t)(sgp4_orbit_propagator->tle_input_cmd_tmp.epoch_year));
  TF_copy_double(&packet[121], (double)(sgp4_orbit_propagator->tle_input_cmd_tmp.epoch_day));
  TF_copy_float(&packet[129], (float)(sgp4_orbit_propagator->tle_input_cmd_tmp.b_star));
  TF_copy_float(&packet[133], (float)(sgp4_orbit_propagator->tle_input_cmd_tmp.inclination_deg));
  TF_copy_float(&packet[137], (float)(sgp4_orbit_propagator->tle_input_cmd_tmp.raan_deg));
  TF_copy_float(&packet[141], (float)(sgp4_orbit_propagator->tle_input_cmd_tmp.eccentricity));
  TF_copy_float(&packet[145], (float)(sgp4_orbit_propagator->tle_input_cmd_tmp.arg_perigee_deg));
  TF_copy_float(&packet[149], (float)(sgp4_orbit_propagator->tle_input_cmd_tmp.mean_anomaly_deg));
  TF_copy_float(&packet[153], (float)(sgp4_orbit_propagator->tle_input_cmd_tmp.mean_motion_rpd));
#endif

  *len = 157;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_AOCS_MANAGER_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (211 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(aocs_manager->mass_sc_kg));
  TF_copy_float(&packet[30], (float)(aocs_manager->rmm_sc_body_Am2[0]));
  TF_copy_float(&packet[34], (float)(aocs_manager->rmm_sc_body_Am2[1]));
  TF_copy_float(&packet[38], (float)(aocs_manager->rmm_sc_body_Am2[2]));
  TF_copy_float(&packet[42], (float)(aocs_manager->inertia_tensor_sc_body_kgm2[0][0]));
  TF_copy_float(&packet[46], (float)(aocs_manager->inertia_tensor_sc_body_kgm2[1][1]));
  TF_copy_float(&packet[50], (float)(aocs_manager->inertia_tensor_sc_body_kgm2[2][2]));
  TF_copy_float(&packet[54], (float)(aocs_manager->inertia_tensor_sc_body_kgm2[0][1]));
  TF_copy_float(&packet[58], (float)(aocs_manager->inertia_tensor_sc_body_kgm2[0][2]));
  TF_copy_float(&packet[62], (float)(aocs_manager->inertia_tensor_sc_body_kgm2[1][2]));
  TF_copy_float(&packet[66], (float)(aocs_manager->ang_mom_est_body_Nms[0]));
  TF_copy_float(&packet[70], (float)(aocs_manager->ang_mom_est_body_Nms[1]));
  TF_copy_float(&packet[74], (float)(aocs_manager->ang_mom_est_body_Nms[2]));
  TF_copy_float(&packet[78], (float)(aocs_manager->limit_maneuver_angle_rad));
  TF_copy_float(&packet[82], (float)(aocs_manager->limit_angular_velocity_rad_s));
  TF_copy_double(&packet[86], (double)(aocs_manager->reference_jday));
  TF_copy_u32(&packet[94], (uint32_t)(aocs_manager->obct_reference.total_cycle));
  TF_copy_float(&packet[98], (float)(aocs_manager->internal_torque_max_body_Nm[0]));
  TF_copy_float(&packet[102], (float)(aocs_manager->internal_torque_max_body_Nm[1]));
  TF_copy_float(&packet[106], (float)(aocs_manager->internal_torque_max_body_Nm[2]));
  TF_copy_float(&packet[110], (float)(aocs_manager->external_torque_max_body_Nm[0]));
  TF_copy_float(&packet[114], (float)(aocs_manager->external_torque_max_body_Nm[1]));
  TF_copy_float(&packet[118], (float)(aocs_manager->external_torque_max_body_Nm[2]));
  TF_copy_float(&packet[122], (float)(aocs_manager->rw_rotation_direction_matrix[0][0]));
  TF_copy_float(&packet[126], (float)(aocs_manager->rw_rotation_direction_matrix[0][1]));
  TF_copy_float(&packet[130], (float)(aocs_manager->rw_rotation_direction_matrix[0][2]));
  TF_copy_float(&packet[134], (float)(aocs_manager->rw_rotation_direction_matrix[1][0]));
  TF_copy_float(&packet[138], (float)(aocs_manager->rw_rotation_direction_matrix[1][1]));
  TF_copy_float(&packet[142], (float)(aocs_manager->rw_rotation_direction_matrix[1][2]));
  TF_copy_float(&packet[146], (float)(aocs_manager->rw_rotation_direction_matrix[2][0]));
  TF_copy_float(&packet[150], (float)(aocs_manager->rw_rotation_direction_matrix[2][1]));
  TF_copy_float(&packet[154], (float)(aocs_manager->rw_rotation_direction_matrix[2][2]));
  TF_copy_float(&packet[158], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[0][0]));
  TF_copy_float(&packet[162], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[0][1]));
  TF_copy_float(&packet[166], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[0][2]));
  TF_copy_float(&packet[170], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[1][0]));
  TF_copy_float(&packet[174], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[1][1]));
  TF_copy_float(&packet[178], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[1][2]));
  TF_copy_float(&packet[182], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[2][0]));
  TF_copy_float(&packet[186], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[2][1]));
  TF_copy_float(&packet[190], (float)(aocs_manager->mtq_magnetic_moment_direction_matrix[2][2]));
  TF_copy_u8(&packet[194], (uint8_t)(aocs_manager->constant_torque_permission));
  TF_copy_float(&packet[195], (float)(aocs_manager->constant_torque_body_Nm[0]));
  TF_copy_float(&packet[199], (float)(aocs_manager->constant_torque_body_Nm[1]));
  TF_copy_float(&packet[203], (float)(aocs_manager->constant_torque_body_Nm[2]));
  TF_copy_float(&packet[207], (float)(time_space_calculator->offset_sec));
#endif

  *len = 211;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SUN_POINTING_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (234 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(sun_pointing->pid_omega[0].gains.p_gain));
  TF_copy_float(&packet[30], (float)(sun_pointing->pid_omega[1].gains.p_gain));
  TF_copy_float(&packet[34], (float)(sun_pointing->pid_omega[2].gains.p_gain));
  TF_copy_float(&packet[38], (float)(sun_pointing->pid_omega[0].gains.i_gain));
  TF_copy_float(&packet[42], (float)(sun_pointing->pid_omega[1].gains.i_gain));
  TF_copy_float(&packet[46], (float)(sun_pointing->pid_omega[2].gains.i_gain));
  TF_copy_float(&packet[50], (float)(sun_pointing->pid_omega[0].gains.d_gain));
  TF_copy_float(&packet[54], (float)(sun_pointing->pid_omega[1].gains.d_gain));
  TF_copy_float(&packet[58], (float)(sun_pointing->pid_omega[2].gains.d_gain));
  TF_copy_float(&packet[62], (float)(sun_pointing->pid_att[0].gains.p_gain));
  TF_copy_float(&packet[66], (float)(sun_pointing->pid_att[1].gains.p_gain));
  TF_copy_float(&packet[70], (float)(sun_pointing->pid_att[2].gains.p_gain));
  TF_copy_float(&packet[74], (float)(sun_pointing->pid_att[0].gains.i_gain));
  TF_copy_float(&packet[78], (float)(sun_pointing->pid_att[1].gains.i_gain));
  TF_copy_float(&packet[82], (float)(sun_pointing->pid_att[2].gains.i_gain));
  TF_copy_float(&packet[86], (float)(sun_pointing->pid_att[0].gains.d_gain));
  TF_copy_float(&packet[90], (float)(sun_pointing->pid_att[1].gains.d_gain));
  TF_copy_float(&packet[94], (float)(sun_pointing->pid_att[2].gains.d_gain));
  TF_copy_float(&packet[98], (float)(sun_pointing->gains_omega_cmd_tmp[0].p_gain));
  TF_copy_float(&packet[102], (float)(sun_pointing->gains_omega_cmd_tmp[1].p_gain));
  TF_copy_float(&packet[106], (float)(sun_pointing->gains_omega_cmd_tmp[2].p_gain));
  TF_copy_float(&packet[110], (float)(sun_pointing->gains_omega_cmd_tmp[0].i_gain));
  TF_copy_float(&packet[114], (float)(sun_pointing->gains_omega_cmd_tmp[1].i_gain));
  TF_copy_float(&packet[118], (float)(sun_pointing->gains_omega_cmd_tmp[2].i_gain));
  TF_copy_float(&packet[122], (float)(sun_pointing->gains_omega_cmd_tmp[0].d_gain));
  TF_copy_float(&packet[126], (float)(sun_pointing->gains_omega_cmd_tmp[1].d_gain));
  TF_copy_float(&packet[130], (float)(sun_pointing->gains_omega_cmd_tmp[2].d_gain));
  TF_copy_float(&packet[134], (float)(sun_pointing->gains_att_cmd_tmp[0].p_gain));
  TF_copy_float(&packet[138], (float)(sun_pointing->gains_att_cmd_tmp[1].p_gain));
  TF_copy_float(&packet[142], (float)(sun_pointing->gains_att_cmd_tmp[2].p_gain));
  TF_copy_float(&packet[146], (float)(sun_pointing->gains_att_cmd_tmp[0].i_gain));
  TF_copy_float(&packet[150], (float)(sun_pointing->gains_att_cmd_tmp[1].i_gain));
  TF_copy_float(&packet[154], (float)(sun_pointing->gains_att_cmd_tmp[2].i_gain));
  TF_copy_float(&packet[158], (float)(sun_pointing->gains_att_cmd_tmp[0].d_gain));
  TF_copy_float(&packet[162], (float)(sun_pointing->gains_att_cmd_tmp[1].d_gain));
  TF_copy_float(&packet[166], (float)(sun_pointing->gains_att_cmd_tmp[2].d_gain));
  TF_copy_float(&packet[170], (float)(sun_pointing->max_direct_feedback_angle_rad));
  TF_copy_float(&packet[174], (float)(sun_pointing->max_direct_feedback_rate_rad_s));
  TF_copy_float(&packet[178], (float)(sun_pointing->max_integral_angle_rad));
  TF_copy_float(&packet[182], (float)(sun_pointing->max_angle_to_run_integral_control_rad));
  TF_copy_float(&packet[186], (float)(sun_pointing->integral_control_permission_angle_rad));
  TF_copy_float(&packet[190], (float)(sun_pointing->spin_rate_around_sun_rad_s));
  TF_copy_float(&packet[194], (float)(sun_pointing->acceptable_angle_error_to_spin_up_rad));
  TF_copy_float(&packet[198], (float)(sun_pointing->mtq_allowable_error_ratio_transient));
  TF_copy_float(&packet[202], (float)(sun_pointing->correction_gain_transient));
  TF_copy_float(&packet[206], (float)(sun_pointing->mtq_allowable_error_ratio_stable));
  TF_copy_float(&packet[210], (float)(sun_pointing->correction_gain_stable));
  TF_copy_float(&packet[214], (float)(sun_pointing->lpf_sample_freq_Hz));
  TF_copy_float(&packet[218], (float)(sun_pointing->lpf_trq_cutoff_freq_Hz));
  TF_copy_float(&packet[222], (float)(sun_pointing->lpf_trq_damping_factor));
  TF_copy_float(&packet[226], (float)(sun_pointing->lpf_trq_cutoff_freq_spin_axis_Hz));
  TF_copy_float(&packet[230], (float)(sun_pointing->lpf_spin_rate_cutoff_freq_Hz));
#endif

  *len = 234;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_THREE_AXIS_MTQ_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (218 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(three_axis_control_mtq->pid_omega[0].gains.p_gain));
  TF_copy_float(&packet[30], (float)(three_axis_control_mtq->pid_omega[1].gains.p_gain));
  TF_copy_float(&packet[34], (float)(three_axis_control_mtq->pid_omega[2].gains.p_gain));
  TF_copy_float(&packet[38], (float)(three_axis_control_mtq->pid_omega[0].gains.i_gain));
  TF_copy_float(&packet[42], (float)(three_axis_control_mtq->pid_omega[1].gains.i_gain));
  TF_copy_float(&packet[46], (float)(three_axis_control_mtq->pid_omega[2].gains.i_gain));
  TF_copy_float(&packet[50], (float)(three_axis_control_mtq->pid_omega[0].gains.d_gain));
  TF_copy_float(&packet[54], (float)(three_axis_control_mtq->pid_omega[1].gains.d_gain));
  TF_copy_float(&packet[58], (float)(three_axis_control_mtq->pid_omega[2].gains.d_gain));
  TF_copy_float(&packet[62], (float)(three_axis_control_mtq->pid_att[0].gains.p_gain));
  TF_copy_float(&packet[66], (float)(three_axis_control_mtq->pid_att[1].gains.p_gain));
  TF_copy_float(&packet[70], (float)(three_axis_control_mtq->pid_att[2].gains.p_gain));
  TF_copy_float(&packet[74], (float)(three_axis_control_mtq->pid_att[0].gains.i_gain));
  TF_copy_float(&packet[78], (float)(three_axis_control_mtq->pid_att[1].gains.i_gain));
  TF_copy_float(&packet[82], (float)(three_axis_control_mtq->pid_att[2].gains.i_gain));
  TF_copy_float(&packet[86], (float)(three_axis_control_mtq->pid_att[0].gains.d_gain));
  TF_copy_float(&packet[90], (float)(three_axis_control_mtq->pid_att[1].gains.d_gain));
  TF_copy_float(&packet[94], (float)(three_axis_control_mtq->pid_att[2].gains.d_gain));
  TF_copy_float(&packet[98], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[0].p_gain));
  TF_copy_float(&packet[102], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[1].p_gain));
  TF_copy_float(&packet[106], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[2].p_gain));
  TF_copy_float(&packet[110], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[0].i_gain));
  TF_copy_float(&packet[114], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[1].i_gain));
  TF_copy_float(&packet[118], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[2].i_gain));
  TF_copy_float(&packet[122], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[0].d_gain));
  TF_copy_float(&packet[126], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[1].d_gain));
  TF_copy_float(&packet[130], (float)(three_axis_control_mtq->gains_omega_cmd_tmp[2].d_gain));
  TF_copy_float(&packet[134], (float)(three_axis_control_mtq->gains_att_cmd_tmp[0].p_gain));
  TF_copy_float(&packet[138], (float)(three_axis_control_mtq->gains_att_cmd_tmp[1].p_gain));
  TF_copy_float(&packet[142], (float)(three_axis_control_mtq->gains_att_cmd_tmp[2].p_gain));
  TF_copy_float(&packet[146], (float)(three_axis_control_mtq->gains_att_cmd_tmp[0].i_gain));
  TF_copy_float(&packet[150], (float)(three_axis_control_mtq->gains_att_cmd_tmp[1].i_gain));
  TF_copy_float(&packet[154], (float)(three_axis_control_mtq->gains_att_cmd_tmp[2].i_gain));
  TF_copy_float(&packet[158], (float)(three_axis_control_mtq->gains_att_cmd_tmp[0].d_gain));
  TF_copy_float(&packet[162], (float)(three_axis_control_mtq->gains_att_cmd_tmp[1].d_gain));
  TF_copy_float(&packet[166], (float)(three_axis_control_mtq->gains_att_cmd_tmp[2].d_gain));
  TF_copy_float(&packet[170], (float)(three_axis_control_mtq->max_direct_feedback_angle_mtq_rad));
  TF_copy_float(&packet[174], (float)(three_axis_control_mtq->max_direct_feedback_rate_mtq_rad_s));
  TF_copy_float(&packet[178], (float)(three_axis_control_mtq->max_integral_angle_mtq_rad));
  TF_copy_float(&packet[182], (float)(three_axis_control_mtq->max_angle_to_run_integral_control_rad));
  TF_copy_float(&packet[186], (float)(three_axis_control_mtq->allowable_error_ratio_transient));
  TF_copy_float(&packet[190], (float)(three_axis_control_mtq->correction_gain_transient));
  TF_copy_float(&packet[194], (float)(three_axis_control_mtq->allowable_error_ratio_stable));
  TF_copy_float(&packet[198], (float)(three_axis_control_mtq->correction_gain_stable));
  TF_copy_float(&packet[202], (float)(three_axis_control_mtq->acceptable_angle_error_as_stable_rad));
  TF_copy_float(&packet[206], (float)(three_axis_control_mtq->mtq_lpf_sample_freq_Hz));
  TF_copy_float(&packet[210], (float)(three_axis_control_mtq->mtq_lpf_cutoff_freq_Hz));
  TF_copy_float(&packet[214], (float)(three_axis_control_mtq->mtq_lpf_damping_factor));
#endif

  *len = 218;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_THREE_AXIS_RW_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (206 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(three_axis_control_rw->pid_omega[0].gains.p_gain));
  TF_copy_float(&packet[30], (float)(three_axis_control_rw->pid_omega[1].gains.p_gain));
  TF_copy_float(&packet[34], (float)(three_axis_control_rw->pid_omega[2].gains.p_gain));
  TF_copy_float(&packet[38], (float)(three_axis_control_rw->pid_omega[0].gains.i_gain));
  TF_copy_float(&packet[42], (float)(three_axis_control_rw->pid_omega[1].gains.i_gain));
  TF_copy_float(&packet[46], (float)(three_axis_control_rw->pid_omega[2].gains.i_gain));
  TF_copy_float(&packet[50], (float)(three_axis_control_rw->pid_omega[0].gains.d_gain));
  TF_copy_float(&packet[54], (float)(three_axis_control_rw->pid_omega[1].gains.d_gain));
  TF_copy_float(&packet[58], (float)(three_axis_control_rw->pid_omega[2].gains.d_gain));
  TF_copy_float(&packet[62], (float)(three_axis_control_rw->pid_att[0].gains.p_gain));
  TF_copy_float(&packet[66], (float)(three_axis_control_rw->pid_att[1].gains.p_gain));
  TF_copy_float(&packet[70], (float)(three_axis_control_rw->pid_att[2].gains.p_gain));
  TF_copy_float(&packet[74], (float)(three_axis_control_rw->pid_att[0].gains.i_gain));
  TF_copy_float(&packet[78], (float)(three_axis_control_rw->pid_att[1].gains.i_gain));
  TF_copy_float(&packet[82], (float)(three_axis_control_rw->pid_att[2].gains.i_gain));
  TF_copy_float(&packet[86], (float)(three_axis_control_rw->pid_att[0].gains.d_gain));
  TF_copy_float(&packet[90], (float)(three_axis_control_rw->pid_att[1].gains.d_gain));
  TF_copy_float(&packet[94], (float)(three_axis_control_rw->pid_att[2].gains.d_gain));
  TF_copy_float(&packet[98], (float)(three_axis_control_rw->gains_omega_cmd_tmp[0].p_gain));
  TF_copy_float(&packet[102], (float)(three_axis_control_rw->gains_omega_cmd_tmp[1].p_gain));
  TF_copy_float(&packet[106], (float)(three_axis_control_rw->gains_omega_cmd_tmp[2].p_gain));
  TF_copy_float(&packet[110], (float)(three_axis_control_rw->gains_omega_cmd_tmp[0].i_gain));
  TF_copy_float(&packet[114], (float)(three_axis_control_rw->gains_omega_cmd_tmp[1].i_gain));
  TF_copy_float(&packet[118], (float)(three_axis_control_rw->gains_omega_cmd_tmp[2].i_gain));
  TF_copy_float(&packet[122], (float)(three_axis_control_rw->gains_omega_cmd_tmp[0].d_gain));
  TF_copy_float(&packet[126], (float)(three_axis_control_rw->gains_omega_cmd_tmp[1].d_gain));
  TF_copy_float(&packet[130], (float)(three_axis_control_rw->gains_omega_cmd_tmp[2].d_gain));
  TF_copy_float(&packet[134], (float)(three_axis_control_rw->gains_att_cmd_tmp[0].p_gain));
  TF_copy_float(&packet[138], (float)(three_axis_control_rw->gains_att_cmd_tmp[1].p_gain));
  TF_copy_float(&packet[142], (float)(three_axis_control_rw->gains_att_cmd_tmp[2].p_gain));
  TF_copy_float(&packet[146], (float)(three_axis_control_rw->gains_att_cmd_tmp[0].i_gain));
  TF_copy_float(&packet[150], (float)(three_axis_control_rw->gains_att_cmd_tmp[1].i_gain));
  TF_copy_float(&packet[154], (float)(three_axis_control_rw->gains_att_cmd_tmp[2].i_gain));
  TF_copy_float(&packet[158], (float)(three_axis_control_rw->gains_att_cmd_tmp[0].d_gain));
  TF_copy_float(&packet[162], (float)(three_axis_control_rw->gains_att_cmd_tmp[1].d_gain));
  TF_copy_float(&packet[166], (float)(three_axis_control_rw->gains_att_cmd_tmp[2].d_gain));
  TF_copy_float(&packet[170], (float)(unloading->angular_velocity_upper_threshold_rad_s[0]));
  TF_copy_float(&packet[174], (float)(unloading->angular_velocity_lower_threshold_rad_s[0]));
  TF_copy_float(&packet[178], (float)(unloading->angular_velocity_target_rad_s[0]));
  TF_copy_float(&packet[182], (float)(unloading->angular_velocity_upper_threshold_rad_s[1]));
  TF_copy_float(&packet[186], (float)(unloading->angular_velocity_lower_threshold_rad_s[1]));
  TF_copy_float(&packet[190], (float)(unloading->angular_velocity_target_rad_s[1]));
  TF_copy_float(&packet[194], (float)(unloading->angular_velocity_upper_threshold_rad_s[2]));
  TF_copy_float(&packet[198], (float)(unloading->angular_velocity_lower_threshold_rad_s[2]));
  TF_copy_float(&packet[202], (float)(unloading->angular_velocity_target_rad_s[2]));
#endif

  *len = 206;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_FILTERS_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (213 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(rw0003_filter->spike_filter_config[0].count_limit_to_accept));
  TF_copy_u8(&packet[27], (uint8_t)(rw0003_filter->spike_filter_config[0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[28], (float)(rw0003_filter->spike_filter_config[0].reject_threshold));
  TF_copy_float(&packet[32], (float)(rw0003_filter->spike_filter_config[0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[36], (uint8_t)(rw0003_filter->filter_error[0]));
  TF_copy_u8(&packet[37], (uint8_t)(rw0003_filter->spike_filter_config[1].count_limit_to_accept));
  TF_copy_u8(&packet[38], (uint8_t)(rw0003_filter->spike_filter_config[1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[39], (float)(rw0003_filter->spike_filter_config[1].reject_threshold));
  TF_copy_float(&packet[43], (float)(rw0003_filter->spike_filter_config[1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[47], (uint8_t)(rw0003_filter->filter_error[1]));
  TF_copy_u8(&packet[48], (uint8_t)(rw0003_filter->spike_filter_config[2].count_limit_to_accept));
  TF_copy_u8(&packet[49], (uint8_t)(rw0003_filter->spike_filter_config[2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[50], (float)(rw0003_filter->spike_filter_config[2].reject_threshold));
  TF_copy_float(&packet[54], (float)(rw0003_filter->spike_filter_config[2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[58], (uint8_t)(rw0003_filter->filter_error[2]));
  TF_copy_u8(&packet[59], (uint8_t)(oem7600_filter->position_spike_filter_config[0].count_limit_to_accept));
  TF_copy_u8(&packet[60], (uint8_t)(oem7600_filter->position_spike_filter_config[0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[61], (float)(oem7600_filter->position_spike_filter_config[0].reject_threshold));
  TF_copy_float(&packet[65], (float)(oem7600_filter->position_spike_filter_config[0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[69], (uint8_t)(oem7600_filter->position_filter_error[0]));
  TF_copy_u8(&packet[70], (uint8_t)(oem7600_filter->velocity_spike_filter_config[0].count_limit_to_accept));
  TF_copy_u8(&packet[71], (uint8_t)(oem7600_filter->velocity_spike_filter_config[0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[72], (float)(oem7600_filter->velocity_spike_filter_config[0].reject_threshold));
  TF_copy_float(&packet[76], (float)(oem7600_filter->velocity_spike_filter_config[0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[80], (uint8_t)(oem7600_filter->velocity_filter_error[0]));
  TF_copy_u8(&packet[81], (uint8_t)(oem7600_filter->position_spike_filter_config[1].count_limit_to_accept));
  TF_copy_u8(&packet[82], (uint8_t)(oem7600_filter->position_spike_filter_config[1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[83], (float)(oem7600_filter->position_spike_filter_config[1].reject_threshold));
  TF_copy_float(&packet[87], (float)(oem7600_filter->position_spike_filter_config[1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[91], (uint8_t)(oem7600_filter->position_filter_error[1]));
  TF_copy_u8(&packet[92], (uint8_t)(oem7600_filter->velocity_spike_filter_config[1].count_limit_to_accept));
  TF_copy_u8(&packet[93], (uint8_t)(oem7600_filter->velocity_spike_filter_config[1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[94], (float)(oem7600_filter->velocity_spike_filter_config[1].reject_threshold));
  TF_copy_float(&packet[98], (float)(oem7600_filter->velocity_spike_filter_config[1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[102], (uint8_t)(oem7600_filter->velocity_filter_error[1]));
  TF_copy_u8(&packet[103], (uint8_t)(oem7600_filter->position_spike_filter_config[2].count_limit_to_accept));
  TF_copy_u8(&packet[104], (uint8_t)(oem7600_filter->position_spike_filter_config[2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[105], (float)(oem7600_filter->position_spike_filter_config[2].reject_threshold));
  TF_copy_float(&packet[109], (float)(oem7600_filter->position_spike_filter_config[2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[113], (uint8_t)(oem7600_filter->position_filter_error[2]));
  TF_copy_u8(&packet[114], (uint8_t)(oem7600_filter->velocity_spike_filter_config[2].count_limit_to_accept));
  TF_copy_u8(&packet[115], (uint8_t)(oem7600_filter->velocity_spike_filter_config[2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[116], (float)(oem7600_filter->velocity_spike_filter_config[2].reject_threshold));
  TF_copy_float(&packet[120], (float)(oem7600_filter->velocity_spike_filter_config[2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[124], (uint8_t)(oem7600_filter->velocity_filter_error[2]));
  TF_copy_u8(&packet[125], (uint8_t)(sagitta_filter->q_i2b_spike_filter_config.count_limit_to_accept));
  TF_copy_u8(&packet[126], (uint8_t)(sagitta_filter->q_i2b_spike_filter_config.count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[127], (float)(sagitta_filter->q_i2b_spike_filter_config.reject_threshold));
  TF_copy_float(&packet[131], (float)(sagitta_filter->q_i2b_spike_filter_config.amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[135], (uint8_t)(sagitta_filter->q_i2b_filter_error));
  TF_copy_u8(&packet[136], (uint8_t)(nanossoc_d60_filter->spike_filter_config.count_limit_to_accept));
  TF_copy_u8(&packet[137], (uint8_t)(nanossoc_d60_filter->spike_filter_config.count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[138], (float)(nanossoc_d60_filter->spike_filter_config.reject_threshold));
  TF_copy_float(&packet[142], (float)(nanossoc_d60_filter->spike_filter_config.amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[146], (uint8_t)(nanossoc_d60_filter->filter_error));
  TF_copy_u8(&packet[147], (uint8_t)(rm3100_filter->spike_filter_config[0][0].count_limit_to_accept));
  TF_copy_u8(&packet[148], (uint8_t)(rm3100_filter->spike_filter_config[0][0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[149], (float)(rm3100_filter->spike_filter_config[0][0].reject_threshold));
  TF_copy_float(&packet[153], (float)(rm3100_filter->spike_filter_config[0][0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[157], (uint8_t)(rm3100_filter->filter_error[0][0]));
  TF_copy_u8(&packet[158], (uint8_t)(rm3100_filter->spike_filter_config[0][1].count_limit_to_accept));
  TF_copy_u8(&packet[159], (uint8_t)(rm3100_filter->spike_filter_config[0][1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[160], (float)(rm3100_filter->spike_filter_config[0][1].reject_threshold));
  TF_copy_float(&packet[164], (float)(rm3100_filter->spike_filter_config[0][1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[168], (uint8_t)(rm3100_filter->filter_error[0][1]));
  TF_copy_u8(&packet[169], (uint8_t)(rm3100_filter->spike_filter_config[0][2].count_limit_to_accept));
  TF_copy_u8(&packet[170], (uint8_t)(rm3100_filter->spike_filter_config[0][2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[171], (float)(rm3100_filter->spike_filter_config[0][2].reject_threshold));
  TF_copy_float(&packet[175], (float)(rm3100_filter->spike_filter_config[0][2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[179], (uint8_t)(rm3100_filter->filter_error[0][2]));
  TF_copy_u8(&packet[180], (uint8_t)(stim210_filter->spike_filter_config[0].count_limit_to_accept));
  TF_copy_u8(&packet[181], (uint8_t)(stim210_filter->spike_filter_config[0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[182], (float)(stim210_filter->spike_filter_config[0].reject_threshold));
  TF_copy_float(&packet[186], (float)(stim210_filter->spike_filter_config[0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[190], (uint8_t)(stim210_filter->filter_error[0]));
  TF_copy_u8(&packet[191], (uint8_t)(stim210_filter->spike_filter_config[1].count_limit_to_accept));
  TF_copy_u8(&packet[192], (uint8_t)(stim210_filter->spike_filter_config[1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[193], (float)(stim210_filter->spike_filter_config[1].reject_threshold));
  TF_copy_float(&packet[197], (float)(stim210_filter->spike_filter_config[1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[201], (uint8_t)(stim210_filter->filter_error[1]));
  TF_copy_u8(&packet[202], (uint8_t)(stim210_filter->spike_filter_config[2].count_limit_to_accept));
  TF_copy_u8(&packet[203], (uint8_t)(stim210_filter->spike_filter_config[2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[204], (float)(stim210_filter->spike_filter_config[2].reject_threshold));
  TF_copy_float(&packet[208], (float)(stim210_filter->spike_filter_config[2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[212], (uint8_t)(stim210_filter->filter_error[2]));
#endif

  *len = 213;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_ANOMALY_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (163 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_PIC].upper_degC);
  TF_copy_float(&packet[30], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_PIC].lower_degC);
  TF_copy_float(&packet[34], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_DCDC].upper_degC);
  TF_copy_float(&packet[38], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_DCDC].lower_degC);
  TF_copy_float(&packet[42], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_ROUGH_GYRO].upper_degC);
  TF_copy_float(&packet[46], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_ROUGH_GYRO].lower_degC);
  TF_copy_float(&packet[50], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_X].upper_degC);
  TF_copy_float(&packet[54], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_X].lower_degC);
  TF_copy_float(&packet[58], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Y].upper_degC);
  TF_copy_float(&packet[62], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Y].lower_degC);
  TF_copy_float(&packet[66], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Z].upper_degC);
  TF_copy_float(&packet[70], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Z].lower_degC);
  TF_copy_float(&packet[74], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_STT_MCU].upper_degC);
  TF_copy_float(&packet[78], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_STT_MCU].lower_degC);
  TF_copy_float(&packet[82], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_STT_FPGA].upper_degC);
  TF_copy_float(&packet[86], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_STT_FPGA].lower_degC);
  TF_copy_float(&packet[90], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_X].upper_degC);
  TF_copy_float(&packet[94], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_X].lower_degC);
  TF_copy_float(&packet[98], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_Y].upper_degC);
  TF_copy_float(&packet[102], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_Y].lower_degC);
  TF_copy_float(&packet[106], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_Z].upper_degC);
  TF_copy_float(&packet[110], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_RW_Z].lower_degC);
  TF_copy_float(&packet[114], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_GPSR].upper_degC);
  TF_copy_float(&packet[118], (float)temperature_anomaly->threshold_list[APP_TEMPERATURE_ANOMALY_IDX_GPSR].lower_degC);
  TF_copy_u8(&packet[122], (uint8_t)(aocs_mode_manager->is_enabled_auto_mode_transition));
  TF_copy_float(&packet[123], (float)(aocs_mode_manager->ang_vel_conv_limit_rad_s));
  TF_copy_float(&packet[127], (float)(aocs_mode_manager->ang_vel_conv_time_limit_s));
  TF_copy_float(&packet[131], (float)(aocs_mode_manager->ang_vel_norm_increase_limit_rad_s));
  TF_copy_float(&packet[135], (float)(aocs_mode_manager->ang_vel_anomaly_detection_period_s));
  TF_copy_float(&packet[139], (float)(aocs_mode_manager->sun_angle_div_limit_rad));
  TF_copy_float(&packet[143], (float)(aocs_mode_manager->sun_angle_div_time_limit_s));
  TF_copy_float(&packet[147], (float)(aocs_mode_manager->three_axis_div_limit_rad));
  TF_copy_float(&packet[151], (float)(aocs_mode_manager->three_axis_div_time_limit_s));
  TF_copy_float(&packet[155], (float)(aocs_mode_manager->sun_invisible_time_limit_s));
  TF_copy_float(&packet[159], (float)(aocs_mode_manager->stt_invisible_time_limit_s));
#endif

  *len = 163;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_APP_TIME_2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (146 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(app_manager->ais[50].init_duration));
  TF_copy_u8(&packet[27], (uint8_t)(app_manager->ais[50].prev));
  TF_copy_u8(&packet[28], (uint8_t)(app_manager->ais[50].min));
  TF_copy_u8(&packet[29], (uint8_t)(app_manager->ais[50].max));
  TF_copy_u8(&packet[30], (uint8_t)(app_manager->ais[51].init_duration));
  TF_copy_u8(&packet[31], (uint8_t)(app_manager->ais[51].prev));
  TF_copy_u8(&packet[32], (uint8_t)(app_manager->ais[51].min));
  TF_copy_u8(&packet[33], (uint8_t)(app_manager->ais[51].max));
  TF_copy_u8(&packet[34], (uint8_t)(app_manager->ais[52].init_duration));
  TF_copy_u8(&packet[35], (uint8_t)(app_manager->ais[52].prev));
  TF_copy_u8(&packet[36], (uint8_t)(app_manager->ais[52].min));
  TF_copy_u8(&packet[37], (uint8_t)(app_manager->ais[52].max));
  TF_copy_u8(&packet[38], (uint8_t)(app_manager->ais[53].init_duration));
  TF_copy_u8(&packet[39], (uint8_t)(app_manager->ais[53].prev));
  TF_copy_u8(&packet[40], (uint8_t)(app_manager->ais[53].min));
  TF_copy_u8(&packet[41], (uint8_t)(app_manager->ais[53].max));
  TF_copy_u8(&packet[42], (uint8_t)(app_manager->ais[54].init_duration));
  TF_copy_u8(&packet[43], (uint8_t)(app_manager->ais[54].prev));
  TF_copy_u8(&packet[44], (uint8_t)(app_manager->ais[54].min));
  TF_copy_u8(&packet[45], (uint8_t)(app_manager->ais[54].max));
  TF_copy_u8(&packet[46], (uint8_t)(app_manager->ais[55].init_duration));
  TF_copy_u8(&packet[47], (uint8_t)(app_manager->ais[55].prev));
  TF_copy_u8(&packet[48], (uint8_t)(app_manager->ais[55].min));
  TF_copy_u8(&packet[49], (uint8_t)(app_manager->ais[55].max));
  TF_copy_u8(&packet[50], (uint8_t)(app_manager->ais[56].init_duration));
  TF_copy_u8(&packet[51], (uint8_t)(app_manager->ais[56].prev));
  TF_copy_u8(&packet[52], (uint8_t)(app_manager->ais[56].min));
  TF_copy_u8(&packet[53], (uint8_t)(app_manager->ais[56].max));
  TF_copy_u8(&packet[54], (uint8_t)(app_manager->ais[57].init_duration));
  TF_copy_u8(&packet[55], (uint8_t)(app_manager->ais[57].prev));
  TF_copy_u8(&packet[56], (uint8_t)(app_manager->ais[57].min));
  TF_copy_u8(&packet[57], (uint8_t)(app_manager->ais[57].max));
  TF_copy_u8(&packet[58], (uint8_t)(app_manager->ais[58].init_duration));
  TF_copy_u8(&packet[59], (uint8_t)(app_manager->ais[58].prev));
  TF_copy_u8(&packet[60], (uint8_t)(app_manager->ais[58].min));
  TF_copy_u8(&packet[61], (uint8_t)(app_manager->ais[58].max));
  TF_copy_u8(&packet[62], (uint8_t)(app_manager->ais[59].init_duration));
  TF_copy_u8(&packet[63], (uint8_t)(app_manager->ais[59].prev));
  TF_copy_u8(&packet[64], (uint8_t)(app_manager->ais[59].min));
  TF_copy_u8(&packet[65], (uint8_t)(app_manager->ais[59].max));
  TF_copy_u8(&packet[66], (uint8_t)(app_manager->ais[60].init_duration));
  TF_copy_u8(&packet[67], (uint8_t)(app_manager->ais[60].prev));
  TF_copy_u8(&packet[68], (uint8_t)(app_manager->ais[60].min));
  TF_copy_u8(&packet[69], (uint8_t)(app_manager->ais[60].max));
  TF_copy_u8(&packet[70], (uint8_t)(app_manager->ais[61].init_duration));
  TF_copy_u8(&packet[71], (uint8_t)(app_manager->ais[61].prev));
  TF_copy_u8(&packet[72], (uint8_t)(app_manager->ais[61].min));
  TF_copy_u8(&packet[73], (uint8_t)(app_manager->ais[61].max));
  TF_copy_u8(&packet[74], (uint8_t)(app_manager->ais[62].init_duration));
  TF_copy_u8(&packet[75], (uint8_t)(app_manager->ais[62].prev));
  TF_copy_u8(&packet[76], (uint8_t)(app_manager->ais[62].min));
  TF_copy_u8(&packet[77], (uint8_t)(app_manager->ais[62].max));
  TF_copy_u8(&packet[78], (uint8_t)(app_manager->ais[63].init_duration));
  TF_copy_u8(&packet[79], (uint8_t)(app_manager->ais[63].prev));
  TF_copy_u8(&packet[80], (uint8_t)(app_manager->ais[63].min));
  TF_copy_u8(&packet[81], (uint8_t)(app_manager->ais[63].max));
  TF_copy_u8(&packet[82], (uint8_t)(app_manager->ais[64].init_duration));
  TF_copy_u8(&packet[83], (uint8_t)(app_manager->ais[64].prev));
  TF_copy_u8(&packet[84], (uint8_t)(app_manager->ais[64].min));
  TF_copy_u8(&packet[85], (uint8_t)(app_manager->ais[64].max));
  TF_copy_u8(&packet[86], (uint8_t)(app_manager->ais[65].init_duration));
  TF_copy_u8(&packet[87], (uint8_t)(app_manager->ais[65].prev));
  TF_copy_u8(&packet[88], (uint8_t)(app_manager->ais[65].min));
  TF_copy_u8(&packet[89], (uint8_t)(app_manager->ais[65].max));
  TF_copy_u8(&packet[90], (uint8_t)(app_manager->ais[66].init_duration));
  TF_copy_u8(&packet[91], (uint8_t)(app_manager->ais[66].prev));
  TF_copy_u8(&packet[92], (uint8_t)(app_manager->ais[66].min));
  TF_copy_u8(&packet[93], (uint8_t)(app_manager->ais[66].max));
  TF_copy_u8(&packet[94], (uint8_t)(app_manager->ais[67].init_duration));
  TF_copy_u8(&packet[95], (uint8_t)(app_manager->ais[67].prev));
  TF_copy_u8(&packet[96], (uint8_t)(app_manager->ais[67].min));
  TF_copy_u8(&packet[97], (uint8_t)(app_manager->ais[67].max));
  TF_copy_u8(&packet[98], (uint8_t)(app_manager->ais[68].init_duration));
  TF_copy_u8(&packet[99], (uint8_t)(app_manager->ais[68].prev));
  TF_copy_u8(&packet[100], (uint8_t)(app_manager->ais[68].min));
  TF_copy_u8(&packet[101], (uint8_t)(app_manager->ais[68].max));
  TF_copy_u8(&packet[102], (uint8_t)(app_manager->ais[69].init_duration));
  TF_copy_u8(&packet[103], (uint8_t)(app_manager->ais[69].prev));
  TF_copy_u8(&packet[104], (uint8_t)(app_manager->ais[69].min));
  TF_copy_u8(&packet[105], (uint8_t)(app_manager->ais[69].max));
  TF_copy_u8(&packet[106], (uint8_t)(app_manager->ais[70].init_duration));
  TF_copy_u8(&packet[107], (uint8_t)(app_manager->ais[70].prev));
  TF_copy_u8(&packet[108], (uint8_t)(app_manager->ais[70].min));
  TF_copy_u8(&packet[109], (uint8_t)(app_manager->ais[70].max));
  TF_copy_u8(&packet[110], (uint8_t)(app_manager->ais[71].init_duration));
  TF_copy_u8(&packet[111], (uint8_t)(app_manager->ais[71].prev));
  TF_copy_u8(&packet[112], (uint8_t)(app_manager->ais[71].min));
  TF_copy_u8(&packet[113], (uint8_t)(app_manager->ais[71].max));
  TF_copy_u8(&packet[114], (uint8_t)(app_manager->ais[72].init_duration));
  TF_copy_u8(&packet[115], (uint8_t)(app_manager->ais[72].prev));
  TF_copy_u8(&packet[116], (uint8_t)(app_manager->ais[72].min));
  TF_copy_u8(&packet[117], (uint8_t)(app_manager->ais[72].max));
  TF_copy_u8(&packet[118], (uint8_t)(app_manager->ais[73].init_duration));
  TF_copy_u8(&packet[119], (uint8_t)(app_manager->ais[73].prev));
  TF_copy_u8(&packet[120], (uint8_t)(app_manager->ais[73].min));
  TF_copy_u8(&packet[121], (uint8_t)(app_manager->ais[73].max));
  TF_copy_u8(&packet[122], (uint8_t)(app_manager->ais[74].init_duration));
  TF_copy_u8(&packet[123], (uint8_t)(app_manager->ais[74].prev));
  TF_copy_u8(&packet[124], (uint8_t)(app_manager->ais[74].min));
  TF_copy_u8(&packet[125], (uint8_t)(app_manager->ais[74].max));
  TF_copy_u8(&packet[126], (uint8_t)(app_manager->ais[75].init_duration));
  TF_copy_u8(&packet[127], (uint8_t)(app_manager->ais[75].prev));
  TF_copy_u8(&packet[128], (uint8_t)(app_manager->ais[75].min));
  TF_copy_u8(&packet[129], (uint8_t)(app_manager->ais[75].max));
  TF_copy_u8(&packet[130], (uint8_t)(app_manager->ais[76].init_duration));
  TF_copy_u8(&packet[131], (uint8_t)(app_manager->ais[76].prev));
  TF_copy_u8(&packet[132], (uint8_t)(app_manager->ais[76].min));
  TF_copy_u8(&packet[133], (uint8_t)(app_manager->ais[76].max));
  TF_copy_u8(&packet[134], (uint8_t)(app_manager->ais[77].init_duration));
  TF_copy_u8(&packet[135], (uint8_t)(app_manager->ais[77].prev));
  TF_copy_u8(&packet[136], (uint8_t)(app_manager->ais[77].min));
  TF_copy_u8(&packet[137], (uint8_t)(app_manager->ais[77].max));
  TF_copy_u8(&packet[138], (uint8_t)(app_manager->ais[78].init_duration));
  TF_copy_u8(&packet[139], (uint8_t)(app_manager->ais[78].prev));
  TF_copy_u8(&packet[140], (uint8_t)(app_manager->ais[78].min));
  TF_copy_u8(&packet[141], (uint8_t)(app_manager->ais[78].max));
  TF_copy_u8(&packet[142], (uint8_t)(app_manager->ais[79].init_duration));
  TF_copy_u8(&packet[143], (uint8_t)(app_manager->ais[79].prev));
  TF_copy_u8(&packet[144], (uint8_t)(app_manager->ais[79].min));
  TF_copy_u8(&packet[145], (uint8_t)(app_manager->ais[79].max));
#endif

  *len = 146;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_SET1_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (229 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[0]));
  TF_copy_u8(&packet[27], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[1]));
  TF_copy_u8(&packet[28], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[2]));
  TF_copy_u8(&packet[29], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[3]));
  TF_copy_u8(&packet[30], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[4]));
  TF_copy_u8(&packet[31], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[5]));
  TF_copy_u8(&packet[32], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[6]));
  TF_copy_u8(&packet[33], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[7]));
  TF_copy_u8(&packet[34], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[8]));
  TF_copy_u8(&packet[35], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[9]));
  TF_copy_u8(&packet[36], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[10]));
  TF_copy_u8(&packet[37], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[11]));
  TF_copy_u8(&packet[38], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[12]));
  TF_copy_u8(&packet[39], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[13]));
  TF_copy_u8(&packet[40], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[14]));
  TF_copy_u8(&packet[41], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.log_level[15]));
  TF_copy_u8(&packet[42], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.action));
  TF_copy_float(&packet[43], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[0]));
  TF_copy_float(&packet[47], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[1]));
  TF_copy_float(&packet[51], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[2]));
  TF_copy_float(&packet[55], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[3]));
  TF_copy_float(&packet[59], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[4]));
  TF_copy_float(&packet[63], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[5]));
  TF_copy_float(&packet[67], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[6]));
  TF_copy_float(&packet[71], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[7]));
  TF_copy_float(&packet[75], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[8]));
  TF_copy_float(&packet[79], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.limits.max_value[9]));
  TF_copy_float(&packet[83], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.mounting.vector_part[0]));
  TF_copy_float(&packet[87], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.mounting.vector_part[1]));
  TF_copy_float(&packet[91], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.mounting.vector_part[2]));
  TF_copy_float(&packet[95], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.mounting.scalar_part));
  TF_copy_u8(&packet[99], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.mode));
  TF_copy_float(&packet[100], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[0]));
  TF_copy_float(&packet[104], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[1]));
  TF_copy_float(&packet[108], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[2]));
  TF_copy_float(&packet[112], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[3]));
  TF_copy_float(&packet[116], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[4]));
  TF_copy_float(&packet[120], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[5]));
  TF_copy_float(&packet[124], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[6]));
  TF_copy_float(&packet[128], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.k0_coefficient[7]));
  TF_copy_float(&packet[132], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[0]));
  TF_copy_float(&packet[136], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[1]));
  TF_copy_float(&packet[140], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[2]));
  TF_copy_float(&packet[144], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[3]));
  TF_copy_float(&packet[148], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[4]));
  TF_copy_float(&packet[152], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[5]));
  TF_copy_float(&packet[156], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[6]));
  TF_copy_float(&packet[160], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.distortion.h0_coefficient[7]));
  TF_copy_u8(&packet[164], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.mode));
  TF_copy_float(&packet[165], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.focal_length_mm));
  TF_copy_float(&packet[169], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.exposure_s));
  TF_copy_float(&packet[173], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.interval_s));
  TF_copy_i16(&packet[177], (int16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.offset_pix));
  TF_copy_u8(&packet[179], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.pga_gain));
  TF_copy_u8(&packet[180], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.adc_gain));
  TF_copy_u8(&packet[181], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[0]));
  TF_copy_u8(&packet[182], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[1]));
  TF_copy_u8(&packet[183], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[2]));
  TF_copy_u8(&packet[184], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[3]));
  TF_copy_u8(&packet[185], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[4]));
  TF_copy_u8(&packet[186], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[5]));
  TF_copy_u8(&packet[187], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[6]));
  TF_copy_u8(&packet[188], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[7]));
  TF_copy_u8(&packet[189], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[8]));
  TF_copy_u8(&packet[190], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[9]));
  TF_copy_u8(&packet[191], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[10]));
  TF_copy_u8(&packet[192], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[11]));
  TF_copy_u8(&packet[193], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[12]));
  TF_copy_u8(&packet[194], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[13]));
  TF_copy_u8(&packet[195], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[14]));
  TF_copy_u8(&packet[196], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.override_register[15]));
  TF_copy_u32(&packet[197], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.camera.freq));
  TF_copy_u8(&packet[201], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.image_processor.mode));
  TF_copy_u8(&packet[202], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.image_processor.store));
  TF_copy_u16(&packet[203], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.image_processor.signal_threshold_pix));
  TF_copy_u16(&packet[205], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.image_processor.dark_threshold_pix));
  TF_copy_u8(&packet[207], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.image_processor.background_compensation));
  TF_copy_float(&packet[208], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.matching.squared_distance_limit_mm));
  TF_copy_float(&packet[212], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.matching.squared_shift_limit_mm));
  TF_copy_float(&packet[216], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.tracking.thin_limit_deg));
  TF_copy_float(&packet[220], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.tracking.outlier_threshold_mm));
  TF_copy_float(&packet[224], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.tracking.outlier_threshold_quest_mm));
  TF_copy_u8(&packet[228], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.tracking.tracker_choice));
#endif

  *len = 229;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_SET2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (231 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.enable_filter));
  TF_copy_float(&packet[27], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.max_quality));
  TF_copy_float(&packet[31], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.dark_threshold_pix));
  TF_copy_float(&packet[35], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.min_quality));
  TF_copy_float(&packet[39], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.max_intensity));
  TF_copy_float(&packet[43], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.min_intensity));
  TF_copy_float(&packet[47], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.max_magnitude));
  TF_copy_float(&packet[51], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.gaussian_cmax_pix));
  TF_copy_float(&packet[55], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.gaussian_cmin_pix));
  TF_copy_float(&packet[59], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.transmatrix_00));
  TF_copy_float(&packet[63], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.transmatrix_01));
  TF_copy_float(&packet[67], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.transmatrix_10));
  TF_copy_float(&packet[71], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.centroiding.transmatrix_11));
  TF_copy_u32(&packet[75], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.mode));
  TF_copy_float(&packet[79], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.prefilter_distance_threshold_mm));
  TF_copy_float(&packet[83], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.prefilter_angle_threshold_rad));
  TF_copy_float(&packet[87], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.field_of_view_width_rad));
  TF_copy_float(&packet[91], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.field_of_view_height_rad));
  TF_copy_float(&packet[95], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.float_star_limit_mm));
  TF_copy_float(&packet[99], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.close_star_limit_mm));
  TF_copy_float(&packet[103], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.rating_weight_close_star_count));
  TF_copy_float(&packet[107], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.rating_weight_fraction_close));
  TF_copy_float(&packet[111], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.rating_weight_mean_sum));
  TF_copy_float(&packet[115], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.rating_weight_db_star_count));
  TF_copy_u8(&packet[119], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.max_combinations));
  TF_copy_u8(&packet[120], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.nr_stars_stop));
  TF_copy_float(&packet[121], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.lisa.fraction_close_stop));
  TF_copy_u8(&packet[125], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.validation.stable_count));
  TF_copy_float(&packet[126], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.validation.max_difference_deg));
  TF_copy_float(&packet[130], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.validation.min_tracker_confidence));
  TF_copy_u8(&packet[134], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.validation.min_matched_stars));
  TF_copy_u8(&packet[135], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.algo.mode));
  TF_copy_float(&packet[136], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.algo.l2t_min_confidence));
  TF_copy_u8(&packet[140], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.algo.l2t_min_matched));
  TF_copy_float(&packet[141], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.algo.t2l_min_confidence));
  TF_copy_u8(&packet[145], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.algo.t2l_min_matched));
  TF_copy_u8(&packet[146], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[0]));
  TF_copy_u8(&packet[147], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[1]));
  TF_copy_u8(&packet[148], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[2]));
  TF_copy_u8(&packet[149], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[3]));
  TF_copy_u8(&packet[150], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[4]));
  TF_copy_u8(&packet[151], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[5]));
  TF_copy_u8(&packet[152], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[6]));
  TF_copy_u8(&packet[153], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[7]));
  TF_copy_u8(&packet[154], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[8]));
  TF_copy_u8(&packet[155], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[9]));
  TF_copy_u8(&packet[156], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[10]));
  TF_copy_u8(&packet[157], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[11]));
  TF_copy_u8(&packet[158], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[12]));
  TF_copy_u8(&packet[159], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[13]));
  TF_copy_u8(&packet[160], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[14]));
  TF_copy_u8(&packet[161], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.subscription[15]));
  TF_copy_u8(&packet[162], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.auto_threshold.mode));
  TF_copy_u8(&packet[163], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.auto_threshold.desired_blobs_count));
  TF_copy_u16(&packet[164], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.auto_threshold.min_threshold));
  TF_copy_u16(&packet[166], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.auto_threshold.max_threshold));
  TF_copy_float(&packet[168], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.auto_threshold.threshold_kp));
  TF_copy_float(&packet[172], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.fast_lisa.limit_angle));
  TF_copy_float(&packet[176], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.fast_lisa.limit_distance));
  TF_copy_u16(&packet[180], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.noise_limits.limit1));
  TF_copy_u16(&packet[182], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.noise_limits.limit2));
  TF_copy_u16(&packet[184], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.noise_limits.min));
  TF_copy_u16(&packet[186], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.noise_limits.max));
  TF_copy_u32(&packet[188], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_oflow_upper));
  TF_copy_u32(&packet[192], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_oflow_lower));
  TF_copy_u32(&packet[196], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_open_upper));
  TF_copy_u32(&packet[200], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_open_lower));
  TF_copy_u32(&packet[204], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_simopen_upper));
  TF_copy_u32(&packet[208], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_simopen_lower));
  TF_copy_u32(&packet[212], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_valid_upper));
  TF_copy_u32(&packet[216], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_valid_lower));
  TF_copy_u32(&packet[220], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.desired_upper));
  TF_copy_u32(&packet[224], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.desired_lower));
  TF_copy_u8(&packet[228], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_width));
  TF_copy_u8(&packet[229], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_height));
  TF_copy_u8(&packet[230], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.set_parameter.blob_filter.max_count));
#endif

  *len = 231;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_READ1_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (229 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[0]));
  TF_copy_u8(&packet[27], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[1]));
  TF_copy_u8(&packet[28], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[2]));
  TF_copy_u8(&packet[29], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[3]));
  TF_copy_u8(&packet[30], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[4]));
  TF_copy_u8(&packet[31], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[5]));
  TF_copy_u8(&packet[32], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[6]));
  TF_copy_u8(&packet[33], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[7]));
  TF_copy_u8(&packet[34], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[8]));
  TF_copy_u8(&packet[35], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[9]));
  TF_copy_u8(&packet[36], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[10]));
  TF_copy_u8(&packet[37], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[11]));
  TF_copy_u8(&packet[38], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[12]));
  TF_copy_u8(&packet[39], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[13]));
  TF_copy_u8(&packet[40], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[14]));
  TF_copy_u8(&packet[41], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.log_level[15]));
  TF_copy_u8(&packet[42], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.action));
  TF_copy_float(&packet[43], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[0]));
  TF_copy_float(&packet[47], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[1]));
  TF_copy_float(&packet[51], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[2]));
  TF_copy_float(&packet[55], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[3]));
  TF_copy_float(&packet[59], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[4]));
  TF_copy_float(&packet[63], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[5]));
  TF_copy_float(&packet[67], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[6]));
  TF_copy_float(&packet[71], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[7]));
  TF_copy_float(&packet[75], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[8]));
  TF_copy_float(&packet[79], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.limits.max_value[9]));
  TF_copy_float(&packet[83], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.mounting.vector_part[0]));
  TF_copy_float(&packet[87], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.mounting.vector_part[1]));
  TF_copy_float(&packet[91], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.mounting.vector_part[2]));
  TF_copy_float(&packet[95], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.mounting.scalar_part));
  TF_copy_u8(&packet[99], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.mode));
  TF_copy_float(&packet[100], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[0]));
  TF_copy_float(&packet[104], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[1]));
  TF_copy_float(&packet[108], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[2]));
  TF_copy_float(&packet[112], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[3]));
  TF_copy_float(&packet[116], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[4]));
  TF_copy_float(&packet[120], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[5]));
  TF_copy_float(&packet[124], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[6]));
  TF_copy_float(&packet[128], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.k0_coefficient[7]));
  TF_copy_float(&packet[132], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[0]));
  TF_copy_float(&packet[136], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[1]));
  TF_copy_float(&packet[140], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[2]));
  TF_copy_float(&packet[144], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[3]));
  TF_copy_float(&packet[148], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[4]));
  TF_copy_float(&packet[152], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[5]));
  TF_copy_float(&packet[156], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[6]));
  TF_copy_float(&packet[160], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.distortion.h0_coefficient[7]));
  TF_copy_u8(&packet[164], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.mode));
  TF_copy_float(&packet[165], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.focal_length_mm));
  TF_copy_float(&packet[169], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.exposure_s));
  TF_copy_float(&packet[173], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.interval_s));
  TF_copy_i16(&packet[177], (int16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.offset_pix));
  TF_copy_u8(&packet[179], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.pga_gain));
  TF_copy_u8(&packet[180], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.adc_gain));
  TF_copy_u8(&packet[181], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[0]));
  TF_copy_u8(&packet[182], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[1]));
  TF_copy_u8(&packet[183], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[2]));
  TF_copy_u8(&packet[184], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[3]));
  TF_copy_u8(&packet[185], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[4]));
  TF_copy_u8(&packet[186], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[5]));
  TF_copy_u8(&packet[187], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[6]));
  TF_copy_u8(&packet[188], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[7]));
  TF_copy_u8(&packet[189], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[8]));
  TF_copy_u8(&packet[190], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[9]));
  TF_copy_u8(&packet[191], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[10]));
  TF_copy_u8(&packet[192], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[11]));
  TF_copy_u8(&packet[193], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[12]));
  TF_copy_u8(&packet[194], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[13]));
  TF_copy_u8(&packet[195], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[14]));
  TF_copy_u8(&packet[196], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.override_register[15]));
  TF_copy_u32(&packet[197], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.camera.freq));
  TF_copy_u8(&packet[201], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.image_processor.mode));
  TF_copy_u8(&packet[202], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.image_processor.store));
  TF_copy_u16(&packet[203], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.image_processor.signal_threshold_pix));
  TF_copy_u16(&packet[205], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.image_processor.dark_threshold_pix));
  TF_copy_u8(&packet[207], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.image_processor.background_compensation));
  TF_copy_float(&packet[208], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.matching.squared_distance_limit_mm));
  TF_copy_float(&packet[212], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.matching.squared_shift_limit_mm));
  TF_copy_float(&packet[216], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.tracking.thin_limit_deg));
  TF_copy_float(&packet[220], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.tracking.outlier_threshold_mm));
  TF_copy_float(&packet[224], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.tracking.outlier_threshold_quest_mm));
  TF_copy_u8(&packet[228], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.tracking.tracker_choice));
#endif

  *len = 229;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA_READ2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (231 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.enable_filter));
  TF_copy_float(&packet[27], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.max_quality));
  TF_copy_float(&packet[31], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.dark_threshold_pix));
  TF_copy_float(&packet[35], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.min_quality));
  TF_copy_float(&packet[39], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.max_intensity));
  TF_copy_float(&packet[43], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.min_intensity));
  TF_copy_float(&packet[47], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.max_magnitude));
  TF_copy_float(&packet[51], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.gaussian_cmax_pix));
  TF_copy_float(&packet[55], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.gaussian_cmin_pix));
  TF_copy_float(&packet[59], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.transmatrix_00));
  TF_copy_float(&packet[63], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.transmatrix_01));
  TF_copy_float(&packet[67], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.transmatrix_10));
  TF_copy_float(&packet[71], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.centroiding.transmatrix_11));
  TF_copy_u32(&packet[75], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.mode));
  TF_copy_float(&packet[79], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.prefilter_distance_threshold_mm));
  TF_copy_float(&packet[83], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.prefilter_angle_threshold_rad));
  TF_copy_float(&packet[87], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.field_of_view_width_rad));
  TF_copy_float(&packet[91], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.field_of_view_height_rad));
  TF_copy_float(&packet[95], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.float_star_limit_mm));
  TF_copy_float(&packet[99], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.close_star_limit_mm));
  TF_copy_float(&packet[103], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.rating_weight_close_star_count));
  TF_copy_float(&packet[107], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.rating_weight_fraction_close));
  TF_copy_float(&packet[111], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.rating_weight_mean_sum));
  TF_copy_float(&packet[115], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.rating_weight_db_star_count));
  TF_copy_u8(&packet[119], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.max_combinations));
  TF_copy_u8(&packet[120], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.nr_stars_stop));
  TF_copy_float(&packet[121], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.lisa.fraction_close_stop));
  TF_copy_u8(&packet[125], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.validation.stable_count));
  TF_copy_float(&packet[126], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.validation.max_difference_deg));
  TF_copy_float(&packet[130], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.validation.min_tracker_confidence));
  TF_copy_u8(&packet[134], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.validation.min_matched_stars));
  TF_copy_u8(&packet[135], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.algo.mode));
  TF_copy_float(&packet[136], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.algo.l2t_min_confidence));
  TF_copy_u8(&packet[140], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.algo.l2t_min_matched));
  TF_copy_float(&packet[141], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.algo.t2l_min_confidence));
  TF_copy_u8(&packet[145], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.algo.t2l_min_matched));
  TF_copy_u8(&packet[146], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[0]));
  TF_copy_u8(&packet[147], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[1]));
  TF_copy_u8(&packet[148], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[2]));
  TF_copy_u8(&packet[149], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[3]));
  TF_copy_u8(&packet[150], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[4]));
  TF_copy_u8(&packet[151], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[5]));
  TF_copy_u8(&packet[152], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[6]));
  TF_copy_u8(&packet[153], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[7]));
  TF_copy_u8(&packet[154], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[8]));
  TF_copy_u8(&packet[155], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[9]));
  TF_copy_u8(&packet[156], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[10]));
  TF_copy_u8(&packet[157], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[11]));
  TF_copy_u8(&packet[158], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[12]));
  TF_copy_u8(&packet[159], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[13]));
  TF_copy_u8(&packet[160], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[14]));
  TF_copy_u8(&packet[161], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.subscription[15]));
  TF_copy_u8(&packet[162], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.auto_threshold.mode));
  TF_copy_u8(&packet[163], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.auto_threshold.desired_blobs_count));
  TF_copy_u16(&packet[164], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.auto_threshold.min_threshold));
  TF_copy_u16(&packet[166], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.auto_threshold.max_threshold));
  TF_copy_float(&packet[168], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.auto_threshold.threshold_kp));
  TF_copy_float(&packet[172], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.fast_lisa.limit_angle));
  TF_copy_float(&packet[176], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.fast_lisa.limit_distance));
  TF_copy_u16(&packet[180], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.noise_limits.limit1));
  TF_copy_u16(&packet[182], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.noise_limits.limit2));
  TF_copy_u16(&packet[184], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.noise_limits.min));
  TF_copy_u16(&packet[186], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.noise_limits.max));
  TF_copy_u32(&packet[188], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_oflow_upper));
  TF_copy_u32(&packet[192], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_oflow_lower));
  TF_copy_u32(&packet[196], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_open_upper));
  TF_copy_u32(&packet[200], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_open_lower));
  TF_copy_u32(&packet[204], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_simopen_upper));
  TF_copy_u32(&packet[208], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_simopen_lower));
  TF_copy_u32(&packet[212], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_valid_upper));
  TF_copy_u32(&packet[216], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_valid_lower));
  TF_copy_u32(&packet[220], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.desired_upper));
  TF_copy_u32(&packet[224], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.desired_lower));
  TF_copy_u8(&packet[228], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_width));
  TF_copy_u8(&packet[229], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_height));
  TF_copy_u8(&packet[230], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.read_parameter.blob_filter.max_count));
#endif

  *len = 231;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_STT_GYRO_EKF1_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (132 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(stt_gyro_ekf->gyro_random_noise.standard_deviation_compo_rad_s[0]));
  TF_copy_float(&packet[30], (float)(stt_gyro_ekf->gyro_random_noise.standard_deviation_compo_rad_s[1]));
  TF_copy_float(&packet[34], (float)(stt_gyro_ekf->gyro_random_noise.standard_deviation_compo_rad_s[2]));
  TF_copy_float(&packet[38], (float)(stt_gyro_ekf->gyro_random_walk.standard_deviation_compo_rad_s2[0]));
  TF_copy_float(&packet[42], (float)(stt_gyro_ekf->gyro_random_walk.standard_deviation_compo_rad_s2[1]));
  TF_copy_float(&packet[46], (float)(stt_gyro_ekf->gyro_random_walk.standard_deviation_compo_rad_s2[2]));
  TF_copy_float(&packet[50], (float)(stt_gyro_ekf->gyro_random_walk.time_constant_s));
  TF_copy_float(&packet[54], (float)(stt_gyro_ekf->stt_error.standard_deviation_compo_rad[0]));
  TF_copy_float(&packet[58], (float)(stt_gyro_ekf->stt_error.standard_deviation_compo_rad[1]));
  TF_copy_float(&packet[62], (float)(stt_gyro_ekf->stt_error.standard_deviation_compo_rad[2]));
  TF_copy_u8(&packet[66], (uint8_t)(stt_gyro_ekf->use_ekf_estimated_attitude));
  TF_copy_u8(&packet[67], (uint8_t)(stt_gyro_ekf->calculation_state));
  TF_copy_float(&packet[68], (float)(stt_gyro_ekf->estimated_result.quaternion_i2b.vector_part[0]));
  TF_copy_float(&packet[72], (float)(stt_gyro_ekf->estimated_result.quaternion_i2b.vector_part[1]));
  TF_copy_float(&packet[76], (float)(stt_gyro_ekf->estimated_result.quaternion_i2b.vector_part[2]));
  TF_copy_float(&packet[80], (float)(stt_gyro_ekf->estimated_result.quaternion_i2b.scalar_part));
  TF_copy_float(&packet[84], (float)(stt_gyro_ekf->estimated_result.rate_bias_body_rad_s[0]));
  TF_copy_float(&packet[88], (float)(stt_gyro_ekf->estimated_result.rate_bias_body_rad_s[1]));
  TF_copy_float(&packet[92], (float)(stt_gyro_ekf->estimated_result.rate_bias_body_rad_s[2]));
  TF_copy_float(&packet[96], (float)(stt_gyro_ekf->estimated_result.angular_velocity_body_rad_s[0]));
  TF_copy_float(&packet[100], (float)(stt_gyro_ekf->estimated_result.angular_velocity_body_rad_s[1]));
  TF_copy_float(&packet[104], (float)(stt_gyro_ekf->estimated_result.angular_velocity_body_rad_s[2]));
  TF_copy_float(&packet[108], (float)(stt_gyro_ekf->initial_covariance.diagonal_component_stt[0]));
  TF_copy_float(&packet[112], (float)(stt_gyro_ekf->initial_covariance.diagonal_component_stt[1]));
  TF_copy_float(&packet[116], (float)(stt_gyro_ekf->initial_covariance.diagonal_component_stt[2]));
  TF_copy_float(&packet[120], (float)(stt_gyro_ekf->initial_covariance.diagonal_component_gyro));
  TF_copy_float(&packet[124], (float)(stt_gyro_ekf->process_noise_covariance.attitude_element));
  TF_copy_float(&packet[128], (float)(stt_gyro_ekf->process_noise_covariance.attitude_rate_element));
#endif

  *len = 132;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_STT_GYRO_EKF2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (170 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[0][0]));
  TF_copy_float(&packet[30], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[0][1]));
  TF_copy_float(&packet[34], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[0][2]));
  TF_copy_float(&packet[38], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[0][3]));
  TF_copy_float(&packet[42], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[0][4]));
  TF_copy_float(&packet[46], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[0][5]));
  TF_copy_float(&packet[50], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[1][0]));
  TF_copy_float(&packet[54], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[1][1]));
  TF_copy_float(&packet[58], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[1][2]));
  TF_copy_float(&packet[62], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[1][3]));
  TF_copy_float(&packet[66], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[1][4]));
  TF_copy_float(&packet[70], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[1][5]));
  TF_copy_float(&packet[74], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[2][0]));
  TF_copy_float(&packet[78], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[2][1]));
  TF_copy_float(&packet[82], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[2][2]));
  TF_copy_float(&packet[86], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[2][3]));
  TF_copy_float(&packet[90], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[2][4]));
  TF_copy_float(&packet[94], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[2][5]));
  TF_copy_float(&packet[98], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[3][0]));
  TF_copy_float(&packet[102], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[3][1]));
  TF_copy_float(&packet[106], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[3][2]));
  TF_copy_float(&packet[110], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[3][3]));
  TF_copy_float(&packet[114], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[3][4]));
  TF_copy_float(&packet[118], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[3][5]));
  TF_copy_float(&packet[122], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[4][0]));
  TF_copy_float(&packet[126], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[4][1]));
  TF_copy_float(&packet[130], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[4][2]));
  TF_copy_float(&packet[134], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[4][3]));
  TF_copy_float(&packet[138], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[4][4]));
  TF_copy_float(&packet[142], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[4][5]));
  TF_copy_float(&packet[146], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[5][0]));
  TF_copy_float(&packet[150], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[5][1]));
  TF_copy_float(&packet[154], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[5][2]));
  TF_copy_float(&packet[158], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[5][3]));
  TF_copy_float(&packet[162], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[5][4]));
  TF_copy_float(&packet[166], (float)(stt_gyro_ekf->estimated_result.covariance_matrix.data[5][5]));
#endif

  *len = 170;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_GPSR_RANGE_P1_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (215 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u16(&packet[26], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[0].prn_slot));
  TF_copy_u8(&packet[28], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[0].is_updated));
  TF_copy_double(&packet[29], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[0].pseudo_range_m));
  TF_copy_double(&packet[37], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[0].carrier_phase_cycle));
  TF_copy_float(&packet[45], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[0].carrier_doppler_Hz));
  TF_copy_float(&packet[49], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[0].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[53], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[1].prn_slot));
  TF_copy_u8(&packet[55], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[1].is_updated));
  TF_copy_double(&packet[56], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[1].pseudo_range_m));
  TF_copy_double(&packet[64], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[1].carrier_phase_cycle));
  TF_copy_float(&packet[72], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[1].carrier_doppler_Hz));
  TF_copy_float(&packet[76], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[1].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[80], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[2].prn_slot));
  TF_copy_u8(&packet[82], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[2].is_updated));
  TF_copy_double(&packet[83], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[2].pseudo_range_m));
  TF_copy_double(&packet[91], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[2].carrier_phase_cycle));
  TF_copy_float(&packet[99], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[2].carrier_doppler_Hz));
  TF_copy_float(&packet[103], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[2].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[107], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[3].prn_slot));
  TF_copy_u8(&packet[109], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[3].is_updated));
  TF_copy_double(&packet[110], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[3].pseudo_range_m));
  TF_copy_double(&packet[118], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[3].carrier_phase_cycle));
  TF_copy_float(&packet[126], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[3].carrier_doppler_Hz));
  TF_copy_float(&packet[130], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[3].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[134], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[4].prn_slot));
  TF_copy_u8(&packet[136], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[4].is_updated));
  TF_copy_double(&packet[137], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[4].pseudo_range_m));
  TF_copy_double(&packet[145], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[4].carrier_phase_cycle));
  TF_copy_float(&packet[153], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[4].carrier_doppler_Hz));
  TF_copy_float(&packet[157], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[4].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[161], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[5].prn_slot));
  TF_copy_u8(&packet[163], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[5].is_updated));
  TF_copy_double(&packet[164], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[5].pseudo_range_m));
  TF_copy_double(&packet[172], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[5].carrier_phase_cycle));
  TF_copy_float(&packet[180], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[5].carrier_doppler_Hz));
  TF_copy_float(&packet[184], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[5].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[188], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[6].prn_slot));
  TF_copy_u8(&packet[190], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[6].is_updated));
  TF_copy_double(&packet[191], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[6].pseudo_range_m));
  TF_copy_double(&packet[199], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[6].carrier_phase_cycle));
  TF_copy_float(&packet[207], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[6].carrier_doppler_Hz));
  TF_copy_float(&packet[211], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[6].carrier_to_noise_ratio_dB_Hz));
#endif

  *len = 215;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_GPSR_RANGE_P2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (161 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u16(&packet[26], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[7].prn_slot));
  TF_copy_u8(&packet[28], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[7].is_updated));
  TF_copy_double(&packet[29], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[7].pseudo_range_m));
  TF_copy_double(&packet[37], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[7].carrier_phase_cycle));
  TF_copy_float(&packet[45], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[7].carrier_doppler_Hz));
  TF_copy_float(&packet[49], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[7].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[53], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[8].prn_slot));
  TF_copy_u8(&packet[55], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[8].is_updated));
  TF_copy_double(&packet[56], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[8].pseudo_range_m));
  TF_copy_double(&packet[64], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[8].carrier_phase_cycle));
  TF_copy_float(&packet[72], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[8].carrier_doppler_Hz));
  TF_copy_float(&packet[76], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[8].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[80], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[9].prn_slot));
  TF_copy_u8(&packet[82], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[9].is_updated));
  TF_copy_double(&packet[83], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[9].pseudo_range_m));
  TF_copy_double(&packet[91], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[9].carrier_phase_cycle));
  TF_copy_float(&packet[99], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[9].carrier_doppler_Hz));
  TF_copy_float(&packet[103], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[9].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[107], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[10].prn_slot));
  TF_copy_u8(&packet[109], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[10].is_updated));
  TF_copy_double(&packet[110], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[10].pseudo_range_m));
  TF_copy_double(&packet[118], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[10].carrier_phase_cycle));
  TF_copy_float(&packet[126], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[10].carrier_doppler_Hz));
  TF_copy_float(&packet[130], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[10].carrier_to_noise_ratio_dB_Hz));
  TF_copy_u16(&packet[134], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[11].prn_slot));
  TF_copy_u8(&packet[136], (uint16_t)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[11].is_updated));
  TF_copy_double(&packet[137], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[11].pseudo_range_m));
  TF_copy_double(&packet[145], (double)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[11].carrier_phase_cycle));
  TF_copy_float(&packet[153], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[11].carrier_doppler_Hz));
  TF_copy_float(&packet[157], (float)(oem7600_driver[OEM7600_IDX_IN_UNIT]->info.range_tlm[11].carrier_to_noise_ratio_dB_Hz));
#endif

  *len = 161;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_ORBIT2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (132 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(orbit_calculator->method));
  TF_copy_u8(&packet[27], (uint8_t)(orbit_calculator->gpsr_update));
  TF_copy_float(&packet[28], (float)(gpsr_orbit_propagator->orbital_elements.semi_major_axis_km));
  TF_copy_float(&packet[32], (float)(gpsr_orbit_propagator->orbital_elements.eccentricity));
  TF_copy_float(&packet[36], (float)(gpsr_orbit_propagator->orbital_elements.inclination_rad));
  TF_copy_float(&packet[40], (float)(gpsr_orbit_propagator->orbital_elements.raan_rad));
  TF_copy_float(&packet[44], (float)(gpsr_orbit_propagator->orbital_elements.arg_perigee_rad));
  TF_copy_double(&packet[48], (double)(gpsr_orbit_propagator->orbital_elements.epoch_jday));
  TF_copy_float(&packet[56], (float)(gpsr_orbit_propagator->orbital_elements_threshold.semi_major_axis_km));
  TF_copy_float(&packet[60], (float)(gpsr_orbit_propagator->orbital_elements_threshold.eccentricity));
  TF_copy_float(&packet[64], (float)(gpsr_orbit_propagator->orbital_elements_threshold.inclination_rad));
  TF_copy_float(&packet[68], (float)(gpsr_orbit_propagator->orbital_elements_threshold.raan_rad));
  TF_copy_float(&packet[72], (float)(gpsr_orbit_propagator->orbital_elements_threshold.arg_perigee_rad));
  TF_copy_double(&packet[76], (double)(gpsr_orbit_propagator->orbital_elements_threshold.epoch_jday));
  TF_copy_float(&packet[84], (float)(orbit_calculator->position_gpsr_kepler_eci_km[0]));
  TF_copy_float(&packet[88], (float)(orbit_calculator->position_gpsr_kepler_eci_km[1]));
  TF_copy_float(&packet[92], (float)(orbit_calculator->position_gpsr_kepler_eci_km[2]));
  TF_copy_float(&packet[96], (float)(orbit_calculator->velocity_gpsr_kepler_eci_km_s[0]));
  TF_copy_float(&packet[100], (float)(orbit_calculator->velocity_gpsr_kepler_eci_km_s[1]));
  TF_copy_float(&packet[104], (float)(orbit_calculator->velocity_gpsr_kepler_eci_km_s[2]));
  TF_copy_float(&packet[108], (float)(aocs_manager->sat_pos_est_eci_m)[0]);
  TF_copy_float(&packet[112], (float)(aocs_manager->sat_pos_est_eci_m)[1]);
  TF_copy_float(&packet[116], (float)(aocs_manager->sat_pos_est_eci_m)[2]);
  TF_copy_float(&packet[120], (float)(aocs_manager->sat_vel_est_eci_m_s)[0]);
  TF_copy_float(&packet[124], (float)(aocs_manager->sat_vel_est_eci_m_s)[1]);
  TF_copy_float(&packet[128], (float)(aocs_manager->sat_vel_est_eci_m_s)[2]);
#endif

  *len = 132;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_FILTERS_2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (197 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(mpu9250_filter->spike_filter_config_mag[0].count_limit_to_accept));
  TF_copy_u8(&packet[27], (uint8_t)(mpu9250_filter->spike_filter_config_mag[0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[28], (float)(mpu9250_filter->spike_filter_config_mag[0].reject_threshold));
  TF_copy_float(&packet[32], (float)(mpu9250_filter->spike_filter_config_mag[0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[36], (uint8_t)(mpu9250_filter->filter_mag_error[0]));
  TF_copy_u8(&packet[37], (uint8_t)(mpu9250_filter->spike_filter_config_mag[1].count_limit_to_accept));
  TF_copy_u8(&packet[38], (uint8_t)(mpu9250_filter->spike_filter_config_mag[1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[39], (float)(mpu9250_filter->spike_filter_config_mag[1].reject_threshold));
  TF_copy_float(&packet[43], (float)(mpu9250_filter->spike_filter_config_mag[1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[47], (uint8_t)(mpu9250_filter->filter_mag_error[1]));
  TF_copy_u8(&packet[48], (uint8_t)(mpu9250_filter->spike_filter_config_mag[2].count_limit_to_accept));
  TF_copy_u8(&packet[49], (uint8_t)(mpu9250_filter->spike_filter_config_mag[2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[50], (float)(mpu9250_filter->spike_filter_config_mag[2].reject_threshold));
  TF_copy_float(&packet[54], (float)(mpu9250_filter->spike_filter_config_mag[2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[58], (uint8_t)(mpu9250_filter->filter_mag_error[2]));
  TF_copy_u8(&packet[59], (uint8_t)(mpu9250_filter->spike_filter_config_gyro[0].count_limit_to_accept));
  TF_copy_u8(&packet[60], (uint8_t)(mpu9250_filter->spike_filter_config_gyro[0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[61], (float)(mpu9250_filter->spike_filter_config_gyro[0].reject_threshold));
  TF_copy_float(&packet[65], (float)(mpu9250_filter->spike_filter_config_gyro[0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[69], (uint8_t)(mpu9250_filter->filter_gyro_error[0]));
  TF_copy_u8(&packet[70], (uint8_t)(mpu9250_filter->spike_filter_config_gyro[1].count_limit_to_accept));
  TF_copy_u8(&packet[71], (uint8_t)(mpu9250_filter->spike_filter_config_gyro[1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[72], (float)(mpu9250_filter->spike_filter_config_gyro[1].reject_threshold));
  TF_copy_float(&packet[76], (float)(mpu9250_filter->spike_filter_config_gyro[1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[80], (uint8_t)(mpu9250_filter->filter_gyro_error[1]));
  TF_copy_u8(&packet[81], (uint8_t)(mpu9250_filter->spike_filter_config_gyro[2].count_limit_to_accept));
  TF_copy_u8(&packet[82], (uint8_t)(mpu9250_filter->spike_filter_config_gyro[2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[83], (float)(mpu9250_filter->spike_filter_config_gyro[2].reject_threshold));
  TF_copy_float(&packet[87], (float)(mpu9250_filter->spike_filter_config_gyro[2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[91], (uint8_t)(mpu9250_filter->filter_gyro_error[2]));
  TF_copy_u8(&packet[92], (uint8_t)(rm3100_filter->spike_filter_config[1][0].count_limit_to_accept));
  TF_copy_u8(&packet[93], (uint8_t)(rm3100_filter->spike_filter_config[1][0].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[94], (float)(rm3100_filter->spike_filter_config[1][0].reject_threshold));
  TF_copy_float(&packet[98], (float)(rm3100_filter->spike_filter_config[1][0].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[102], (uint8_t)(rm3100_filter->filter_error[1][0]));
  TF_copy_u8(&packet[103], (uint8_t)(rm3100_filter->spike_filter_config[1][1].count_limit_to_accept));
  TF_copy_u8(&packet[104], (uint8_t)(rm3100_filter->spike_filter_config[1][1].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[105], (float)(rm3100_filter->spike_filter_config[1][1].reject_threshold));
  TF_copy_float(&packet[109], (float)(rm3100_filter->spike_filter_config[1][1].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[113], (uint8_t)(rm3100_filter->filter_error[1][1]));
  TF_copy_u8(&packet[114], (uint8_t)(rm3100_filter->spike_filter_config[1][2].count_limit_to_accept));
  TF_copy_u8(&packet[115], (uint8_t)(rm3100_filter->spike_filter_config[1][2].count_limit_to_reject_continued_warning));
  TF_copy_float(&packet[116], (float)(rm3100_filter->spike_filter_config[1][2].reject_threshold));
  TF_copy_float(&packet[120], (float)(rm3100_filter->spike_filter_config[1][2].amplitude_limit_to_accept_as_step));
  TF_copy_u8(&packet[124], (uint8_t)(rm3100_filter->filter_error[1][2]));
  TF_copy_float(&packet[125], (float)(mpu9250_filter->sampling_freq_Hz));
  TF_copy_float(&packet[129], (float)(mpu9250_filter->cut_off_freq_lpf_1st_mag_Hz[0]));
  TF_copy_float(&packet[133], (float)(mpu9250_filter->cut_off_freq_lpf_1st_mag_Hz[1]));
  TF_copy_float(&packet[137], (float)(mpu9250_filter->cut_off_freq_lpf_1st_mag_Hz[2]));
  TF_copy_float(&packet[141], (float)(mpu9250_filter->cut_off_freq_lpf_1st_gyro_Hz[0]));
  TF_copy_float(&packet[145], (float)(mpu9250_filter->cut_off_freq_lpf_1st_gyro_Hz[1]));
  TF_copy_float(&packet[149], (float)(mpu9250_filter->cut_off_freq_lpf_1st_gyro_Hz[2]));
  TF_copy_float(&packet[153], (float)(rm3100_filter->sampling_freq_Hz));
  TF_copy_float(&packet[157], (float)(rm3100_filter->cut_off_freq_lpf_1st_Hz[0][0]));
  TF_copy_float(&packet[161], (float)(rm3100_filter->cut_off_freq_lpf_1st_Hz[0][1]));
  TF_copy_float(&packet[165], (float)(rm3100_filter->cut_off_freq_lpf_1st_Hz[0][2]));
  TF_copy_float(&packet[169], (float)(rm3100_filter->cut_off_freq_lpf_1st_Hz[1][0]));
  TF_copy_float(&packet[173], (float)(rm3100_filter->cut_off_freq_lpf_1st_Hz[1][1]));
  TF_copy_float(&packet[177], (float)(rm3100_filter->cut_off_freq_lpf_1st_Hz[1][2]));
  TF_copy_float(&packet[181], (float)(stim210_filter->sampling_freq_Hz));
  TF_copy_float(&packet[185], (float)(stim210_filter->cut_off_freq_lpf_1st_Hz[0]));
  TF_copy_float(&packet[189], (float)(stim210_filter->cut_off_freq_lpf_1st_Hz[1]));
  TF_copy_float(&packet[193], (float)(stim210_filter->cut_off_freq_lpf_1st_Hz[2]));
#endif

  *len = 197;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_DR_ALGORITHM_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (130 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(aocs_data_recorder->read_tlm_algorithm.counter));
  TF_copy_u32(&packet[30], (uint32_t)(aocs_data_recorder->read_tlm_algorithm.obct_total_cycle_recorded));
  TF_copy_double(&packet[34], (double)(aocs_data_recorder->read_tlm_algorithm.reference_jday));
  TF_copy_float(&packet[42], (float)(aocs_data_recorder->read_tlm_algorithm.quaternion_est_i2b.vector_part[0]));
  TF_copy_float(&packet[46], (float)(aocs_data_recorder->read_tlm_algorithm.quaternion_est_i2b.vector_part[1]));
  TF_copy_float(&packet[50], (float)(aocs_data_recorder->read_tlm_algorithm.quaternion_est_i2b.vector_part[2]));
  TF_copy_float(&packet[54], (float)(aocs_data_recorder->read_tlm_algorithm.quaternion_est_i2b.scalar_part));
  TF_copy_float(&packet[58], (float)(aocs_data_recorder->read_tlm_algorithm.ang_vel_est_body_rad_s[0]));
  TF_copy_float(&packet[62], (float)(aocs_data_recorder->read_tlm_algorithm.ang_vel_est_body_rad_s[1]));
  TF_copy_float(&packet[66], (float)(aocs_data_recorder->read_tlm_algorithm.ang_vel_est_body_rad_s[2]));
  TF_copy_float(&packet[70], (float)(aocs_data_recorder->read_tlm_algorithm.sun_vec_est_body[0]));
  TF_copy_float(&packet[74], (float)(aocs_data_recorder->read_tlm_algorithm.sun_vec_est_body[1]));
  TF_copy_float(&packet[78], (float)(aocs_data_recorder->read_tlm_algorithm.sun_vec_est_body[2]));
  TF_copy_float(&packet[82], (float)(aocs_data_recorder->read_tlm_algorithm.mag_vec_est_body_nT[0]));
  TF_copy_float(&packet[86], (float)(aocs_data_recorder->read_tlm_algorithm.mag_vec_est_body_nT[1]));
  TF_copy_float(&packet[90], (float)(aocs_data_recorder->read_tlm_algorithm.mag_vec_est_body_nT[2]));
  TF_copy_float(&packet[94], (float)(aocs_data_recorder->read_tlm_algorithm.internal_torque_target_body_Nm[0]));
  TF_copy_float(&packet[98], (float)(aocs_data_recorder->read_tlm_algorithm.internal_torque_target_body_Nm[1]));
  TF_copy_float(&packet[102], (float)(aocs_data_recorder->read_tlm_algorithm.internal_torque_target_body_Nm[2]));
  TF_copy_float(&packet[106], (float)(aocs_data_recorder->read_tlm_algorithm.external_torque_target_body_Nm[0]));
  TF_copy_float(&packet[110], (float)(aocs_data_recorder->read_tlm_algorithm.external_torque_target_body_Nm[1]));
  TF_copy_float(&packet[114], (float)(aocs_data_recorder->read_tlm_algorithm.external_torque_target_body_Nm[2]));
  TF_copy_float(&packet[118], (float)(aocs_data_recorder->read_tlm_algorithm.rw_angular_velocity_rad_s[0]));
  TF_copy_float(&packet[122], (float)(aocs_data_recorder->read_tlm_algorithm.rw_angular_velocity_rad_s[1]));
  TF_copy_float(&packet[126], (float)(aocs_data_recorder->read_tlm_algorithm.rw_angular_velocity_rad_s[2]));
#endif

  *len = 130;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_RW_DETAILED_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (98 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(rw0003_driver[RW0003_IDX_ON_X]->info.speed_rad_s));
  TF_copy_float(&packet[30], (float)(rw0003_driver[RW0003_IDX_ON_Y]->info.speed_rad_s));
  TF_copy_float(&packet[34], (float)(rw0003_driver[RW0003_IDX_ON_Z]->info.speed_rad_s));
  TF_copy_float(&packet[38], (float)(rw0003_driver[RW0003_IDX_ON_X]->info.temperature_degC));
  TF_copy_float(&packet[42], (float)(rw0003_driver[RW0003_IDX_ON_Y]->info.temperature_degC));
  TF_copy_float(&packet[46], (float)(rw0003_driver[RW0003_IDX_ON_Z]->info.temperature_degC));
  TF_copy_float(&packet[50], (float)(rw0003_driver[RW0003_IDX_ON_X]->info.vdd_V));
  TF_copy_float(&packet[54], (float)(rw0003_driver[RW0003_IDX_ON_Y]->info.vdd_V));
  TF_copy_float(&packet[58], (float)(rw0003_driver[RW0003_IDX_ON_Z]->info.vdd_V));
  TF_copy_float(&packet[62], (float)(rw0003_driver[RW0003_IDX_ON_X]->info.seu_count));
  TF_copy_float(&packet[66], (float)(rw0003_driver[RW0003_IDX_ON_Y]->info.seu_count));
  TF_copy_float(&packet[70], (float)(rw0003_driver[RW0003_IDX_ON_Z]->info.seu_count));
  TF_copy_float(&packet[74], (float)(rw0003_driver[RW0003_IDX_ON_X]->info.fault_state));
  TF_copy_float(&packet[78], (float)(rw0003_driver[RW0003_IDX_ON_Y]->info.fault_state));
  TF_copy_float(&packet[82], (float)(rw0003_driver[RW0003_IDX_ON_Z]->info.fault_state));
  TF_copy_i32(&packet[86], (int32_t)(rw0003_driver[RW0003_IDX_ON_X]->info.diagnostic_reset_reason));
  TF_copy_i32(&packet[90], (int32_t)(rw0003_driver[RW0003_IDX_ON_Y]->info.diagnostic_reset_reason));
  TF_copy_i32(&packet[94], (int32_t)(rw0003_driver[RW0003_IDX_ON_Z]->info.diagnostic_reset_reason));
#endif

  *len = 98;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA1_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (214 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_float(&packet[26], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.vector_part[0]));
  TF_copy_float(&packet[30], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.vector_part[1]));
  TF_copy_float(&packet[34], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.vector_part[2]));
  TF_copy_float(&packet[38], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.quaternion_i2c.scalar_part));
  TF_copy_float(&packet[42], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.track_confidence));
  TF_copy_float(&packet[46], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.track_quaternion_i2c.vector_part[0]));
  TF_copy_float(&packet[50], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.track_quaternion_i2c.vector_part[1]));
  TF_copy_float(&packet[54], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.track_quaternion_i2c.vector_part[2]));
  TF_copy_float(&packet[58], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.track_quaternion_i2c.scalar_part));
  TF_copy_u8(&packet[62], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.num_stars_removed));
  TF_copy_u8(&packet[63], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.num_stars_centroided));
  TF_copy_u8(&packet[64], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.num_stars_matched));
  TF_copy_float(&packet[65], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.lisa_quaternion_i2c.vector_part[0]));
  TF_copy_float(&packet[69], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.lisa_quaternion_i2c.vector_part[1]));
  TF_copy_float(&packet[73], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.lisa_quaternion_i2c.vector_part[2]));
  TF_copy_float(&packet[77], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.lisa_quaternion_i2c.scalar_part));
  TF_copy_float(&packet[81], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.lisa_percentage_close));
  TF_copy_u8(&packet[85], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.num_stars_lisa_close));
  TF_copy_u8(&packet[86], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.star_tracker_mode));
  TF_copy_u8(&packet[87], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.is_valid_quaternion));
  TF_copy_u32(&packet[88], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.stable_count));
  TF_copy_u8(&packet[92], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.solution.solution_strategy));
  TF_copy_u8(&packet[93], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.count));
  TF_copy_u32(&packet[94], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[0]));
  TF_copy_float(&packet[98], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[0]));
  TF_copy_float(&packet[102], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[0]));
  TF_copy_float(&packet[106], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[0]));
  TF_copy_float(&packet[110], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[0]));
  TF_copy_u32(&packet[114], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[1]));
  TF_copy_float(&packet[118], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[1]));
  TF_copy_float(&packet[122], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[1]));
  TF_copy_float(&packet[126], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[1]));
  TF_copy_float(&packet[130], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[1]));
  TF_copy_u32(&packet[134], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[2]));
  TF_copy_float(&packet[138], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[2]));
  TF_copy_float(&packet[142], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[2]));
  TF_copy_float(&packet[146], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[2]));
  TF_copy_float(&packet[150], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[2]));
  TF_copy_u32(&packet[154], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[3]));
  TF_copy_float(&packet[158], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[3]));
  TF_copy_float(&packet[162], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[3]));
  TF_copy_float(&packet[166], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[3]));
  TF_copy_float(&packet[170], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[3]));
  TF_copy_u32(&packet[174], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[4]));
  TF_copy_float(&packet[178], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[4]));
  TF_copy_float(&packet[182], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[4]));
  TF_copy_float(&packet[186], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[4]));
  TF_copy_float(&packet[190], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[4]));
  TF_copy_u32(&packet[194], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[5]));
  TF_copy_float(&packet[198], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[5]));
  TF_copy_float(&packet[202], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[5]));
  TF_copy_float(&packet[206], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[5]));
  TF_copy_float(&packet[210], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[5]));
#endif

  *len = 214;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA2_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (226 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[6]));
  TF_copy_float(&packet[30], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[6]));
  TF_copy_float(&packet[34], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[6]));
  TF_copy_float(&packet[38], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[6]));
  TF_copy_float(&packet[42], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[6]));
  TF_copy_u32(&packet[46], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[7]));
  TF_copy_float(&packet[50], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[7]));
  TF_copy_float(&packet[54], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[7]));
  TF_copy_float(&packet[58], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[7]));
  TF_copy_float(&packet[62], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[7]));
  TF_copy_u32(&packet[66], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[8]));
  TF_copy_float(&packet[70], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[8]));
  TF_copy_float(&packet[74], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[8]));
  TF_copy_float(&packet[78], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[8]));
  TF_copy_float(&packet[82], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[8]));
  TF_copy_u32(&packet[86], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[9]));
  TF_copy_float(&packet[90], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[9]));
  TF_copy_float(&packet[94], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[9]));
  TF_copy_float(&packet[98], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[9]));
  TF_copy_float(&packet[102], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[9]));
  TF_copy_u32(&packet[106], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[10]));
  TF_copy_float(&packet[110], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[10]));
  TF_copy_float(&packet[114], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[10]));
  TF_copy_float(&packet[118], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[10]));
  TF_copy_float(&packet[122], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[10]));
  TF_copy_u32(&packet[126], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[11]));
  TF_copy_float(&packet[130], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[11]));
  TF_copy_float(&packet[134], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[11]));
  TF_copy_float(&packet[138], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[11]));
  TF_copy_float(&packet[142], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[11]));
  TF_copy_u32(&packet[146], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[12]));
  TF_copy_float(&packet[150], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[12]));
  TF_copy_float(&packet[154], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[12]));
  TF_copy_float(&packet[158], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[12]));
  TF_copy_float(&packet[162], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[12]));
  TF_copy_u32(&packet[166], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[13]));
  TF_copy_float(&packet[170], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[13]));
  TF_copy_float(&packet[174], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[13]));
  TF_copy_float(&packet[178], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[13]));
  TF_copy_float(&packet[182], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[13]));
  TF_copy_u32(&packet[186], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[14]));
  TF_copy_float(&packet[190], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[14]));
  TF_copy_float(&packet[194], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[14]));
  TF_copy_float(&packet[198], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[14]));
  TF_copy_float(&packet[202], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[14]));
  TF_copy_u32(&packet[206], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.id[15]));
  TF_copy_float(&packet[210], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.x_coordinate[15]));
  TF_copy_float(&packet[214], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.y_coordinate[15]));
  TF_copy_float(&packet[218], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_x_coordinate[15]));
  TF_copy_float(&packet[222], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.matched_centroids.error_y_coordinate[15]));
#endif

  *len = 226;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA3_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (226 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u16(&packet[26], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.count));
  TF_copy_u16(&packet[28], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.count_used));
  TF_copy_u16(&packet[30], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.four_lines_skipped));
  TF_copy_u16(&packet[32], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[0]));
  TF_copy_u16(&packet[34], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[0]));
  TF_copy_u16(&packet[36], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[1]));
  TF_copy_u16(&packet[38], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[1]));
  TF_copy_u16(&packet[40], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[2]));
  TF_copy_u16(&packet[42], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[2]));
  TF_copy_u16(&packet[44], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[3]));
  TF_copy_u16(&packet[46], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[3]));
  TF_copy_u16(&packet[48], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[4]));
  TF_copy_u16(&packet[50], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[4]));
  TF_copy_u16(&packet[52], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[5]));
  TF_copy_u16(&packet[54], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[5]));
  TF_copy_u16(&packet[56], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[6]));
  TF_copy_u16(&packet[58], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[6]));
  TF_copy_u16(&packet[60], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.x_coordinate[7]));
  TF_copy_u16(&packet[62], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blobs.y_coordinate[7]));
  TF_copy_u16(&packet[64], (uint16_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.count));
  TF_copy_float(&packet[66], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[0]));
  TF_copy_float(&packet[70], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[0]));
  TF_copy_u8(&packet[74], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[0]));
  TF_copy_float(&packet[75], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[1]));
  TF_copy_float(&packet[79], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[1]));
  TF_copy_u8(&packet[83], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[1]));
  TF_copy_float(&packet[84], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[2]));
  TF_copy_float(&packet[88], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[2]));
  TF_copy_u8(&packet[92], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[2]));
  TF_copy_float(&packet[93], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[3]));
  TF_copy_float(&packet[97], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[3]));
  TF_copy_u8(&packet[101], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[3]));
  TF_copy_float(&packet[102], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[4]));
  TF_copy_float(&packet[106], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[4]));
  TF_copy_u8(&packet[110], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[4]));
  TF_copy_float(&packet[111], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[5]));
  TF_copy_float(&packet[115], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[5]));
  TF_copy_u8(&packet[119], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[5]));
  TF_copy_float(&packet[120], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[6]));
  TF_copy_float(&packet[124], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[6]));
  TF_copy_u8(&packet[128], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[6]));
  TF_copy_float(&packet[129], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[7]));
  TF_copy_float(&packet[133], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[7]));
  TF_copy_u8(&packet[137], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[7]));
  TF_copy_float(&packet[138], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[8]));
  TF_copy_float(&packet[142], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[8]));
  TF_copy_u8(&packet[146], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[8]));
  TF_copy_float(&packet[147], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[9]));
  TF_copy_float(&packet[151], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[9]));
  TF_copy_u8(&packet[155], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[9]));
  TF_copy_float(&packet[156], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[10]));
  TF_copy_float(&packet[160], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[10]));
  TF_copy_u8(&packet[164], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[10]));
  TF_copy_float(&packet[165], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[11]));
  TF_copy_float(&packet[169], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[11]));
  TF_copy_u8(&packet[173], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[11]));
  TF_copy_float(&packet[174], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[12]));
  TF_copy_float(&packet[178], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[12]));
  TF_copy_u8(&packet[182], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[12]));
  TF_copy_float(&packet[183], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[13]));
  TF_copy_float(&packet[187], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[13]));
  TF_copy_u8(&packet[191], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[13]));
  TF_copy_float(&packet[192], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[14]));
  TF_copy_float(&packet[196], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[14]));
  TF_copy_u8(&packet[200], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[14]));
  TF_copy_float(&packet[201], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.x_coordinate[15]));
  TF_copy_float(&packet[205], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.y_coordinate[15]));
  TF_copy_u8(&packet[209], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.centroids.magnitude[15]));
  TF_copy_float(&packet[210], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.auto_blob_threshold));
  TF_copy_float(&packet[214], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.temperature.mcu_degC));
  TF_copy_float(&packet[222], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.temperature.fpga_degC));
#endif

  *len = 226;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA4_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (234 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[0]));
  TF_copy_u32(&packet[30], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[1]));
  TF_copy_u32(&packet[34], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[2]));
  TF_copy_u32(&packet[38], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[3]));
  TF_copy_u32(&packet[42], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[4]));
  TF_copy_u32(&packet[46], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[5]));
  TF_copy_u32(&packet[50], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[6]));
  TF_copy_u32(&packet[54], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[7]));
  TF_copy_u32(&packet[58], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_a_pix[8]));
  TF_copy_u32(&packet[62], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[0]));
  TF_copy_u32(&packet[66], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[1]));
  TF_copy_u32(&packet[70], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[2]));
  TF_copy_u32(&packet[74], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[3]));
  TF_copy_u32(&packet[78], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[4]));
  TF_copy_u32(&packet[82], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[5]));
  TF_copy_u32(&packet[86], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[6]));
  TF_copy_u32(&packet[90], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[7]));
  TF_copy_u32(&packet[94], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_b_pix[8]));
  TF_copy_u32(&packet[98], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[0]));
  TF_copy_u32(&packet[102], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[1]));
  TF_copy_u32(&packet[106], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[2]));
  TF_copy_u32(&packet[110], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[3]));
  TF_copy_u32(&packet[114], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[4]));
  TF_copy_u32(&packet[118], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[5]));
  TF_copy_u32(&packet[122], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[6]));
  TF_copy_u32(&packet[126], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[7]));
  TF_copy_u32(&packet[130], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_c_pix[8]));
  TF_copy_u32(&packet[134], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[0]));
  TF_copy_u32(&packet[138], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[1]));
  TF_copy_u32(&packet[142], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[2]));
  TF_copy_u32(&packet[146], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[3]));
  TF_copy_u32(&packet[150], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[4]));
  TF_copy_u32(&packet[154], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[5]));
  TF_copy_u32(&packet[158], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[6]));
  TF_copy_u32(&packet[162], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[7]));
  TF_copy_u32(&packet[166], (uint32_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.histogram_d_pix[8]));
  TF_copy_float(&packet[170], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.mcu_current_A));
  TF_copy_float(&packet[174], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.mcu_voltage_V));
  TF_copy_float(&packet[178], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.fpga_core_current_A));
  TF_copy_float(&packet[182], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.fpga_core_voltage_V));
  TF_copy_float(&packet[186], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.fpga_18_current_A));
  TF_copy_float(&packet[190], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.fpga_18_voltage_V));
  TF_copy_float(&packet[194], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.fpga_25_current_A));
  TF_copy_float(&packet[198], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.fpga_25_voltage_V));
  TF_copy_float(&packet[202], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_21_current_A));
  TF_copy_float(&packet[206], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_21_voltage_V));
  TF_copy_float(&packet[210], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_pix_current_A));
  TF_copy_float(&packet[214], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_pix_voltage_V));
  TF_copy_float(&packet[218], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_33_current_A));
  TF_copy_float(&packet[222], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_33_voltage_V));
  TF_copy_float(&packet[226], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_res_current_A));
  TF_copy_float(&packet[230], (float)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.power.cmv_res_voltage_V));
#endif

  *len = 234;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_SAGITTA5_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (90 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u8(&packet[26], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[0]));
  TF_copy_u8(&packet[27], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[1]));
  TF_copy_u8(&packet[28], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[2]));
  TF_copy_u8(&packet[29], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[3]));
  TF_copy_u8(&packet[30], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[4]));
  TF_copy_u8(&packet[31], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[5]));
  TF_copy_u8(&packet[32], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[6]));
  TF_copy_u8(&packet[33], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[7]));
  TF_copy_u8(&packet[34], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[8]));
  TF_copy_u8(&packet[35], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[9]));
  TF_copy_u8(&packet[36], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[10]));
  TF_copy_u8(&packet[37], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[11]));
  TF_copy_u8(&packet[38], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[12]));
  TF_copy_u8(&packet[39], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[13]));
  TF_copy_u8(&packet[40], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[14]));
  TF_copy_u8(&packet[41], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.noise_limit[15]));
  TF_copy_u8(&packet[42], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[0]));
  TF_copy_u8(&packet[43], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[1]));
  TF_copy_u8(&packet[44], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[2]));
  TF_copy_u8(&packet[45], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[3]));
  TF_copy_u8(&packet[46], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[4]));
  TF_copy_u8(&packet[47], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[5]));
  TF_copy_u8(&packet[48], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[6]));
  TF_copy_u8(&packet[49], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[7]));
  TF_copy_u8(&packet[50], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[8]));
  TF_copy_u8(&packet[51], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[9]));
  TF_copy_u8(&packet[52], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[10]));
  TF_copy_u8(&packet[53], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[11]));
  TF_copy_u8(&packet[54], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[12]));
  TF_copy_u8(&packet[55], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[13]));
  TF_copy_u8(&packet[56], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[14]));
  TF_copy_u8(&packet[57], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.brightness_threshold[15]));
  TF_copy_u8(&packet[58], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[0]));
  TF_copy_u8(&packet[59], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[1]));
  TF_copy_u8(&packet[60], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[2]));
  TF_copy_u8(&packet[61], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[3]));
  TF_copy_u8(&packet[62], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[4]));
  TF_copy_u8(&packet[63], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[5]));
  TF_copy_u8(&packet[64], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[6]));
  TF_copy_u8(&packet[65], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[7]));
  TF_copy_u8(&packet[66], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[8]));
  TF_copy_u8(&packet[67], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[9]));
  TF_copy_u8(&packet[68], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[10]));
  TF_copy_u8(&packet[69], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[11]));
  TF_copy_u8(&packet[70], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[12]));
  TF_copy_u8(&packet[71], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[13]));
  TF_copy_u8(&packet[72], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[14]));
  TF_copy_u8(&packet[73], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_valid_blobs[15]));
  TF_copy_u8(&packet[74], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[0]));
  TF_copy_u8(&packet[75], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[1]));
  TF_copy_u8(&packet[76], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[2]));
  TF_copy_u8(&packet[77], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[3]));
  TF_copy_u8(&packet[78], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[4]));
  TF_copy_u8(&packet[79], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[5]));
  TF_copy_u8(&packet[80], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[6]));
  TF_copy_u8(&packet[81], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[7]));
  TF_copy_u8(&packet[82], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[8]));
  TF_copy_u8(&packet[83], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[9]));
  TF_copy_u8(&packet[84], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[10]));
  TF_copy_u8(&packet[85], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[11]));
  TF_copy_u8(&packet[86], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[12]));
  TF_copy_u8(&packet[87], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[13]));
  TF_copy_u8(&packet[88], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[14]));
  TF_copy_u8(&packet[89], (uint8_t)(sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.telemetry.blob_stats.num_overflows[15]));
#endif

  *len = 90;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

#pragma section
