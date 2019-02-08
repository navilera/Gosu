/*
 * Uart.c
 *
 *  Created on: Sep 8, 2018
 *      Author: maanu
 */

#include "stdint.h"
#include "stdbool.h"

#include "stm32f1xx_hal.h"

#include "HalUart.h"

UART_HandleTypeDef huart1;

void Hal_uart_init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.gState = HAL_UART_STATE_RESET;

	HAL_UART_Init(&huart1);

	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void Hal_uart_put_char(uint8_t ch)
{
	HAL_UART_Transmit(&huart1, &ch, 1, 1000);
}

uint8_t Hal_uart_get_char(void)
{
	uint8_t ch;
	HAL_UART_Receive(&huart1, &ch, 1, 1000);
	return ch;
}

void Hal_uart_isr(void)
{
	uint32_t stack = 0;
	debug_printf("ISR stack=%x\n", &stack);

	Hal_uart_put_char(Hal_uart_get_char());
}

