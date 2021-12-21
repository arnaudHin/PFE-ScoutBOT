/*
 * stm32mp1xx_gpio.c
 *
 *  Created on: 9 oct. 2021
 *      Author: martin
 */

#include <lib/stm32mp1xx_gpio.h>

void GPIO_Init (void){

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	  BSP_GPIO_PinCfg(LED4, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0);
	  BSP_GPIO_PinCfg(LED6, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0);
	  BSP_GPIO_PinCfg(LED7, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0);
	  BSP_GPIO_PinCfg(LED8, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0);
	  BSP_GPIO_PinCfg(LED1, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0);
	  BSP_GPIO_PinCfg(LED2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0);

}


void BSP_GPIO_PinCfg(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed, uint32_t GPIO_Alternate)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//Structure contenant les arguments de la fonction GPIO_Init

	GPIO_InitStructure.Pin = GPIO_Pin;
	GPIO_InitStructure.Mode = GPIO_Mode;
	GPIO_InitStructure.Pull = GPIO_Pull;
	GPIO_InitStructure.Speed = GPIO_Speed;
	GPIO_InitStructure.Alternate = GPIO_Alternate;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStructure);

	OFF_Led(LED4_GREEN);
	OFF_Led(LED6_RED);
	OFF_Led(LED7_ORANGE);
	OFF_Led(LED8_BLUE);
	OFF_Led(LED1_BLUE);
	OFF_Led(LED2_GREEN);
}

void PIN_On(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin){
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, SET);
}

void PIN_Off(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin){
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, RESET);
}

void PIN_Toggle(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin){
	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

void ON_Led(LED_e led){
	switch (led) {
		case LED4_GREEN:
			HAL_GPIO_WritePin(LED4, RESET);
			break;
		case LED6_RED:
			HAL_GPIO_WritePin(LED6, RESET);
			break;
		case LED7_ORANGE:
			HAL_GPIO_WritePin(LED7, SET);
			break;
		case LED8_BLUE:
			HAL_GPIO_WritePin(LED8, SET);
			break;
		case LED1_BLUE:
			HAL_GPIO_WritePin(LED1, SET);
			break;
		case LED2_GREEN:
			HAL_GPIO_WritePin(LED2, SET);
			break;
		default:
			break;
	}
}

void OFF_Led(LED_e led){
	switch (led) {
		case LED4_GREEN:
			HAL_GPIO_WritePin(LED4, SET);
			break;
		case LED6_RED:
			HAL_GPIO_WritePin(LED6, SET);
			break;
		case LED7_ORANGE:
			HAL_GPIO_WritePin(LED7, RESET);
			break;
		case LED8_BLUE:
			HAL_GPIO_WritePin(LED8, RESET);
			break;
		case LED1_BLUE:
			HAL_GPIO_WritePin(LED1, RESET);
			break;
		case LED2_GREEN:
			HAL_GPIO_WritePin(LED2, RESET);
			break;
		default:
			break;
	}
}

void TOGGLE_Led(LED_e led){
	switch (led) {
		case LED4_GREEN:
			HAL_GPIO_TogglePin(LED4);
			break;
		case LED6_RED:
			HAL_GPIO_TogglePin(LED6);
			break;
		case LED7_ORANGE:
			HAL_GPIO_TogglePin(LED7);
			break;
		case LED8_BLUE:
			HAL_GPIO_TogglePin(LED8);
			break;
		case LED1_BLUE:
			HAL_GPIO_TogglePin(LED1);
			break;
		case LED2_GREEN:
			HAL_GPIO_TogglePin(LED2);
			break;
		default:
			break;
	}
}




