/*
 * Flash.h
 *
 *  Created on: Mar 9, 2019
 *      Author: maanu
 */

#ifndef APP_BLUEPILL_FLASH_H_
#define APP_BLUEPILL_FLASH_H_

#include "stdint.h"

void Flash_write_page(uint8_t* pageBuffer, uint32_t PageAddress);
#endif /* APP_BLUEPILL_FLASH_H_ */
