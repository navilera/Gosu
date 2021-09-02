/*
 * KeyHw.h
 *
 *  Created on: Feb 11, 2019
 *      Author: maanu
 */

#ifndef APP_KEYHW_H_
#define APP_KEYHW_H_

#include "stdbool.h"
#include "stdint.h"

typedef struct KeyHwAddr
{
	uint8_t col;
	uint8_t row;
	uint8_t fn;
} KeyHwAddr_t;

typedef struct Keypin
{
	uint32_t port;
	uint32_t num;
} Keypin_t;

uint32_t KeyHw_polling(KeyHwAddr_t* keyHwAddrBuff, uint32_t max_count);
bool	 KeyHw_IsPressed(uint32_t col, uint32_t row);

#endif /* APP_KEYHW_H_ */
