/**
 * file: usb_kmapdl.h
 * auther: @eunchan
 *
 * description: Keymap file downloader via USB endpoint
 *
 * &copy; COPYRIGHT(c) 2019 Polypeak LLC,
*/

#ifndef APP_KMAPDL_USB_KMAPDL_H_
#define APP_KMAPDL_USB_KMAPDL_H_

/* Include HAL layer: `hal/bluepill/drivers/` */
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

#include "usbd_cdc.h"

extern USBD_CDC_ItfTypeDef  USBD_KeymapDl_fops;

#define DEVICE_CDC 1

/* TODO: Derived from task function template */
void kmapdl_init   (void);
void kmapdl_cleanup(void);

#endif /* APP_KMAPDL_USB_KMAPDL_H_ */
