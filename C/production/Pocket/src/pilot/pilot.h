/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  pilot.h
 * @author Debus Alexy
 * @version 2.0
 * @date 30/04/2020
 * @brief pilot entity header file
 *
 */


#ifndef SRC_POCKET_V1_PILOT_H_
#define SRC_POCKET_V1_PILOT_H_

/* \struct Direction
 * \brief Direction structure with the list of available direction to be send over the vuart communication to the robot
 *
 */
#include "../commun.h"


/************************ PROTOTYPE DECLARATION **************************************/

/** \fn static void pilot_start ()
 *  \brief Function dedicated to start the pilot
 */
extern void pilot_start();

/** \fn extern void pilot_stop()
 *  \brief Function dedicated to stop the pilot
 */
extern void pilot_stop();

/** \fn extern void pilot_set_direction(Direction dir)
 *  \brief Function dedicated to send an event in the mailbox to set the direction
 *  \param  Direction the direction to send to the robot entity
 */
extern void pilot_set_direction(Direction_e dir);

/** \fn extern void pilot_signal_exit()
 *  \brief Function dedicated to send an event in the mailbox to signal the end of peripheral initialization of the robot
 */
extern void pilot_signal_exit();

/** \fn extern void pilot_signal_connection_success()
 *  \brief Function dedicated to send an event in the mailbox to signal the connection between the android app and Pocket
 */
extern void pilot_signal_connection_success();

/** \fn extern void pilot_signal_connection_failed()
 *  \brief Function dedicated to send an event in the mailbox to signal the lost of connection between the android app and Pocket
 */
extern void pilot_signal_connection_failed();

/** \fn extern void pilot_signal_emergency_stop()
 *  \brief Function dedicated to send an event in the mailbox to signal that the user asked for an emergency stop of the robot
 */
extern void pilot_signal_emergency_stop();



/************************ END OF PROTOTYPE DECLARATION **************************************/

#endif /* SRC_POCKET_V1_pilot_H_ */
