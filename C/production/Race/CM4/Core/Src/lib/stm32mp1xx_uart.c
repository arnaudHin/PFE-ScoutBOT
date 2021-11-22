/*
 * stm32mp1xx_uart.c
 *
 *  Created on: Oct 18, 2021
 *      Author: martin
 */

#include <lib/stm32mp1xx_uart.h>
#include "stm32mp1xx_hal_uart.h"
#include "stm32mp1xx_hal_uart_ex.h"

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar (void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE * f)
#endif /* __GNUC__ */


#define BUFFER_RX_SIZE 128

static UART_HandleTypeDef UART_HandleStructure[UART_ID_NB];	//Ce tableau contient les structures qui sont utilisées pour piloter chaque UART avec la librairie HAL.
const USART_TypeDef * instance_array[UART_ID_NB] = {USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8};
const IRQn_Type nvic_irq_array[UART_ID_NB] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn, USART6_IRQn, UART7_IRQn, UART8_IRQn};

static volatile int buffer_rx_data_ready[UART_ID_NB];

static volatile int uart_initialized[UART_ID_NB] = {0};


void UART_init(uart_id_e uart_id, uint32_t baudrate)
{

	UART_HandleStructure[uart_id].Instance = (USART_TypeDef*)instance_array[uart_id];
	UART_HandleStructure[uart_id].Init.BaudRate = baudrate;
	UART_HandleStructure[uart_id].Init.WordLength = UART_WORDLENGTH_8B;
	UART_HandleStructure[uart_id].Init.StopBits = UART_STOPBITS_1;
	UART_HandleStructure[uart_id].Init.Parity = UART_PARITY_NONE;
	UART_HandleStructure[uart_id].Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_HandleStructure[uart_id].Init.Mode = UART_MODE_TX_RX;
	UART_HandleStructure[uart_id].Init.OverSampling = UART_OVERSAMPLING_8;
	UART_HandleStructure[uart_id].Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	UART_HandleStructure[uart_id].Init.ClockPrescaler = UART_PRESCALER_DIV1;
	UART_HandleStructure[uart_id].AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;


	if (HAL_UART_Init(&UART_HandleStructure[uart_id]) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&UART_HandleStructure[uart_id], UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&UART_HandleStructure[uart_id], UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&UART_HandleStructure[uart_id]) != HAL_OK)
	{
	Error_Handler();
	}
	if(uart_id == UART3_ID){
	  BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART3);
	  BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_12, GPIO_MODE_AF, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF8_USART3);
	}
	uart_initialized[uart_id] = 1;
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Write a character to the USART7 and Loop until the end of transmission */
	HAL_UART_Transmit(&UART_HandleStructure[UART3_ID], (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

//send a string
void UART_puts(uart_id_e uart_id, uint8_t * str, uint32_t len)
{
	HAL_StatusTypeDef state;
	HAL_UART_StateTypeDef uart_state;
	if(uart_initialized[uart_id])
	{
		do
		{
			NVIC_DisableIRQ(nvic_irq_array[uart_id]);
			state = HAL_UART_Transmit(&UART_HandleStructure[uart_id], str, (uint16_t)len,100);
			NVIC_EnableIRQ(nvic_irq_array[uart_id]);
		}while(state == HAL_BUSY);

		do{
			uart_state = HAL_UART_GetState(&UART_HandleStructure[uart_id]);
		}while(uart_state == HAL_UART_STATE_BUSY_TX || uart_state == HAL_UART_STATE_BUSY_TX_RX);	//Blocant.
	}
}





