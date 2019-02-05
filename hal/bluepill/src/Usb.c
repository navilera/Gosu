/*
 * Usb.c
 *
 *  Created on: Jan 31, 2019
 *      Author: maanu
 */

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

#include "HalUsb.h"


USBD_HandleTypeDef hUsbDeviceFS;

void Hal_usb_init(void)
{
	/* Init Device Library,Add Supported Class and Start the library*/
	__disable_irq();

	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID);
	USBD_Start(&hUsbDeviceFS);

	debug_printf("USB init Done\n");

	__enable_irq();
}

