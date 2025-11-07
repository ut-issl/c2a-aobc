/**
 * @file
 * @brief SpacePacketの パケット型に関するユーザー設定
 */
#ifndef SPACE_PACKET_TYPEDEF_PARAMS_H_
#define SPACE_PACKET_TYPEDEF_PARAMS_H_

#undef TSP_MAX_LEN
#undef CSP_MAX_LEN

// 223 だったが， 2022/02/01 に Space Packet 化したことで，ヘッダサイズが 13 byte 増えたので，とりあえず 240 に
#define TSP_MAX_LEN  (240)

// double引数6個程度確保できるので64byteで十分
#define CSP_MAX_LEN  (64) // 内15byteはヘッダなど

#endif
