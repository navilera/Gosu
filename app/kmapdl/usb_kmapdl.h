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
#include "hal/bluepill/drivers/stm32f1xx.h"
#include "hal/bluepill/drivers/stm32f1xx_hal.h"

/* TODO: Derived from task function template */
void kmapdl_task(void);

/* TODO: Derived from task initialization template */
void kmapdl_init(void);

/* TODO: Derived from task cleanup template */
void kmapdl_cleanup(void);

#endif /* APP_KMAPDL_USB_KMAPDL_H_ */
