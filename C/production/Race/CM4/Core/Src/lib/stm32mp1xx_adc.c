/*
 * stm32mp1xx_adc.c
 *
 *  Created on: Dec 20, 2021
 *      Author: martin
 */

#include "lib/stm32mp1xx_adc.h"
#include "stm32mp1xx_hal_adc.h"
#include "stm32mp1xx_hal_adc_ex.h"

static ADC_HandleTypeDef hadc[ADC_NB];


void ADC_Init(ADC_TypeDef* ADCx){
	ADC_ID_e id = ((ADCx == ADC1)?ADC1_ID:ADC2_ID);
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	ADC_MultiModeTypeDef multimode = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	if(id == ADC1_ID)
	{
		if(IS_ENGINEERING_BOOT_MODE())
		  {
				PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PER;
				if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
				{
				  Error_Handler();
				}
		  }

		__HAL_RCC_ADC12_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		BSP_GPIO_PinCfg(ADC1_IN13, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, 0);
	}

	hadc[id].Instance = ADC1;
	hadc[id].Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc[id].Init.Resolution = ADC_RESOLUTION_12B;
	hadc[id].Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc[id].Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc[id].Init.LowPowerAutoWait = DISABLE;
	hadc[id].Init.ContinuousConvMode = DISABLE;
	hadc[id].Init.NbrOfConversion = 1;
	hadc[id].Init.DiscontinuousConvMode = DISABLE;
	hadc[id].Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc[id].Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
	hadc[id].Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc[id].Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc[id].Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc[id]) != HAL_OK)
	{
		Error_Handler();
	}

	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc[id], &multimode) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_13;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc[id], &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

HAL_StatusTypeDef ADC_Start(ADC_TypeDef* ADCx){
	ADC_ID_e id = ((ADCx == ADC1)?ADC1_ID:ADC2_ID);
	return HAL_ADC_Start(&hadc[id]);
}

HAL_StatusTypeDef ADC_PollForConversion(ADC_TypeDef* ADCx){
	ADC_ID_e id = ((ADCx == ADC1)?ADC1_ID:ADC2_ID);
	return HAL_ADC_PollForConversion(&hadc[id],HAL_MAX_DELAY);
}

uint32_t ADC_GetValue(ADC_TypeDef* ADCx){
	ADC_ID_e id = ((ADCx == ADC1)?ADC1_ID:ADC2_ID);
	return HAL_ADC_GetValue(&hadc[id]);
}



