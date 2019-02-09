/**
 * file: usb_kmapdl.c
 * auther: @eunchan
 *
 * description: Keymap file downloader via USB endpoint
 *
 * &copy; COPYRIGHT(c) 2019 Polypeak LLC,
 */

#include "app/bluepill/keymap.h"
#include "usb_kmapdl.h"
#include "usbd_cdc.h"
#include "usbd_vcp_desc.h"
#include "MemoryMap.h" /*eFlash Address */
#include "stdio.h"
#include "usbd_core.h"

#define KMAPDL_RX_DATA_SIZE 2048
#define KMAPDL_TX_DATA_SIZE 256 

static int8_t kmapdl_cdc_init(void);
static int8_t kmapdl_cdc_deinit(void);
static int8_t kmapdl_cdc_control(uint8_t, uint8_t *, uint16_t);
static int8_t kmapdl_cdc_receive(uint8_t *, uint32_t *);

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
  uint32_t checksum;
  uint8_t  row, col;

  KeymapFile_t keyfile = {0};

  /* keymap:
        - Bit [7]:   1 for Fn Mode, 0 for Normal
        - Bit [6:4]: row
        - Bit [3:0]: column
     keycode: Actual keycode to be sent to host when key pressed
   */
  if (*len < (6+ (TOTAL_KEY_NUM*NUM_LAYERS))) {
      debug_printf("Recevied size is smaller than Keymap");
      return (USBD_OK);
  }

  checksum = (*buf) | (*(buf+1)<<8) | (*(buf+2)<<16) | (*(buf+3)<<24);
  row      = *(buf+4);
  col      = *(buf+5);

  if ((row != KEYMAP_ROW_NUM) || (col != KEYMAP_COL_NUM)) {
    /* Error. Recevied keymap doesn't match */
    debug_printf("Received Keymap struct is not compatible: R(%d), C(%d)", row, col);
    return (USBD_OK);
  }

  /* Convert buf to Keymap file */
  debug_printf("Received Keymap buffer content");
  keyfile.checksum = checksum;
  keyfile.num_row = row;
  keyfile.num_col = col;
  for (idx = 0 ; idx < NUM_LAYERS ; idx++) {
    memncpy((uint8_t*)(buf + 6 + (idx * TOTAL_KEY_NUM)),
            (uint8_t*)keyfile.keymap[idx],
            TOTAL_KEY_NUM);
  }

  /* Store Keymap struct to eFlash */
  
  if (true != WriteKeyMapToFlash(&keyfile, sizeof(KeymapFile_t))) {
    debug_printf("Writing to eFlash failed...");
  }

  return (USBD_OK); 
}
