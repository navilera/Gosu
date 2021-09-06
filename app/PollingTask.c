/*
 * PollingTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"
#include "stdlib.h"

#include "usbd_hid.h"

#include "KeyHw.h"
#include "keymap.h"

#include "Kernel.h"

#define HID_KEY_INTERVAL_100MS		(10)		// 10 times per 1sec -> 100ms
#define DELAY_REPEAT_COUNT			(50)		// 30 count delay

void Polling_task(void)
{
    DBG_PRINT("Polling Task....\n");

    bool pressedFnKey = false;
    uint32_t pollingCount = 0;
	uint32_t repeatDelayCount = 0;

    KeyHwAddr_t hwPollingAddrs[HID_MAX_MULTIPLE_INPUT];

    uint8_t HIDKeyboardReport[HID_KBD_REPORT_BYTE];
    uint8_t HIDKeyboardReportOld[HID_KBD_REPORT_BYTE];

    LoadKeymap();

    while (true)
    {
    	USBD_Delay(HID_KEY_INTERVAL_100MS);

    	memclr(hwPollingAddrs, HID_MAX_MULTIPLE_INPUT * sizeof(KeyHwAddr_t));
    	memclr(HIDKeyboardReport, HID_KBD_REPORT_BYTE);

    	pollingCount = KeyHw_polling(hwPollingAddrs, HID_MAX_MULTIPLE_INPUT);

    	pressedFnKey = KeyMap_checkFnKey(hwPollingAddrs, pollingCount);
    	KeyMap_getReport(pressedFnKey, HIDKeyboardReport, hwPollingAddrs, pollingCount);

		if (memncmp(HIDKeyboardReportOld, HIDKeyboardReport, HID_KBD_REPORT_BYTE))
		{
			if (HIDKeyboardReport[HID_MODIKEY_IDX] == 0x00 && HIDKeyboardReport[HID_KEY_START_IDX] == 0x00)
			{	// no input
				repeatDelayCount = 0;
				Kernel_yield();
				continue;
			}
			else
			{	// repeat any key
				// 500ms delay before repeat
				repeatDelayCount++;
				if (repeatDelayCount < DELAY_REPEAT_COUNT)
				{
					continue;
				}
			}
		}
		else
		{
			repeatDelayCount = 0;
		}

		Kernel_send_msg(KernelMsgQ_D2hData, HIDKeyboardReport, HID_KBD_REPORT_BYTE);
		Kernel_send_events(KernelEventFlag_SendD2H);

		memncpy(HIDKeyboardReportOld, HIDKeyboardReport, HID_KBD_REPORT_BYTE);

        Kernel_yield();
    }
}
