/**
 * @file
 * @brief  PHのCoreTlmパラメタのオーバーライド用ヘッダー
 */
#ifndef PACKET_HANDLER_PARAMS_H_
#define PACKET_HANDLER_PARAMS_H_

#undef TL_TLM_PAGE_SIZE
#undef TL_TLM_PAGE_MAX

#undef PH_GSC_LIST_MAX
#undef PH_RTC_LIST_MAX
#undef PH_TLC_GS_LIST_MAX
#undef PH_TLC_BC_LIST_MAX
#undef PH_TLC_TLM_LIST_MAX
#undef PH_TLC_MIS_LIST_MAX
#undef PH_RT_TLM_LIST_MAX

#define TL_TLM_PAGE_SIZE (16)
#define TL_TLM_PAGE_MAX  (1)

#define PH_GSC_LIST_MAX  (4)
#define PH_RTC_LIST_MAX  (16)
#define PH_TLC_GS_LIST_MAX  (TL_TLM_PAGE_SIZE * TL_TLM_PAGE_MAX) // コメント追加（2019/08/19 鈴本）
                                                                 // これが最長じゃないといけない
                                                                 // timeline_command_dispatcher.tlm_info_.tl_list の長さがこれなので
#define PH_TLC_BC_LIST_MAX  (TL_TLM_PAGE_SIZE * 2)
#define PH_TLC_TLM_LIST_MAX (TL_TLM_PAGE_SIZE * 1)
#define PH_RT_TLM_LIST_MAX  (4)

#endif
