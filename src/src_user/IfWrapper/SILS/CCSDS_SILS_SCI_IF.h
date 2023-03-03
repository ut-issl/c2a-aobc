#ifndef CCSDS_SILS_SCI_IF_H_
#define CCSDS_SILS_SCI_IF_H_

#include <Windows.h>

// ZEUS SILSからのコピー
class SCIComPort
{
public:
  SCIComPort(void);
  ~SCIComPort(void);

  int Send(unsigned char* buffer, size_t length, size_t offset);
  int Receive(unsigned char* buffer, size_t length, size_t offset);

private:
  HANDLE myHComPort_;
  DCB config_;
  bool init_success;
};

int SILS_SIC_IF_init();
int SILS_SIC_IF_TX(unsigned char* data_v, int count);
int SILS_SIC_IF_RX(unsigned char* data_v, int count);

#endif
