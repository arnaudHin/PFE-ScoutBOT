/*
 * stm32mp1xx_vuart.c
 *
 *  Created on: Oct 19, 2021
 *      Author: martin
 */

#include <lib/stm32mp1xx_vuart.h>

#define MAX_BUFFER_SIZE (125)

VIRT_UART_HandleTypeDef huart0;
VIRT_UART_HandleTypeDef huart1;

__IO FlagStatus VirtUart0RxMsg = RESET;
uint8_t VirtUart0ChannelBuffRx[MAX_BUFFER_SIZE];
uint16_t VirtUart0ChannelRxSize = 0;

__IO FlagStatus VirtUart1RxMsg = RESET;
uint8_t VirtUart1ChannelBuffRx[MAX_BUFFER_SIZE];
uint16_t VirtUart1ChannelRxSize = 0;

extern void VIRT_UART_Init_0(void){
	if (VIRT_UART_Init(&huart0) != VIRT_UART_OK) {
	  printf("VIRT_UART_Init_0 UART0 failed.\r\n");
	  Error_Handler();
	}
	else{
		printf("VIRT_UART0 rpmsg channel creation\n");
	}

	VIRT_UART_RegisterCallback(&huart0, VIRT_UART_RXCPLT_CB_ID, VIRT_UART_RxCpltCallback_0);

}

extern void VIRT_UART_Init_1(void){
	if (VIRT_UART_Init(&huart1) != VIRT_UART_OK) {
	  printf("VIRT_UART_Init_1 UART1 failed.\r\n");
	  Error_Handler();
	}
	else{
	  printf("VIRT_UART0 rpmsg channel creation\n");
	}
}

extern void VIRT_UART_RxCpltCallback_0(VIRT_UART_HandleTypeDef *huart)
{

    printf("Msg received on VIRTUAL UART0 channel:  %s \n\r", (char *) huart->pRxBuffPtr);

    /* copy received msg in a variable to sent it back to master processor in main infinite loop*/
    VirtUart0ChannelRxSize = huart->RxXferSize < MAX_BUFFER_SIZE? huart->RxXferSize : MAX_BUFFER_SIZE-1;
    memcpy(VirtUart0ChannelBuffRx, huart->pRxBuffPtr, VirtUart0ChannelRxSize);
    VirtUart0RxMsg = SET;
}

extern void VIRT_UART_RxCpltCallback_1(VIRT_UART_HandleTypeDef *huart)
{

    printf("Msg received on VIRTUAL UART1 channel:  %s \n\r", (char *) huart->pRxBuffPtr);

    /* copy received msg in a variable to sent it back to master processor in main infinite loop*/
    VirtUart1ChannelRxSize = huart->RxXferSize < MAX_BUFFER_SIZE? huart->RxXferSize : MAX_BUFFER_SIZE-1;
    memcpy(VirtUart1ChannelBuffRx, huart->pRxBuffPtr, VirtUart1ChannelRxSize);
    VirtUart1RxMsg = SET;
}

extern uint8_t* VIRT_UART_read_message_0(){
	printf("MESSAGE READ FROM VUART_0\n");
	return (VirtUart0ChannelBuffRx);
}

extern uint8_t* VIRT_UART_read_message_1(){
	printf("MESSAGE READ FROM VUART_1\n");
	return (VirtUart1ChannelBuffRx);
}

extern void VIRT_UART_send_message_0(unsigned char buffer[BUFFER_SEND_MESSAGE],int buffer_size){
	VIRT_UART_StatusTypeDef state = VIRT_UART_Transmit(&huart0, buffer, buffer_size);
	if (state == VIRT_UART_ERROR){
			printf ("Error trying to transmit data through VUART\n");
	}
}

extern void VIRT_UART_send_message_1(unsigned char buffer[BUFFER_SEND_MESSAGE],int buffer_size){
	VIRT_UART_StatusTypeDef state = VIRT_UART_Transmit(&huart1, buffer, buffer_size);
	if (state == VIRT_UART_ERROR){
			printf ("Error trying to transmit data through VUART\n");
	}
}

extern __IO FlagStatus get_RX_msg_0(){
	return VirtUart0RxMsg;
}

extern __IO FlagStatus get_RX_msg_1(){
	return VirtUart1RxMsg;
}

extern void set_RX_msg_0(__IO FlagStatus flag){
	VirtUart0RxMsg = flag;
}

extern void set_RX_msg_1(__IO FlagStatus flag){
	VirtUart1RxMsg = flag;
}

extern void VIRT_UART_Callback_0(){
	if(VIRT_UART_RegisterCallback(&huart0, VIRT_UART_RXCPLT_CB_ID, VIRT_UART_RxCpltCallback_0) != VIRT_UART_OK)
	{
		Error_Handler();
	}
	OPENAMP_check_for_message();
}

extern void VIRT_UART_Callback_1(){
	if(VIRT_UART_RegisterCallback(&huart0, VIRT_UART_RXCPLT_CB_ID, VIRT_UART_RxCpltCallback_1) != VIRT_UART_OK)
	{
		Error_Handler();
	}
	OPENAMP_check_for_message();
}
















