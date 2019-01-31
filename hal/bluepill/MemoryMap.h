extern uint32_t __bss_end__;

#define TASK_STACK_START 	((uint32_t)(&__bss_end__) + 4)
#define TASK_STACK_SIZE     0x300

