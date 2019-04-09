/*
 * HostCommTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"
#include "stdlib.h"

#include "usb_hid_keyboard.h"

#include "KeyHw.h"
#include "keymap.h"

#include "Kernel.h"

static void SendUSBHID(void);

void Host_comm_task(void)
{
    debug_printf("Host_comm_task....\n");

    while (true)
    {
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_SendD2H);
        if (handle_event == KernelEventFlag_SendD2H)
        {
        	SendUSBHID();
        }
        Kernel_yield();
    }
}

static void SendUSBHID(void)
{
	uint8_t HIDKeyboardReport[HID_KBD_REPORT_BYTE];

	Kernel_recv_msg(KernelMsgQ_D2hData, HIDKeyboardReport, HID_KBD_REPORT_BYTE);

	App_hid_send(HIDKeyboardReport, HID_KBD_REPORT_BYTE);

	// ignore unexpected data
	Kernel_flush_msg(KernelMsgQ_D2hData);
}
