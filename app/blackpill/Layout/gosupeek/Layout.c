#include "KeyHw.h"
#include "Layout.h"

#include "stm32f1xx_hal.h"

/* Gosupeek Keylayout
 *                                                    COL (input)
 *      +--------+----------------------------------------------------------------------------------------------+
 *      | Col#   |  0   |  1    |  2  | 3   |  4    |  5  | 6    | 7   | 8   |  9  | 10  |  11  | 12    |   13  |
 *      +--------+------+-------+-----+-----+-------+-----+------+-----+-----+-----+-----+------+-------+-------+
 *      |R# |    | PA6  | PA7   | PB0 | PB1 | PB3   | PB4 | PB5  | PB6 | PB7 | PB8 | PB9 | PB13 | PB14  | PB15  |
 *      +---+----+------+-------+-----+-----+-------+-----+------+-----+-----+-----+-----+------+-------+-------+
 *      | 0 |PA0 |                                                                                              |
 *      | 1 |PA1 |                                                                                              |
 * ROW  | 2 |PA2 |              DEFAULT KEY MAP defined by Keymap.c                                             |
 *(out) | 3 |PA3 |                                  or User custom keymap                                       |
 *      | 4 |PA4 |                                                                                              |
 *      | 5 |PA5 |                                                                                              |
 *      +-- +---------------------------------------------------------------------------------------------------+
 *
 *
 *
 *  key_polling_code
 *        +-----+---------------+--------------------+
 *  bit#  |  7  |     6 : 4     |      3 : 0         |
 *        +-----+---------------+--------------------+
 *        | Fn  |    Row Num    |      Col Num       |
 *        +-----+---------------+--------------------+
 *
 *        For example : Press (2, 6)
 *          - Row 2 (PA2 out) and Col 6 (PB5 input)
 *          - 0 (0) 010 (2) 0110 (6) -> 00100110 -> 0x26
 */

/* Extra Pin usage
 * PA9,  PA10 - UART1 - Debug CLI
 * PA11, PA12 - USB
 * PB10, PB11 - UART3 - Bluetooth
 */
Keypin_t gRowPin[KEYMAP_ROW_NUM] = {
		{(uint32_t)GPIOA, GPIO_PIN_0},
		{(uint32_t)GPIOA, GPIO_PIN_1},
		{(uint32_t)GPIOA, GPIO_PIN_2},
		{(uint32_t)GPIOA, GPIO_PIN_3},
		{(uint32_t)GPIOA, GPIO_PIN_4},
		{(uint32_t)GPIOA, GPIO_PIN_5}
};

Keypin_t gColPin[KEYMAP_COL_NUM] = {
		{(uint32_t)GPIOA, GPIO_PIN_6},  {(uint32_t)GPIOA, GPIO_PIN_7},
		{(uint32_t)GPIOB, GPIO_PIN_0},  {(uint32_t)GPIOB, GPIO_PIN_1},
		{(uint32_t)GPIOB, GPIO_PIN_3},  {(uint32_t)GPIOB, GPIO_PIN_4},  {(uint32_t)GPIOB, GPIO_PIN_5}, {(uint32_t)GPIOB, GPIO_PIN_6}, {(uint32_t)GPIOB, GPIO_PIN_7}, {(uint32_t)GPIOB, GPIO_PIN_8}, {(uint32_t)GPIOB, GPIO_PIN_9},
		{(uint32_t)GPIOB, GPIO_PIN_13}, {(uint32_t)GPIOB, GPIO_PIN_14}, {(uint32_t)GPIOB, GPIO_PIN_15}
};


/* Default Keymap
 *                                                    COL (input)
 *      +----+----------------------------------------------------------------------------------------------+
 *      |Col#|  0   |  1    |  2  | 3   |  4    |  5  | 6    | 7   | 8   |  9  | 10  |  11  | 12    |   13  |
 *      +----+------+-------+-----+-----+-------+-----+------+-----+-----+-----+-----+------+-------+-------+
 *      | 0  | ESC  | F1    | F2  | F3  | F4    | F5  | F6   | F7  | F8  | F9  | F10 | F11  |  F12  | back  |
 *      | 1  | Home | ~     | 1   | 2   | 3     | 4   | 5    | 6   | 7   | 8   | 9   | 0    | -     |  +    |
 * ROW  | 2  | End  | Tab   | Q   | W   | E     | R   | T    | Y   | U   | I   | O   | P    | [     |  ]    |
 *(out) | 3  | Inst | Fn    | A   | S   | D     | F   | G    | H   | J   | K   | L   | ;    | '     | Enter |
 *      | 4  | PgUp | shift | Z   | X   | C     | V   | B    | N   | M   | <   | >   | ?    | shift |  ↑    |
 *      | 5  | PgDn | ctrl  | Win | alt | space | alt | ctrl | ←   | ↓   | →   | \   | del  |       |       |
 *      +----+----------------------------------------------------------------------------------------------+
 *
 */
uint8_t gKeymap_buffer_layer0[KEYMAP_ROW_NUM][KEYMAP_COL_NUM] =
{           /*  Col#0       Col#1      Col#2       Col#3     Col#4   Col#5     Col#6      Col#7	Col#8  Col#9   Col#10      Col#11      Col#12        Col#13 */
/* Row#0 */	{kEsc,      kF1,       kF2,        kF3,      kF4,    kF5,      kF6,       kF7,  kF8,   kF9,    kF10,       kF11,       kF12,         kBackspace},
/* Row#1 */	{kHome,     kGrave,    k1,         k2,       k3,     k4,       k5,        k6,   k7,    k8,     k9,         k0,         kMinus,       kEqual},
/* Row#2 */	{kEnd,      kTab,      kQ,         kW,       kE,     kR,       kT,        kY,   kU,    kI,     kO,         kP,         kLeftbrace,   kRightbrace},
/* Row#3 */	{kInsert,   kFunction, kA,         kS,       kD,     kF,       kG,        kH,   kJ,    kK,     kL,         kSemicolon, kApostrophe,  kEnter},
/* Row#4 */	{kPageup,   kLeftshift,kZ,         kX,       kC,     kV,       kB,        kN,   kM,    kComma, kDot,       kSlash,     kRightshift,  kUp},
/* Row#5 */	{kPagedown, kLeftctrl, kLeftmeta,  kLeftalt, kSpace, kRightalt,kRightctrl,kLeft,kDown, kRight, kBackslash, kDelete,    kNone,        kNone}
};

uint8_t gKeymap_buffer_layer1[KEYMAP_ROW_NUM][KEYMAP_COL_NUM] =
{           /*  Col#0       Col#1      Col#2       Col#3     Col#4   Col#5     Col#6      Col#7	Col#8  Col#9   Col#10      Col#11      Col#12        Col#13 */
/* Row#0 */	{0},
/* Row#1 */	{0},
/* Row#2 */	{0},
/* Row#3 */	{0},
/* Row#4 */	{0},
/* Row#5 */	{0}
};

