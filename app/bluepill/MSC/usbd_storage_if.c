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
  'G', 'o', 's', 'u', ' ', 'F', 'W', ' ', /* Manufacturer : 8 bytes */
  'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd', /* Product      : 16 Bytes */
  ' ', 'D', 'n', 'l', 'd', 'M', 'o', 'd',
  '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
};

const uint8_t FAT16_BootSector[FATBootSize]=
{
    0xEB,           /*00 - BS_jmpBoot */
    0x3C,           /*01 - BS_jmpBoot */
    0x90,           /*02 - BS_jmpBoot */
    'M','S','D','O','S','5','.','0',    /* 03-10 - BS_OEMName */
    0x00,           /*11(0B) - BPB_BytesPerSec = 512 */
    0x02,           /*12(0C) - BPB_BytesPerSec =  */
    0x08,           /*13(0D) - BPB_Sec_PerClus = 8 sec (512 * 8 =  4K)*/
    1,              /*14(0E) - BPB_RsvdSecCnt = 1 */
    0,              /*15(0F) - BPB_RsvdSecCnt =  */
    2,              /*16(10) - BPB_NumFATs = 2 */
    0x00,           /*17(11) - BPB_RootEntCnt = 512 */
    0x02,           /*18(12) - BPB_RootEntCnt =  */
    0,              /*19(13) - BPB_TotSec16 = 0 */
    0,              /*20(14) - BPB_TotSec16 = 0 */
    0xF8,           /*21(15) - BPB_Media = 0xF8 */
    0x01,           /*22(16) - BPBFATSz16 = 0x0001 */
    0,              /*23(17) - BPBFATSz16 =  */
    0x3F,           /*24(18) - BPB_SecPerTrk = 0x003F */
    0,              /*25(19) - BPB_SecPerTrk = 0x003F */
    0xFF,           /*26(1A) - BPB_NumHeads = 255 */
    0,              /*27(1B) - BPB_NumHeads = 255 */
    0,              /*28(1C) - BPB_HiddSec = 0 */
    0,              /*29(1D) - BPB_HiddSec = 0 */
    0,              /*30(1E) - BPB_HiddSec = 0 */
    0,              /*31(1F) - BPB_HiddSec = 0 */
    0x00,           /*32(20) - BPB_TotSec32 = */
    0xC8,           /*33(21) - BPB_TotSec32 = 0x0000C800 25MB*/
    0x00,           /*34(22) - BPB_TotSec32 = */
    0x00,           /*35(23) - BPB_TotSec32 = */
    0x80,           /*36(24) - BS_DrvNum = 0x80 */
    0,              /*37(25) - BS_Reserved1 = 0 , dirty bit = 0*/ /* Updated from FSL*/
    0x29,           /*38(26) - BS_BootSig = 0x29 */
    0xBD,           /*39(27) - BS_VolID = 0x02DDA5BD */
    0xA5,           /*40(28) - BS_VolID = 0x02DDA5BD */
    0xDD,           /*41(29) - BS_VolID = 0x02DDA5BD */
    0x02,           /*42(2A) - BS_VolID = 0x02DDA5BD */
    'N','O',' ','N','A','M','E',' ',' ',' ',' ',  /*43-53 - BS_VolLab */
    'F','A','T','1','6',' ',' ',' '   /*54-61 (36-3D) - BS_FilSysType */
};

const uint8_t FAT16_TableSector0[FATTableSize]=
{
    0xF8,0xFF,0xFF,0xFF
};

static uint8_t FAT16_ReadyFileName[FATFileNameSize]=
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
    'G','O','S','U','F','W','_','K','B','D',' ',  /*32-42 - Volume label */
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

static FAT_DIR_t FileAttr;
static uint32_t filesize_total = 0;
static uint8_t sectorBuffer[1024];
static uint32_t sectorBuffIdx = 0;

static int8_t STORAGE_Init_FS (uint8_t lun);
static int8_t STORAGE_GetCapacity_FS (uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS (uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS (uint8_t lun);
static int8_t STORAGE_Read_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS (void);

static uint32_t FATSetStatusFileName(const char * name);
static uint32_t FAT_RootDirWriteRequest(uint32_t FAT_LBA,uint8_t* data, uint32_t len);
static uint32_t FAT_DataSectorWriteRequest(uint32_t FAT_LBA,uint8_t* data, uint32_t len);

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
	filesize_total = 0;
	sectorBuffIdx = 0;
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
	memclr(buf, FATBytesPerSec);

    int32_t i = 0;
    switch (blk_addr)
    {
    	/* FAT Boot Sector */
        case 0:
            for(i=0;i<FATBootSize;i++)
            {
                *buf++ = FAT16_BootSector[i];
            } /* EndFor */

            /* Boot Sector requires these 2 bytes at end */
            buf[FATBytesPerSec - 2] = 0x55;
            buf[FATBytesPerSec - 1] = 0xAA;
            break;

        /* FAT Table Sector */
        case 1: //FAT0
        case 2: //FAT1
            /* Write FAT Table Sector */
            for(i=0;i<FATTableSize;i++)
            {
                *buf++ = FAT16_TableSector0[i];
            } /* EndFor */
            break;

        /* Root Directory Sector */
        case 3:
            for(i=0;i<FATFileNameSize;i++)
            {
                *buf++ = FAT16_ReadyFileName[i];
            } /* EndFor */

            /* Write rest of file FAT structure */
            for(i=0;i<FATDirSize;i++)
            {
                *buf++ = FAT16_RootDirSector[i];
            } /* EndFor */
            break;

          /* All other sectors empty */
        default:
            break;
    } /* EndSwitch */

    return (USBD_OK);
}

int8_t STORAGE_Write_FS (uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	debug_printf("W:%x %x\n", blk_addr, blk_len);

    switch(blk_addr)
    {
        case 0: // Boot Sector
        case 1: // FAT0
        case 2: // FAT1
             break;
        case 3: // 16K  Root Directory
             FAT_RootDirWriteRequest(blk_addr, buf, blk_len);
             break;
        default:
             {
                //if(blk_addr >= 0x12000)
                {
                    FAT_DataSectorWriteRequest(blk_addr, buf, blk_len);
                }
             }
             break;
    }

    return (USBD_OK);
}

int8_t STORAGE_GetMaxLun_FS (void)
{
  return (STORAGE_LUN_NBR - 1);
}


static uint32_t FATSetStatusFileName(const char * name)
{
    uint8_t i;
    uint8_t len = (uint8_t)strlen(name);

    for(i=0; i<8 && i<len; i++)
    {
        FAT16_ReadyFileName[i] = name[i];
    }

    for(; i < 8; i++)
    {
        FAT16_ReadyFileName[i] = ' ';
    }

    return i;
}

static uint32_t FAT_RootDirWriteRequest(uint32_t FAT_LBA,uint8_t* data, uint32_t len)
{
    FAT_DIR_t* pFile = (FAT_DIR_t*) data;
    uint32_t   index = 2;

    pFile++; // Skip Root Dir
    pFile++; // Skip Status File

    while((pFile->DIR_Attr != 0x20) && (index++ < 512))
    {
        pFile++;
    }

    // Find it
    if(index <= 512)
    {
    	memncpy((uint8_t*)&FileAttr, (uint8_t*)pFile, 32);
        FileAttr.DIR_WriteTime = 0;
        FileAttr.DIR_WriteDate = 0;

        debug_printf("--> %s [%x]\n", FileAttr.DIR_Name, FileAttr.DIR_FileSize);
        //if (!memncmp((uint8_t*)FileAttr.DIR_Name, (uint8_t*)"KEYMAP", 6))
        // copy temporary data into the keymap Flash page
        //if dir_name is "FW"
        // copy temporary data into the eflash starting page (0x8000)

        /*
         * FATSetStatusFileName("SUCCESS");
         * FATSetStatusFileName("LARGE");
         * FATSetStatusFileName("UNKOWN");
         */
    }
    else
    {
    	memclr(&FileAttr, 32);
    }

    return len;
}

static uint32_t FAT_DataSectorWriteRequest(uint32_t FAT_LBA,uint8_t* data, uint32_t len)
{
    debug_printf("DW:%x [%x %x %x %x] %x\n", FAT_LBA, data[0], data[1], data[2], data[3], len);

    // 0x2A = RootDirSectors (32) + FAT1 sectors(1) + FAT2 sectors(1) + FirstClusterSectors(8) = 42 (0x2A)
    if (FAT_LBA > 0x2A)
    {
        //uint16_t flashStartAddr = *(volatile uint16_t *) 0x1FFFF7E0;
        uint32_t flashMaxLen  =  30 * FLASH_PAGE_SIZE; //30KB

        filesize_total += len;

        if (filesize_total > flashMaxLen)
        {
        	FATSetStatusFileName("LARGE");
        	return len;
        }

        if (sectorBuffIdx == 0)
        {
        	memncpy(sectorBuffer, data, len);
        	sectorBuffIdx = len;
        }
        else
        {
        	memncpy(&sectorBuffer[sectorBuffIdx], data, len);
			sectorBuffIdx = 0;

			// FlashPageWrite(sectorBuffer);
        }
    }

    return len;
}
