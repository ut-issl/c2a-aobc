/**
 * @file
 * @brief  HW 依存 Print, つまり ログ情報のフォーマット出力（デバッグ出力）
 */

#include <src_core/Library/print.h>

#include <stdio.h>
#include <stdarg.h>

#include "./print_arduino.h"

// バッファサイズよりでかい文字列が来ると死ぬ
static char PRINT_buffer_[512];

#ifndef SILS_FW

void Printf(const char* format, ...)
{
  int threshold = 0;
  va_list argptr;

  va_start(argptr, format);
  threshold = vsprintf(PRINT_buffer_, format, argptr);

  printf_debug(PRINT_buffer_); // H/W dependent
  va_end(argptr);
}

#else // SILS用

void Printf(const char* format, ...)
{
  // SILS環境が不明なので一旦コメントアウト 2020/08/22 鈴本
  // va_list args;
  // va_start(args, format);
  // //printf(format, args);
  // va_end(args);
}

#endif
