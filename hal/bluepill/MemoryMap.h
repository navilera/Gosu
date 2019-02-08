extern uint32_t __bss_end__;

#define TASK_STACK_START    ((uint32_t)&__bss_end__)
#define USR_TASK_STACK_SIZE (2 * 1024)
#define TASK_STACK_SIZE     (USR_TASK_STACK_SIZE * 3)

