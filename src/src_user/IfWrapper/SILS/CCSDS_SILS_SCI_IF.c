#pragma section REPRO
/**
 * @file
 * @brief   CCSDS_SILS_SCI_IF
 * @author  鈴本 遼
 * @date    2020/12/28
 * @details WINGS TMTC IFとCCSDSのTransfer FrameをSCI COMでやりとりするIF
            Windows上でcom0comを使うことを想定
            SCIComPort classは基本的にEQU ZEUSのコードを流用
 */

#include "CCSDS_SILS_SCI_IF.h"


int SILS_SIC_IF_init()
{
  return 0;
}

int SILS_SIC_IF_TX(unsigned char* data_v, int count)
{
  SILS_SCI_IF_sci_com_.Send(data_v, 0, count);
  return 0;
}

int SILS_SIC_IF_RX(unsigned char* data_v, int count)
{
  return SILS_SCI_IF_sci_com_.Receive(data_v, 0, count);
}

#pragma section
