/**
 * file: keymap.h
 * auther: @eunchan
 *
 * description: Keymap Structure used in HID, KeymapDl
 *
 * &copy; COPYRIGHT(c) 2019 Polypeak LLC,
 */

#ifndef APP_CORE_KEYMAP_H_
#define APP_CORE_KEYMAP_H_

#include "stdint.h"
#include "stdbool.h"

#include "KeyHw.h"

#define KEYMAP_MAGIC_H_WORD		0xC0FFEED0
#define KEYMAP_MAGIC_L_WORD		0xBEEFC0DE

#define HID_MODIKEY_IDX		0
#define HID_KEY_START_IDX	2
#define HID_KBD_REPORT_BYTE 	8
#define HID_MAX_MULTIPLE_INPUT	7		// 1 modi key + 6 char key

/*
 * public functions
 */
void LoadKeymap(void);
bool VerifyKeyMapFile(uint8_t* keyfile);
bool WriteKeyMapToFlash(uint8_t* keyfile, uint32_t size);

bool KeyMap_checkFnKey(KeyHwAddr_t* hwPollingAddrs, uint32_t pollingCount);
void KeyMap_getReport(bool isPressedFnKey, uint8_t* hidKeyboardReport, KeyHwAddr_t* hwPollingAddrs, uint32_t pollingCount);

#endif /*APP_CORE_KEYMAP_H_*/
