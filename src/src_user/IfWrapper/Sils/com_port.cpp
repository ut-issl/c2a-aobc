#pragma section REPRO

#include "com_port.h"

#ifdef WIN32
ComPort::ComPort(int port)
  :init_success_{true}
{
  char port_settings[15];
  snprintf(port_settings, 15, "%s%d", "\\\\.\\COM", port);
  com_port_handle_ = CreateFile(port_settings, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if ((int)com_port_handle_ == -1)
  {
    // 正常にポートオープンできていない場合は終了
    CloseHandle(com_port_handle_);
    init_success_ = false;
    return;
  }

  // ポートのボーレート、パリティ等を設定
  // TODO: 外部ファイルから設定できるようにする
  config_.BaudRate = 115200;
  config_.Parity   = PARITY_NONE;
  config_.ByteSize = 8;
  config_.StopBits = STOPBITS_10;

  // Parity、StopBits、DataBitsも同様に設定
  SetCommState(com_port_handle_, &config_);
}

ComPort::~ComPort(void)
{
  if (init_success_ == true)
  {
    CloseHandle(com_port_handle_);
  }
}

int ComPort::send(unsigned char* buffer, size_t offset, size_t count)
{
  DWORD bytes_to_send = count; // 送信したいバイト数
  DWORD bytes_sent;            // 実際に送信されたバイト数

  if (init_success_ == true)
  {
    WriteFile(com_port_handle_, buffer + offset, bytes_to_send, &bytes_sent, NULL);
    return bytes_sent;
  }
  else
  {
    return 0;
  }
}

int ComPort::receive(unsigned char* buffer, size_t offset, size_t count)
{
  DWORD bytes_to_receive = count; // 受信したいバイト数
  DWORD bytes_received;           // 受信したバイト数
  DWORD dw_errors;
  COMSTAT com_stat;

  if (init_success_ == true)
  {
    ClearCommError(com_port_handle_, &dw_errors, &com_stat);
    bytes_received = com_stat.cbInQue;

    if (bytes_received > 0)
    {
      if (bytes_received < count)
      {
        bytes_to_receive = bytes_received;
        ReadFile(com_port_handle_, buffer + offset, bytes_to_receive, &bytes_received, NULL);
      }
      else
      {
        bytes_to_receive = count; // 読み込みすぎるとデータが失われるので読み込む量を制御
        ReadFile(com_port_handle_, buffer + offset, bytes_to_receive, &bytes_received, NULL);
      }
    }

    return bytes_received;
  }
  else
  {
    return 0;
  }
}

#else

ComPort::ComPort(int port)
  :init_success_{true}
{
  char port_settings[13];
  snprintf(port_settings, 13, "%s%d", "/dev/tnt", port);
  if ((com_port_handle_ = open(port_settings, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0)
  {
    close(com_port_handle_);
    init_success_ = false;
    return;
  }

  // TODO: 外部ファイルから設定できるようにする
  cfsetispeed(&config_, 115200);
  cfsetospeed(&config_, 115200);
  config_.c_cflag &= ~PARENB; // No Parity
  config_.c_cflag &= ~CSTOPB; // 1 Stop Bit
  config_.c_cflag &= ~CSIZE;
  config_.c_cflag |= CS8;     // 8 Bits
  tcsetattr(com_port_handle_, TCSANOW, &config_);
}

ComPort::~ComPort(void)
{
  if (init_success_ == true)
  {
    close(com_port_handle_);
  }
}

int ComPort::send(unsigned char* buffer, size_t offset, size_t count)
{
  if (init_success_ == true)
  {
    return write(com_port_handle_, buffer + offset, count);
  }
  else
  {
    return 0;
  }
}

int ComPort::receive(unsigned char* buffer, size_t offset, size_t count)
{
  if (init_success_ == true)
  {
    return read(com_port_handle_, buffer + offset, count);
  }
  else
  {
    return 0;
  }
}

#endif

#pragma section
