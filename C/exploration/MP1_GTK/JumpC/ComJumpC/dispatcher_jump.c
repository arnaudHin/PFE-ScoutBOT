// /* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
// /**
//  * @file  dispatcher_jump.c
//  * @author Montet Julien & Marbeuf Pierre
//  * @version 3.0
//  * @date 11/06/2020
//  * @brief Dispatcher network
//  *
//  */
// #include "../../JumpC/ComJumpC/dispatcher_jump.h"

// #include <pthread.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>
// #include <errno.h>
// #include "../../util.h"
// #include <unistd.h>
// #include <arpa/inet.h>

// #include "../../JumpC/ComJumpC/postman_jump.h"
// #include "../../JumpC/commun.h"
// //#include "../../JumpC/.h"
// //#include "../../JumpC/network_pilot.h"
// #include "../../JumpC/pilot.h"
// #include "../map_raw.h"

// #define SIZE_MSG_CMD (1)
// #define SIZE_MSG_SIZE (3)
// #define SIZE_COORD (2)


// static pthread_t pthread;


// /**
//  *  \fn static void dispatcher_jump_dispatch(Network_msg network_msg)
//  *
//  *  \brief Function dedicated to look after the network_msg in order to execute a specific action
//  *
//  *  \param Network_msg a structure with the order (two characters) and value (ten characters)
//  *             Values come from the Android app
//  */

// static void dispatcher_jump_dispatch(Network_msg network_msg)
// {
// 	TRACE("CMD = %d\r\n",network_msg.cmd);
// 	switch(network_msg.cmd)
// 	{
// 		case(TRY_DIR):
// 			TRACE("NETWORK MSG ORDER : forward \r\n");
// 			pilot_set_direction(FORWARD);
// 			break;
// 		case(SET_MODE):
// 			TRACE("NETWORK MSG ORDER : backward \r\n");
// 			pilot_set_direction(BACKWARD);
// 			break;
// 		case(CHECK_CONNECTION):
// 			TRACE("NETWORK MSG ORDER : left \r\n");
// 			pilot_set_direction(LEFT);
// 			break;
// 		case(ASK_QUIT):
// 			TRACE("NETWORK MSG ORDER : right \r\n");
// 			pilot_set_direction(RIGHT);
// 			break;
// 		case(LIDAR_MAPPING):
// 			TRACE("NETWORK MSG ORDER : break \r\n");
// 			pilot_set_direction(BREAK);
// 			break;
// 		case(POSITION):
// 			TRACE("NETWORK MSG ORDER : socket closed  \r\n");
// 			pilot_set_direction(BREAK);
// 			postman_jump_stop();
// 			break;

// 		default:
// 			TRACE("NETWORK MSG ORDER : not recognized \r\n");
// 	}
// }


// /**
//  *  \fn void dispatcher_jump_stop()
//  *
//  *  \brief Function dedicated stop the dispatcher
//  *
//  */
// extern void dispatcher_jump_stop()
// {
//         TRACE("DISPATCHER STOP \r\n");
//         int returnCode = pthread_join(pthread,NULL);
//         TRACE("AFTER DISPATCHER STOP \r\n");
//         if (returnCode == -1)
//         {
//                 perror("Error : join thread incorrect");
//                 exit(EXIT_FAILURE);
//         }
//         pilot_stop();
// }


// /**
//  *  \fn void interpret_24_bit_as_int32()
//  *
//  *  \brief Function dedicated convert an hexa-string of 3 bytes in int
//  *
//  *  \param byte_array (char *) : byte_array of 3 bytes to convert
//  *
//  *  \return int : value converted
//  *
//  */
// static int interpret_24_bit_as_int32(unsigned char * byte_array) {
//     return (
//         (byte_array[0] << 24) |
//         (byte_array[1] << 16) |
//         (byte_array[2] << 8)) >> 8;
// }




// /**
//  *  \fn void dispatcher_decod(char * buffer, Network_msg * network_msg)
//  *
//  *  \brief Function dedicated to decode the message received (cmd, size, data) and organize it in a struct
//  *
//  *  \param buffer (char *) : msg received
//  *
//  *  \param network_msg (Network_msg *) : data decoded at the end
//  *
//  */
// static void dispatcher_decod(unsigned char * buffer, Network_msg * network_msg)
// {

// 	network_msg->cmd = NOP;
// 	network_msg->size = 0;
// 	network_msg->coord.x = 0;
// 	network_msg->coord.y = 0;

// 	unsigned char size[SIZE_MSG_SIZE];

// 	// Get value of CMD
// 	memcpy(&network_msg->cmd, &buffer[0], SIZE_MSG_CMD);

// 	// Get value of SIZE
// 	for(int j=0;j<sizeof(size);j++){
// 		strcpy((char *)&size[j],(char *)&buffer[SIZE_MSG_CMD+j]);
// 	}
// 	network_msg->size = interpret_24_bit_as_int32(size);

// 	#ifdef DEBUG
// 	//Show more details
// 	printf("ALL array: ");
// 	for(int i=0;i<SIZE_MSG_CMD+SIZE_MSG_SIZE;i++){
// 		printf("%02X ",buffer[i]);
// 	}printf("\n");

// 	TRACE("CMD : %d\n",network_msg->cmd);
// 	TRACE("Size : %d\n",network_msg->size);

// 	#endif

// 	// If size > 0, data has been sent
// 	if (network_msg->size > 0)
// 	{
// 		// Continue to read the msg
// 		unsigned char buffer_data[network_msg->size];
// 		memset(buffer_data,0x00,sizeof(buffer_data));

// 		// Continue to read the socket
// 		postman_jump_receive_msg(buffer_data,network_msg->size);

// 		#ifdef DEBUG
// 		TRACE("DATA : ");
// 		for(int i=0;i<sizeof(buffer_data);i++){
// 			printf("%02X ",buffer_data[i]);
// 		}printf("\n");
// 		#endif

// 		// Can become a switch case in the future
// 		if (network_msg->cmd == POSITION_ROBOT_DEST){

// 			// Get value of abscissa and ordinate
// 			memcpy(&network_msg->coord.x, &buffer_data[0], SIZE_COORD);
// 			memcpy(&network_msg->coord.y, &buffer_data[SIZE_COORD], SIZE_COORD);
// 			network_msg->coord.x = ntohs(network_msg->coord.x);
// 			network_msg->coord.y = ntohs(network_msg->coord.y);

// 			TRACE("Abscissa : %d - Ordinate : %d\r\n",network_msg->coord.x,network_msg->coord.y);

// 		}else{
// 			TRACE("Expect data but CMD not defined");
// 		}
// 	}
// }


// /**
//  *  \fn static void dispatcher_jump_run()
//  *
//  *  \brief Function dedicated to constantly read the socket
//  *             Once the message is received, it decodes it and requests a dispatch
//  */
// static void * dispatcher_jump_run()
// {
// 	Network_msg network_msg = {.cmd = 30, .size=0, .coord ={0,0}}; //CLOSE
// 	int size_msg =SIZE_MSG_CMD+SIZE_MSG_SIZE;
// 	unsigned char buffer[size_msg];
// 	do
// 	{
// 		TRACE("DISPATCHER RUN \r\n");
// 		network_msg.cmd = CLOSE;
// 		postman_jump_receive_msg(buffer,size_msg);
// 		dispatcher_decod(buffer,&network_msg);
// 		dispatcher_jump_dispatch(network_msg);
// 	}while(network_msg.cmd!=CLOSE);
// 	return 0;
// }

// /**
//  *  \fn static void new_dispatcher_jump_run()
//  *
//  *  \brief Function dedicated to constantly read test messages
//  *             Once the message is received, it decodes it and requests a dispatch
//  */
// static void * new_dispatcher_jump_run()
// {
// 	Network_msg network_msg = {.cmd = 30, .size=0, .coord ={0,0}}; //CLOSE
// 	int size_msg =SIZE_MSG_CMD+SIZE_MSG_SIZE;
// 	unsigned char buffer[size_msg];
// 	do
// 	{
// 		TRACE("DISPATCHER RUN \r\n");
// 		network_msg.cmd = CLOSE;
// 		postman_jump_receive_msg(buffer,size_msg);
// 		dispatcher_decod(buffer,&network_msg);
// 		dispatcher_jump_dispatch(network_msg);
// 	}while(network_msg.cmd!=CLOSE);
// 	return 0;
// }


// /**
//  *  \fn static void dispatcher_jump_start()
//  *
//  *  \brief Function dedicated to start the dispatcher and postman and pilot.
//  */
// extern void dispatcher_jump_start()
// {
//         postman_jump_start();
//         pilot_start();
//         int returnError = pthread_create(&pthread,NULL,&dispatcher_jump_run,NULL);
//         if (returnError == -1)
//         {
//                 perror("Error : creation of thread incorrect in remoteUI start");
//                 exit(EXIT_FAILURE);
//         }
// }

// /**
//  *  \fn static void new_dispatcher_jump_start()
//  *
//  *  \brief Function dedicated to start the dispatcher  with pilot and without postman.
//  */
// extern void new_dispatcher_jump_start()
// {
//         //postman_jump_start();
//         pilot_start();
//         int returnError = pthread_create(&pthread,NULL,&new_dispatcher_jump_run,NULL);
//         if (returnError == -1)
//         {
//                 perror("Error : creation of thread incorrect in remoteUI start");
//                 exit(EXIT_FAILURE);
//         }
// }

