// Arduino用Print関数

#include "./print_arduino.h"
#include <Arduino.h>

void printf_debug_init(void)
{
  Serial.begin(115200);
}

void printf_debug(char* s)
{
  Serial.print(s);
}
