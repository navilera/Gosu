/*
 * armcpu.c
 *
 *  Created on: Sep 22, 2018
 *      Author: maanu
 */

#include "armcpu.h"

void enable_irq(void)
{
	__asm__ ("mov r12, 0x00000000");
	__asm__ ("msr BASEPRI, r12":::"r12");
}

void enable_fiq(void)
{

}

void disable_irq(void)
{
	__asm__ ("mov r12, 0x00000080");
	__asm__ ("msr BASEPRI, r12":::"r12");
}

void disable_fiq(void)
{

}
