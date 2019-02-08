#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "stm32f1xx_hal.h"

#include "usb_device.h"
#include "usbd_hid.h"
#include "usbhid.h"

#include "HalGpio.h"
#include "HalUart.h"


#define SYSTEM_US_TICKS (SystemCoreClock / 1000000) // cycles per microsecond

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static inline void HID_Send(const void *data,const size_t size)
{
  debug_printf("Sending: ");
  for (int i=0; i<size; ++i)
  {
   debug_printf("%u ",((uint8_t*)data)[i]);
  }
  debug_printf("\n\r");
  uint8_t status;
  do
  {
    status = USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)data, size);
    if (status==USBD_FAIL)
    {
      debug_printf("USB: fail\n\r");
    }
  }
  while (status == USBD_BUSY);
}

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	Hal_gpio_init();
	Hal_uart_init();

	MX_USB_DEVICE_Init();

	debug_printf("Navilos Start..\n");

	uint8_t hidpack[9] = { 0 };

	hidpack[0] = 2;




	while (1)
	{
		USBD_LL_Delay(1000);
		hidpack[3] = HIDKEY_A;
		HID_Send(hidpack, 9);

		USBD_LL_Delay(1000);
		hidpack[3] = 0;
		HID_Send(hidpack, 9);
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

