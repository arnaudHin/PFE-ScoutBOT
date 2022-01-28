/*
 * postman_robot.h
 *
 *  Created on: 21 oct. 2021
 *      Author: martin
 */


#ifndef INC_LIB_COM_POCKET_POSTMAN_ROBOT_H_
#define INC_LIB_COM_POCKET_POSTMAN_ROBOT_H_


#include "stm32mp1xx.h"
#include "lib/stm32mp1xx_vuart.h"
#include <utils.h>


extern void postman_robot_run();
extern ScoutBOT_Status_e postman_robot_init();

#endif /* INC_LIB_COM_POCKET_POSTMAN_ROBOT_H_ */



