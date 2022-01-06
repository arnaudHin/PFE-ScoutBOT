/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  dispatcher_pilot.h
 * @author Debus Alexy
 * @version 0.3
 * @date 01/05/2020
 * @brief Dispatcher_pilot entity header file
 *
 * The purpose of the dispatcher_pilot entity is to decode and redirect the contents of the receiving message
 *
 */

#ifndef SRC_POCKET_COM_RACE_DISPATCHER_PILOT_H_
#define SRC_POCKET_COM_RACE_DISPATCHER_PILOT_H_

#include "postman_race.h"

/* \fn extern void dispatcher_pilot_start()
 * \brief Function dedicated to start the dispatcher_pilot (Initialize and open the mailbox + create a thread)
 */
extern void dispatcher_race_start();



/* \fn extern void dispatcher_pilot_stop()
 * \brief Function dedicated to stop the dispatcher_pilot (Close the mailbox + destroy the current thread)
 */
extern void dispatcher_race_stop();

#endif /* SRC_POCKET_COM_RACE_DISPATCHER_PILOT_H_ */
