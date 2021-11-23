/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  proxy_robot.h
 * @author Debus Alexy
 * @version 0.2
 * @date 01/05/2020
 * @brief Proxy_robot entity header file
 *
 * The purpose of the proxy_robot entity is to hide communication from the other entity (Mainly for the Pilot entity)
 *
 */


#ifndef SRC_POCKET_COM_RACE_PROXY_ROBOT_H_
#define SRC_POCKET_COM_RACE_PROXY_ROBOT_H_

#include "../../pocket/pilot.h"
#include "../robot_state.h"

/* \fn extern void robot_start_scanning()
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to start a scanning sequence
 */
extern void robot_start_scanning();

/* \fn extern void robot_stop_scanning()
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to stop the scanning sequence
 */
extern void robot_stop_scanning();

/* \fn extern void robot_try_mvmt(Direction dir)
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to move in the given direction
 */
extern void robot_try_mvmt(Direction dir);

/* \fn extern void robot_urgent_break()
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to stop urgently
 */
extern void robot_urgent_break();

/* \fn extern void robot_try_path()
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to move onto the choosen position
 */
extern void robot_try_path(Odometry * displacement, uint8_t nb_displacement);

/* \fn extern void robot_reset_odometry()
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask the robot to reset current position + current angle
 */
extern void robot_reset_odometry();

/* \fn extern void robot_ask_close_race()
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask to close Race
 */
extern void robot_ask_close_race();

#endif /* SRC_POCKET_COM_RACE_PROXY_ROBOT_H_ */
