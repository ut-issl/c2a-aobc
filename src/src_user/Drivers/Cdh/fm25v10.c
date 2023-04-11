/**
* @file
* @brief  FM25V10 FRAMのドライバ
*/
#pragma section REPRO

#include "./fm25v10.h"

#include <string.h>
#include <src_core/Library/print.h>

#define FM25V10_STREAM_TLM_CMD  (0)

const uint8_t FM25V10_kCmdWrite       = 0x02;
const uint8_t FM25V10_kCmdRead        = 0x03;
const uint8_t FM25V10_kCmdWriteEnable = 0x06;
const uint8_t FM25V10_kCmdReadStatus  = 0x05;
const uint8_t FM25V10_kCmdWriteStatus = 0x01;

const uint8_t  FM25V10_kLengthAddress = 3;

static DS_ERR_CODE FM25V10_load_driver_super_init_settings_(DriverSuper* super);
static DS_ERR_CODE FM25V10_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver);

static DS_CMD_ERR_CODE FM25V10_convert_address_to_tx_data_(uint8_t* address_byte, uint32_t address_u32);

// 公開関数
DS_INIT_ERR_CODE FM25V10_init(FM25V10_Driver* fm25v10_driver, uint8_t comm_ch, uint8_t gpio_ch, DS_StreamRecBuffer* rx_buffer)
{
  DS_ERR_CODE ret;

  fm25v10_driver->driver.spi_config.comm_ch = comm_ch;
  fm25v10_driver->driver.spi_config.gpio_ch = gpio_ch;
  fm25v10_driver->driver.spi_config.frequency_khz = 40000;  // max 40MHz
  fm25v10_driver->driver.spi_config.mode = SPI_MODE_0;      // MODE_0 or MODE_3
  fm25v10_driver->driver.spi_config.tx_data_for_rx = 0x00;  // default

  ret = DS_init(&(fm25v10_driver->driver.super),
                &(fm25v10_driver->driver.spi_config),
                rx_buffer,
                FM25V10_load_driver_super_init_settings_);

  fm25v10_driver->info.status = 0x00;
  fm25v10_driver->info.last_start_address = 0;
  fm25v10_driver->info.last_read_data_byte = 0;
  fm25v10_driver->info.last_write_data_byte = 0;

  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;
  return DS_INIT_OK;
}

DS_CMD_ERR_CODE FM25V10_write_status(FM25V10_Driver* fm25v10_driver, const uint8_t status)
{
  DS_StreamConfig* stream_config;
  DS_ERR_CODE ret;
  stream_config = &(fm25v10_driver->driver.super.stream_config[FM25V10_STREAM_TLM_CMD]);

  const size_t kTxLength = 2;
  uint8_t tx_data[kTxLength];
  tx_data[0] = FM25V10_kCmdWriteStatus;
  tx_data[1] = status;

  SPI_set_cs_state_after_tx(&(fm25v10_driver->driver.spi_config), SPI_CS_STATE_AFTER_TX_HIGH);
  DSSC_set_tx_frame(stream_config, tx_data);
  DSSC_set_tx_frame_size(stream_config, kTxLength);
  ret = DS_send_general_cmd(&(fm25v10_driver->driver.super), FM25V10_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK) return DS_CMD_DRIVER_SUPER_ERR;

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE FM25V10_read_status(FM25V10_Driver* fm25v10_driver)
{
  DS_StreamConfig* stream_config;
  DS_ERR_CODE ret;
  stream_config = &(fm25v10_driver->driver.super.stream_config[FM25V10_STREAM_TLM_CMD]);

  // Set Write Enable
  uint8_t tx_data = FM25V10_kCmdReadStatus;
  SPI_set_cs_state_after_tx(&(fm25v10_driver->driver.spi_config), SPI_CS_STATE_AFTER_TX_LOW);
  DSSC_set_tx_frame(stream_config, &tx_data);
  DSSC_set_tx_frame_size(stream_config, sizeof(tx_data));
  ret = DS_send_general_cmd(&(fm25v10_driver->driver.super), FM25V10_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK) return DS_CMD_DRIVER_SUPER_ERR;

  // Receive data
  SPI_set_rx_length(&(fm25v10_driver->driver.spi_config), 1);
  DSSC_set_rx_frame_size(stream_config, 1);
  DS_clear_rx_buffer(&(fm25v10_driver->driver.super));
  ret = DS_receive(&(fm25v10_driver->driver.super));
  if (ret != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  // Analyze
  ret = DS_analyze_rec_data(&(fm25v10_driver->driver.super), FM25V10_STREAM_TLM_CMD, fm25v10_driver);
  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

DS_CMD_ERR_CODE FM25V10_write_bytes(FM25V10_Driver* fm25v10_driver, const uint32_t start_address,
                       const uint8_t* write_bytes, const uint16_t length)
{
  if (length > FM25V10_MAX_LENGTH) return DS_CMD_ILLEGAL_PARAMETER;
  uint32_t stop_address = start_address + (uint32_t)length;
  if (stop_address > FM25V10_STOP_ADDRESS) return DS_CMD_ILLEGAL_PARAMETER;

  DS_StreamConfig* stream_config;
  DS_ERR_CODE ret;
  stream_config = &(fm25v10_driver->driver.super.stream_config[FM25V10_STREAM_TLM_CMD]);

  // Set Write Enable
  uint8_t write_enable = FM25V10_kCmdWriteEnable;
  SPI_set_cs_state_after_tx(&(fm25v10_driver->driver.spi_config), SPI_CS_STATE_AFTER_TX_HIGH);
  DSSC_set_tx_frame(stream_config, &write_enable);
  DSSC_set_tx_frame_size(stream_config, sizeof(write_enable));
  ret = DS_send_general_cmd(&(fm25v10_driver->driver.super), FM25V10_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK) return DS_CMD_DRIVER_SUPER_ERR;

  // Set Tx data
  uint8_t  tx_data[FM25V10_MAX_LENGTH + FM25V10_kLengthAddress];
  uint16_t tx_length = 0;
  tx_data[0] = FM25V10_kCmdWrite;
  tx_length += sizeof(FM25V10_kCmdWrite);
  FM25V10_convert_address_to_tx_data_(tx_data + tx_length, start_address);
  tx_length += FM25V10_kLengthAddress;
  memcpy(tx_data + tx_length, write_bytes, length);
  tx_length += length;
  DSSC_set_tx_frame(stream_config, tx_data);
  DSSC_set_tx_frame_size(stream_config, tx_length);

  ret = DS_send_general_cmd(&(fm25v10_driver->driver.super), FM25V10_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK) return DS_CMD_DRIVER_SUPER_ERR;

  fm25v10_driver->info.last_start_address = start_address;
  fm25v10_driver->info.last_write_data_byte = write_bytes[0];

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE FM25V10_read_bytes(FM25V10_Driver* fm25v10_driver, const uint32_t start_address,
                      uint8_t* read_bytes, const uint16_t length)
{
  if (length > FM25V10_MAX_LENGTH) return DS_CMD_ILLEGAL_PARAMETER;
  uint32_t stop_address = start_address + (uint32_t)length;
  if (stop_address > FM25V10_STOP_ADDRESS) return DS_CMD_ILLEGAL_PARAMETER;

  DS_StreamConfig* stream_config;
  DS_ERR_CODE ret;
  stream_config = &(fm25v10_driver->driver.super.stream_config[FM25V10_STREAM_TLM_CMD]);

  // Set Tx data
  uint8_t tx_data[FM25V10_kLengthAddress + 1];
  uint16_t tx_length = 0;
  tx_data[0] = FM25V10_kCmdRead;
  tx_length += sizeof(FM25V10_kCmdRead);
  FM25V10_convert_address_to_tx_data_(tx_data + tx_length, start_address);
  tx_length += FM25V10_kLengthAddress;

  DSSC_set_tx_frame(stream_config, tx_data);
  DSSC_set_tx_frame_size(stream_config, tx_length);
  SPI_set_cs_state_after_tx(&(fm25v10_driver->driver.spi_config), SPI_CS_STATE_AFTER_TX_LOW);
  ret = DS_send_req_tlm_cmd(&(fm25v10_driver->driver.super), FM25V10_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK)  return DS_CMD_DRIVER_SUPER_ERR;

  // Receive data
  SPI_set_rx_length(&(fm25v10_driver->driver.spi_config), length);
  DSSC_set_rx_frame_size(stream_config, length);
  DS_clear_rx_buffer(&(fm25v10_driver->driver.super));
  ret = DS_receive(&(fm25v10_driver->driver.super));
  if (ret != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  // Analyze
  const uint8_t* fm25v10_rx_data = DSSC_get_rx_frame(stream_config);
  memcpy(read_bytes, fm25v10_rx_data, length);

  fm25v10_driver->info.last_start_address = start_address;
  fm25v10_driver->info.last_read_data_byte = read_bytes[0];

  return DS_CMD_OK;
}

// Static Functions
static DS_ERR_CODE FM25V10_load_driver_super_init_settings_(DriverSuper* super)
{
  DS_StreamConfig* stream_config;

  super->interface = SPI;

  stream_config = &(super->stream_config[FM25V10_STREAM_TLM_CMD]);
  DSSC_enable(stream_config);

  DSSC_set_rx_frame_size(stream_config, FM25V10_MAX_LENGTH);  // 可変だがサイズ情報なし, 都度設定する
  DSSC_set_data_analyzer(stream_config, FM25V10_analyze_rec_data_);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE FM25V10_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver)
{
  const uint8_t* fm25v10_rx_data = DSSC_get_rx_frame(stream_config);
  FM25V10_Driver* fm25v10_driver = (FM25V10_Driver*)driver;

  // status analyze
  fm25v10_driver->info.status = fm25v10_rx_data[0];

  return DS_ERR_CODE_OK;
}

static DS_CMD_ERR_CODE FM25V10_convert_address_to_tx_data_(uint8_t* address_byte, uint32_t address_u32)
{
  if (address_u32 >= FM25V10_STOP_ADDRESS) return DS_CMD_ILLEGAL_PARAMETER;
  for (uint8_t i = 0; i < FM25V10_kLengthAddress; i++)
  {
    address_byte[i] = address_u32 >> (8 * (2 - i));
  }
  return DS_CMD_OK;
}

#pragma section
