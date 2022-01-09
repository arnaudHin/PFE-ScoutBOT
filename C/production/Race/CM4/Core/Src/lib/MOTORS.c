/*
 * MOTORS.c
 *
 *  Created on: Oct 29, 2021
 *      Author: Arnaud
 */

#include <lib/TIMERS.h>


extern void MOTORS_stop(){
  TIMER4_STOP();
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //
}

extern void MOTORS_forward(){
  TIMER4_RUN();
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //
  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, !SET ); //forward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, !RESET ); //forward
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
}

extern void MOTORS_backward(){
	TIMER4_RUN();
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //

  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, SET ); //backward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, RESET ); //backward

  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
}

extern void MOTORS_turn_left(){
	TIMER4_RUN();
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //

  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, !SET ); //backward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, RESET ); //forward

  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
}

extern void MOTORS_turn_right(){
	TIMER4_RUN();
  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, RESET ); //

  HAL_GPIO_WritePin( DIR_MOTOR_LEFT_NEGATIVE, SET ); //forward
  HAL_GPIO_WritePin( DIR_MOTOR_RIGHT_NEGATIVE, !RESET ); //backward

  HAL_GPIO_WritePin( ENABLE_MOTOR_NEGATIVE, SET ); //
}











