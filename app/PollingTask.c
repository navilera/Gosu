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


void Polling_task(void)
{
    DBG_PRINT("Polling Task....\n");

    bool pressedFnKey = false;
    uint32_t pollingCount = 0;

    KeyHwAddr_t hwPollingAddrs[HID_MAX_MULTIPLE_INPUT];

    uint8_t HIDKeyboardReport[HID_KBD_REPORT_BYTE];
    uint8_t HIDKeyboardReportOld[HID_KBD_REPORT_BYTE];

    LoadKeymap();

    while (true)
    {
    	USBD_Delay(HID_FS_BINTERVAL);

    	memclr(hwPollingAddrs, HID_MAX_MULTIPLE_INPUT * sizeof(KeyHwAddr_t));
    	memclr(HIDKeyboardReport, HID_KBD_REPORT_BYTE);

    	pollingCount = KeyHw_polling(hwPollingAddrs, HID_MAX_MULTIPLE_INPUT);

    	pressedFnKey = KeyMap_checkFnKey(hwPollingAddrs, pollingCount);
    	KeyMap_getReport(pressedFnKey, HIDKeyboardReport, hwPollingAddrs, pollingCount);

		if (memncmp(HIDKeyboardReportOld, HIDKeyboardReport, HID_KBD_REPORT_BYTE) == false)
		{
			Kernel_send_msg(KernelMsgQ_D2hData, HIDKeyboardReport, HID_KBD_REPORT_BYTE);
			Kernel_send_events(KernelEventFlag_SendD2H);

			memncpy(HIDKeyboardReportOld, HIDKeyboardReport, HID_KBD_REPORT_BYTE);
		}

        Kernel_yield();
    }
}
