// Arduino用Print関数

#include "./print_arduino.h"
#include "arduino.h"

void printf_debug(char* s)
{
  Serial.print(s);
}
