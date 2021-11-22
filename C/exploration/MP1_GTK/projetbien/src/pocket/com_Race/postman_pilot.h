/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  proxy_robot.c
 * @author Debus Alexy
 * @version 0.2
 * @date 01/05/2020
 * @brief Postman_pilot entity header file
 *
 * The purpose of the postman_pilot is to send / receive message on a serial port open by the M4
 * and regularly called /dev/ttyRPMSG0 on the channel 0 and /dev/ttyRMPSG1 on the channel 1
 *
 */

#ifndef SRC_POCKET_COM_RACE_POSTMAN_PILOT_H_
#define SRC_POCKET_COM_RACE_POSTMAN_PILOT_H_

#define MAX_MESSAGE_LENGTH (128)

#include "../../pocket/pilot.h"
#include "../../pocket/robot_state.h"
#include <stdio.h>
#include <stdlib.h>

#define PARSE_SYMBOL \

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
	ASK_FOR_DEFAULT_ORDER_S// Ask to close Race software

} Order_s;

typedef enum
{
	SET_ODOMETRY = 0,   // Update the current position + angle
	SET_SENSORS,    // Update the current value of the 4 bumpers
	SET_LIDAR_SENSORS,			// Update the current value of the VL53L1 sensors
	NOTIFY_END_INIT,          // Peripheral initialization ended
	ASK_FOR_DEFAULT_ORDER_R
} Order_r;


/* \struct Pilot_message_s
 * \brief Pilot_message structure with the message to send on the serial port
 *
 */
typedef struct {
	uint8_t start_flag;
	uint8_t slave_addr;
	Order_s order_s;                   // An order dedicated to define the message type
	uint8_t max_data;
	Odometry * multipts_data;
	uint8_t stop_flag;
}Pilot_message_s;

/* \struct Pilot_message_r
 * \brief Pilot_message structure with the message to receive on the serial port
 *
 */
typedef struct {
	uint8_t start_flag;
	Order_r order_r;                   // An order dedicated to define the message type
	Sensors_state my_sensors_state;     // The current value of the 4 bumpers
	Odometry my_current_position;       // The current position + angle
	Lidar_state my_lidar_state;			// The current value of the vl53l1x sensors
	uint8_t stop_flag;
}Pilot_message_r;

/* \fn extern void postman_start()
 *  \brief Function dedicated to start the postman (Open the serial port)
 */
extern void postman_start();

/* \fn extern void postman_stop()
 *  \brief Function dedicated to stop the postman (Close the serial port)
 */
extern void postman_stop();

/* \fn extern pilot_message postman_read_message()
 * \brief Function dedicated to read message on the given serial port
 * \return Pilot_message the structure which contain a read message
 */
extern Pilot_message_r postman_read_message();

/* \fn extern void postman_send_message(Pilot_message msg)
 * \brief Function dedicated to send message on the given serial port
 * \param Pilot_message the structure which contain the message to send
 */
extern void postman_send_message(Pilot_message_s msg);

#endif /* SRC_POCKET_COM_RACE_POSTMAN_PILOT_H_ */
