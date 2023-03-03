#pragma section REPRO
/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
            ccsds_sils_sci_if.c/hのほぼコピー
 */

#include "uart_sils_sci_if.h"

 // 最初だけ初期化して、プログラム終了時にポートを閉じるようにしたい
static SciComPortUart sci_com_uart_;

int SILS_SCI_UART_IF_init(void)
{
  return 0;
}

int SILS_SCI_UART_IF_tx(unsigned char* data_v, int count)
{
  sci_com_uart_.send(data_v, 0, count);
  return 0;
}

int SILS_SCI_UART_IF_rx(unsigned char* data_v, int count)
{
  return sci_com_uart_.receive(data_v, 0, count);
}


SciComPortUart::SciComPortUart(void)
{
  myHComPort_ = CreateFile("\\\\.\\COM11", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if ((int)myHComPort_ == -1)
  {
    // 正常にポートオープンできていない場合は終了
    CloseHandle(myHComPort_);
    init_success_ = false;
    return;
  }

  // どうやら正常ポートopenにならないっぽく，これが必要
  init_success_ = true;

  // SetCommStateを呼び出す前に，DCB構造体に現在の構成を入力する必要がある
  GetCommState(myHComPort_, &config_);

  // ポートのボーレート、パリティ等を設定
  config_.BaudRate = CBR_115200;
  config_.Parity = NOPARITY;
  config_.ByteSize = 8;
  config_.StopBits = ONESTOPBIT;

  // Parity、StopBits、DataBitsも同様に設定
  SetCommState(myHComPort_, &config_);
}

SciComPortUart::~SciComPortUart(void)
{
  if (init_success_ == true)
  {
    CloseHandle(myHComPort_);
  }
}

int SciComPortUart::send(unsigned char* buffer, size_t offset, size_t count)
{
  DWORD toWriteBytes = count; // 送信したいバイト数
  DWORD writeBytes;           // 実際に送信されたバイト数

  if (init_success_ == true)
  {
    WriteFile(myHComPort_, buffer + offset, toWriteBytes, &writeBytes, NULL);

    return writeBytes;
  }
  else
  {
    return 0;
  }
}

int SciComPortUart::receive(unsigned char* buffer, size_t offset, size_t count)
{
  DWORD fromReadBytes = count; // 受信したいバイト数
  DWORD dwErrors;
  COMSTAT ComStat;
  DWORD dwCount;               // 受信したバイト数

  if (init_success_ == true)
  {
    ClearCommError(myHComPort_, &dwErrors, &ComStat);
    dwCount = ComStat.cbInQue;

    if (dwCount > 0)
    {
      if (dwCount < count)
      {
        fromReadBytes = dwCount;
        ReadFile(myHComPort_, buffer + offset, fromReadBytes, &dwCount, NULL);
      }
      else
      {
        fromReadBytes = count; // 読み込みすぎるとデータが失われるので読み込む量を制御
        ReadFile(myHComPort_, buffer + offset, fromReadBytes, &dwCount, NULL);
      }
    }

    return dwCount;
  }
  else
  {
    return 0;
  }
}

#pragma section
