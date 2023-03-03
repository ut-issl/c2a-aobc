#pragma section REPRO // いずれこのアプリは消すので#pragma section REPROは不要かもだがいちおう

#include "module_test_bench.h"

#include <src_core/Library/print.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/TimeManager/obc_time.h>
#include "../../Library/c2a_math.h"
#include "../../Library/math_constants.h"
#include "../../Library/physical_constants.h"
#include "AOCS/aocs_manager.h"

// #define RUN_EXAMPLE //!< Enable test of the test bench

static void APP_MODULE_TEST_BENCH_init_(void);
static void APP_MODULE_TEST_BENCH_exec_(void);

AppInfo APP_MODULE_TEST_BENCH_create_app(void)
{
  return AI_create_app_info("ModuleTestBench",
                            APP_MODULE_TEST_BENCH_init_, APP_MODULE_TEST_BENCH_exec_);
}

static void APP_MODULE_TEST_BENCH_init_(void)
{
  // add initialization process here if necessary

  return;
}

// このテストベンチを使う際には，"src_user/Settings/Modes/TaskLists/InitialTL.c"中の，
// BCL_tool_register_app(90, AR_APP_MODULE_TEST_BENCH)のコメントアウトを解除すること．
static void APP_MODULE_TEST_BENCH_exec_(void)
{
  // the following is an example
#ifdef RUN_EXAMPLE
  float angle_origin = MATH_CONST_2PI * 10.0f;
  float angle_ans[10];
  for (int i = 0; i < 10; i++)
  {
    float angle_input = angle_origin + 0.5f * MATH_CONST_PI * (float)(i);
    angle_ans[i] = C2A_MATH_clip_angle_zero_2pi(angle_input);
    Printf("angle in , %f, angle out, %f \n", angle_input, angle_ans[i]);
  }
#endif

  return;
}

#pragma section
