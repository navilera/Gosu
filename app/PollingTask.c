/*
 * PollingTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"
#include "usbd_hid.h"

#include "Kernel.h"

void Polling_task(void)
{
    debug_printf("Polling Task....\n");

    while (true)
    {
    	USBD_Delay(HID_FS_BINTERVAL);
        //debug_printf("Polling Task before context switch : %x %x\n", &a, &b);


        Kernel_yield();

        //debug_printf("Polling Task after context switch : %x -> %u\n", &c, HAL_GetTick());

    }
}
