/**
 * @file
 * @brief  AMのCoreパラメタのオーバーライド用ヘッダー
 */
#ifndef APP_MANAGER_PARAMS_H_
#define APP_MANAGER_PARAMS_H_

#undef AM_TLM_PAGE_SIZE
#undef AM_TLM_PAGE_MAX
#undef AM_MAX_APPS

#define AM_TLM_PAGE_SIZE (16)
#define AM_TLM_PAGE_MAX (5)
#define AM_MAX_APPS (AM_TLM_PAGE_SIZE * AM_TLM_PAGE_MAX)

#endif
