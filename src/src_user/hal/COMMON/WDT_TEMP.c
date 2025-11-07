// C2A_MainがWatchDogを持っているので、対応する空関数を準備してコンパイルを無理やり通す
#include <src_core/IfWrapper/wdt.h>

#pragma section

int WDT_initialize(void* my_wdt_v)
{
  return 0;
}

int WDT_clear(void* my_wdt_v)
{
  return 0;
}

int WDT_enable(void* my_wdt_v)
{
  return 0;
}

int WDT_disable(void* my_wdt_v)
{
  return 0;
}

int WDT_set_timer(void* my_wdt_v, int time)
{
  return 0;
}
