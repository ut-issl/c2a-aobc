#pragma section REPRO
/**
 * @file
 * @brief   CCSDS_SILS_SCI_IF
 * @author  鈴本 遼
 * @date    2020/12/28
 * @details WINGS TMTC IFとCCSDSのTransfer FrameをSCI COMでやりとりするIF
            Windows上でcom0comを使うことを想定
            ComPort classは基本的にEQU ZEUSのコードを流用
 */

#include "ccsds_sils_if.h"


int ccsds_sils_if_init()
{
  return 0;
}

int ccsds_sils_if_tx(unsigned char* data_v, int count)
{
  ccsds_sils_com_port.send(data_v, 0, count);
  return 0;
}

int ccsds_sils_if_rx(unsigned char* data_v, int count)
{
  return ccsds_sils_com_port.receive(data_v, 0, count);
}

#pragma section
