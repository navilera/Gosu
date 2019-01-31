#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "Kernel.h"

#include "HalUart.h"

void User_task0(void);
void User_task1(void);
void User_task2(void);

static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();

    taskId = Kernel_task_create(User_task0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task0 creation fail\n");
    }

    taskId = Kernel_task_create(User_task1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task1 creation fail\n");
    }

    taskId = Kernel_task_create(User_task2);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task2 creation fail\n");
    }

    Kernel_start();
}

int main(void) {

	Hal_uart_init();

	putstr("Start Navilos\n");

    Kernel_init();

    // main loop
    while (1);
}

void User_task0(void)
{
	int a = 1;
	int b = 2;
	int c = 0;

	while (1)
	{
		debug_printf("Task0 before context switch : %u %u\n", a, b);

		c = a + b;

		Kernel_yield();

		debug_printf("Task0 after context switch : %u\n", c);

		a++;
		b++;
	}
}

void User_task1(void)
{
	int a = 3;
	int b = 4;
	int c = 0;

	while (1)
	{
		debug_printf("Task1 before context switch : %u %u\n", a, b);

		c = a + b;

		Kernel_yield();

		debug_printf("Task1 after context switch : %u\n", c);

		a++;
		b++;
	}
}

void User_task2(void)
{
	int a = 5;
	int b = 6;
	int c = 0;

	while (1)
	{
		debug_printf("Task2 before context switch : %u %u\n", a, b);

		c = a + b;

		Kernel_yield();

		debug_printf("Task2 after context switch : %u\n", c);

		a++;
		b++;
	}
}
