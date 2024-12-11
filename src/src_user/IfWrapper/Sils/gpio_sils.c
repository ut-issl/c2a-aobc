#include <stdbool.h>
#include "../GPIO.h"

int  s2e::components::OBC_C2A_GpioWrite(int port_id, const bool is_high);
bool s2e::components::OBC_C2A_GpioRead (int port_id);

int GPIO_set_direction(const uint8_t port, const GPIO_DIRECTION direction)
{
  return 0;
}

int GPIO_set_output(const uint8_t port, const GPIO_HL output)
{
  s2e::components::OBC_C2A_GpioWrite((int)port, (bool)output);
  return 0;
}

int GPIO_read_input(const uint8_t port)
{
  return s2e::components::OBC_C2A_GpioRead((int)port);
}
