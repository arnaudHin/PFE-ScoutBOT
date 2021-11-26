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

#include "../pilot/pilot.h"
#include "../commun.h"
#include "race_protocol.h"



/* \fn extern void robot_ask_close_race()
 *  \brief Function dedicated to send a message to the robot through the postman_pilot to ask to close Race
 */
extern void proxy_robot_try_dir(DATA_to_race_t * dataToRace);

#endif /* SRC_POCKET_COM_RACE_PROXY_ROBOT_H_ */
