/*
 * stdlib.c
 *
 *  Created on: Sep 27, 2018
 *      Author: yiman
 */

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#include "HalTimer.h"


void delay(uint32_t ms)
{
    uint32_t goal = Hal_timer_get_1ms_counter() + ms;

    while(goal != Hal_timer_get_1ms_counter());
}

void memclr(void* dst, uint32_t count)
{
    uint8_t* d = (uint8_t*)dst;

    while(count--)
    {
        *d++ = 0;
    }
}

void halt(char* filename, uint32_t line)
{
	debug_printf("HALT [%s] at (%u)\n", filename, line);
	while(1);
}
