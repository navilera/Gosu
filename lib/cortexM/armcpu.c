/*
 * armcpu.c
 *
 *  Created on: Sep 22, 2018
 *      Author: maanu
 */

#include "armcpu.h"

void enable_irq(void)
{
	__asm__ ("cpsie i");
}

void enable_fiq(void)
{

}

void disable_irq(void)
{
	__asm__ ("cpsid i");
}

void disable_fiq(void)
{

}

void set_CONTROL(uint32_t control)
{
  __asm__ ("MSR control, %0" : : "r" (control) : "memory");
}
