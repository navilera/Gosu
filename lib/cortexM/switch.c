#include "stdint.h"
#include "stdbool.h"
#include "armcpu.h"
#include "switch.h"
#include "task.h"

extern KernelTcb_t* gCurrent_tcb;
extern KernelTcb_t* gNext_tcb;

void Arch_start(void)
{
	Arch_Restore_context();
}

/*
__attribute__ ((naked)) void Arch_context_switching(void)
{
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

void PendSV_Handler(void)
{
	if (IsStart == false)
	{
		__asm__ ("B Arch_Save_context");
	}
	else
	{
		IsStart = false;
		__asm__ ("B Arch_Restore_context");
	}
}

__attribute__ ((naked)) void Arch_Save_context(void)
{
	__asm__ ("mrs	r12,	PSP");				// Get PSP for current process.
	__asm__ ("stmfd	r12!,	{r4-r11}");			// Save remaining registers. r0-r3 are already saved into the stack by exception HW logic
	__asm__ ("ldr	r0,		=gCurrent_tcb");	// r0 = &gCurrent_tcb.
	__asm__ ("ldr	r1,		[r0]");				// r1 = gCurrent_tcb
	__asm__ ("str	r12,	[r1]");				// Update SP in PCB.
}

__attribute__ ((naked)) void Arch_Restore_context(void)
{
	__asm__ ("ldr	r1,		=gNext_tcb");		// r1 = &gNext_tcb
	__asm__ ("ldr	r1,		[r1]");				// r1 = gNext_tcb
	__asm__ ("ldr	r12,	[r1]");				// r12 = SP for new process.
	__asm__ ("ldmfd	r12!,	{r4-r11}");			// Restore r4-r11 for new process.
	__asm__ ("msr	PSP,	r12");				// Update SP for new process.
	__asm__ ("orr 	LR, 	LR, #0x04");		// Use process stack when returning.
	__asm__ ("bx 	LR");						// Return to new process.
}
*/

__attribute__ ((naked)) void Arch_context_switching(void)
{
	__asm__ ("B Arch_Save_context");
	__asm__ ("B Arch_Restore_context");
}

__attribute__ ((naked)) void Arch_Save_context(void)
{
    // save current task context into the current task stack
    __asm__ ("PUSH {lr}");
    __asm__ ("PUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("MRS  r0, PSR");
    __asm__ ("PUSH {r0}");
    // save current task stack pointer into the current TCB
    __asm__ ("LDR   r0, =gCurrent_tcb");
    __asm__ ("LDR   r0, [r0]");
    __asm__ ("MRS  	r1, MSP");
    __asm__ ("STMIA r0!,{r1}");
}

__attribute__ ((naked)) void Arch_Restore_context(void)
{
    // restore next task stack pointer from the next TCB
    __asm__ ("LDR   r0, =gNext_tcb");
    __asm__ ("LDR   r0, [r0]");
    __asm__ ("LDMIA r0!,{r1}");
    __asm__ ("MSR  	MSP,r1");
    // restore next task context from the next task stack
    __asm__ ("POP  {r0}");
    __asm__ ("MSR  PSR_nzcvq, r0");
    __asm__ ("POP  {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("POP  {pc}");
}
