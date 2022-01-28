/**
 * @file postman_jump.h
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SRC_JUMPC_COM_JUMPC_POSTMAN_JUMPC_H_
#define SRC_JUMPC_COM_JUMPC_POSTMAN_JUMPC_H_

#include "commun.h"
//#include <unistd.h>
#include <sys/types.h>

/************************ PROTOTYPE DECLARATION **************************************/

/** \fn void postman_pocket_send_msg (char * buffer, int length)
 *  \brief Function dedicated to send a message stored in buffer that has a "length" size.
 *  \param buffer : buffer in byte to send with a socket
 *  \param length : size of the buffer
 */
extern void postman_jumpC_send_msg(Message_to_pocket_t * msg);


/** \fn ssize_t postman_jumpC_receive_msg (uint8_t * bufferToReceive, __ssize_t nbBytes)
 *  \brief Function dedicated to receive a message of a specific length and store it in a buffer.
 *  \param bufferToReceive : buffer where the msg in bytes received is stored
 *  \param nbBytes : expected size of the msg
 */
extern ssize_t postman_jumpC_receive_msg(uint8_t * bufferToReceive, __ssize_t nbBytes);

/** \fn void postman_jumpC_start ()
 *  \brief Function dedicated to create a new socket, bind it, listen it, and accept it.
 */
extern void postman_jumpC_start();

/** \fn void postman_jumpC_stop ()
 *  \brief Function dedicated to close a socket and force an Exit
 */
extern void postman_jumpC_stop();


#endif /* SRC_JUMPC_COM_JUMPC_POSTMAN_JUMPC_H_ */
