/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  proxy_robot.h
 * @author Debus Alexy
 * @version 0.2
 * @date 01/05/2020
 * @brief Proxy_robot entity source file
 *
 */

#include "../../pocket/com_Race/proxy_robot.h"

#include "../../util.h"
#include "stdlib.h"

#include "../../pocket/com_Race/postman_pilot.h"

/*
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to move in the given direction
 */
extern void robot_try_mvmt(Direction dir){
	TRACE("Try_mvmt send to proxy\n");

	Pilot_message_s my_message = {.order_s = ASK_FOR_STOP};

	switch(dir){
		case LEFT :
			my_message.order_s = ASK_FOR_LEFT_DIRECTION;
			break;
		case RIGHT :
			my_message.order_s = ASK_FOR_RIGHT_DIRECTION;
			break;
		case FORWARD :
			my_message.order_s = ASK_FOR_FORWARD_DIRECTION;
			break;
		case BACKWARD :
			my_message.order_s = ASK_FOR_BACKWARD_DIRECTION;
			break;
		case BREAK :
			my_message.order_s = ASK_FOR_STOP;
			break;
		default :
			TRACE("Default dir on A7\n")
			my_message.order_s = ASK_FOR_STOP;
	}

	postman_send_message(my_message);
}

/*
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to stop urgently
 */
extern void robot_urgent_break(){
	TRACE("Urgent break request send to proxy\n");
	Pilot_message_s my_message = {.order_s =  ASK_FOR_URGENT_BREAK, .max_data = 0};
	postman_send_message(my_message);
}

/*
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to move onto the choosen position
 */
extern void robot_try_path(Odometry * displacement,uint8_t nb_displacement){
	TRACE("Try path request send to proxy\n");

	Odometry pathfind_control_coordinates[nb_displacement];
	memcpy(pathfind_control_coordinates,displacement,sizeof(Odometry) * nb_displacement);

	Pilot_message_s my_message = {.order_s = ASK_FOR_AUTO_DISPLACEMENT, .multipts_data  = pathfind_control_coordinates, .max_data = nb_displacement};
	postman_send_message(my_message);
}

/*
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to reset current position + current angle
 */
extern void robot_reset_odometry(){
	TRACE("Reset odometry request send to proxy\n");
	Pilot_message_s my_message = {.order_s = ASK_FOR_RESET_POSITION, .max_data = 0};
	postman_send_message(my_message);
}

/*
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to start a scanning sequence
 */
extern void robot_start_scanning(){
	TRACE("Start scanning request send to proxy\n");
	Pilot_message_s my_message = {.order_s = ASK_FOR_START_TO_UPDATE, .max_data = 0};
	postman_send_message(my_message);
}

/*
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to stop the scanning sequence
 */
extern void robot_stop_scanning(){
	TRACE("Stop scanning request send to proxy\n");
	Pilot_message_s my_message = {.order_s = ASK_FOR_STOP_TO_UPDATE, .max_data = 0};
	postman_send_message(my_message);
}

/*
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask to close Race
 */
extern void robot_ask_close_race(){
	TRACE("Close Race request send to proxy\n");
	Pilot_message_s my_message ={.order_s = ASK_FOR_CLOSE, .max_data = 0};
	postman_send_message(my_message);
}
