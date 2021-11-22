/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <robot/robot.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  if(IS_ENGINEERING_BOOT_MODE())
  {
    /* Configure the system clock */
    SystemClock_Config();
  }

  __HAL_RCC_HSEM_CLK_ENABLE();
  MX_IPCC_Init();
  MX_OPENAMP_Init(RPMSG_REMOTE, NULL);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  /* USER CODE BEGIN 2 */
	GPIO_Init();

	#if USE_UART7
	  UART_init(UART7_ID,UART7_BAUDRATE);
	#endif

	#if USE_UART3
	  UART_init(UART3_ID,UART3_BAUDRATE);
	#endif
  /* USER CODE END 2 */
	  TIMER_INIT();
	  TIMER4_RUN();
	  MOTORS_stop();

  /* USER CODE BEGIN SysInit */
  printf("Cortex-M4 boot successful with STM32Cube FW version: v%ld.%ld.%ld \r\n",
											((HAL_GetHalVersion() >> 24) & 0x000000FF),
											((HAL_GetHalVersion() >> 16) & 0x000000FF),
											((HAL_GetHalVersion() >> 8) & 0x000000FF));

  printf("Virtual UART0 OpenAMP-rpmsg channel creation\r\n");



  VIRT_UART_Init_0();

  postman_robot_init();
  robot_init();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {

	  postman_robot_run();


  }
  /* USER CODE END 3 */
}


/* USER CODE END 4 */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
