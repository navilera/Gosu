/**
 * file: usb_kmapdl.h
 * auther: @eunchan
 *
 * description: Keymap file downloader via USB endpoint
*/

#ifndef __usb_kmapdl_H
#define __usb_kmapdl_H

/* Include HAL layer: `hal/bluepill/drivers/` */
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/* TODO: Derived from task function template */
void kmapdl_task(void);

/* TODO: Derived from task initialization template */
void kmapdl_init(void);

/* TODO: Derived from task cleanup template */
void kmapdl_cleanup(void);

#endif /* __usb_kmapdl_H */
