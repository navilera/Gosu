/**
  ******************************************************************************
  * @file           : USB_DEVICE  
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Device 
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "usb_msc_device.h"

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_msc.h"

extern USBD_DescriptorsTypeDef MSC_Desc;
extern USBD_StorageTypeDef  USBD_Storage_Interface_fops_FS;

/* USB Device Core handle declaration */
static USBD_HandleTypeDef hUsbMscDeviceFS;

/* init function */				        
void App_msc_Init(void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbMscDeviceFS, &MSC_Desc, DEVICE_FS);

  HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)hUsbMscDeviceFS.pData , MSC_EPIN_ADDR , PCD_SNG_BUF, 0x98);
  HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)hUsbMscDeviceFS.pData , MSC_EPOUT_ADDR , PCD_SNG_BUF, 0xD8);

  USBD_RegisterClass(&hUsbMscDeviceFS, &USBD_MSC);

  USBD_MSC_RegisterStorage(&hUsbMscDeviceFS, &USBD_Storage_Interface_fops_FS);

  USBD_Start(&hUsbMscDeviceFS);

}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
