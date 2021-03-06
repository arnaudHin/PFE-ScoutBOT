/*
 * dispatcher_robot.h
 *
 *  Created on: 21 oct. 2021
 *      Author: martin
 */

#ifndef INC_LIB_COM_POCKET_DISPATCHER_ROBOT_H_
#define INC_LIB_COM_POCKET_DISPATCHER_ROBOT_H_

#include "stm32mp1xx.h"
#include "com_pocket/postman_robot.h"
#include "com_pocket/intercore_protocol.h"
#include "lib/MOTORS.h"


extern void dispatcher_robot_set_msg(Message_with_race_t msg);


#endif /* INC_LIB_COM_POCKET_DISPATCHER_ROBOT_H_ */
