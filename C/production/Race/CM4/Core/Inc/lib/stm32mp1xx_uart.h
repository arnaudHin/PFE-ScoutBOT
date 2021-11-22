/*
 * stm32mp1xx_uart.h
 *
 *  Created on: Oct 18, 2021
 *      Author: martin
 */

#ifndef INC_LIB_STM32MP1XX_UART_H_
#define INC_LIB_STM32MP1XX_UART_H_

#include "stm32mp1xx_hal.h"
#include "config.h"
#include <lib/stm32mp1xx_error_handler.h>
#include <lib/stm32mp1xx_gpio.h>

typedef enum
{
	UART1_ID = 0,
	UART2_ID,
	UART3_ID,
	UART4_ID,
	UART5_ID,
	UART6_ID,
	UART7_ID,
	UART8_ID,
	UART_ID_NB
}uart_id_e;

void UART_init(uart_id_e uart_id, uint32_t baudrate);
void UART_puts(uart_id_e uart_id, uint8_t * str, uint32_t len);


#endif /* INC_LIB_STM32MP1XX_UART_H_ */
