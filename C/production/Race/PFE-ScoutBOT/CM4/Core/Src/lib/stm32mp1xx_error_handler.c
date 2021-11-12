/*
 * stm32mp1xx_error_handler.c
 *
 *  Created on: Oct 19, 2021
 *      Author: martin
 */


#include <lib/stm32mp1xx_error_handler.h>


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
