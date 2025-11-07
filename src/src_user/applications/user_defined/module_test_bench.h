/**
* @file   module_test_bench.h
* @brief  新しい関数モジュール(**.h/.c)追加時のテストベンチ用アプリ
* @note   関数モジュール(**.h/.c)追加作業がひと段落した時点で，ファイルごと消去することを想定
* @note   このテストベンチを使う際には，"src_user/Settings/Modes/TaskLists/InitialTL.c"中の，
*         BCL_tool_register_app(90, AR_APP_MODULE_TEST_BENCH)のコメントアウトを解除すること．
*/
#ifndef MODULE_TEST_BENCH_H_
#define MODULE_TEST_BENCH_H_

#include <src_core/System/ApplicationManager/app_info.h>

AppInfo APP_MODULE_TEST_BENCH_create_app(void);


#endif
