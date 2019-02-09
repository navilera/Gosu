/**
 * file: usb_kmapdl.c
 * auther: @eunchan
 *
 * description: Keymap file downloader via USB endpoint
 *
 * &copy; COPYRIGHT(c) 2019 Polypeak LLC,
 */

#include "app/core/keymap.h"
#include "app/kmapdl/usb_kmapdl.h"
#include "app/kmapdl/usbd_cdc.h"
#include "app/kmapdl/usbd_desc.h"
#include "hal/bluepill/MemoryMap.h" /*eFlash Address */
#include "hal/bluepill/drivers/stm32f1xx_hal_flash.h" /*eFlash Program*/
//#include "hal/bluepill/drivers/stm32f1xx_hal_flash_ex.h" /*eFlash Erase*/
#include "lib/stdio.h"
#include "usbd_core.h"

#define KMAPDL_RX_DATA_SIZE 2048
#define KMAPDL_TX_DATA_SIZE 256 

static int8_t kmapdl_cdc_init(void);
static int8_t kmapdl_cdc_deinit(void);
static int8_t kmapdl_cdc_control(uint8_t, uint8_t *, uint16_t);
static int8_t kmapdl_cdc_receive(uint8_t *, uint32_t *);

extern void FLASH_PageErase(uint32_t PageAddress);

USBD_CDC_ItfTypeDef USBD_KeymapDl_fops = {
    kmapdl_cdc_init, kmapdl_cdc_deinit, kmapdl_cdc_control, kmapdl_cdc_receive};

USBD_CDC_LineCodingTypeDef linecoding = {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

uint8_t UserRxBuffer[KMAPDL_RX_DATA_SIZE];
uint8_t UserTxBuffer[KMAPDL_TX_DATA_SIZE];

uint32_t BuffLength;
uint32_t UserTxBufPtrIn = 0;
uint32_t UserTxBufPtrOut = 0;

USBD_HandleTypeDef hUsbKeymap;

void kmapdl_task(void) {}

void kmapdl_init(void) {
  /* Create USB Endpoint 1 if doesn't exist.
  ** Then create interrupt event for incoming USB traffic of Endpoint 1
  ** Receiving data and checking signature are done in kmapdl_task function
  */
  USBD_Init(&hUsbKeymap, &VCP_Desc, DEVICE_CDC);
  USBD_RegisterClass(&hUsbKeymap, &USBD_CDC);
  USBD_CDC_RegisterInterface(&hUsbKeymap, &USBD_KeymapDl_fops);

  USBD_Start(&hUsbKeymap);
}

void kmapdl_cleanup(void) {}

static int8_t kmapdl_cdc_init(void) {
  debug_printf("Keymap Download USB Init...");

  /* Set Buffer */
  USBD_CDC_SetTxBuffer(&hUsbKeymap, UserTxBuffer, 0);
  USBD_CDC_SetRxBuffer(&hUsbKeymap, UserRxBuffer);

  return (USBD_OK);
}

static int8_t kmapdl_cdc_deinit(void) {
  debug_printf("Keymap Download USB Cleanup...");
  return (USBD_OK);
}

static int8_t kmapdl_cdc_control(uint8_t cmd, uint8_t *pbuf, uint16_t length) {
  switch (cmd) {
  case CDC_SEND_ENCAPSULATED_COMMAND:
    /* Add your code here */
    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:
    /* Add your code here */
    break;

  case CDC_SET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_GET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_CLEAR_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_SET_LINE_CODING:
    linecoding.bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) |
                                    (pbuf[3] << 24));
    linecoding.format = pbuf[4];
    linecoding.paritytype = pbuf[5];
    linecoding.datatype = pbuf[6];

    /* Add your code here */
    break;

  case CDC_GET_LINE_CODING:
    pbuf[0] = (uint8_t)(linecoding.bitrate);
    pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
    pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
    pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
    pbuf[4] = linecoding.format;
    pbuf[5] = linecoding.paritytype;
    pbuf[6] = linecoding.datatype;

    /* Add your code here */
    break;

  case CDC_SET_CONTROL_LINE_STATE:
    /* Add your code here */
    break;

  case CDC_SEND_BREAK:
    /* Add your code here */
    break;

  default:
    break;
  }

  return (0);
}

static int8_t kmapdl_cdc_receive(uint8_t *buf, uint32_t *len) { 
  /* buf: buffer of data to be transmitted
  ** len: number of data received in bytes
  */
  uint32_t idx;
  uint16_t entry;

  HAL_StatusTypeDef status;
  /* keymap:
        - Bit [7]:   1 for Fn Mode, 0 for Normal
        - Bit [6:4]: row
        - Bit [3:0]: column
     keycode: Actual keycode to be sent to host when key pressed
   */
  
  /* Convert buf to Keymap file */
  debug_printf("Received Keymap buffer content");

  /* Store Keymap struct to eFlash */
  FLASH_PageErase((uint32_t)KEYMAP_PAGENUM);

  for(idx = 0 ; idx < *len ; idx += 2) {
    entry = *(buf+idx) | (*(buf+idx+1) << 8);
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, KEYMAP_BADDR + idx, entry);
    if (status != HAL_OK) {
      debug_printf("eFlash Program Error: %x", KEYMAP_BADDR + idx);
    }
  }

  return (USBD_OK); 
}
