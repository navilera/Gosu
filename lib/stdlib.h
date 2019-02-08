/*
 * stdlib.h
 *
 *  Created on: Sep 27, 2018
 *      Author: yiman
 */

#ifndef LIB_STDLIB_H_
#define LIB_STDLIB_H_

void delay(uint32_t ms);
void memclr(void* dst, uint32_t count);
void halt(char* filename, uint32_t line);

#endif /* LIB_STDLIB_H_ */
