/*
 * TIMERS.c
 *
 *  Created on: Oct 18, 2021
 *: Arnaud
 */


#include "stm32mp1xx_hal.h"
#include "TIMERS.h"
#include "GPIOS.h"


static TIM_HandleTypeDef htim4;


static void TIM4_INIT();
static void TIM4_GPIO_INIT();



extern void TIMER_INIT(){



  TIM4_INIT();




}


static void TIM4_INIT(){

  //struct config d'horloge
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  //struct de config du mode PWM
  TIM_OC_InitTypeDef sConfigOC = {0};

  //config du timer
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 104*1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 100;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  //init du timer
  HAL_TIM_Base_Init(&htim4);

  //config horloge
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  //Active clock
  __HAL_RCC_TIM4_CLK_ENABLE();

  //init horloge
  HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

  //
  HAL_TIM_PWM_Init(&htim4);

  //
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 85;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  //init
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);
  sConfigOC.Pulse = 85;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

  TIM4_GPIO_INIT();

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

  TIMER4_STOP();

}

static void TIM4_GPIO_INIT(){

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  BSP_GPIO_PinCfg(PULSE_MOTOR_LEFT_NEGATIVE, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF2_TIM4);
  BSP_GPIO_PinCfg(PULSE_MOTOR_RIGHT_NEGATIVE, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF2_TIM4);

  BSP_GPIO_PinCfg(ENABLE_MOTOR_NEGATIVE, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, 0);
  BSP_GPIO_PinCfg(DIR_MOTOR_LEFT_NEGATIVE, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, 0);
  BSP_GPIO_PinCfg(DIR_MOTOR_RIGHT_NEGATIVE, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, 0);

  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET );
  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, SET ); //forward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, RESET ); //forward

}


extern void TIM4_setDuty(uint32_t duty){

  __HAL_TIM_SET_COUNTER(&htim4, duty);

}


extern void TIMER4_RUN(){
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	  __HAL_TIM_ENABLE(&htim4);
}

extern void TIMER4_STOP(){
	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
	__HAL_TIM_DISABLE(&htim4);
}
