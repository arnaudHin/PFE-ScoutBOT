/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  maper.c
 * @author ARONDEL Martin
 * @version 2.0
 * @date 01/122021
 * @brief Make lidar calcul
 * The purpose of the mapper is to retrieve objects positions with the lidar
 */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include "mapper.h"
#include "../../utils/util.h"
#include "../cartographer/cartographer.h"
/************************************** DEFINE ****************************************************************/
#define DISPLAY 1
/************************************** STATIC VARIABLE *******************************************************/


#define MQ_MAX_MESSAGES (10)
#define TAILLE_MAX 720


typedef enum{
	S_IDLE_M=0,
	S_SCANNING_M,
    S_SET_LIDAR_DATA_M,
	S_DEATH_M,
	NB_STATE_M
}Mapper_state_e;

static char * stateM[4] = 
	{"S_IDLE_M",
	"S_SCANNING_M",
	"S_SET_LIDAR_DATA_M",
	"S_DEATH_M",
	};

typedef enum{
	T_NOP_M = 0,
	T_START_MAPPER_M,
	T_STOP_MAPPER_M,
    T_SCANNING_M,
	T_SET_LIDAR_DATA_M,
	T_ACK_LIDAR_DATA_M,
	T_NB_TRANS_M
}Mapper_transistion_action_e;


typedef enum{
    E_NOP_M=0,
    E_SCAN_M,
    E_START_MAPPER_M,
	E_STOP_MAPPER_M,
    E_SET_LIDAR_DATA_M,
	E_ACK_LIDAR_DATA_M,
	E_NB_EVENT_M
}Mapper_event_e;

static char * eventM[6] = 
	{"E_NOP_M",
	"E_SCAN_M",
	"E_START_MAPPER_M",
	"E_STOP_MAPPER_M",
	"E_SET_LIDAR_DATA_M",
	"E_ACK_LIDAR_DATA_M",
	};


/**
 * @brief : 
 * @struct : 
 */
typedef struct{
	Mapper_state_e destinationState;
	Mapper_transistion_action_e actionToPerform;
} Transition_t;


/** 
 * @struct Mq_message_t
 * @brief Mq_message_t structure dedicated to pass the event between multiples threads
 */
typedef struct
{
	Mapper_event_e event;
} Mq_message_t;

/**
 * @brief : 
 * @struct : 
 */
typedef struct{
	Mapper_state_e myMappereState;
	Lidar_data_t lidarData;

}Mapper_t;





static Transition_t myTransition[NB_STATE_M][E_NB_EVENT_M] = {

	[S_IDLE_M][E_START_MAPPER_M] = {S_SCANNING_M, T_START_MAPPER_M},

	[S_SCANNING_M][E_SCAN_M] = {S_SCANNING_M, T_SCANNING_M},
	[S_SCANNING_M][E_SET_LIDAR_DATA_M] = {S_SET_LIDAR_DATA_M, T_SET_LIDAR_DATA_M},

	[S_SET_LIDAR_DATA_M][E_ACK_LIDAR_DATA_M] = {S_SCANNING_M, T_ACK_LIDAR_DATA_M},
	[S_SET_LIDAR_DATA_M][E_SCAN_M] = {S_SET_LIDAR_DATA_M, T_NOP_M},
	[S_SET_LIDAR_DATA_M][E_SET_LIDAR_DATA_M] = {S_SET_LIDAR_DATA_M, T_NOP_M},
	
	[S_SCANNING_M][E_STOP_MAPPER_M] = {S_DEATH_M, T_STOP_MAPPER_M},
	[S_SET_LIDAR_DATA_M][E_STOP_MAPPER_M] = {S_DEATH_M, T_STOP_MAPPER_M},
	[S_IDLE_M][E_STOP_MAPPER_M] = {S_DEATH_M, T_STOP_MAPPER_M},
};


/************************************** DECLARATIONS *******************************************************/


static const char queue_name[] = "/mapper"; //name of mailbox
static mqd_t id_bal;	
static pthread_t mythread;
static pthread_mutex_t myMutex=PTHREAD_MUTEX_INITIALIZER;
static Mapper_t * myMapper;



/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/
static void mapper_mq_init();
static void mapper_mq_send();
static Mq_message_t mapper_mq_receive();
static void mapper_mq_done();

static void * mapper_run();
static void mapper_waitTaskTermination();
static void mapper_getLidarValues();
static void mapper_performAction(Mapper_transistion_action_e action);

static void mapper_start();
static void perform_mapper_stop();
static void mapper_perform_setLidarData();


/**********************************  PUBLIC FUNCTIONS ************************************************/

extern void mapper_signal_start(){
	mapper_start();
}

extern void mapper_signal_ackLidarData(){
	fprintf(stderr, "mapper_signal_ackLidarData() \n");
    mapper_mq_send(E_ACK_LIDAR_DATA_M);
}

extern void mapper_signal_stop(){
    mapper_mq_send(E_STOP_MAPPER_M);
}

//called by cartographer
extern void mapper_setLidaraData(Lidar_data_t * lidarData){
	*lidarData = myMapper->lidarData;
	//memcpy(lidarData, &myMapper->lidarData, sizeof(Lidar_data_t) );
}

/**********************************  PRIVATE FUNCTIONS ************************************************/


/** 
 * @fn static void *mapper_run()
 * @brief Function run executed by the thread of mapper
 * @retval NULL
 */
static void * mapper_run(){

	Mq_message_t mapper_msg;
	Mapper_transistion_action_e an_action = T_NOP_M;

	while (myMapper->myMappereState != S_DEATH_M)
	{
		mapper_msg = mapper_mq_receive();

/*
		fprintf(stderr, "mapper_run() : state : %s \n", stateM[ myMapper->myMappereState ]);
		fprintf(stderr, "mapper_run() : event : %s \n", eventM[mapper_msg.event]);
*/

		if (myTransition[myMapper->myMappereState ][mapper_msg.event].destinationState != S_DEATH_M)
		{
			an_action = myTransition[myMapper->myMappereState][mapper_msg.event].actionToPerform;
			mapper_performAction(an_action);

			myMapper->myMappereState = myTransition[myMapper->myMappereState][mapper_msg.event].destinationState;
		}
	}
	return NULL;
}




static void mapper_performAction(Mapper_transistion_action_e action)
{

    switch(action)
    {
            case T_NOP_M :
                break;
			case T_START_MAPPER_M:
				mapper_mq_send(E_SCAN_M);
			case T_SCANNING_M :
				/* BEGIN LIDAR SCANNING */
				// mapper_getLidarValues();
				mapper_getLidarValues();
				/* END LIDAR SCANNING */
				mapper_mq_send(E_SET_LIDAR_DATA_M);
				break;

            case T_SET_LIDAR_DATA_M :
				mapper_perform_setLidarData();
                break;
			case T_ACK_LIDAR_DATA_M : 
				mapper_mq_send(E_SCAN_M);
				break;

            case T_STOP_MAPPER_M :
				perform_mapper_stop();
                break;

            default :
                break;
        
    }
}


static void mapper_getLidarValues(){
    FILE* fichier = NULL;
    uint32_t buf_index = 0;
    int16_t x;
    int16_t y;
	int16_t bufX[360];
	int16_t bufY[360];

    // fprintf(stderr, "Ouverture du fichier lidar.sh\n");
	system("chmod +x ./lidar.sh");
    system("./lidar.sh");
    fichier = fopen("position_gtk.txt","r");

	if (fichier != NULL)
    {
        do{
            int a = fscanf(fichier, "%hd,%hd,", &x, &y);
			bufX[buf_index] = x;
			bufY[buf_index] = y;
            buf_index++;
        }while( !feof(fichier));

        fclose(fichier);
    }

    else{
        fprintf(stderr,"Error! opening file 'position_gtk.txt'\n");
    }

	for (size_t i = 0; i < 360 ; i++){
		myMapper->lidarData.X_buffer[i] = (int16_t) bufX[i] ;
		myMapper->lidarData.Y_buffer[i] = (int16_t) bufY[i];
	}
	sleep(3);
}




/** @fn extern void mapper_start()
 *  @brief Function dedicated to execute the mapper thread
 *  @retval NULL
 */
static void mapper_start(){

    myMapper = NULL;
    myMapper = calloc(1, sizeof(Mapper_t) );
    if (myMapper == NULL)
    {
        perror("Error calloc");
    }
    myMapper->myMappereState = S_IDLE_M;

    mapper_mq_init();

    int return_thread = pthread_create(&mythread, NULL, &mapper_run, NULL);
	assert(return_thread == 0 && "Error Pthread_create adminPositioning\n");

	mapper_mq_send(E_START_MAPPER_M);
	
	fprintf(stderr, "mapper_start\n");

}

/** \fn static void mapper_stop()
 *  \brief Function dedicated to stop the mapper thread
 *  \retval NULL
 */
static void perform_mapper_stop(){
	mapper_mq_done();
	free(myMapper);

}



static void mapper_perform_setLidarData(){

	fprintf(stderr, "mapper_perform_setLidarData\n");
	//** BEGIN Call mapper function **
	cartographer_signal_setLidarData();
	//** END Call mapper function **
	
}


/** \fn static void mapper_waitTaskTermination ()
 *  \brief Function stop executed by the thread of mapper
 */
static void mapper_waitTaskTermination()
{
	int error_code = pthread_join(mythread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}


/*********************************** MAILBOX FUNCTION *************************************************************/


static void mapper_mq_init()
{
	/* INIT BAL */
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MQ_MAX_MESSAGES;
	attr.mq_msgsize = sizeof(Mq_message_t);
	attr.mq_curmsgs = 0;

	/* Destruct a pre-existing mailbox */
	mq_unlink(queue_name);

	/* Create and open the mailbox */

	id_bal = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &attr); // Creation of a new mailbox if any was created before.
	assert(id_bal != -1 && "Error mq_open mapper\n");

}


/** @fn static void mapper_mq_send(Event mapper_event)
 *  @brief Function dedicated to send a special stop message to the mailbox
 *  @param Event the event to send through the mailbox
 */

static void mapper_mq_send(Mapper_event_e mapper_event)
{
	pthread_mutex_lock(&myMutex);
	//***** RACE CONDITION *****//
	Mq_message_t mapper_msg;
	mapper_msg.event = mapper_event;
	
	mqd_t bal_send = mq_send(id_bal, (const char *)&mapper_msg, sizeof(Mq_message_t), 0); //Priority 0 to 31 (highest priority first)
	assert(id_bal != -1 && "Error mq_send mapper\n");
	//***** END RACE CONDITION *****//
	pthread_mutex_unlock(&myMutex);
}

/** \fn static Mq_message_t mapper_mq_receive()
 *  \brief Function dedicated to receive a message from the mailbox
 *  \return Mq_message_t the message received in the mailbox
 */

static Mq_message_t mapper_mq_receive()
{
	/*Reception of a message from the BAL*/

	Mq_message_t mapper_msg;
	ssize_t bal_receive_msg = mq_receive(id_bal, (char *)&mapper_msg, sizeof(Mq_message_t), NULL); //Null priority => mailbox use as a FIFO
	assert(bal_receive_msg != -1 && "Error mq_receive mapper\n");

	return mapper_msg;
}

/** \fn static void mapper_mq_done ()
 *  \brief Function dedicated to destroy the mailbox
 */
static void mapper_mq_done()
{
	/* close the mailbox */
	mqd_t bal_close = (mq_close(id_bal));
	assert(bal_close != -1 && "x>>> mq_close mapper\n");

	/* destruct the mailbox */
	mqd_t bal_destruct = mq_unlink((const char *)queue_name);
	assert(bal_destruct != -1 && "x>>> mq_unlink mapper\n");
}

