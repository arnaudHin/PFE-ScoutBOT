/*
 * stm32mp1xx_adc.h
 *
 *  Created on: Dec 20, 2021
 *      Author: martin
 */

#ifndef INC_LIB_STM32MP1XX_ADC_H_
#define INC_LIB_STM32MP1XX_ADC_H_

#include "stm32mp1xx_hal.h"
#include "config.h"
#include <lib/stm32mp1xx_error_handler.h>
#include <lib/stm32mp1xx_gpio.h>


typedef enum
{
	ADC1_ID = 0,
	ADC2_ID,
	ADC_NB
}ADC_ID_e;

void ADC_Init(ADC_TypeDef* ADCx);
HAL_StatusTypeDef ADC_Start(ADC_TypeDef* ADCx);
HAL_StatusTypeDef ADC_PollForConversion(ADC_TypeDef* ADCx);
uint32_t ADC_GetValue(ADC_TypeDef* ADCx);

#endif /* INC_LIB_STM32MP1XX_ADC_H_ */
