/*
 * stdio.h
 *
 *  Created on: Sep 17, 2018
 *      Author: maanu
 */

#ifndef LIB_STDIO_H_
#define LIB_STDIO_H_

#include "stdint.h"
#include "stdarg.h"

#if (DEBUG_PRINT == 1)
#define DBG_PRINT(...)      debug_printf(__VA_ARGS__)
#else
void dummy_print(const char* format, ...);
#define DBG_PRINT(...)      dummy_print(__VA_ARGS__)
#endif

typedef enum utoa_t
{
    utoa_dec = 10,
    utoa_hex = 16,
} utoa_t;

uint32_t putstr(const char* s);
uint32_t debug_printf(const char* format, ...);
uint32_t vsprintf(char* buf, const char* format, va_list arg);
uint32_t utoa(char* buf, uint32_t val, utoa_t base);

#endif /* LIB_STDIO_H_ */
