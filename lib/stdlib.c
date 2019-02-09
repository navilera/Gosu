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

uint32_t memfind(uint8_t* src, uint8_t byte, uint32_t count)
{
	for(uint32_t i = 0 ; i < count ; i++)
	{
		if (*src++ == byte)
		{
			return i;
		}
	}
	return (count + 1);
}

bool memncmp(uint8_t* m1, uint8_t* m2, uint32_t count)
{
	while(count--)
	{
		if (*m1++ != *m2++)
		{
			return false;
		}
	}

	return true;
}

void memncpy(uint8_t* dst, uint8_t* src, uint32_t count)
{
	while(count--)
	{
		*dst++ = *src++;
	}
}

void halt(char* filename, uint32_t line)
{
	debug_printf("HALT [%s] at (%u)\n", filename, line);
	while(1);
}
