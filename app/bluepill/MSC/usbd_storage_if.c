/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @brief          : Memory management layer
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

#include "usbd_storage_if.h"

#define STORAGE_LUN_NBR                  1  
#define STORAGE_BLK_NBR                  0x10000  
#define STORAGE_BLK_SIZ                  0x200

const int8_t  STORAGE_Inquirydata_FS[] = {/* 36 */
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'C', 'P', ' ', 't', 'e', 'c', 'h', ' ', /* Manufacturer : 8 bytes */
  'M', 'y', ' ', 'm', 'a', 's', 's', ' ', /* Product      : 16 Bytes */
  's', 't', 'o', 'r', 'a', 'g', 'e', ' ',
  '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
};

const uint8_t FAT16_BootSector[FATBootSize]=
{
    0xEB,           /*00 - BS_jmpBoot */
    0x3C,           /*01 - BS_jmpBoot */
    0x90,           /*02 - BS_jmpBoot */
    'M','S','D','O','S','5','.','0',    /* 03-10 - BS_OEMName */
    0x00,           /*11 - BPB_BytesPerSec = 2048 */
    0x08,           /*11 - BPB_BytesPerSec = 2048 */
    0x04,           /*13 - BPB_Sec_PerClus = 2K*4 = 8K*/
    2,              /*14 - BPB_RsvdSecCnt = 2 */
    0,              /*15 - BPB_RsvdSecCnt = 2 */
    2,              /*16 - BPB_NumFATs = 2 */
    0x0,            /*17 - BPB_RootEntCnt = 512 */
    0x2,            /*18 - BPB_RootEntCnt = 512 */
    0,              /*19 - BPB_TotSec16 = 0 */
    0,              /*20 - BPB_TotSec16 = 0 */
    0xF8,           /*21 - BPB_Media = 0xF8 */
    0x0D,           /*22 - BPBFATSz16 = 0x000D */
    0,              /*23 - BPBFATSz16 = 0x000D */
    0x3F,           /*24 - BPB_SecPerTrk = 0x003F */
    0,              /*25 - BPB_SecPerTrk = 0x003F */
    0xFF,           /*26 - BPB_NumHeads = 255 */
    0,              /*27 - BPB_NumHeads = 255 */
    0,              /*28 - BPB_HiddSec = 0 */
    0,              /*29 - BPB_HiddSec = 0 */
    0,              /*30 - BPB_HiddSec = 0 */
    0,              /*31 - BPB_HiddSec = 0 */
    0x00,           /*32 - BPB_TotSec32 = */
    0xC8,           /*33 - BPB_TotSec32 = 0x0000C800 100Mb*/
    0x00,           /*34 - BPB_TotSec32 = */
    0x00,           /*35 - BPB_TotSec32 = */
    0x80,           /*36 - BS_DrvNum = 0x80 */
    0,              /*37 - BS_Reserved1 = 0 , dirty bit = 0*/ /* Updated from FSL*/
    0x29,           /*38 - BS_BootSig = 0x29 */
    0xBD,           /*39 - BS_VolID = 0x02DDA5BD */
    0xA5,           /*40 - BS_VolID = 0x02DDA5BD */
    0xDD,           /*41 - BS_VolID = 0x02DDA5BD */
    0x02,           /*42 - BS_VolID = 0x02DDA5BD */
    'N','O',' ','N','A','M','E',' ',' ',' ',' ',  /*43-53 - BS_VolLab */
    'F','A','T','1','6',' ',' ',' '   /*54-61 - BS_FilSysType */
};

const uint8_t FAT16_TableSector0[FATTableSize]=
{
    0xF8,0xFF,0xFF,0xFF
};

uint8_t FAT16_ReadyFileName[FATFileNameSize]=
{
    'R','E','A','D','Y',' ',' ',' ','T','X','T'    /*00-10 - Short File Name */
};

const uint8_t FAT16_RootDirSector[FATDirSize]=
{
    0x20,           /*11 - Archive Attribute set */
    0x00,           /*12 - Reserved */
    0x4B,           /*13 - Create Time Tenth */
    0x9C,           /*14 - Create Time */
    0x42,           /*15 - Create Time */
    0x92,           /*16 - Create Date */
    0x38,           /*17 - Create Date */
    0x92,           /*18 - Last Access Date */
    0x38,           /*19 - Last Access Date */
    0x00,           /*20 - Not used in FAT16 */
    0x00,           /*21 - Not used in FAT16 */
    0x9D,           /*22 - Write Time */
    0x42,           /*23 - Write Time */
    0x92,           /*24 - Write Date */
    0x38,           /*25 - Write Date */
    0x00,           /*26 - First Cluster (none, because file is empty) */
    0x00,           /*27 - First Cluster (none, because file is empty) */
    0x00,           /*28 - File Size */
    0x00,           /*29 - File Size */
    0x00,           /*30 - File Size */
    0x00,           /*31 - File Size */
    'B','O','O','T','L','O','A','D','E','R',' ',  /*32-42 - Volume label */
    0x08,           /*43 - File attribute = Volume label */
    0x00,           /*44 - Reserved */
    0x00,           /*45 - Create Time Tenth */
    0x00,           /*46 - Create Time */
    0x00,           /*47 - Create Time */
    0x00,           /*48 - Create Date */
    0x00,           /*49 - Create Date */
    0x00,           /*50 - Last Access Date */
    0x00,           /*51 - Last Access Date */
    0x00,           /*52 - Not used in FAT16 */
    0x00,           /*53 - Not used in FAT16 */
    0x9D,           /*54 - Write Time */
    0x42,           /*55 - Write Time */
    0x92,           /*56 - Write Date */
    0x38,           /*57 - Write Date */
};

FAT_DIR_t FileAttr;

static int8_t STORAGE_Init_FS (uint8_t lun);
static int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS (uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS (uint8_t lun);
static int8_t STORAGE_Read_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS (void);

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS,
};



int8_t STORAGE_Init_FS (uint8_t lun)
{
  return (USBD_OK);
}

int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
	*block_num  = STORAGE_BLK_NBR;
	*block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
}

int8_t  STORAGE_IsReady_FS (uint8_t lun)
{
  return (USBD_OK);
}

int8_t  STORAGE_IsWriteProtected_FS (uint8_t lun)
{
  return (USBD_OK);
}

int8_t STORAGE_Read_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  return (USBD_OK);
}

int8_t STORAGE_Write_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  return (USBD_OK);
}

int8_t STORAGE_GetMaxLun_FS (void)
{
  return (STORAGE_LUN_NBR - 1);
}
