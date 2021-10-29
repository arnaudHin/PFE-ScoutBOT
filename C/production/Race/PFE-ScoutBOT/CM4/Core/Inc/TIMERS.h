/*
 * TIMERS.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Arnaud
 */

#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

#include "stm32mp1xx_hal.h"

extern void TIMER_INIT();
extern void TIM4_setDuty(uint32_t duty);
extern void TIMER4_RUN();
extern void TIMER4_STOP();


#endif /* INC_TIMERS_H_ */
