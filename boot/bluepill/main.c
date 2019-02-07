#include "stm32f1xx_hal.h"
#include "usb_device.h"

#include "stdint.h"
#include "usbd_hid.h"
#include "usbhid.h"
#include "dwt_stm32_delay.h"

#include "stdio.h"

#include "HalGpio.h"
#include "HalUart.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;


/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define SYSTEM_US_TICKS (SystemCoreClock / 1000000) // cycles per microsecond



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_TIM1_Init(void);

static inline void HID_Send(const void *data,const size_t size)
{
  debug_printf("Sending: ");
  for (int i=0; i<size; ++i)
  {
   debug_printf("%02x ",((uint8_t*)data)[i]);
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

// microsecond version
uint32_t HAL_GetTicks_us()
{
  return DWT->CYCCNT / SYSTEM_US_TICKS;
}

/**********************************************************
// microsecond version
void HAL_Delay_us(uint32_t uSec)
{
  volatile uint32_t DWT_START = DWT->CYCCNT;
  if (uSec > (UINT_MAX / SYSTEM_US_TICKS))
  {
    uSec = (UINT_MAX / SYSTEM_US_TICKS);
  }

  volatile uint32_t DWT_TOTAL = (SYSTEM_US_TICKS * uSec);

  while((DWT->CYCCNT - DWT_START) < DWT_TOTAL);
}
*/

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

int main(void)
{
  HAL_Init();

  SystemClock_Config();
  MX_TIM1_Init();

  Hal_gpio_init();
  Hal_uart_init();

  MX_USB_DEVICE_Init();



  debug_printf("Navilos Start..\n");

  while (1)
  {
      //HID_Send(&keyboardHID,sizeof(keyboardHID));
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
    _Error_Handler(__FILE__, __LINE__);
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
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
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

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}



