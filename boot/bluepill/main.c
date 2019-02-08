#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "stm32f1xx_hal.h"

#include "usb_device.h"
#include "app/kmapdl/usb_kmapdl.h"

#include "HalGpio.h"
#include "HalUart.h"

#include "Kernel.h"
#include "bluepill_def.h" /* boot/bluepill/bluepill_def.h */

#define SYSTEM_US_TICKS (SystemCoreClock / 1000000) // cycles per microsecond

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
BootMode CheckBootMode(void);

void User_task0(void);
void User_task1(void);
void User_task2(void);

static void Kernel_Init(BootMode mode)
{
    uint32_t taskId;

    Kernel_task_init();

    taskId = Kernel_task_create(User_task0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        //putstr("Task0 creation fail\n");
        debug_printf("Task0 creation fail\n");
    }

    taskId = Kernel_task_create(User_task1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        //putstr("Task1 creation fail\n");
        debug_printf("Task1 creation fail\n");
    }

    taskId = Kernel_task_create(User_task2);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        //putstr("Task2 creation fail\n");
        debug_printf("Task2 creation fail\n");
    }

    if (mode != bootKeymapDl) return;

    taskId = Kernel_task_create(kmapdl_task);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        //putstr("Task2 creation fail\n");
        debug_printf("Keymap Dl creation fail\n");
    }
}

int main(void)
{
    BootMode bmode;
    HAL_Init();
    SystemClock_Config();

    Hal_gpio_init();
    Hal_uart_init();

    bmode = CheckBootMode();

    /* Initialization stage */
    switch(bmode) {
      case bootNormal:
        App_usb_Init();
        break;

      case bootKeymapDl:
        kmapdl_init();
        break;

      case bootDFU:
        /* Do Nothing at this time */
        break;

      default:
        /* Error */
        break;
    }

    Kernel_Init(bmode);
    debug_printf("Navilos Start..\n");
    Kernel_start();

    while (1)
    {
        /*
        //uint8_t hidpack[8] = { 0 };
        USBD_Delay(1000);
        hidpack[2] = 4;
        App_hid_send(hidpack, sizeof(hidpack));

        USBD_Delay(1000);
        hidpack[2] = 0;
        App_hid_send(hidpack, sizeof(hidpack));
        */
    }

    /* Cleanup stage */
    switch(bmode) {
      case bootNormal:
        break;

      case bootKeymapDl:
        kmapdl_cleanup();
        break;

      case bootDFU:
        /* Do Nothing at this time */
        break;

      default:
        /* Error */
        break;
    }
}

void User_task0(void)
{
    int a = 1;
    int b = 2;
    int c = 0;

    while (1)
    {
        debug_printf("Task0 before context switch : %x %x\n", &a, &b);

        c = a + b;

        Kernel_yield();

        debug_printf("Task0 after context switch : %x\n", &c);

        a++;
        b++;
    }
}

BootMode CheckBootMode(void)
{
    // get Fn key pressed during keyboard power-up time.
    // while user presses Fn key, FW enters a keymap download mode or FW update mode.
    return bootNormal;
}

void User_task1(void)
{
    int a = 3;
    int b = 4;
    int c = 0;

        while (1)
    {
        debug_printf("Task1 before context switch : %x %x\n", &a, &b);

        c = a + b;

                Kernel_yield();

        debug_printf("Task1 after context switch : %x\n", &c);
        a++;
        b++;
    }
}

void User_task2(void)
{
    int a = 5;
    int b = 6;
    int c = 0;

    while (1)
    {
        debug_printf("Task2 before context switch : %x %x\n", &a, &b);

        c = a + b;

        Kernel_yield();

        debug_printf("Task2 after context switch : %x\n", &c);

        a++;
        b++;
    }
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
      {
        halt(__FILE__, __LINE__);
      }

    /**Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
      |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
      {
        halt(__FILE__, __LINE__);
      }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
      {
        halt(__FILE__, __LINE__);
      }

    /**Configure the Systick interrupt time 
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
     */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

