/*
 * Uart.c
 *
 *  Created on: Sep 8, 2018
 *      Author: maanu
 */

#include "stdint.h"
#include "stdbool.h"

#include "HalUart.h"
#include "HalInterrupt.h"

#include "Kernel.h"

void Hal_uart_init(void)
{
}

void Hal_uart_put_char(uint8_t ch)
{
}

uint8_t Hal_uart_get_char(void)
{
    uint32_t data = 0;

    return (uint8_t)(data & 0xFF);
}

void interrupt_handler(void)
{
    uint8_t ch = Hal_uart_get_char();

    if (ch == 'U')
    {
        Kernel_send_events(KernelEventFlag_Unlock);
        return;
    }

    if (ch == 'X')
    {
        Kernel_send_events(KernelEventFlag_CmdOut);
        return;
    }

    Hal_uart_put_char(ch);
    Kernel_send_msg(KernelMsgQ_Task0, &ch, 1);
    Kernel_send_events(KernelEventFlag_UartIn);
}
