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
    debug_printf("Host_comm_task....\n");

    while (true)
    {
    	//USBD_Delay(HID_FS_BINTERVAL);
        //debug_printf("Host_comm_task before context switch : %x %x\n", &a, &b);

        Kernel_yield();

        //debug_printf("Host_comm_task after context switch : %x\n", &c);

        /*
               //uint8_t hidpack[8] = { 0 };
               USBD_Delay(1000);
               hidpack[2] = 4;
               App_hid_send(hidpack, sizeof(hidpack));

               USBD_Delay(1000);
               hidpack[2] = 0;
               App_hid_send(hidpack, sizeof(hidpack));
               */
    }
}
