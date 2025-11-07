
#include "./CHIPKIT/GPIO_CHIPKIT.h"

int GPIO_set_direction(const uint8_t port, const GPIO_DIRECTION direction)
{
  return GPIO_CHIPKIT_set_direction(port, direction);
}

int GPIO_set_output(const uint8_t port, const GPIO_HL direction)
{
  return GPIO_CHIPKIT_set_output(port, direction);
}

int GPIO_read_input(const uint8_t port)
{
  return GPIO_CHIPKIT_read_input(port);
}
