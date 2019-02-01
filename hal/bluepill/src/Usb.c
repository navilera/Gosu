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
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"

#include "HalUsb.h"


USBD_HandleTypeDef hUsbDeviceFS;

void Hal_usb_init(void)
{
	  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

	  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID);

	  USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS);

	  USBD_Start(&hUsbDeviceFS);
}
