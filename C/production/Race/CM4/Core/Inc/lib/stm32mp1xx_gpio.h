/*
 * stm32mp1xx_gpio.h
 *
 *  Created on: 9 oct. 2021
 *      Author: martin
 */

#ifndef INC_LIB_STM32MP1XX_GPIO_H_
#define INC_LIB_STM32MP1XX_GPIO_H_

#include "stm32mp1xx_hal.h"

//////////////////////////////////////////////////////////////////////////////////
//				Define															//
//////////////////////////////////////////////////////////////////////////////////
#define LED5_PORT 				GPIOA
#define LED5_PIN 				GPIO_PIN_14
#define LED6_PORT 				GPIOA
#define LED6_PIN 				GPIO_PIN_13
#define LED7_PORT 				GPIOH
#define LED7_PIN 				GPIO_PIN_7
#define LED8_PORT 				GPIOD
#define LED8_PIN 				GPIO_PIN_11

#define BUTTON_USER1_PORT			GPIOA
#define BUTTON_USER1_PIN			GPIO_PIN_14
#define BUTTON_USER2_PORT			GPIOA
#define BUTTON_USER2_PIN			GPIO_PIN_13


#define ENABLE_MOTOR_NEGATIVE			GPIOE, GPIO_PIN_7 //D0

#define DIR_MOTOR_LEFT_NEGATIVE			GPIOE, GPIO_PIN_1 //D2 IO
#define DIR_MOTOR_RIGHT_NEGATIVE		GPIOE, GPIO_PIN_10 //D4 IO

#define PULSE_MOTOR_LEFT_NEGATIVE		GPIOD, GPIO_PIN_14 //D3 T4_CH3
#define PULSE_MOTOR_RIGHT_NEGATIVE		GPIOD, GPIO_PIN_15 //D5 T4_CH4




//////////////////////////////////////////////////////////////////////////////////
//				Structures														//
//////////////////////////////////////////////////////////////////////////////////

typedef enum{
	LED5_GREEN=0,
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

