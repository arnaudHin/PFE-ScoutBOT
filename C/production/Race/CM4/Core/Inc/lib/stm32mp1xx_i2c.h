/*
 * stm32mp1xx_i2c.h
 *
 *  Created on: 9 déc. 2021
 *      Author: martin
 */

#ifndef INC_LIB_STM32MP1XX_I2C_H_
#define INC_LIB_STM32MP1XX_I2C_H_

#include "stm32mp1xx_hal.h"
#include "config.h"
#include <lib/stm32mp1xx_error_handler.h>
#include <lib/stm32mp1xx_gpio.h>



typedef enum
{
	I2C1_ID = 0,
	I2C2_ID,
	I2C3_ID,
	I2C4_ID,
	I2C5_ID,
	I2C6_ID,
	I2C_NB
}I2C_ID_e;

void I2C_Init(I2C_TypeDef* I2Cx, uint32_t clockSpeed);
HAL_StatusTypeDef I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *received_data, uint8_t size_received_data);
HAL_StatusTypeDef I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *writen_data, uint8_t size_writen_data);

#endif /* INC_LIB_STM32MP1XX_I2C_H_ */
