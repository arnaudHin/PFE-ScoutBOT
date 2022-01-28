/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  proxy_robot.c
 * @author Debus Alexy
 * @version 0.2
 * @date 01/05/2020
 * @brief Postman_pilot entity header file
 *
 * The purpose of the postman_pilot is to send / receive message on a serial port open by the M4
 * and regularly called /dev/ttyRPMSG0 on the channel 0 and /dev/ttyRMPSG1 on the channel 1
 *
 */

#ifndef SRC_POCKET_COM_RACE_POSTMAN_PILOT_H_
#define SRC_POCKET_COM_RACE_POSTMAN_PILOT_H_

#define MAX_MESSAGE_LENGTH (128)

#include "../pilot/pilot.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define PARSE_SYMBOL \




/* \fn extern void postman_start()
 *  \brief Function dedicated to start the postman (Open the serial port)
 */
extern void postman_race_start();

/* \fn extern void postman_stop()
 *  \brief Function dedicated to stop the postman (Close the serial port)
 */
extern void postman_race_stop();


/* \fn extern void postman_send_message(Pilot_message msg)
 * \brief Function dedicated to send message on the given serial port
 * \param Pilot_message the structure which contain the message to send
 */
extern void postman_race_send_message(uint8_t * bufferToWrite, uint16_t bytesToSend);

#endif /* SRC_POCKET_COM_RACE_POSTMAN_PILOT_H_ */
