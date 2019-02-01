/*
 * Uart.c
 *
 *  Created on: Sep 8, 2018
 *      Author: maanu
 */

#include "stdint.h"
#include "stdbool.h"

#include "HalUart.h"

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

void Hal_uart_init(void)
{
    GPIO_InitTypeDef GPIO_Settings;
    USART_InitTypeDef USART_Settrings;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_StructInit(&GPIO_Settings);
    GPIO_Settings.GPIO_Pin = GPIO_Pin_9;
    GPIO_Settings.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_Settings);

    GPIO_Settings.GPIO_Pin = GPIO_Pin_10;
    GPIO_Settings.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_Settings);

    USART_StructInit(&USART_Settrings);
    USART_Settrings.USART_BaudRate = 9600;
    USART_Settrings.USART_WordLength = USART_WordLength_8b;
    USART_Settrings.USART_StopBits = USART_StopBits_1;
    USART_Settrings.USART_Parity = USART_Parity_No;
    USART_Settrings.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Settrings.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_Settrings);

    USART_Cmd(USART1, ENABLE);

    __enable_irq();
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}

void Hal_uart_put_char(uint8_t ch)
{
	while(!(USART1->SR & USART_SR_TC));
	USART1->DR = ch;
}

uint8_t Hal_uart_get_char(void)
{
    return (uint8_t)USART_ReceiveData(USART1);
}

void USART1_IRQHandler()
{
    unsigned char received;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        received = Hal_uart_get_char();
        Hal_uart_put_char(received);
    }
}

