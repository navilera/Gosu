/*
 * PollingTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"
#include "usbd_hid.h"

#include "Kernel.h"

/* Default Keymap
 *                                                    COL (input)
 *      +--------+----------------------------------------------------------------------------------------------+
 *      | Col#   |  0   |  1    |  2  | 3   |  4    |  5  | 6    | 7   | 8   |  9  | 10  |  11  | 12    |   13  |
 *      +--------+------+-------+-----+-----+-------+-----+------+-----+-----+-----+-----+------+-------+-------+
 *      |R# |    | PA6  | PA7   | PB0 | PB1 | PB3   | PB4 | PB5  | PB6 | PB7 | PB8 | PB9 | PB12 | PB13  | PB14  |
 *      +---+----+------+-------+-----+-----+-------+-----+------+-----+-----+-----+-----+------+-------+-------+
 *      | 0 |PA0 | Fn   | ESC   | F1  | F2  | F3    | F4  | F5   | F6  | F7  | F8  | F9  | F10  | F11   |  F12  |
 *      | 1 |PA1 | PgUp | ~     | 1   | 2   | 3     | 4   | 5    | 6   | 7   | 8   | 9   | 0    | -     |  +    |
 * ROW  | 2 |PA2 | PgDn | Tab   | Q   | W   | E     | R   | T    | Y   | U   | I   | O   | P    | [     |  ]    |
 *(out) | 3 |PA3 | Inst | A     | S   | D   | F     | G   | H    | J   | K   | L   | ;   | '    | Enter |  back |
 *      | 4 |PA4 | Home | shift | Z   | X   | C     | V   | B    | N   | M   | <   | >   | ?    | shift |  del  |
 *      | 5 |PA5 | End  | ctrl  | Win | alt | space | alt | ctrl | ←   | ↓   | →   | ↑   | \    |       |       |
 *      +-- +---------------------------------------------------------------------------------------------------+
 *
 *  key_polling_code
 *        +-----+---------------+--------------------+
 *  bit#  |  7  |     6 : 4     |      3 : 0         |
 *        +-----+---------------+--------------------+
 *        | Fn  |    Row Num    |      Col Num       |
 *        +-----+---------------+--------------------+
 *
 *        For example : Press 'T'
 *          - 'T' is Row 2 (PA2 out) and Col 6 (PB5 input)
 *          - 0 (0) 010 (2) 0110 (6) -> 00100110 -> 0x26
 *        For example : Press 'T' with Fn
 *          - 1 (1) 010 (2) 0110 (6) -> 10100110 -> 0xA6
 */

/* Extra Pin usage
 * PA9,  PA10 - UART1 - Debug CLI
 * PA11, PA12 - USB
 * PB10, PB11 - UART3 - Bluetooth
 */

typedef union KeyPolling
{
	uint8_t val;
	struct {
		uint8_t col:4;
		uint8_t row:3;
		uint8_t fn:1;
	} bit;
} KeyPolling_t;

void Polling_task(void)
{
    debug_printf("Polling Task....\n");

    while (true)
    {
    	USBD_Delay(HID_FS_BINTERVAL);
        //debug_printf("Polling Task before context switch : %x %x\n", &a, &b);


        Kernel_yield();

        //debug_printf("Polling Task after context switch : %x -> %u\n", &c, HAL_GetTick());

    }
}
