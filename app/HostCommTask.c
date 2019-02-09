/*
 * HostCommTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"

#include "Kernel.h"

void Host_comm_task(void)
{
    int a = 3;
    int b = 4;
    int c = 0;

    debug_printf("Host_comm_task....\n");

    while (1)
    {
    	//USBD_Delay(HID_FS_BINTERVAL);
        debug_printf("Host_comm_task before context switch : %x %x\n", &a, &b);

        c = a + b;

        Kernel_yield();

        debug_printf("Host_comm_task after context switch : %x\n", &c);
        a++;
        b++;
    }
}
