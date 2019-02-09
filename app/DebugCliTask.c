/*
 * DebugCliTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"

#include "Kernel.h"

void Debug_cli_task(void)
{
    int a = 5;
    int b = 6;
    int c = 0;

    debug_printf("Debug_cli_task....\n");

    while (1)
    {
    	//USBD_Delay(HID_FS_BINTERVAL);
        debug_printf("Debug_cli_task before context switch : %x %x\n", &a, &b);

        c = a + b;

        Kernel_yield();

        debug_printf("Debug_cli_task after context switch : %x\n", &c);

        a++;
        b++;
    }
}
