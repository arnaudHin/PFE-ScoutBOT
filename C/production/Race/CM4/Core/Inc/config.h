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
 * UART3 ET UART7 (printf) NUCLEO
 */

#define USE_UART3								0			//USART3_TX PB10 (PIN8)    USART3_RX PB12 (PIN10)
#define UART3_BAUDRATE 							115200

#define USE_UART7								1		  //UART7_TX PE8 (D1)  UART7_RX PE7 (D0)
#define UART7_BAUDRATE 							115200


/*
 * I2C
 */
#define I2C_TIMEOUT	1000

#define I2C5_SDA_GPIO							GPIOA
#define I2C5_SDA_PIN							GPIO_PIN_12

#define I2C5_SCL_GPIO							GPIOA
#define I2C5_SCL_PIN							GPIO_PIN_11

/*
 * MPU6050 I2C
 */

#define MPU6050_I2C								I2C5

/*
 * MOTORS
 */


#define ENABLE_MOTOR_NEGATIVE			GPIOE, GPIO_PIN_7 //D0

#define DIR_MOTOR_LEFT_NEGATIVE			GPIOE, GPIO_PIN_1 //D2 IO
#define DIR_MOTOR_RIGHT_NEGATIVE		GPIOE, GPIO_PIN_10 //D4 IO

#define PULSE_MOTOR_LEFT_NEGATIVE		GPIOD, GPIO_PIN_14 //D3 T4_CH3
#define PULSE_MOTOR_RIGHT_NEGATIVE		GPIOD, GPIO_PIN_15 //D5 T4_CH4

/*
 * BATTERY
 */

#define BAT_ENABLE						GPIOD, GPIO_PIN_1		//D7


#endif /* INC_CONFIG_H_ */
