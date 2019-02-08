/**
 * file: usb_kmapdl.c
 * auther: @eunchan
 *
 * description: Keymap file downloader via USB endpoint
 *
 * &copy; COPYRIGHT(c) 2019 Polypeak LLC,
 */

#include "usbd_core.h"
#include "app/kmapdl/usbd_cdc.h"
#include "app/kmapdl/usbd_desc.h"
#include "app/kmapdl/usb_kmapdl.h"

static int8_t kmapdl_cdc_init   (void);
static int8_t kmapdl_cdc_deinit (void);
static int8_t kmapdl_cdc_control(uint8_t, uint8_t*, uint16_t);
static int8_t kmapdl_cdc_receive(uint8_t*, uint32_t*);

USBD_CDC_ItfTypeDef USBD_KeymapDl_fops = {
    kmapdl_cdc_init, kmapdl_cdc_deinit, kmapdl_cdc_control, kmapdl_cdc_receive};

USBD_CDC_LineCodingTypeDef linecoding = {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

USBD_HandleTypeDef hUsbKeymap;

void kmapdl_task(void) {}

void kmapdl_init(void) {
  /* Create USB Endpoint 1 if doesn't exist.
  ** Then create interrupt event for incoming USB traffic of Endpoint 1
  ** Receiving data and checking signature are done in kmapdl_task function
  */
  USBD_Init(&hUsbKeymap, &VCP_Desc , DEVICE_CDC);
  USBD_RegisterClass(&hUsbKeymap, &USBD_CDC);
  USBD_CDC_RegisterInterface(&hUsbKeymap, &USBD_KeymapDl_fops);

  USBD_Start(&hUsbKeymap);
}

void kmapdl_cleanup(void) {}

static int8_t kmapdl_cdc_init(void) {
  return 0;
}

static int8_t kmapdl_cdc_deinit(void) {
  return 0;
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

static int8_t kmapdl_cdc_receive(uint8_t *buf, uint32_t *len) { return (0); }
