/*
 * stm32mp1xx_i2c.c
 *
 *  Created on: 9 déc. 2021
 *      Author: martin
 */


#include <lib/stm32mp1xx_i2c.h>
#include "stm32mp1xx_hal_i2c.h"
#include "stm32mp1xx_hal_i2c_ex.h"



static I2C_HandleTypeDef  hi2c[I2C_NB];

void I2C_Init(I2C_TypeDef* I2Cx, uint32_t clockSpeed){
	I2C_ID_e id = ((I2Cx == I2C1)?I2C1_ID:(I2Cx == I2C2)?I2C2_ID:(I2Cx == I2C3)?I2C3_ID:(I2Cx == I2C4)?I2C4_ID:(I2Cx == I2C5)?I2C5_ID:I2C6_ID);
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	if(id == I2C5_ID)
	{
		if(IS_ENGINEERING_BOOT_MODE()){
				PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C35;
			    PeriphClkInit.I2c35ClockSelection = RCC_I2C35CLKSOURCE_PCLK1;
				if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
				{
					Error_Handler();
				}

		}
		__HAL_RCC_I2C5_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		BSP_GPIO_PinCfg(I2C5_SDA, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF4_I2C5);
		BSP_GPIO_PinCfg(I2C5_SCL, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF4_I2C5);
	}

	hi2c[id].Instance = I2Cx;
	hi2c[id].Init.Timing = clockSpeed;
	hi2c[id].Init.OwnAddress1 = 0;
	hi2c[id].Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c[id].Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c[id].Init.OwnAddress2 = 0;
	hi2c[id].Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c[id].Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c[id]) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_I2CEx_ConfigAnalogFilter(&hi2c[id], I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_I2CEx_ConfigDigitalFilter(&hi2c[id], 0) != HAL_OK)
	{
			Error_Handler();
	}
}

HAL_StatusTypeDef I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *received_data, uint8_t size_received_data)
{
	I2C_ID_e id = ((I2Cx == I2C1)?I2C1_ID:(I2Cx == I2C2)?I2C2_ID:(I2Cx == I2C3)?I2C3_ID:(I2Cx == I2C4)?I2C4_ID:(I2Cx == I2C5)?I2C5_ID:I2C6_ID);
	return HAL_I2C_Mem_Read(&hi2c[id],address,reg,I2C_MEMADD_SIZE_8BIT,received_data,size_received_data,I2C_TIMEOUT);
}


HAL_StatusTypeDef I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *writen_data, uint8_t size_writen_data)
{
	I2C_ID_e id = ((I2Cx == I2C1)?I2C1_ID:(I2Cx == I2C2)?I2C2_ID:(I2Cx == I2C3)?I2C3_ID:(I2Cx == I2C4)?I2C4_ID:(I2Cx == I2C5)?I2C5_ID:I2C6_ID);
	return HAL_I2C_Mem_Write(&hi2c[id],address,reg,I2C_MEMADD_SIZE_8BIT,writen_data,size_writen_data,I2C_TIMEOUT);
}





