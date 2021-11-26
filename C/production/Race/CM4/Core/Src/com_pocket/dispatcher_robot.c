/*
 * dispatcher_robot.c
 *
 *  Created on: 21 oct. 2021
 *      Author: martin
 */


#include <com_pocket/dispatcher_robot.h>
#include "com_pocket/intercore_protocol.h"
#include "robot/robot.h"
#include "lib/stm32mp1xx_gpio.h"



/*********************************************************************************/
/**									Private prototypes							**/
/*********************************************************************************/

static void dispatcher_robot_dispatch_msg(Message_with_race_t msg);

/*********************************************************************************/
/**									Public functions							**/
/*********************************************************************************/

extern void dispatcher_robot_set_msg(Message_with_race_t msg){
	printf("je suis dans le set message\n");
	dispatcher_robot_dispatch_msg(msg);
}





/*********************************************************************************/
/**									Private functions							**/
/*********************************************************************************/
static void dispatcher_robot_dispatch_msg(Message_with_race_t msg){

	Robot_mq_t messageToRobot;

	switch (msg.command) {
		case TRY_DIR:
			messageToRobot.directionToPerform = msg.data.direction;
			messageToRobot.event = E_TRY_DIR;
			TOGGLE_Led(LED4_GREEN);
			robot_run(messageToRobot);
			break;


		default:
			break;
	}

}
