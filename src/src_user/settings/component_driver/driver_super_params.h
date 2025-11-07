/**
 * @file
 * @brief  DriverSUperパラメタのオーバーライド用ヘッダー
 */
#ifndef DRIVER_SUPER_PARAMS_H_
#define DRIVER_SUPER_PARAMS_H_

#undef CDS_STREAM_MAX
#undef CDS_IF_RX_BUFFER_SIZE

#define CDS_STREAM_MAX         (1)

// TODO: 適切な値を考える
// 最大であるOEM7600のRangeテレメに合わせて564byteとしている
#define CDS_IF_RX_BUFFER_SIZE  (564)

#endif
