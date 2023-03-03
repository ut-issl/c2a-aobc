/**
 * @file
 * @brief  DriverSUperパラメタのオーバーライド用ヘッダー
 */
#ifndef DRIVER_SUPER_PARAMS_H_
#define DRIVER_SUPER_PARAMS_H_

#undef DS_STREAM_MAX
#undef DS_RX_BUFFER_SIZE_MAX
#undef DS_RX_FRAME_SIZE_MAX

#define DS_STREAM_MAX          (1)
// NOTE: ここは，物理的なバッファサイズと合わせてください．
//       つまり，IF_RXで呼び出される受信関数で，一度に取得されうる最大値を確保しておく必要があります．
//       ないしは，IF_RX関数内部で，このサイズ以上読まないようにロジックを組むかです．
// TODO_L: OEM7600Driverに合わせて64から144に変更したが、変更せずに済ませる手立てがないか考える
#define DS_RX_BUFFER_SIZE_MAX  (144)
#define DS_RX_FRAME_SIZE_MAX   (144)

#endif
