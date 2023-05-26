/**
 * @file  crc8.h
 * @brief CRC8用のライブラリ
 * @note  CRC16用のライブラリはsrc_core/Library/crc.cにある
 */
#ifndef CRC8_H_
#define CRC8_H_

#include <stddef.h> // for size_t
#include <stdbool.h>
#include <stdint.h>

/**
  * @brief CRC-8-ATM
  *
  *        生成多項式: x^8 + x^2 + x + 1
  *        ビット送り: 左送り, POLLY: 0x07
  *        読み出し: 1byte(8 bit)
  * @param[in/out] crc: in:CRC初期値, out:CRC計算結果
  * @param[in] data: CRCを計算するbyte列
  * @param[in] size: 列の長さ
  * @param[in] rev_flag: 反転しない:false, 反転する:true
  * @return uint8_t: 計算結果
  */
uint8_t crc_8_atm_left(uint8_t crc, const unsigned char* data, const size_t size, const bool rev_flag);

#endif
