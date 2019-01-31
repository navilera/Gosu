/*
 * armcpu.h
 *
 *  Created on: Sep 22, 2018
 *      Author: maanu
 */

#ifndef LIB_ARMCPU_H_
#define LIB_ARMCPU_H_

void enable_irq(void);
void enable_fiq(void);
void disable_irq(void);
void disable_fiq(void);

#define NVIC_INT_CTRL		0xE000ED04
#define NVIC_PENDSVSET		0x10000000
#define NVIC_SYSPRI2		0xE000ED22
#define NVIC_PENDSV_PRI		0x000000FF

#define MEM32(addr)			*(volatile unsigned long *)(addr)
#define MEM8(addr) 			*(volatile unsigned char *)(addr)

#endif /* LIB_ARMCPU_H_ */
