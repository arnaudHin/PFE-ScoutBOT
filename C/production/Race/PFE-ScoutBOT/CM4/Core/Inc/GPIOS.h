/*
 * GPIOS.c
 *
 *  Created on: Oct 18, 2021
 *: Arnaud
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

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
//				Structures					//
//////////////////////////////////////////////////////////////////////////////////


typedef enum{
	LED5_GREEN=0,
	LED6_RED,
	LED7_ORANGE,
	LED8_BLUE
}LED_e;



/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

extern void GPIO_Init(void);

extern void BSP_GPIO_PinCfg(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed, uint32_t GPIO_Alternate);

extern void ON_LED(LED_e led);

extern void OFF_LED(LED_e led);

extern void TOGGLE_LED(LED_e led);






/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
