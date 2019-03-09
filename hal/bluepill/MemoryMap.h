#ifndef _HAL_MEMORYMAP_H_
#define _HAL_MEMORYMAP_H_

extern uint32_t __bss_end__;

#define TASK_STACK_START    ((uint32_t)&__bss_end__)
#define USR_TASK_STACK_SIZE (2 * 1024)
#define TASK_STACK_SIZE     (USR_TASK_STACK_SIZE * 3)

#define EFLASH_TOTALNUM		(128 - 1)
#define EFLASH_START        0x08000000
#define EFLASH_PAGESIZE     0x400
#define EFLASH_BADDR(X)     (EFLASH_START + EFLASH_PAGESIZE * (X))

#define KEYMAP_PAGENUM      (EFLASH_TOTALNUM - 1)  	/* last page of eFlash */
#define KEYMAP_BADDR        EFLASH_BADDR(KEYMAP_PAGENUM)

#define TEMP_FLASH_SIZE		30
#define TEMP_FLASH_PAGENUM	(KEYMAP_PAGENUM - TEMP_FLASH_SIZE)	/* 30 pages upper of keymap page */
#define TEMP_FLASH_BADDR    EFLASH_BADDR(TEMP_FLASH_PAGENUM)


#endif /*_HAL_MEMORYMAP_H_ */
