/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  postman_pocket.c
 * @author Montet Julien & Marbeuf Pierre
 * @version 3.0
 * @date 11/06/2020
 * @brief Postman pocket network
 *
 */

#include "../../pocket/com_Pocket/postman_pocket.h"

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
#include "../../util.h"
#include <pthread.h>

/************************************** DEFINE ****************************************************************/
#define IP_ADDRESS "192.168.1.80"
//#define IP_ADDRESS "127.0.0.1"
#define PORT_DU_SERVEUR (20000) // 19999+1
#define MAX_PENDING_CONNECTIONS (5)
/************************************** END DEFINE ************************************************************/

static int a_socket = -1;
static pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER;


void postman_pocket_send_msg(unsigned char * data, int length)
{
	int mutex_lock = pthread_mutex_lock(&mutex_thread);
	assert(mutex_lock == 0 && "Error to lock mutex\n");
	int error_code = 0;
	for(int i=0;i<length;i++){
		printf("%02X ",data[i]);
	}printf("\n");

	TRACE("POSTMAN WRITING : %s\r\n",data);
	if(write (a_socket, data, length)==-1)
	{
		perror("Error : Information not send\r\n");
		exit(EXIT_FAILURE);
	}else{
		TRACE("Success : Information send \r\n");
	}

	mutex_lock = pthread_mutex_unlock(&mutex_thread);
	assert(mutex_lock == 0 && "Error to lock mutex\n");
}


void postman_pocket_receive_msg(unsigned char * buffer, int size)
{
	char buffer_read[size];
	memset(buffer,0,size);
	memset(buffer_read,0,size);

	int current_size = 0;
	int returnBytes = 0;

	TRACE("Postman pocket is receiving ... \r\n");
	if (a_socket == -1)
	{
		perror("Error : incorrect port socket data");
		exit(EXIT_FAILURE);
	}else{
		while(current_size<size){
			returnBytes = read (a_socket, buffer_read, size);
			if (returnBytes==-1) ///< save informations in buffer, use of the address directly to counter the malloc
			{
				perror("Error : lecture impossible\r\n");
				exit(EXIT_FAILURE);
			}

			// returnBytes can be equal to 0 in the end of the connection
			if (returnBytes!=0){
				for(int i=0;i<returnBytes;i++){
					buffer[current_size+i] = buffer_read[i];
				};

				current_size = current_size + returnBytes;
				TRACE("returnBytes : %d\r\n",size);
				TRACE("returnBytes : %d\r\n",returnBytes);
			}else{
				break;
			}
		}
	}
}


void postman_pocket_start()
{
	// Create socket
	a_socket = -1;

	struct sockaddr_in mon_adresse;
	a_socket = socket (PF_INET, SOCK_STREAM, 0);
	if (a_socket==-1)
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
	if (setsockopt(a_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0)
	{
	    perror("Error : cannot give setsockopt value");
	    exit(EXIT_FAILURE);
	}
	// Bind
	TRACE("BIND\r\n");
	if (bind (a_socket, (struct sockaddr *)&mon_adresse, sizeof (mon_adresse))<0)
	{
		TRACE("BIND ERROR \r\n");
		perror("Error: bind - local adress locale not assign");
		exit(EXIT_FAILURE);
	}

	// Listen
	if (listen (a_socket, MAX_PENDING_CONNECTIONS) == -1)
	{
		TRACE("SOCKET LISTEN ERROR \r\n");
		perror("Error : Problem listen server of socket");
		exit(EXIT_FAILURE);
	}else{
		printf("Success : listen serveur success !\r\n");
	}


	// Accept
	a_socket = accept (a_socket, NULL, 0);
	if (a_socket == -1)
	{
		TRACE("SOCKET ACCEPT ERROR \r\n");
		perror("Error : Problem accept of socket \n");
		exit(EXIT_FAILURE);
	}else{
		TRACE("SOCKET ACCEPTED \r\n");
	}
}



void postman_pocket_stop()
{
	TRACE("SERVER STOP\r\n");
	int id_error;
	id_error = close(a_socket);
	if (id_error==-1)
	{
		perror("ERROR : SERVER STOP INCORRECT \n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}