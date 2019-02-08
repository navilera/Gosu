/**
 * file: usb_kmapdl.c
 * auther: @eunchan
 *
 * description: Keymap file downloader via USB endpoint
*/

#include "usb_kmapdl.h"

void kmapdl_task(void) {
}

void kmapdl_init(void) {
    /* Create USB Endpoint 1 if doesn't exist.
     * Then create interrupt event for incoming USB traffic of Endpoint 1
     * Receiving data and checking signature are done in kmapdl_task function
     */
    
}

void kmapdl_cleanup(void) {
}
