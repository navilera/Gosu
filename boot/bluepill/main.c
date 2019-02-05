#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "Kernel.h"

#include "stm32f1xx_hal.h"

#include "HalUart.h"
#include "HalTimer.h"
#include "HalGpio.h"
#include "HalUsb.h"

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

static void Hw_init(void)
{
	HAL_Init();

	Hal_timer_init();
	Hal_uart_init();
	Hal_gpio_init();
	Hal_usb_init();

	debug_printf("Hw_init Done\n");
}

int main(void) {

	Hw_init();

	debug_printf("Start Navilos\n");
	debug_printf("=============\n");

    //Kernel_init();

    // main loop
    while (1);
}

void User_task0(void)
{
	int a = 1;
	int b = 2;
	int c = 0;

	debug_printf("Task0 start stack : %x %x\n", &a, &b);

	while (1)
	{
		//debug_printf("Task0 before context switch : %u %u\n", a, b);

		c = a + b;

		Kernel_yield();

		//debug_printf("Task0 after context switch (0x%x) : %u\n", &c, c);

		a++;
		b++;
	}
}

void User_task1(void)
{
	int a = 3;
	int b = 4;
	int c = 0;

	debug_printf("Task1 start stack : %x %x\n", &a, &b);

	while (1)
	{
		//debug_printf("Task1 before context switch : %u %u\n", a, b);

		c = a + b;

		Kernel_yield();

		//debug_printf("Task1 after context switch (0x%x) : %u\n", &c, c);

		a++;
		b++;
	}
}

void User_task2(void)
{
	int a = 5;
	int b = 6;
	int c = 0;

	debug_printf("Task2 start stack : %x %x\n", &a, &b);

	while (1)
	{
		//debug_printf("Task2 before context switch : %u %u\n", a, b);

		c = a + b;

		Kernel_yield();

		//debug_printf("Task2 after context switch (0x%x) : %u\n", &c, c);

		a++;
		b++;
	}
}
