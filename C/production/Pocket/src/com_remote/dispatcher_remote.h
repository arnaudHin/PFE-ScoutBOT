/**
 * @file  dispatcher_remote.h
 * @author Montet Julien & Marbeuf Pierre
 * @version 3.0
 * @date 12/05/2020
 * @brief dispatcher remote
 */

#ifndef SRC_REMOTE_COM_REMOTE_DISPATCHER_REMOTE_H_
#define SRC_REMOTE_COM_REMOTE_DISPATCHER_REMOTE_H_


/************************ PROTOTYPE DECLARATION **************************************/

/** \fn static void Pilot_start ()
 *  \brief Function dedicated to start the dispatcher and postman (Initialize and open the mailbox + create a thread)
 */
extern void dispatcher_remote_start();

/** \fn extern void Pilot_set_direction(Direction dir)
 *  \brief Function dedicated to wait the end of thread to stop the dispatcher
 */
extern void dispatcher_remote_stop();

/************************ END PROTOTYPE DECLARATION **********************************/

#endif /* SRC_remote_COM_remote_DISPATCHER_remote_H_ */
