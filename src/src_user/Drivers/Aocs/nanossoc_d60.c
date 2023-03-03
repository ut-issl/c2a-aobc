#pragma section REPRO
/**
* @file   nanossoc_d60.c
* @brief  nanoSSOC_D60のDriver
*/

#include "./nanossoc_d60.h"
#include "../../Library/vector3.h"
#include <string.h> // for memcpy
#include <math.h>

#define NANOSSOC_D60_STREAM_TLM_CMD (0)  //!< テレコマで使うストリーム
#define NANOSSOC_D60_RX_FRAME_SIZE  (15) //!< 1回の受信で取得するデータ長（バイト）
#define NANOSSOC_D60_TX_FRAME_SIZE  (1)  //!< コマンドコードのみ送信すればよいので，1バイト

const static uint8_t NANOSSOC_D60_kCmdIdAngularPosition_ = 0x04;   //!< サンセンサ太陽方向・太陽捕捉率・エラーコード受信コマンド
static uint8_t NANOSSOC_D60_tx_frame_[NANOSSOC_D60_TX_FRAME_SIZE]; //!< 送信フレーム

static DS_ERR_CODE NANOSSOC_D60_load_driver_super_init_settings_(DriverSuper* super);
static DS_ERR_CODE NANOSSOC_D60_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver);

static void NANOSSOC_D60_convert_sun_angle_to_vector_(NANOSSOC_D60_Driver* nanossoc_d60_driver);

int NANOSSOC_D60_init(NANOSSOC_D60_Driver* nanossoc_d60_driver, uint8_t ch, uint8_t i2c_address)
{
  DS_ERR_CODE ret;

  nanossoc_d60_driver->driver.i2c_config.ch = ch;
  nanossoc_d60_driver->driver.i2c_config.device_address = i2c_address;
  nanossoc_d60_driver->driver.i2c_config.frequency_khz = 100;
  nanossoc_d60_driver->driver.i2c_config.stop_flag = 1;
  nanossoc_d60_driver->driver.i2c_config.timeout_threshold = 1000; // 500だとタイムアウトするので、長めにとる

  ret = DS_init(&(nanossoc_d60_driver->driver.super),
                          &(nanossoc_d60_driver->driver.i2c_config),
                          NANOSSOC_D60_load_driver_super_init_settings_);

  if (ret != DS_ERR_CODE_OK) return 1;

  nanossoc_d60_driver->info.sun_intensity_percent = 0.0f;
  nanossoc_d60_driver->info.err_code = NANOSSOC_D60_ERROR_CODE_NOT_ENOUGH_RADIATION;

  nanossoc_d60_driver->info.checksum_state = NANOSSOC_D60_CHECKSUM_STATE_OK;

  for (int i = 0; i < NANOSSOC_D60_ANGLE_ELEMENT_MAX; i++)
  {
    nanossoc_d60_driver->info.sun_angle_compo_deg[i] = 0.0f;
  }

  nanossoc_d60_driver->info.frame_transform_c2b = QUATERNION_make_unit();
  NANOSSOC_D60_convert_sun_angle_to_vector_(nanossoc_d60_driver);

  return 0;
}

static DS_ERR_CODE NANOSSOC_D60_load_driver_super_init_settings_(DriverSuper* super)
{
  DS_StreamConfig* stream_config;

  super->interface = I2C;

  // streamは0のみ
  stream_config = &(super->stream_config[NANOSSOC_D60_STREAM_TLM_CMD]);
  DSSC_enable(stream_config);

  // 送信
  DSSC_set_tx_frame(stream_config, NANOSSOC_D60_tx_frame_);
  DSSC_set_tx_frame_size(stream_config, NANOSSOC_D60_TX_FRAME_SIZE);

  // 定期的な受信はする
  DSSC_set_rx_frame_size(stream_config, NANOSSOC_D60_RX_FRAME_SIZE);
  DSSC_set_data_analyzer(stream_config, NANOSSOC_D60_analyze_rec_data_);

  // 定期TLMの監視機能の有効化しない → ので設定上書きなし

  return DS_ERR_CODE_OK;
}

C2A_MATH_ERROR NANOSSOC_D60_set_frame_transform_c2b(NANOSSOC_D60_Driver* nanossoc_d60_driver, const Quaternion q_c2b)
{
  C2A_MATH_ERROR ret;
  ret = QUATERNION_is_normalized(q_c2b);
  if (ret != C2A_MATH_ERROR_OK) return ret;

  nanossoc_d60_driver->info.frame_transform_c2b = q_c2b;
  return ret;
}

DS_CMD_ERR_CODE NANOSSOC_D60_observe(NANOSSOC_D60_Driver* nanossoc_d60_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* stream_config;

  stream_config = &(nanossoc_d60_driver->driver.super.stream_config[NANOSSOC_D60_STREAM_TLM_CMD]);

  // Rx settings
  DSSC_set_rx_frame_size(stream_config, NANOSSOC_D60_RX_FRAME_SIZE);
  I2C_set_rx_length(&(nanossoc_d60_driver->driver.i2c_config), (uint32_t)NANOSSOC_D60_RX_FRAME_SIZE);

  // send command
  NANOSSOC_D60_tx_frame_[0] = NANOSSOC_D60_kCmdIdAngularPosition_;
  ret = DS_send_req_tlm_cmd(&(nanossoc_d60_driver->driver.super), NANOSSOC_D60_STREAM_TLM_CMD);
  if (ret != DS_ERR_CODE_OK)  return DS_CMD_DRIVER_SUPER_ERR;

  // Receive data
  ret = DS_receive(&(nanossoc_d60_driver->driver.super));
  if (ret != DS_ERR_CODE_OK)
  {
    DS_clear_rx_buffer(&(nanossoc_d60_driver->driver.super));
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  ret = DS_analyze_rec_data(&(nanossoc_d60_driver->driver.super), NANOSSOC_D60_STREAM_TLM_CMD, nanossoc_d60_driver);
  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}


static DS_ERR_CODE NANOSSOC_D60_analyze_rec_data_(DS_StreamConfig* stream_config, void* driver)
{
  NANOSSOC_D60_Driver* nanossoc_d60_driver = (NANOSSOC_D60_Driver*)driver;

  const uint8_t* nanossoc_d60_rx_data = DSSC_get_rx_frame(stream_config);

  uint8_t pos = 0;

  pos += sizeof(uint8_t); // テレメの1バイト目はテレメ長データだが、使わないので読み飛ばす
  memcpy(&(nanossoc_d60_driver->info.sun_angle_compo_deg[NANOSSOC_D60_ANGLE_ELEMENT_ALPHA]), nanossoc_d60_rx_data + pos, sizeof(float));

  pos += sizeof(float);
  memcpy(&(nanossoc_d60_driver->info.sun_angle_compo_deg[NANOSSOC_D60_ANGLE_ELEMENT_BETA]), nanossoc_d60_rx_data + pos, sizeof(float));

  pos += sizeof(float);
  memcpy(&(nanossoc_d60_driver->info.sun_intensity_percent), nanossoc_d60_rx_data + pos, sizeof(float));

  pos += sizeof(float);
  memcpy(&(nanossoc_d60_driver->info.err_code), nanossoc_d60_rx_data + pos, sizeof(uint8_t));

  uint8_t tlm_checksum;
  pos += sizeof(uint8_t);
  memcpy(&(tlm_checksum), nanossoc_d60_rx_data + pos, sizeof(uint8_t));

  NANOSSOC_D60_convert_sun_angle_to_vector_(nanossoc_d60_driver);

  // Calculate checksum on AOBC
  unsigned int calculated_checksum_tmp = 0;
  for (int i = 0; i < NANOSSOC_D60_RX_FRAME_SIZE - 1; i++)
  {
    calculated_checksum_tmp += (unsigned int)nanossoc_d60_rx_data[i];
  }
  uint8_t calculated_checksum = ((uint8_t)calculated_checksum_tmp & 0xff);

  // チェックサムが正しいかどうかの判断だけDriver側で行う．エラー処理はDI側で行う．
  if (tlm_checksum != calculated_checksum)
  {
    nanossoc_d60_driver->info.checksum_state = NANOSSOC_D60_CHECKSUM_STATE_NG;
  }
  else
  {
    nanossoc_d60_driver->info.checksum_state = NANOSSOC_D60_CHECKSUM_STATE_OK;
  }

  return DS_ERR_CODE_OK;
}

static void NANOSSOC_D60_convert_sun_angle_to_vector_(NANOSSOC_D60_Driver* nanossoc_d60_driver)
{
  float sun_angle_compo_rad[NANOSSOC_D60_ANGLE_ELEMENT_MAX];
  for (int i = 0; i < NANOSSOC_D60_ANGLE_ELEMENT_MAX; i++)
  {
    sun_angle_compo_rad[i] = PHYSICAL_CONST_degree_to_radian(nanossoc_d60_driver->info.sun_angle_compo_deg[i]);
  }

  float sun_vec_compo_tmp[PHYSICAL_CONST_THREE_DIM];

  sun_vec_compo_tmp[0] = (float)tan(sun_angle_compo_rad[NANOSSOC_D60_ANGLE_ELEMENT_ALPHA]);
  sun_vec_compo_tmp[1] = (float)tan(sun_angle_compo_rad[NANOSSOC_D60_ANGLE_ELEMENT_BETA]);
  sun_vec_compo_tmp[2] = 1.0f;

  VECTOR3_normalize(nanossoc_d60_driver->info.sun_vec_compo, sun_vec_compo_tmp);

  QUATERNION_trans_coordinate(nanossoc_d60_driver->info.sun_vec_body,
                              nanossoc_d60_driver->info.frame_transform_c2b,
                              nanossoc_d60_driver->info.sun_vec_compo);
}

#pragma section
