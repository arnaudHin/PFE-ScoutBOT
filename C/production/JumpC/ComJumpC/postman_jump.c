/**
 * @file postman_jump.c
 * @author Adrien LE ROUX 
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "postman_jump.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include "commun.h"
#include "util.h"
#include <pthread.h>
#include "../App/notice_popup.h"
/************************************** DEFINE ****************************************************************/
#define IP_ADDRESS "192.168.72.1"
//#define IP_ADDRESS "127.0.0.1"
#define PORT_DU_SERVEUR (20000) // 19999+1
#define MAX_PENDING_CONNECTIONS (5)
/************************************** END DEFINE ************************************************************/

static int a_socket = -1;
static pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER;

extern void postman_jumpC_send_msg(Message_to_pocket_t *msg)
{
	int mutex_lock = pthread_mutex_lock(&mutex_thread);
	assert(mutex_lock == 0 && "Error to lock mutex\n");

	uint8_t buffToSend[4];
	memset(buffToSend, 0x00, sizeof(buffToSend));
	buffToSend[0] = msg->cmd;
	buffToSend[1] = (msg->sizeData >> 8) & 0xFF;
	buffToSend[2] = msg->sizeData & 0xFF;
	buffToSend[3] = msg->data.direction;

	for (size_t i = 0; i < 4; i++)
	{
		printf("Envoie Buffer[%ld] : %d \n", i, buffToSend[i]);
	}

	ssize_t s = write(a_socket, buffToSend, sizeof(buffToSend));

	if (s == -1)
	{
		perror("Error : Information not send\r\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		TRACE("Success : Information send \r\n");
	}

	mutex_lock = pthread_mutex_unlock(&mutex_thread);
	assert(mutex_lock == 0 && "Error to lock mutex\n");
}

ssize_t postman_jumpC_receive_msg(uint8_t *bufferToReceive, ssize_t nbBytes)
{
	int resultRead = 0;
	resultRead = recv(a_socket, bufferToReceive, nbBytes, 0);
	if (resultRead == -1)
	{
		perror("ERROR POSTMAN MONITOR APP READ\n");
	}

	return resultRead;
	// TRACE("Postman jumpC is receiving ... \r\n");
	// if (a_socket == -1)
	// {
	// 	perror("Error : incorrect port socket data");
	// 	exit(EXIT_FAILURE);
	// }else{
	// 	while(current_size<size){
	// 		returnBytes = read (a_socket, buffer_read, size);
	// 		if (returnBytes==-1) ///< save informations in buffer, use of the address directly to counter the malloc
	// 		{
	// 			perror("Error : lecture impossible\r\n");
	// 			exit(EXIT_FAILURE);
	// 		}

	// 		// returnBytes can be equal to 0 in the end of the connection
	// 		if (returnBytes!=0){
	// 			for(int i=0;i < returnBytes;i++){
	// 				bufferToReceive[nbBytes] = buffer_read[i];
	// 			};

	// 			current_size = current_size + returnBytes;
	// 			TRACE("returnBytes : %d\r\n",size);
	// 			TRACE("returnBytes : %d\r\n",returnBytes);
	// 		}else{
	// 			break;
	// 		}
	// 	}
	// }
}

void postman_jumpC_start()
{
	// Create socket
	a_socket = -1;

	struct sockaddr_in mon_adresse;
	a_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (a_socket == -1)
	{
		perror("Error : initialization socket incorrect");
		exit(EXIT_FAILURE);
	}

	mon_adresse.sin_family = AF_INET;					 /* address type = IP */
	mon_adresse.sin_port = htons(PORT_DU_SERVEUR);		 /* Port TCP or the service is accessible */
	mon_adresse.sin_addr.s_addr = inet_addr(IP_ADDRESS); /* attach to all interfaces */

	/*
	 *  We may reuse this address (if connection problem), that is why we use :setsockopt with SO_REUSEADDR
	 * int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	 * SOL_SOCKET : Use this constant as the level argument to getsockopt or setsockopt to manipulate
	 * the socket-level options described in this section.
	 * SO_REUSEADDR :This option controls whether bind (see Setting Address) should permit reuse of local
	 * addresses for this socket.
	*/

	if (connect(a_socket, (struct sockaddr *)&mon_adresse, sizeof(mon_adresse)) != 0)
	{
		perror("Error : cannot give setsockopt value");
		noticePopup_new(CON_UNSUCCESS);
		exit(EXIT_FAILURE);
	}
	noticePopup_new(CON_SUCCESS);
}

void postman_jumpC_stop()
{

	TRACE("CLIENT STOP\r\n");
	int id_error;
	id_error = close(a_socket);
	if (id_error == -1)
	{
		perror("ERROR : CLEINT STOP INCORRECT \n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
