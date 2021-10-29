/*
 * postmanPocket.h
 *
 * @file  postman_pocket.c
 * @author Montet Julien & Marbeuf Pierre
 * @version 3.0
 * @date 11/06/2020
 * @brief Postman pocket network
 *
 */
#ifndef SRC_POCKET_COM_POCKET_POSTMAN_POCKET_H_
#define SRC_POCKET_COM_POCKET_POSTMAN_POCKET_H_

#include "commun.h"

/************************ PROTOTYPE DECLARATION **************************************/

/** \fn void postman_pocket_send_msg (char * buffer, int length)
 *  \brief Function dedicated to send a message stored in buffer that has a "length" size.
 *  \param buffer : buffer in byte to send with a socket
 *  \param length : size of the buffer
 */
extern void postman_pocket_send_msg(unsigned char * buffer, int length);


/** \fn void postman_pocket_receive_msg (char * buffer, int length)
 *  \brief Function dedicated to receive a message of a specific length and store it in a buffer.
 *  \param buffer : buffer where the msg in bytes received is stored
 *  \param length : expected size of the msg
 */
extern void postman_pocket_receive_msg(unsigned char * buffer, int length);

/** \fn void postman_pocket_start ()
 *  \brief Function dedicated to create a new socket, bind it, listen it, and accept it.
 */
extern void postman_pocket_start();

/** \fn void postman_pocket_stop ()
 *  \brief Function dedicated to close a socket and force an Exit
 */
extern void postman_pocket_stop();


#endif /* SRC_POCKET_COM_POCKET_POSTMAN_POCKET_H_ */
