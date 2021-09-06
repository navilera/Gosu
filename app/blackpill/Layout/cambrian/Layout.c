#include "KeyHw.h"
#include "keymap.h"
#include "Layout.h"

#include "stm32f1xx_hal.h"

/* Cambrian Keylayout
 *                                                    COL (input)
 *      +--------+------------------------------------------------       -----------------------------------------------------+------+
 *      | Col#   |  0   |  1    |  2  | 3   |  4  |  5    | 6    |       | 7   | 8   |  9  | 10  |  11  | 12    |   13  | 14  |  15  |
 *      +--------+------+-------+-----+-----+-----+-------+------+       +-----+-----+-----+-----+------+-------+-------+-----+------+
 *      |R# |    | PA5  | PA6   | PA7 | PB0 | PB1 | PB10  | PB11 |       | PB7 | PB6 | PB5 | PB4 | PB3  | PB15  | PB14  | PB13| PB12 |
 *      +---+----+------+-------+-----+-----+-----+-------+------+       +-----+-----+-----+-----+------+-------+-------+-----+------+
 *      | 0 |PA0 |                                               |       |                                                           |
 *      | 1 |PA1 |                                               |       |                                                           |
 * ROW  | 2 |PA2 |                      Left Part  (7cols)       |       |        Right Part  (9cols)                                |
 *(out) | 3 |PA3 |                                               |       |                                                           |
 *      | 4 |PA4 |                                               |       |                                                           |
 *      +-- +----------------------------------------------------+       +-----------------------------------------------------------+
 *
 */

/* Extra Pin usage
 * PA9,  PA10 - UART1 - Debug CLI
 * PA11, PA12 - USB
 */

Keypin_t gRowPin[KEYMAP_ROW_NUM] = {
		{(uint32_t)GPIOA, GPIO_PIN_4},
		{(uint32_t)GPIOA, GPIO_PIN_3},
		{(uint32_t)GPIOA, GPIO_PIN_2},
		{(uint32_t)GPIOA, GPIO_PIN_1},
		{(uint32_t)GPIOA, GPIO_PIN_0}
};

Keypin_t gColPin[KEYMAP_COL_NUM] = {
		// Left Part
		{(uint32_t)GPIOA, GPIO_PIN_5},  
		{(uint32_t)GPIOA, GPIO_PIN_6},  
		{(uint32_t)GPIOA, GPIO_PIN_7},  
		{(uint32_t)GPIOB, GPIO_PIN_0},  
		{(uint32_t)GPIOB, GPIO_PIN_1},  
		{(uint32_t)GPIOB, GPIO_PIN_10},  
		{(uint32_t)GPIOB, GPIO_PIN_11},  
		// Right Part
		{(uint32_t)GPIOB, GPIO_PIN_7},  
		{(uint32_t)GPIOB, GPIO_PIN_6},
		{(uint32_t)GPIOB, GPIO_PIN_5},  
		{(uint32_t)GPIOB, GPIO_PIN_4},
		{(uint32_t)GPIOB, GPIO_PIN_3},  
		{(uint32_t)GPIOB, GPIO_PIN_15},  
		{(uint32_t)GPIOB, GPIO_PIN_14}, 
		{(uint32_t)GPIOB, GPIO_PIN_13}
};


/* Default Keymap
 *  **** Layer 0 ****
 *                                                    COL (input)
 *      +----+---------------------------------------+    +-----------------------------------------------
 *      |Col#|  0   |  1  |  2  | 3   |  4 | 5  | 6  |    | 7  | 8   | 9   | 10  | 11 | 12 |   13  | 14  |
 *      +----+------+-----+-----+-----+----+----+----+    +----+-----+-----+-----+----+----+-------+-----+
 *      | 0  | ESC  | 1   | 2   |  3  | 4  | 5  |  6 |    | 7  |  8  | 9   | 0   | -  | +  | Back  | Del |
 *      | 1  | Tab  | Q   | W   | E   | R  | T  |    |    | Y  |  U  | I   | O   | P  | [  |  ]    |  \  |
 * ROW  | 2  | Fn   | A   | S   | D   | F  | G  |    |    | H  |  J  | K   | L   | ;  | "  | Enter | End |
 *(out) | 3  |Shift | Z   | X   | C   | V  | B  |    |    | B  |  N  | M   | <   | >  | ?  | Shift |  ↑  |
 *      | 4  | Ctrl | Win | Alt | Spc |    |    |    |    | Spc| Fn  | Alt |Ctrl | ←  | ↓  | →     |Home |
 *      +----+---------------------------------------+    +----------------------------------------------+
 * 
 * **** Layer 1 ****
 *                                                     COL (input)
 *      +----+---------------------------------------+    +-----------------------------------------------
 *      |Col#|  0   |  1  |  2  | 3   |  4 | 5  | 6  |    | 7  | 8   | 9   | 10  | 11 | 12 |   13  | 14  |
 *      +----+------+-----+-----+-----+----+----+----+    +----+-----+-----+-----+----+----+-------+-----+
 *      | 0  | `    | F1  | F2  | F3  | F4 | F5 | F6 |    | F7 |  F8 | F9  | F10 | F11| F12|       |     |
 *      | 1  |      |     |     |     |    |    |    |    |    |     |     |     |    |    |       |     |
 * ROW  | 2  |      |     |     |     |    |    |    |    |    |     |     |     |    |    |       | PgD |
 *(out) | 3  |      |     |     |     |    |    |    |    |    |     |     |     |    |    |       |     |
 *      | 4  |      |     |     |     |    |    |    |    |    |     |     |     |    |    |       |PgUp |
 *      +----+---------------------------------------+    +----------------------------------------------+
 *
 */
uint8_t gKeymap_buffer_layer0[KEYMAP_ROW_NUM][KEYMAP_COL_NUM] =
{        /*  Col#0      Col#1      Col#2     Col#3   Col#4   Col#5  Col#6  ||  Col#7   Col#8      Col#9      Col#10      Col#11      Col#12       Col#13       Col#14 */
/* Row#0 */	{kEsc,      k1,        k2,       k3,     k4,     k5,    k6,        k7,     k8,        k9,        k0,         kMinus,     kEqual,      kBackspace,  kDelete},
/* Row#1 */	{kTab,      kQ,        kW,       kE,     kR,     kT,    kNone,     kY,     kU,        kI,        kO,         kP,         kLeftbrace,  kRightbrace, kBackslash},
/* Row#2 */	{kFunction, kA,        kS,       kD,     kF,     kG,    kNone,     kH,     kJ,        kK,        kL,         kSemicolon, kApostrophe, kEnter,      kEnd},
/* Row#3 */	{kLeftshift,kZ,        kX,       kC,     kV,     kB,    kNone,     kB,     kN,        kM,        kComma,     kDot,       kSlash,      kRightshift, kUp},
/* Row#4 */	{kLeftctrl, kLeftmeta, kLeftalt, kSpace, kNone,  kNone, kNone,     kSpace, kFunction, kRightalt, kRightctrl, kLeft,      kDown,       kRight,      kHome}
};

uint8_t gKeymap_buffer_layer1[KEYMAP_ROW_NUM][KEYMAP_COL_NUM] =
{        /*  Col#0   Col#1 Col#2  Col#3 Col#4   Col#5  Col#6   Col#7  Col#8  Col#9  Col#10   Col#11   Col#12  Col#13 Col14 */
/* Row#0 */	{kGrave, kF1,  kF2,   kF3,  kF4,    kF5,   kF6,    kF7,   kF8,   kF9,   kF10,    kF11,    kF12,   kNone, kNone},
/* Row#1 */	{0},
/* Row#2 */	{0,      0,    0,     0,    0,      0,     0,      0,     0,     0,     0,       0,       0,      0,     kPagedown},
/* Row#3 */	{0},
/* Row#4 */	{0,      0,    0,     0,    0,      0,     0,      0,     0,     0,     0,       0,       0,      0,     kPageup}
};

void ChangeReportByLayout(uint8_t *report)
{
	/* Modifier bitmap

	  kModLctrl  = 0x01,
  	  kModLshift = 0x02,
  	  kModLalt   = 0x04,
  	  kModLmeta  = 0x08,
  	  kModRctrl  = 0x10,
  	  kModRshift = 0x20,
  	  kModRalt   = 0x40,
  	  kModRmeta  = 0x80
	 */

	// Check it has left-shift or right-shift
	uint8_t left_right_shift_bitmap = 0x02 | 0x20;
	if ((report[HID_MODIKEY_IDX] & left_right_shift_bitmap) == 0)
	{
		// No shift, do nothing
		return;
	}

	// Check it has ESC
	for (uint8_t i = HID_KEY_START_IDX ; i < HID_KBD_REPORT_BYTE ; i++)
	{
		if (report[i] == kEsc)
		{
			// Change kEsc to kGrave
			report[i] = kGrave;
		}
	}
}