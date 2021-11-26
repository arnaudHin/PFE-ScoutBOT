/*
 * postmanremote.h
 *
 * @file  postman_remote.c
 * @author Montet Julien & Marbeuf Pierre
 * @version 3.0
 * @date 11/06/2020
 * @brief Postman remote network
 *
 */
#ifndef POSTMAN_REMOTE_H_
#define POSTMAN_REMOTE_H_

#include "../commun.h"
#include <stdio.h>


/************************ PROTOTYPE DECLARATION **************************************/

/** \fn void postman_remote_send_msg (char * buffer, int length)
 *  \brief Function dedicated to send a message stored in buffer that has a "length" size.
 *  \param buffer : buffer in byte to send with a socket
 *  \param length : size of the buffer
 */
extern ssize_t postman_remote_write(uint8_t * bufferToWrite , ssize_t nbBytes);


/** \fn void postman_remote_receive_msg (char * buffer, int length)
 *  \brief Function dedicated to receive a message of a specific length and store it in a buffer.
 *  \param buffer : buffer where the msg in bytes received is stored
 *  \param length : expected size of the msg
 */
extern ssize_t postman_remote_receive(uint8_t * bufferToReceive , ssize_t nbBytes);

/** \fn void postman_remote_start ()
 *  \brief Function dedicated to create a new socket, bind it, listen it, and accept it.
 */
extern void postman_remote_start();

/** \fn void postman_remote_stop ()
 *  \brief Function dedicated to close a socket and force an Exit
 */
extern void postman_remote_stop();


#endif /* POSTMAN_REMOTE_H_ */
