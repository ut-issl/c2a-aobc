#ifndef CCSDS_SILS_IF_H_
#define CCSDS_SILS_IF_H_

#include <stddef.h>
#include "com_port.h"


// 最初だけ初期化して、プログラム終了時にポートを閉じるようにしたい
// TODO: ポート番号を外部ファイルから設定できるようにする
#ifdef WIN32
static ComPort ccsds_sils_com_port(11);
#else
static ComPort ccsds_sils_com_port(1);
#endif

int ccsds_sils_if_init();
int ccsds_sils_if_tx(unsigned char* data_v, int count);
int ccsds_sils_if_rx(unsigned char* data_v, int count);

#endif
