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

void Flash_erase_page_mainfw(void)
{
	uint32_t addr = MAIN_FW_BADDR;

	HAL_FLASH_Unlock();

	for (uint32_t i = 0 ; i < MAIN_FW_FLASH_SIZE ; i++)
	{
		FLASH_PageErase(addr);
		addr += EFLASH_PAGESIZE;
	}

	HAL_FLASH_Lock();
}

void Flash_write_mainfw(uint8_t* buffer, uint32_t length)
{
	uint32_t alignedLen = (length + 3) & (~3);
	uint32_t len = alignedLen / 4;
	uint32_t addr = MAIN_FW_BADDR;
	uint32_t* pData = (uint32_t*)buffer;

	HAL_FLASH_Unlock();
	while(len--)
	{
		HAL_FLASH_Program(2, addr, *pData++);
		addr += 4;
	}
	HAL_FLASH_Lock();

	debug_printf("Write Len : %x\n", alignedLen);
}
