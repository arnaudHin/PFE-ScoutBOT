/*
 * stm32mp1xx_gpio.h
 *
 *  Created on: 9 oct. 2021
 *      Author: martin
 */

#ifndef INC_LIB_STM32MP1XX_GPIO_H_
#define INC_LIB_STM32MP1XX_GPIO_H_

#include "stm32mp1xx_hal.h"
#include "config.h"
#include <lib/stm32mp1xx_error_handler.h>

//////////////////////////////////////////////////////////////////////////////////
//				Define															//
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//				Structures														//
//////////////////////////////////////////////////////////////////////////////////

typedef enum{
	LED4_GREEN=0,
	LED6_RED,
	LED7_ORANGE,
	LED8_BLUE
}LED_e;

//////////////////////////////////////////////////////////////////////////////////
//				Fonctions														//
//////////////////////////////////////////////////////////////////////////////////


void GPIO_Init (void);
void BSP_GPIO_PinCfg(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed, uint32_t GPIO_Alternate);
void PIN_On(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin);
void PIN_Off(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin);
void PIN_Toggle(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin);
void ON_Led(LED_e led);
void OFF_Led(LED_e led);
void TOGGLE_Led(LED_e led);

#endif /* INC_LIB_STM32MP1XX_GPIO_H_ */

