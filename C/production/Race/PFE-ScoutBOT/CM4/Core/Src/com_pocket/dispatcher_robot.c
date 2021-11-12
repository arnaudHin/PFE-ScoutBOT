/*
 * dispatcher_robot.c
 *
 *  Created on: 21 oct. 2021
 *      Author: martin
 */


#include <com_pocket/dispatcher_robot.h>

/**********************************  STATIC FUNCTION DECLARATION ************************************************/

static void dispatcher_robot_decode_msg(Pilot_message_r msg);

/********************************** END OF STATIC FUNCTION DECLARATION *******************************************/


/** \fn static void dispatcher_robot_decode_msg(Pilot_message msg)
 *  \brief Function dedicated to look after the message type in order to execute a specific action
 *  \brief (ask the robot to move onto the given direction, start to scan or stop to scan)
 *  \param Pilot_message a structure with data for Robot entity
 */
static void dispatcher_robot_decode_msg(Pilot_message_r msg){
	printf("CMD:%d\n",msg.order_r);
	printf("je suis dans dispatcher robot\n");

	switch(msg.order_r){

	case ASK_FOR_LEFT_DIRECTION:
		printf("left\n");
		MOTORS_turn_left();
		break;

	case ASK_FOR_RIGHT_DIRECTION:
		printf("right\n");
		MOTORS_turn_right();
		break;

	case ASK_FOR_FORWARD_DIRECTION:
		printf("forward\n");
		MOTORS_forward();
		break;

	case ASK_FOR_BACKWARD_DIRECTION:
		printf("backward\n");
		break;

	case ASK_FOR_STOP:
		printf("stop\n");
		MOTORS_backward();
		break;

	case ASK_FOR_URGENT_BREAK:
		printf("urgentstop\n");
		MOTORS_stop();
		break;

	case ASK_FOR_DEFAULT_ORDER_S:
		//NOTHING TO DO

	default:
		break;
	}
}

extern void dispatcher_robot_set_msg(Pilot_message_r msg){
	printf("je suis dans le set message\n");
	dispatcher_robot_decode_msg(msg);
}
