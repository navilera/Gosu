/**
  ******************************************************************************
  * @file           : usbd_storage_if.h
  * @brief          : header file for the usbd_storage_if.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __USBD_STORAGE_IF_H_
#define __USBD_STORAGE_IF_H_

#include "usbd_msc.h"

#define FATBytesPerSec  MSC_MEDIA_PACKET
#define FATBootSec        0
#define FATTable0Sec0     2
#define FATTable1Sec0   245
#define FATRootDirSec0  488
#define FATDataSec0     520

#define FATFileNameSize  11
#define FATDirSize       47
#define FATBootSize      62
#define FATTableSize      4


typedef struct
{
	uint8_t  DIR_Name[11];          // File Name
	uint8_t  DIR_Attr;              // File Attribute
	uint8_t  DIR_NTRes;             // Reserved
	uint8_t  DIR_CreateTime_Tenth;  // Component of the file creation time
	uint16_t DIR_CreateTime;        // Component of the file creation time
	uint16_t DIR_CreateDate;        // Component of the file creation date
	uint16_t DIR_LastAccessDate;    // Last Access date
	uint16_t DIR_ClusHigh;          // High word of first data cluster
	uint16_t DIR_WriteTime;         // Last modification write time
	uint16_t DIR_WriteDate;         // Last modification write date
	uint16_t DIR_ClusLow;           // Low word of first data cluster
	uint32_t DIR_FileSize;          // Filesize
} __attribute__((packed)) FAT_DIR_t;

#endif /* __USBD_STORAGE_IF_H */

