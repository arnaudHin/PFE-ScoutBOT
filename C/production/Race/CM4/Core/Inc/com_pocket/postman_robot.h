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



/* \struct Order
 * \brief Order structure with the type of the message to be sent/received
 *
 */



typedef enum
{
    ASK_FOR_DEFAULT_ORDER=0,    // Default message for debug
	ASK_FOR_LEFT_DIRECTION,     // Ask the robot to move into the left direction
	ASK_FOR_RIGHT_DIRECTION,    // Ask the robot to move into the left direction
	ASK_FOR_FORWARD_DIRECTION,  // Ask the robot to move into the left direction
	ASK_FOR_BACKWARD_DIRECTION, // Ask the robot to move into the left direction
	ASK_FOR_STOP,               // Ask the robot to move into the left direction
	ASK_FOR_URGENT_BREAK,       // Ask the robot to stop immediatly
	ASK_FOR_AUTO_DISPLACEMENT,  // Ask the robot to move onto the desire position
	ASK_FOR_RESET_POSITION,     // Ask the robot to reset current position + current angle

	ASK_FOR_START_TO_UPDATE,    // Ask the robot to start the scanning sequence
	ASK_FOR_STOP_TO_UPDATE,     // Ask the robot to stop the scanning sequence
	ASK_FOR_CLOSE,
	ASK_FOR_DEFAULT_ORDER_S     // Ask to close Race software

} Order_r;

/* \struct Odometry
 * \brief Odometry structure with the current position of the robot (x,y) & the current angle
 *
 */
typedef struct {
	int32_t x;		// Unity : mm
	int32_t y;		// Unity : mm
	int32_t teta;	// UNity : rad
}Odometry;

/**@struct Pilot_message_s
 * @brief Pilot_message structure with the message to send on the serial port
 *
 */
typedef struct {
	uint8_t start_flag;
	uint8_t slave_addr;
	Order_r order_r;                    // An order dedicated to define the message type
	uint8_t max_data;
	Odometry * multipts;
	uint8_t stop_flag;
}Pilot_message_r;


extern void postman_robot_run();
extern ScoutBOT_Status_e postman_robot_init();

#endif /* INC_LIB_COM_POCKET_POSTMAN_ROBOT_H_ */



