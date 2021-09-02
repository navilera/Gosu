/*
 * KeyHw.c
 *
 *  Created on: Feb 11, 2019
 *      Author: maanu
 */
#include "KeyHw.h"
#include "keymap.h"
#include "Layout.h"

#include "stm32f1xx_hal.h"

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

	HAL_Delay(1);

	bool pressed = false;

	for (uint32_t row = 0 ; row < KEYMAP_ROW_NUM ; row++)
	{
		SetRowToHigh(row);
		HAL_Delay(1);

		for (uint32_t col = 0 ; col < KEYMAP_COL_NUM ; col++)
		{
			pressed = GetColInput(col);

			if (pressed)
			{
				if (cnt > 0)
				{
					if (keyHwAddrBuff->bit.row == (uint8_t)row && keyHwAddrBuff->bit.col == (uint8_t)col)
					{
						continue;
					}
					else
					{
						keyHwAddrBuff++;
					}
				}

				keyHwAddrBuff->bit.row = (uint8_t)row;
				keyHwAddrBuff->bit.col = (uint8_t)col;

				DBG_PRINT("HW Polling (R:%u C:%u) %u\n", row, col, cnt);

				cnt++;

				if (cnt > max_count)
				{
					return cnt;
				}
			}
		}

		SetRowToLow(row);
	}

	return cnt;
}

bool KeyHw_IsPressed(uint32_t col, uint32_t row)
{
	bool isPressed = false;

	SetAllRowToLow();
	SetAllColToInput();

	SetRowToHigh(row);
	HAL_Delay(1);
	isPressed = GetColInput(col);

	return isPressed;
}

static void SetAllRowToLow(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	for (uint32_t row = 0 ; row < KEYMAP_ROW_NUM ; row++)
	{
		GPIO_InitStruct.Pin = gRowPin[row].num;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		HAL_GPIO_Init((GPIO_TypeDef*)gRowPin[row].port, &GPIO_InitStruct);
		HAL_GPIO_WritePin((GPIO_TypeDef*)gRowPin[row].port, gRowPin[row].num, GPIO_PIN_RESET);
	}
}

static void SetAllColToInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	for (uint32_t col = 0 ; col < KEYMAP_COL_NUM ; col++)
	{
		GPIO_InitStruct.Pin = gColPin[col].num;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;

		HAL_GPIO_Init((GPIO_TypeDef*)gColPin[col].port, &GPIO_InitStruct);
	}
}

static void SetRowToHigh(uint32_t idx)
{
	HAL_GPIO_WritePin((GPIO_TypeDef*)gRowPin[idx].port, gRowPin[idx].num, GPIO_PIN_SET);
}

static void SetRowToLow(uint32_t idx)
{
	HAL_GPIO_WritePin((GPIO_TypeDef*)gRowPin[idx].port, gRowPin[idx].num, GPIO_PIN_RESET);
}

static bool GetColInput(uint32_t idx)
{
	return (bool)HAL_GPIO_ReadPin((GPIO_TypeDef*)gColPin[idx].port, gColPin[idx].num);
}
