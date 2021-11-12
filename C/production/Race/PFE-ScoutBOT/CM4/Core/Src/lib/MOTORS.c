/*
 * MOTORS.c
 *
 *  Created on: Oct 29, 2021
 *      Author: Arnaud
 */

#include <lib/TIMERS.h>


extern void MOTORS_stop(){
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //
  TIMER4_STOP();
}

extern void MOTORS_forward(){
  TIM4_setDuty(85);
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
  TIMER4_RUN();
}

extern void MOTORS_backward(){
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //
  TIMER4_STOP();

  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, !SET ); //backward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, !RESET ); //backward

  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
  TIMER4_RUN();
}

extern void MOTORS_turn_left(){
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //
  TIMER4_STOP();

  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, !SET ); //backward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, RESET ); //forward

  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
  TIMER4_RUN();
}

extern void MOTORS_turn_right(){
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //
  TIMER4_STOP();

  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, SET ); //forward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, !RESET ); //backward

  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
  TIMER4_RUN();
}











