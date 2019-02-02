//extern uint32_t __bss_end__;
extern uint32_t _estack;

#define TASK_STACK_SIZE     	0xC00	// 3KB total user task stack space
#define USR_TASK_STACK_SIZE     0x400	// 1KB stack for each task

#define TASK_STACK_START 		((uint32_t)(&_estack) - TASK_STACK_SIZE)
