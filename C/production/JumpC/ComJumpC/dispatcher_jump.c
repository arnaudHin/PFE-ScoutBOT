/**
 * @file dispatcher_jumpC.c
 * @author Adrien LE ROUX
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../../JumpC/ComJumpC/dispatcher_jump.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "util.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "../Mapping/map_viewer.h"
#include "protocol_jump.h"
#include "../../JumpC/ComJumpC/postman_jump.h"
#include "commun.h"
#include "map_manager.h"

//#include "../../JumpC/.h"
//#include "../../JumpC/network_pilot.h"

#define SIZE_MSG_CMD (1)
#define SIZE_MSG_SIZE (2)

static pthread_t pthread;

/**
 *  \fn static void dispatcher_jump_dispatch(Message_from_pocket_t network_msg)
 *
 *  \brief Function dedicated to look after the network_msg in order to execute a specific action
 *
 *  \param Network_msg a structure with the order (two characters) and value (ten characters)
 *             Values come from the Android app
 */

static void dispatcher_jumpC_dispatch(Message_from_pocket_t network_msg)
{
    TRACE("CMD = %d\r\n", network_msg.command);
    switch (network_msg.command)
    {
    case CHECK_CONNECTION:

        break;
    case SET_CHECK_CONNECTION:

        break;
    case SET_DATA:
        MapManager_setMapPositions(network_msg.data);
        break;
    default:
        TRACE("NETWORK MSG ORDER : not recognized \r\n");
        break;
    }
}

/**
 *  \fn void dispatcher_jumpC_stop()
 *
 *  \brief Function dedicated stop the dispatcher
 *
 */
extern void dispatcher_jumpC_stop()
{
    TRACE("DISPATCHER STOP \r\n");
    int returnCode = pthread_join(pthread, NULL);
    TRACE("AFTER DISPATCHER STOP \r\n");
    if (returnCode == -1)
    {
        perror("Error : join thread incorrect");
        exit(EXIT_FAILURE);
    }
}

/**
 *  \fn void interpret_24_bit_as_int32()
 *
 *  \brief Function dedicated convert an hexa-string of 3 bytes in int
 *
 *  \param byte_array (char *) : byte_array of 3 bytes to convert
 *
 *  \return int : value converted
 *
 */
// static int interpret_24_bit_as_int32(unsigned char *byte_array)
// {
//     return (
//                (byte_array[0] << 24) |
//                (byte_array[1] << 16) |
//                (byte_array[2] << 8)) >>
//            8;
// }

/**
 *  \fn void dispatcher_decod(char * buffer, Network_msg * network_msg)
 *
 *  \brief Function dedicated to decode the message received (cmd, size, data) and organize it in a struct
 *
 *  \param buffer (char *) : msg received
 *
 *  \param network_msg (Network_msg *) : data decoded at the end
 *
 */
static void dispatcher_decod(uint8_t *myTempBuffer, Message_from_pocket_t *network_msg)
{
    ssize_t resultRead = 0;
    ssize_t byteToRead = SIZE_MSG_CMD + SIZE_MSG_SIZE;
    protocol_jump_decode(myTempBuffer, network_msg, byteToRead);
    TRACE("[dispatcher_decod] décoder message->size : %d] \n", network_msg->size);
    if (network_msg->size != 0)
    {
        TRACE("[dispatcher_decod] size != 0 \n");
        byteToRead = network_msg->size;
        uint8_t myTempBuffer1[byteToRead];
        memset(myTempBuffer1, 0x00, sizeof(myTempBuffer1) * network_msg->size);

        resultRead = postman_jumpC_receive_msg(myTempBuffer1, byteToRead);
        TRACE("[dispatcher_decod] après receive msg \n");
        if (resultRead == -1)
        {
            fprintf(stderr, "ERROR POSTMAN RECEIVE DATA : %ld\n", resultRead);
        }

        //DECODE myTempBuffer -> myMessageFromJump (DATA)
        protocol_jump_decode(myTempBuffer1, network_msg, byteToRead);
    }

    fprintf(stderr, "\nCMD : %d | ", network_msg->command);
    fprintf(stderr, "Size : %d | ", network_msg->size);
    //fprintf(stderr, "Data dir : %d \n", network_msg->data.lidarData);
}

/**
 *  \fn static void dispatcher_jumpC_run()
 *
 *  \brief Function dedicated to constantly read the socket
 *             Once the message is received, it decodes it and requests a dispatch
 */
static void *dispatcher_jumpC_run()
{
    ssize_t byteToRead = SIZE_MSG_CMD + SIZE_MSG_SIZE + LIDAR_TOTAL_DATA * 2;
    uint8_t myBufferFromJump[BUFF_SIZE_TO_RECEIVE + CMD_SIZE_BYTE + sizeof(uint16_t)];
    Message_from_pocket_t network_msg = {.command = NOP_CMD, .size = 0}; //SET_ASK_QUIT
    memset(network_msg.data.lidarData.X_buffer, 0, sizeof(int16_t) * LIDAR_TOTAL_DEGREE);
    memset(network_msg.data.lidarData.Y_buffer, 0, sizeof(int16_t) * LIDAR_TOTAL_DEGREE);

    do
    {
        TRACE("DISPATCHER RUN \r\n");
        network_msg.command = SET_ASK_QUIT;
        postman_jumpC_receive_msg(myBufferFromJump, byteToRead);
        dispatcher_decod(myBufferFromJump, &network_msg);

        dispatcher_jumpC_dispatch(network_msg);
    } while (network_msg.command != SET_ASK_QUIT);
    return 0;
}

/**
 *  \fn static void new_dispatcher_jumpC_run()
 *
 *  \brief Function dedicated to constantly read test messages
 *             Once the message is received, it decodes it and requests a dispatch
 */
static void *new_dispatcher_jumpC_run()
{
    Message_from_pocket_t network_msg = {.command = NOP_CMD, .size = 0}; //SET_ASK_QUIT
    memset(network_msg.data.lidarData.X_buffer, 0, sizeof(int16_t) * LIDAR_TOTAL_DEGREE);
    memset(network_msg.data.lidarData.Y_buffer, 0, sizeof(int16_t) * LIDAR_TOTAL_DEGREE);
    uint16_t size_msg = SIZE_MSG_CMD + SIZE_MSG_SIZE;
    unsigned char buffer[size_msg];
    do
    {
        TRACE("DISPATCHER RUN \r\n");
        network_msg.command = NOP_CMD;
        postman_jumpC_receive_msg(buffer, size_msg);
        dispatcher_decod(buffer, &network_msg);
        dispatcher_jumpC_dispatch(network_msg);
    } while (network_msg.command != SET_ASK_QUIT);
    return 0;
}

/**
 *  \fn static void dispatcher_jumpC_start()
 *
 *  \brief Function dedicated to start the dispatcher and postman and pilot.
 */
extern void dispatcher_jumpC_start()
{
    postman_jumpC_start();
    int returnError = pthread_create(&pthread, NULL, &dispatcher_jumpC_run, NULL);
    if (returnError == -1)
    {
        perror("Error : creation of thread incorrect in remoteUI start");
        exit(EXIT_FAILURE);
    }
}

/**
 *  \fn static void new_dispatcher_jumpC_start()
 *
 *  \brief Function dedicated to start the dispatcher  with pilot and without postman.
 */
extern void new_dispatcher_jumpC_start()
{
    //postman_jumpC_start();
    int returnError = pthread_create(&pthread, NULL, &new_dispatcher_jumpC_run, NULL);
    if (returnError == -1)
    {
        perror("Error : creation of thread incorrect in remoteUI start");
        exit(EXIT_FAILURE);
    }
}
