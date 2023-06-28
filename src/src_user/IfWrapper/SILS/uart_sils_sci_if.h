/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
 */
#ifndef UART_SILS_SCI_IF_H_
#define UART_SILS_SCI_IF_H_

#include "sils_sci_if.h"


// 最初だけ初期化して、プログラム終了時にポートを閉じるようにしたい
// TODO: ポート番号を外部ファイルから設定できるようにする
#ifdef WIN32
static SCIComPort SILS_SCI_IF_sci_com_uart_(11);
#else
static SCIComPort SILS_SCI_IF_sci_com_uart_(1);
#endif

int SILS_SCI_UART_IF_init();
int SILS_SCI_UART_IF_tx(unsigned char* data_v, int count);
int SILS_SCI_UART_IF_rx(unsigned char* data_v, int count);

#endif
