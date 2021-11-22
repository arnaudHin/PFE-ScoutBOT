/**
 * @file  dispatcher_pocket.h
 * @author Montet Julien & Marbeuf Pierre
 * @version 3.0
 * @date 12/05/2020
 * @brief dispatcher pocket
 */

#ifndef SRC_POCKET_COM_POCKET_DISPATCHER_POCKET_H_
#define SRC_POCKET_COM_POCKET_DISPATCHER_POCKET_H_


/************************ PROTOTYPE DECLARATION **************************************/

/** \fn static void Pilot_start ()
 *  \brief Function dedicated to start the dispatcher and postman (Initialize and open the mailbox + create a thread)
 */
extern void dispatcher_pocket_start();

/**
 *  \fn static void new_dispatcher_pocket_start()
 *
 *  \brief Function dedicated to start the dispatcher  with pilot and without postman.
 */
extern void new_dispatcher_pocket_start();

/** \fn extern void Pilot_set_direction(Direction dir)
 *  \brief Function dedicated to wait the end of thread to stop the dispatcher
 */
extern void dispatcher_pocket_stop();

/************************ END PROTOTYPE DECLARATION **********************************/

#endif /* SRC_POCKET_COM_POCKET_DISPATCHER_POCKET_H_ */
