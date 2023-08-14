/**
 * @file
 * @brief  DriverSUperパラメタのオーバーライド用ヘッダー
 */
#ifndef DRIVER_SUPER_PARAMS_H_
#define DRIVER_SUPER_PARAMS_H_

#undef DS_STREAM_MAX
#undef DS_IF_RX_BUFFER_SIZE

#define DS_STREAM_MAX         (1)

// TODO: 適切な値を考える
// 最大であるSagittaのテレメに合わせて350byteとしている
#define DS_IF_RX_BUFFER_SIZE  (350)

#endif
