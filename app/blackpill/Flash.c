/*
 * Flash.c
 *
 *  Created on: Mar 9, 2019
 *      Author: maanu
 */

#include "stdint.h"
#include "MemoryMap.h"

#include "stm32f1xx_hal_flash.h"

extern void FLASH_PageErase(uint32_t PageAddress);
extern HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data);

void Flash_write_page(uint8_t* pageBuffer, uint32_t PageAddress)
{
	uint32_t len = EFLASH_PAGESIZE / 4;
	uint32_t addr = EFLASH_BADDR(PageAddress);

	HAL_FLASH_Unlock();

	FLASH_PageErase(addr);

	uint32_t* pData = (uint32_t*)pageBuffer;

	while(len--)
	{
		HAL_FLASH_Program(2, addr, *pData++);
		addr += 4;
	}

	HAL_FLASH_Lock();
}


