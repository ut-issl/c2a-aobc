/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
 */
#ifndef UART_SILS_IF_H_
#define UART_SILS_IF_H_

#include "com_port.h"


// 最初だけ初期化して、プログラム終了時にポートを閉じるようにしたい
// TODO: ポート番号を外部ファイルから設定できるようにする
#ifdef WIN32
static ComPort uart_sils_com_port(11);
#else
static ComPort uart_sils_com_port(1);
#endif

int uart_sils_if_init();
int uart_sils_if_tx(unsigned char* data_v, int count);
int uart_sils_if_rx(unsigned char* data_v, int count);

#endif
