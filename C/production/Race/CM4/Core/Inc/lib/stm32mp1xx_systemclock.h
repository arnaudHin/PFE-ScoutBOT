/*
 * stm32mp1xx_systemclock.h
 *
 *  Created on: Oct 19, 2021
 *      Author: martin
 */

#ifndef INC_LIB_STM32MP1XX_SYSTEMCLOCK_H_
#define INC_LIB_STM32MP1XX_SYSTEMCLOCK_H_

#include "stm32mp1xx_hal.h"
#include <lib/stm32mp1xx_error_handler.h>

void SystemClock_Config(void);
void PeriphCommonClock_Config(void);


#endif /* INC_LIB_STM32MP1XX_SYSTEMCLOCK_H_ */
