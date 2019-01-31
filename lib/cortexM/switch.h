/*
 * switch.h
 *
 *  Created on: Jan 29, 2019
 *      Author: maanu
 */

#ifndef LIB_CORTEXM_SWITCH_H_
#define LIB_CORTEXM_SWITCH_H_

typedef struct ArchTaskContext_t
{
	  uint32_t r0;
	  uint32_t r1;
	  uint32_t r2;
	  uint32_t r3;
	  uint32_t r12;
	  uint32_t lr;
	  uint32_t pc;
	  uint32_t spsr;

	  uint32_t r4;
	  uint32_t r5;
	  uint32_t r6;
	  uint32_t r7;
	  uint32_t r8;
	  uint32_t r9;
	  uint32_t r10;
	  uint32_t r11;
} ArchTaskContext_t;

void Arch_start(void);
void Arch_context_switching(void);
void Arch_Restore_context(void);

#endif /* LIB_CORTEXM_SWITCH_H_ */
