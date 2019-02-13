/*
 * KeyHw.c
 *
 *  Created on: Feb 11, 2019
 *      Author: maanu
 */

#include "KeyHw.h"
#include "keymap.h"

#include "stm32f1xx_hal.h"

/* Default Keymap
 *                                                    COL (input)
 *      +--------+----------------------------------------------------------------------------------------------+
 *      | Col#   |  0   |  1    |  2  | 3   |  4    |  5  | 6    | 7   | 8   |  9  | 10  |  11  | 12    |   13  |
 *      +--------+------+-------+-----+-----+-------+-----+------+-----+-----+-----+-----+------+-------+-------+
 *      |R# |    | PA6  | PA7   | PB0 | PB1 | PB3   | PB4 | PB5  | PB6 | PB7 | PB8 | PB9 | PB12 | PB13  | PB14  |
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

typedef struct Keypin
{
	uint32_t port;
	uint32_t num;
} Keypin_t;

Keypin_t sRowPin[KEYMAP_ROW_NUM] = {
		{(uint32_t)GPIOA, GPIO_PIN_0},
		{(uint32_t)GPIOA, GPIO_PIN_1},
		{(uint32_t)GPIOA, GPIO_PIN_2},
		{(uint32_t)GPIOA, GPIO_PIN_3},
		{(uint32_t)GPIOA, GPIO_PIN_4},
		{(uint32_t)GPIOA, GPIO_PIN_5}
};

Keypin_t sColPin[KEYMAP_COL_NUM] = {
		{(uint32_t)GPIOA, GPIO_PIN_6},  {(uint32_t)GPIOA, GPIO_PIN_7},
		{(uint32_t)GPIOB, GPIO_PIN_0},  {(uint32_t)GPIOB, GPIO_PIN_1},
		{(uint32_t)GPIOB, GPIO_PIN_3},  {(uint32_t)GPIOB, GPIO_PIN_4},  {(uint32_t)GPIOB, GPIO_PIN_5}, {(uint32_t)GPIOB, GPIO_PIN_6}, {(uint32_t)GPIOB, GPIO_PIN_7}, {(uint32_t)GPIOB, GPIO_PIN_8}, {(uint32_t)GPIOB, GPIO_PIN_9},
		{(uint32_t)GPIOB, GPIO_PIN_12}, {(uint32_t)GPIOB, GPIO_PIN_13}, {(uint32_t)GPIOB, GPIO_PIN_14}
};

static void SetAllRowToLow(void);
static void SetAllColToInput(void);
static void SetRowToHigh(uint32_t idx);
static void SetRowToLow(uint32_t idx);
static bool GetColInput(uint32_t idx);

uint32_t KeyHw_polling(KeyHwAddr_t* keyHwAddrBuff, uint32_t max_count)
{
	uint32_t cnt = 0;

	SetAllRowToLow();
	SetAllColToInput();

	bool pressed = false;

	for (uint32_t row = 0 ; row < KEYMAP_ROW_NUM ; row++)
	{
		SetRowToHigh(row);

		for (uint32_t col = 0 ; col < KEYMAP_COL_NUM ; col++)
		{
			pressed = GetColInput(col);

			if (pressed)
			{
				cnt++;

				if (cnt > max_count)
				{
					return cnt;
				}

				keyHwAddrBuff->bit.row = (uint8_t)row;
				keyHwAddrBuff->bit.col = (uint8_t)col;
				keyHwAddrBuff++;
			}
		}

		SetRowToLow(row);
	}

	return cnt;
}

static void SetAllRowToLow(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	for (uint32_t row = 0 ; row < KEYMAP_ROW_NUM ; row++)
	{
		GPIO_InitStruct.Pin = sRowPin[row].num;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		HAL_GPIO_Init((GPIO_TypeDef*)sRowPin[row].port, &GPIO_InitStruct);
		HAL_GPIO_WritePin((GPIO_TypeDef*)sRowPin[row].port, sRowPin[row].num, GPIO_PIN_RESET);
	}
}

static void SetAllColToInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	for (uint32_t col = 0 ; col < KEYMAP_COL_NUM ; col++)
	{
		GPIO_InitStruct.Pin = sColPin[col].num;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;

		HAL_GPIO_Init((GPIO_TypeDef*)sColPin[col].port, &GPIO_InitStruct);
	}
}

static void SetRowToHigh(uint32_t idx)
{
	HAL_GPIO_WritePin((GPIO_TypeDef*)sRowPin[idx].port, sRowPin[idx].num, GPIO_PIN_SET);
}

static void SetRowToLow(uint32_t idx)
{
	HAL_GPIO_WritePin((GPIO_TypeDef*)sRowPin[idx].port, sRowPin[idx].num, GPIO_PIN_RESET);
}

static bool GetColInput(uint32_t idx)
{
	return (bool)HAL_GPIO_ReadPin((GPIO_TypeDef*)sColPin[idx].port, sColPin[idx].num);
}
