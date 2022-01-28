/**
 * @file dispatcher_jump.h
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SRC_POCKET_COM_POCKET_DISPATCHER_JUMP_H_
#define SRC_POCKET_COM_POCKET_DISPATCHER_JUMP_H_

#define BUFF_SIZE_TO_RECEIVE (1440)

/************************ PROTOTYPE DECLARATION **************************************/

/** 
 *  \brief Function dedicated to start the dispatcher and postman (Initialize and open the mailbox + create a thread)
 */
extern void dispatcher_jumpC_start();

/**
 *  \fn static void new_dispatcher_jumpC_start()
 *
 *  \brief Function dedicated to start the dispatcher without postman.
 */
extern void new_dispatcher_jumpC_start();

/**
 * @brief stop the dispatcher
 * 
 */
extern void dispatcher_jumpC_stop();

/************************ END PROTOTYPE DECLARATION **********************************/

#endif /* SRC_POCKET_COM_POCKET_DISPATCHER_JUMP_H_ */
