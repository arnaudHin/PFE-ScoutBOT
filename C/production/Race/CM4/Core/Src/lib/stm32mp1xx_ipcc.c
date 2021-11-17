/*
 * stm32mp1xx_ipcc.c
 *
 *  Created on: Oct 19, 2021
 *      Author: martin
 */


#include <lib/stm32mp1xx_ipcc.h>
#include <lib/stm32mp1xx_error_handler.h>

IPCC_HandleTypeDef hipcc;


/**
  * @brief IPCC Initialization Function
  * @param None
  * @retval None
  */
extern void MX_IPCC_Init(void)
{

  /* USER CODE BEGIN IPCC_Init 0 */

  /* USER CODE END IPCC_Init 0 */

  /* USER CODE BEGIN IPCC_Init 1 */

  /* USER CODE END IPCC_Init 1 */
  hipcc.Instance = IPCC;
  if (HAL_IPCC_Init(&hipcc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IPCC_Init 2 */

  /* USER CODE END IPCC_Init 2 */

}
