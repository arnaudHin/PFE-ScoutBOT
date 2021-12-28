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
#define LED4 					GPIOA, GPIO_PIN_14
//LED RED
#define LED6					GPIOA, GPIO_PIN_13
//LED ORANGE
#define LED7					GPIOH, GPIO_PIN_7
//LED BLUE
#define LED8 					GPIOD, GPIO_PIN_11
//LED SHIELD BLUE
#define LED1					GPIOG, GPIO_PIN_3
//LED SHIELD GREEN
#define LED2					GPIOE, GPIO_PIN_9


/*
 * UART3 ET UART7 (printf) NUCLEO
 */

#define USE_UART3								0			//USART3_TX PB10 (PIN8)    USART3_RX PB12 (PIN10)
#define UART3_BAUDRATE 							115200

#define USE_PRINTF_UART7								1		  //UART7_TX PE8 (D1)  UART7_RX PE7 (D0)
#define UART7_BAUDRATE 							115200


/*
 * I2C
 */
#define I2C_TIMEOUT	1000

#define I2C5_SDA							GPIOA, GPIO_PIN_12

#define I2C5_SCL							GPIOA, GPIO_PIN_11

/*
 * ADC
 */

#define ADC1_IN13							GPIOC, GPIO_PIN_3		//A4 PC3

/*
 * MPU6050 I2C
 */

#define USE_MPU6050								1
#define MPU6050_I2C								I2C5
#define MPU6050_VCC								GPIOE, GPIO_PIN_11

/*
 * MOTORS
 */

#define USE_MOTORS								1
#define ENABLE_MOTOR_NEGATIVE			GPIOE, GPIO_PIN_7 //D0

#define DIR_MOTOR_LEFT_NEGATIVE			GPIOE, GPIO_PIN_1 //D2 IO
#define DIR_MOTOR_RIGHT_NEGATIVE		GPIOE, GPIO_PIN_10 //D4 IO

#define PULSE_MOTOR_LEFT_NEGATIVE		GPIOD, GPIO_PIN_14 //D3 T4_CH3
#define PULSE_MOTOR_RIGHT_NEGATIVE		GPIOD, GPIO_PIN_15 //D5 T4_CH4

/*
 * BATTERY
 */
#define USE_BATTERY								0
#define BAT_ENABLE						GPIOD, GPIO_PIN_1		//D7
#define BAT_ADC 						ADC1


#endif /* INC_CONFIG_H_ */
