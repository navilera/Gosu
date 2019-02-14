/*
 * Keymap.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "keymap.h"
#include "MemoryMap.h"

extern void FLASH_PageErase(uint32_t PageAddress);

/* Default Keymap
 *                                                    COL (input)
 *      +----+----------------------------------------------------------------------------------------------+
 *      |Col#|  0   |  1    |  2  | 3   |  4    |  5  | 6    | 7   | 8   |  9  | 10  |  11  | 12    |   13  |
 *      +----+------+-------+-----+-----+-------+-----+------+-----+-----+-----+-----+------+-------+-------+
 *      | 0  | ESC  | F1    | F2  | F3  | F4    | F5  | F6   | F7  | F8  | F9  | F10 | F11  |  F12  | back  |
 *      | 1  | PgUp | ~     | 1   | 2   | 3     | 4   | 5    | 6   | 7   | 8   | 9   | 0    | -     |  +    |
 * ROW  | 2  | PgDn | Tab   | Q   | W   | E     | R   | T    | Y   | U   | I   | O   | P    | [     |  ]    |
 *(out) | 3  | Inst | Fn    | A   | S   | D     | F   | G    | H   | J   | K   | L   | ;    | '     | Enter |
 *      | 4  | Home | shift | Z   | X   | C     | V   | B    | N   | M   | <   | >   | ?    | shift |  ↑    |
 *      | 5  | End  | ctrl  | Win | alt | space | alt | ctrl | ←   | ↓   | →   | \   | del  |       |       |
 *      +----+----------------------------------------------------------------------------------------------+
 *
 */

static Scancode_t sKeymap_buffer_layer0[KEYMAP_ROW_NUM][KEYMAP_COL_NUM] =
{           /* Col#0	Col#1		Col#2		Col#3		Col#4	Col#5		Col#6		Col#7	Col#8	Col#9	Col#10		Col#11		Col#12		Col#13 */
/* Row#0 */	{kEsc,		kF1,		kF2,		kF3,		kF4,	kF5,		kF6,		kF7,	kF8,	kF9,	kF10,		kF11,		kF12,		kBackspace},
/* Row#1 */	{kPageup,   kGrave,		k1,			k2,			k3,		k4,			k5,			k6,		k7,		k8,		k9,			k0,			kMinus,		kEqual},
/* Row#2 */	{kPagedown, kTab,		kQ,			kW, 		kE, 	kR, 		kT, 		kY, 	kU, 	kI, 	kO, 		kP,			kLeftbrace,	kRightbrace},
/* Row#3 */	{kInsert,	kFunction,	kA,			kS,			kD,		kF,			kG,			kH,		kJ,		kK,		kL,			kSemicolon,	kApostrophe,kEnter},
/* Row#4 */	{kHome,		kLeftshift,	kZ,			kX,			kC,		kV,			kB,			kN,		kM,		kComma,	kDot,		kSlash,		kRightshift,kUp},
/* Row#5 */	{kEnd,		kLeftctrl,	kLeftmeta, 	kLeftalt,	kSpace,	kRightalt,	kRightctrl,	kLeft,	kDown,	kRight,	kBackslash,	kDelete,	kNone,		kNone}
};

static Scancode_t sKeymap_buffer_layer1[KEYMAP_ROW_NUM][KEYMAP_COL_NUM] =
{			/* Col#0	Col#1		Col#2		Col#3		Col#4	Col#5		Col#6		Col#7	Col#8	Col#9	Col#10		Col#11		Col#12		Col#13 */
/* Row#0 */	{0},
/* Row#1 */	{0},
/* Row#2 */	{0},
/* Row#3 */	{0},
/* Row#4 */	{0},
/* Row#5 */	{0}
};

static bool ReadKeyMapFromFlash(KeymapFile_t* keyfile, uint32_t size);
static uint32_t GetChecksum(uint8_t* data, uint32_t count);
static uint8_t GetModifierKeyBitmap(uint8_t scancode);
static void	SortReport(uint8_t* hidKeyboardReport);

void LoadKeymap(void)
{
	KeymapFile_t saved_keymap = {0};
	if (ReadKeyMapFromFlash(&saved_keymap, sizeof(KeymapFile_t)) != true)
	{
		return;
	}

	uint32_t checksum = GetChecksum((uint8_t*)saved_keymap.keymap, sizeof(saved_keymap.keymap));
	if (saved_keymap.checksum != checksum)
	{
		return;
	}

	memncpy((uint8_t*)sKeymap_buffer_layer0, (uint8_t*)saved_keymap.keymap[0], TOTAL_KEY_NUM);
	memncpy((uint8_t*)sKeymap_buffer_layer1, (uint8_t*)saved_keymap.keymap[1], TOTAL_KEY_NUM);
}

bool WriteKeyMapToFlash(KeymapFile_t* keyfile, uint32_t size) {
  //uint32_t idx;
  //uint16_t entry;

  //HAL_StatusTypeDef status;
  FLASH_PageErase((uint32_t)KEYMAP_PAGENUM);

  memncpy((uint8_t*)KEYMAP_BADDR, (uint8_t*)keyfile, size);
  //for(idx = 0 ; idx < size ; idx += 2) {
  //  entry = *((uint8_t*)keyfile+idx) | (((uint8_t*)keyfile+idx+1) << 8);
  //  status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, KEYMAP_BADDR + idx, entry);
  //  if (status != HAL_OK) {
  //    debug_printf("eFlash Program Error: %x", KEYMAP_BADDR + idx);
  //    return false;
  //  }
  //}

  return true;
}

bool KeyMap_checkFnKey(KeyHwAddr_t* hwPollingAddrs, uint32_t pollingCount)
{
	uint32_t row = 0;
	uint32_t col = 0;

	while (pollingCount--)
	{
		row = hwPollingAddrs->bit.row;
		col = hwPollingAddrs->bit.col;

		if (sKeymap_buffer_layer0[row][col] == kFunction)
		{
			hwPollingAddrs->bit.fn = 1;
			return true;
		}
		hwPollingAddrs++;
	}

	return false;
}

void KeyMap_getReport(bool isPressedFnKey, uint8_t* hidKeyboardReport, KeyHwAddr_t* hwPollingAddrs, uint32_t pollingCount)
{
	uint32_t idx = HID_KEY_START_IDX;
	uint32_t row = 0;
	uint32_t col = 0;

	uint8_t modifierKeyBitmap = 0;
	uint8_t scancode = 0;

	while (pollingCount--)
	{
		row = hwPollingAddrs->bit.row;
		col = hwPollingAddrs->bit.col;
		hwPollingAddrs++;

		if (!isPressedFnKey)
		{
			scancode = sKeymap_buffer_layer0[row][col];
		}
		else
		{
			scancode = sKeymap_buffer_layer1[row][col];
		}

		modifierKeyBitmap = GetModifierKeyBitmap(scancode);

		if (modifierKeyBitmap == 0)	// Not modifier key pressed
		{
			hidKeyboardReport[idx++] = scancode;

			if (idx >= HID_KBD_REPORT_BYTE)
			{
				break;
			}
		}
		else
		{
			hidKeyboardReport[HID_MODIKEY_IDX] |= modifierKeyBitmap;
		}
	}

	SortReport(hidKeyboardReport);
}

static bool ReadKeyMapFromFlash(KeymapFile_t* keyfile, uint32_t size)
{
	return false;
}

static uint32_t GetChecksum(uint8_t* data, uint32_t count)
{
	uint32_t checksum = 0;
	for (uint32_t i = 0 ; i < count ; i++)
	{
		checksum += *data;
		data++;
	}

	return checksum;
}

static uint8_t GetModifierKeyBitmap(uint8_t scancode)
{
	/*
	  kLeftctrl = 0xe0,   // Keyboard Left Control
  	  kLeftshift = 0xe1,  // Keyboard Left Shift
  	  kLeftalt = 0xe2,    // Keyboard Left Alt
  	  kLeftmeta = 0xe3,   // Keyboard Left GUI
  	  kRightctrl = 0xe4,  // Keyboard Right Control
  	  kRightshift = 0xe5, // Keyboard Right Shift
  	  kRightalt = 0xe6,   // Keyboard Right Alt
  	  kRightmeta = 0xe7,  // Keyboard Right GUI
	 */

	/*
	  kModLctrl  = 0x01,
  	  kModLshift = 0x02,
  	  kModLalt   = 0x04,
  	  kModLmeta  = 0x08,
  	  kModRctrl  = 0x10,
  	  kModRshift = 0x20,
  	  kModRalt   = 0x40,
  	  kModRmeta  = 0x80
	 */

	uint8_t modifierKeyBitmap = 0;

	if (kLeftctrl <= scancode && scancode >= kRightmeta)
	{
		modifierKeyBitmap = 1 << (scancode & 0xF);
	}

	return modifierKeyBitmap;
}

static void	SortReport(uint8_t* hidKeyboardReport)
{
	uint8_t temp = 0;

	for (uint32_t idx = HID_KEY_START_IDX ; idx < HID_KBD_REPORT_BYTE ; idx++)
	{
		if (hidKeyboardReport[idx] == 0)
		{
			continue;
		}

		for (uint32_t p = (idx+1) ; p < HID_KBD_REPORT_BYTE ; p++)
		{
			if (hidKeyboardReport[p] == 0)
			{
				continue;
			}

			if (hidKeyboardReport[idx] > hidKeyboardReport[p])
			{
				temp = hidKeyboardReport[idx];
				hidKeyboardReport[idx] = hidKeyboardReport[p];
				hidKeyboardReport[p] = temp;
			}
		}
	}
}
