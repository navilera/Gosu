/*
 * KeyHw.h
 *
 *  Created on: Feb 11, 2019
 *      Author: maanu
 */

#ifndef APP_KEYHW_H_
#define APP_KEYHW_H_

#include "stdint.h"

#define HID_KBD_REPORT_BYTE 8
#define MAX_MULTIPLE_INPUT	7		// 1 modi key + 6 char key

typedef union KeyHwAddr
{
	uint8_t val;
	struct {
		uint8_t col:4;
		uint8_t row:3;
		uint8_t fn:1;
	} bit;
} KeyHwAddr_t;

uint32_t KeyHw_polling(KeyHwAddr_t* keyHwAddrBuff, uint32_t max_count);

#endif /* APP_KEYHW_H_ */
