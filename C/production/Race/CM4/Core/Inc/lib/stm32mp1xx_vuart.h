/*
 * stm32mp1xx_vuart.h
 *
 *  Created on: Oct 19, 2021
 *      Author: martin
 */

#ifndef INC_LIB_STM32MP1XX_VUART_H_
#define INC_LIB_STM32MP1XX_VUART_H_

#define VUART_TRAME_SIZE (8)
#define BUFFER_SEND_MESSAGE (13)

#include "stm32mp1xx.h"
#include "openamp.h"
#include "openamp_log.h"
#include "virt_uart.h"
#include <lib/stm32mp1xx_error_handler.h>
#include <lib/stm32mp1xx_gpio.h>


extern void VIRT_UART_Init_0(void);
extern void VIRT_UART_RxCpltCallback_0(VIRT_UART_HandleTypeDef *huart);
extern void VIRT_UART_Callback_0();
extern __IO FlagStatus get_RX_msg_0();
extern void set_RX_msg_0(__IO FlagStatus flag);
extern uint8_t* VIRT_UART_read_message_0();
extern void VIRT_UART_send_message_0(unsigned char buffer[BUFFER_SEND_MESSAGE],int buffer_size);

extern void VIRT_UART_Init_1(void);
extern void VIRT_UART_RxCpltCallback_1(VIRT_UART_HandleTypeDef *huart);
extern void VIRT_UART_Callback_1();
extern __IO FlagStatus get_RX_msg_1();
extern void set_RX_msg_1(__IO FlagStatus flag);
extern uint8_t* VIRT_UART_read_message_1();
extern void VIRT_UART_send_message_1(unsigned char buffer[BUFFER_SEND_MESSAGE],int buffer_size);



#endif /* INC_LIB_STM32MP1XX_VUART_H_ */
