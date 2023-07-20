#ifndef COM_PORT_H_
#define COM_PORT_H_

#ifdef WIN32
#include <Windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <stddef.h>
#include <stdio.h>

class ComPort
{
public:
  ComPort(int port);
  ~ComPort(void);

  int send(unsigned char* buffer, size_t length, size_t offset);
  int receive(unsigned char* buffer, size_t length, size_t offset);

private:
#ifdef WIN32
  HANDLE com_port_handle_;
  DCB config_;
#else
  int com_port_handle_;
  struct termios config_;
#endif
  bool init_success_;
};

#endif
