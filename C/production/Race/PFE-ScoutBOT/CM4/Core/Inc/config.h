/*
 * config.h
 *
 *  Created on: 9 oct. 2021
 *      Author: martin
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

//////////////////////////////////////////////////////////////////////////////////
//									Define										//
//////////////////////////////////////////////////////////////////////////////////

/*
 * GPIOS LED
 */

//LED GREEN
#define LED4_PORT 					GPIOA
#define LED4_PIN 					GPIO_PIN_14
//LED RED
#define LED6_PORT 					GPIOA
#define LED6_PIN 					GPIO_PIN_13
//LED ORANGE
#define LED7_PORT 					GPIOH
#define LED7_PIN 					GPIO_PIN_7
//LED BLUE
#define LED8_PORT 					GPIOD
#define LED8_PIN 					GPIO_PIN_11

/*
 * UART 1 ET UART2 (printf) NUCLEO
 */

#define USE_UART1								0
#define UART1_BAUDRATE							115200

#define USE_UART2								0
#define UART2_BAUDRATE 							115200

#define USE_UART3								1			//USART3_TX PB10 (PIN8)    USART3_RX PB12 (PIN10)
#define UART3_BAUDRATE 							115200

#define USE_UART4								0
#define UART4_BAUDRATE 							115200

#define USE_UART5								0
#define UART5_BAUDRATE 							115200

#define USE_UART6								0
#define UART6_BAUDRATE 							115200

#define USE_UART7								0		  //UART7_TX PE8 (D1)  UART7_RX PE7 (D0)
#define UART7_BAUDRATE 							115200

#define USE_UART8								0
#define UART8_BAUDRATE 							115200


#define ENABLE_MOTOR_NEGATIVE			GPIOE, GPIO_PIN_7 //D0

#define DIR_MOTOR_LEFT_NEGATIVE			GPIOE, GPIO_PIN_1 //D2 IO
#define DIR_MOTOR_RIGHT_NEGATIVE		GPIOE, GPIO_PIN_10 //D4 IO

#define PULSE_MOTOR_LEFT_NEGATIVE		GPIOD, GPIO_PIN_14 //D3 T4_CH3
#define PULSE_MOTOR_RIGHT_NEGATIVE		GPIOD, GPIO_PIN_15 //D5 T4_CH4


#endif /* INC_CONFIG_H_ */
