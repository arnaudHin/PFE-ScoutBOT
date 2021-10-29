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

#include "../../pocket/com_Race/postman_pilot.h"

/* \fn extern void dispatcher_pilot_start()
 * \brief Function dedicated to start the dispatcher_pilot (Initialize and open the mailbox + create a thread)
 */
extern void dispatcher_pilot_start();

/* \fn extern void dispatcher_pilot_set_msg(Pilot_message msg)
 * \brief Function dedicated to send through the mailbox a message received through the serial port
 * \param Pilot_message the structure with the content of the message to send through the mailbox
 */
extern void dispatcher_pilot_set_msg(Pilot_message_r msg);

/* \fn extern void dispatcher_pilot_stop()
 * \brief Function dedicated to stop the dispatcher_pilot (Close the mailbox + destroy the current thread)
 */
extern void dispatcher_pilot_stop();

#endif /* SRC_POCKET_COM_RACE_DISPATCHER_PILOT_H_ */
