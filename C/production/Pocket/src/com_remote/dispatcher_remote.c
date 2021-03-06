/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  dispatcher_remote.c
 * @author Montet Julien & Marbeuf Pierre
 * @version 3.0
 * @date 11/06/2020
 * @brief Dispatcher network
 *
 */
#include "dispatcher_remote.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "../utils/util.h"
#include <unistd.h>
#include <arpa/inet.h>

#include "postman_remote.h"
#include "../commun.h"
#include "../localizer/cartographer/cartographer.h"
#include "remote_protocol.h"
#include "../pilot/pilot.h"

#define SIZE_MSG_CMD (1)
#define SIZE_MSG_SIZE (3)
#define SIZE_COORD (2)


static pthread_t pthread;
static Message_from_jump_t myMessageFromJump;
// static uint8_t myBufferFromJump[3];
static uint8_t ask_quit = 0;





static void * dispatcher_remote_run();
static void dispatcher_remote_check_for_message();



/**
 *  \fn static void dispatcher_remote_start()
 *
 *  \brief Function dedicated to start the dispatcher and postman and pilot.
 */
extern void dispatcher_remote_start()
{
        postman_remote_start();
        
		int returnError = pthread_create(&pthread,NULL,&dispatcher_remote_run,NULL);
        if (returnError == -1)
        {
                perror("Error : creation of thread incorrect in remoteUI start");
                exit(EXIT_FAILURE);
        }

		pilot_signal_connection_success();
}


/**
 *  \fn void dispatcher_remote_stop()
 *
 *  \brief Function dedicated stop the dispatcher
 *
 */
extern void dispatcher_remote_stop()
{

        int returnCode = pthread_join(pthread,NULL);
        
        if (returnCode == -1)
        {
                perror("Error : join thread incorrect");
                exit(EXIT_FAILURE);
        }

		postman_remote_stop();

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
static int interpret_24_bit_as_int32(unsigned char * byte_array) {
    return (
        (byte_array[0] << 24) |
        (byte_array[1] << 16) |
        (byte_array[2] << 8)) >> 8;
}







/**
 *  \fn static void dispatcher_remote_run()
 *
 *  \brief Function dedicated to constantly read the socket
 *             Once the message is received, it decodes it and requests a dispatch
 */
static void * dispatcher_remote_run(){

	while (ask_quit != 1){
		//fprintf(stderr, "Attente msg from jump...\n");
		dispatcher_remote_check_for_message(); //function block !! 

		CMD_from_jump_e commandMsg = myMessageFromJump.command;

		switch (commandMsg)
		{
			case ASK_TRY_DIR:
				fprintf(stderr, "dispatcher_remote_run receive ASK_TRY_DIR\n");
				pilot_set_direction(myMessageFromJump.data.direction);
				break;
			
			case ASK_STOP_CALIBRATION:
				fprintf(stderr, "dispatcher_remote_run receive ASK_STOP_CALIBRATION\n");
				pilot_set_direction(STOP);
				cartographer_signal_stop();

				break;

			case ASK_QUIT:
				fprintf(stderr, "dispatcher_remote_run receive ASK_QUIT\n");
				ask_quit = 1;
				pilot_set_direction(STOP);
				pilot_signal_exit();
				dispatcher_remote_stop();
				cartographer_signal_stop();
				break;

			case ASK_4_DATA:

				fprintf(stderr, "dispatcher_remote_run receive ASK_4_DATA\n");

				// if cartographer is not strated, we start it
				if( cartographer_getStartState() == 0 ){
					cartographer_signal_start();
				}
				// if cartographer active, we ask data
				
				cartographer_signal_ask4data();

				break;
				

			default:
				break;
		}
	}
	return NULL;
}


static void dispatcher_remote_check_for_message(){
	uint8_t myBufferFromJump[3];
	ssize_t resultRead = 0;
    ssize_t byteToRead = CMD_SIZE_BYTE + 2;

	memset(myBufferFromJump, 0, sizeof(myBufferFromJump));

	resultRead = postman_remote_receive(myBufferFromJump, byteToRead);

	if(resultRead == -1 || resultRead == 0){
		fprintf(stderr, "ERROR POSTMAN RECEIVE 0 : %d\n", resultRead);
		fprintf(stderr, "\n****************** Application Closing... *******************\n");
		myMessageFromJump.sizeData = 0;
		myMessageFromJump.command = ASK_QUIT;
		return NULL;
	}

	//DECODE myBufferFromJump -> myMessageFromJump (CMD + SIZEdata)
	remote_protocol_decode(myBufferFromJump, &myMessageFromJump, byteToRead);

	// if(myMessageFromJump.sizeData == 1280){
	// 	myMessageFromJump.sizeData = 0;
	// 	myMessageFromJump.command = ASK_4_DATA;
	// }
	// if(myMessageFromJump.sizeData == 5){
	// 	myMessageFromJump.sizeData = 0;
	// 	myMessageFromJump.command = ASK_4_DATA;
	// }

	fprintf(stderr, "\nCMD : %d | Size : %d | \n", myMessageFromJump.command, myMessageFromJump.sizeData);

	if(myMessageFromJump.sizeData != 0){
		byteToRead = myMessageFromJump.sizeData;
		uint8_t myTempBuffer[byteToRead];
		memset(myTempBuffer, 0x00, sizeof(myTempBuffer) );

		resultRead = postman_remote_receive(myTempBuffer, byteToRead);
		if(resultRead == -1){
			fprintf(stderr, "ERROR POSTMAN RECEIVE DATA : %d\n", resultRead);
		}
		
		//DECODE myTempBuffer -> myMessageFromJump (DATA)
		remote_protocol_decode(myTempBuffer, &myMessageFromJump, byteToRead);

	}

}






