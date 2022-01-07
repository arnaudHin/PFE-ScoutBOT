/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  postman_remote.c
 * @author Montet Julien & Marbeuf Pierre
 * @version 3.0
 * @date 11/06/2020
 * @brief Postman remote network
 *
 */

#include "postman_remote.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <stdlib.h>
#include "commun.h"
#include <sys/types.h>
#include "../utils/util.h"
#include <pthread.h>

/************************************** DEFINE ****************************************************************/
#define IP_ADDRESS "192.168.1.80"
//#define IP_ADDRESS "127.0.0.1"
#define PORT_DU_SERVEUR (20000) // 19999+1
#define MAX_PENDING_CONNECTIONS (5)
/************************************** END DEFINE ************************************************************/



static int mySocket = -1; 
static pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER;


extern ssize_t postman_remote_write(uint8_t * bufferToWrite , ssize_t nbBytes)
{

	int mutex_lock = pthread_mutex_lock(&mutex_thread);
	assert(mutex_lock == 0 && "Error to lock mutex\n");

	//---- LOCK SECTION ---

  	int resultWrite = 0;

  	resultWrite = write (mySocket, bufferToWrite, nbBytes);

  	if (resultWrite == -1){
    perror("ERROR POSTMAN POCKET WRITE\n");
  	}

	//---- UNLOCK SECTION ---

	mutex_lock = pthread_mutex_unlock(&mutex_thread);
	assert(mutex_lock == 0 && "Error to lock mutex\n");

	return resultWrite;
}


extern ssize_t postman_remote_receive(uint8_t * bufferToReceive , ssize_t nbBytes){

	int resultRead = 0;
	resultRead  = recv(mySocket, bufferToReceive, nbBytes, MSG_WAITALL);
	if (resultRead == -1){
		perror("ERROR POSTMAN POCKET READ\n");
	}

/*
	fprintf(stderr, "   0 : %d\n", bufferToReceive[0]);
	fprintf(stderr, "   1 : %d\n", bufferToReceive[1]);
    fprintf(stderr, "   2 : %d\n", bufferToReceive[2]);
*/
	return resultRead;
}


void postman_remote_start()
{
	// Create socket
	mySocket = -1;

	struct sockaddr_in mon_adresse;
	mySocket = socket (PF_INET, SOCK_STREAM, 0);
	if (mySocket==-1)
	{
		perror("Error : initialization socket incorrect");
		exit(EXIT_FAILURE);
	}

	mon_adresse.sin_family = AF_INET; /* address type = IP */
	mon_adresse.sin_port = htons (PORT_DU_SERVEUR); /* Port TCP or the service is accessible */
	mon_adresse.sin_addr.s_addr = htonl (INADDR_ANY); /* attach to all interfaces */

	/*
	 *  We may reuse this address (if connection problem), that is why we use :setsockopt with SO_REUSEADDR
	 * int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	 * SOL_SOCKET : Use this constant as the level argument to getsockopt or setsockopt to manipulate
	 * the socket-level options described in this section.
	 * SO_REUSEADDR :This option controls whether bind (see Setting Address) should permit reuse of local
	 * addresses for this socket.
	*/

	int enable = 1;
	if (setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0)
	{
	    perror("Error : cannot give setsockopt value");
	    exit(EXIT_FAILURE);
	}
	
	// Bind
	if (bind (mySocket, (struct sockaddr *)&mon_adresse, sizeof (mon_adresse))<0)
	{
		perror("Error: bind - local adress locale not assign");
		exit(EXIT_FAILURE);
	}

	// Listen
	if (listen (mySocket, MAX_PENDING_CONNECTIONS) == -1)
	{
		perror("Error : Problem listen server of socket");
		exit(EXIT_FAILURE);
	}else{
		printf("Success : listen serveur success !\r\n");
	}


	// Accept
	mySocket = accept (mySocket, NULL, 0);
	if (mySocket == -1)
	{
		perror("Error : Problem accept of socket \n");
		exit(EXIT_FAILURE);
	}else{
		printf("SOCKET ACCEPTED \r\n");
	}
}



void postman_remote_stop()
{
	int id_error;
	id_error = close(mySocket);
	if (id_error==-1)
	{
		perror("ERROR : SERVER STOP INCORRECT \n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
