
#include "./GPIO_CHIPKIT.h"
#include <src_user/Settings/port_config.h>
#include <arduino.h>


int GPIO_CHIPKIT_set_direction(const uint8_t port, const GPIO_DIRECTION direction)
{
  // ポート範囲チェック
  if (port >= PORT_CH_MAX_GPIO_PORT) return (int)GPIO_ERR_CODE::GPIO_PORT_ERR;

  // Mode設定実行
  if (direction == GPIO_DIRECTION::GPIO_OUTPUT)
  {
    pinMode(port, OUTPUT);
  }
  else if (direction == GPIO_DIRECTION::GPIO_INPUT)
  {
    pinMode(port, INPUT);
  }
  else
  {
    return (int)GPIO_ERR_CODE::GPIO_DIRECTION_ERR;
  }

  return (int)GPIO_ERR_CODE::GPIO_OK;
}

int GPIO_CHIPKIT_set_output(const uint8_t port, const GPIO_HL output)
{
  // ポート範囲チェック
  if (port >= PORT_CH_MAX_GPIO_PORT) return (int)GPIO_ERR_CODE::GPIO_PORT_ERR;

  // モード確認
  if (getPinMode(port) == INPUT) return (int)GPIO_ERR_CODE::GPIO_DIRECTION_ERR;

  // 出力設定実行
  if (output == GPIO_HL::GPIO_HIGH)
  {
    digitalWrite(port, HIGH);
  }
  else if (output == GPIO_HL::GPIO_LOW)
  {
    digitalWrite(port, LOW);
  }
  else
  {
    return (int)GPIO_ERR_CODE::GPIO_LOGIC_ERR;
  }

  return (int)GPIO_ERR_CODE::GPIO_OK;
}

int GPIO_CHIPKIT_read_input(const uint8_t port)
{
  int i_ret = 0;

  // ポート範囲チェック
  if (port >= PORT_CH_MAX_GPIO_PORT) return (int)GPIO_ERR_CODE::GPIO_PORT_ERR;

  // モード確認
  if (getPinMode(port) == OUTPUT) return (int)GPIO_ERR_CODE::GPIO_DIRECTION_ERR;

  // 読み取り実行
  i_ret = digitalRead(port);
  if (i_ret == HIGH)
  {
    return (int)GPIO_HL::GPIO_HIGH;
  }
  else if (i_ret == LOW)
  {
    return (int)GPIO_HL::GPIO_LOW;
  }
  else
  {
    // Not Reached
  }

  return (int)GPIO_ERR_CODE::GPIO_UNKNOWN_ERR;
}
