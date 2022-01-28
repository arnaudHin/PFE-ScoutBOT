/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  cartographer.c
 * @author Brient Nathan & Arnaud Hincelin
 * @version 2.0
 * @date 01/12/2021
 * @brief Make cartography
 * The purpose of the cartographer is to make calcul and estimation of the position
 */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "cartographer.h"
#include "../../utils/util.h"
#include "../../utils/Watchdog/watchdog.h"
#include "../mapping/mapper.h"
#include "../positioning/adminPositioning.h"
#include "../../com_remote/proxy_mapviewer.h"


#define MQ_MAX_MESSAGES (10)



typedef enum{

	S_IDLE_C=0,
	S_RUNNING_C,
	S_SET_LIDAR_DATA_C,
	S_SET_POSITION_DATA_C,
	S_ASK_4_DATA_C,
	S_STANDBY_C,
	S_DEATH_C,
	NB_STATE_C
}Cartographer_state_e;

static char * stateC[6] = 
	{
	"S_IDLE_C",
	"S_RUNNING_C",
	"S_SET_LIDAR_DATA_C",
	"S_SET_POSITION_DATA_C",
	"S_ASK_4_DATA_C",
	"S_STANDBY_C",
	"S_DEATH_C"};

typedef enum{
	T_NOP_C = 0,
	T_START_CARTO_C,
	T_STOP_CARTO_C,
	T_ASK_4_DATA_C,
	T_UPDATE_LIDAR_DATA_C,
	T_UPDATE_POSITION_DATA_C,
	T_NB_TRANS_C
}Cartographer_transistion_action_e;


typedef enum{
	E_NOP_C=0,
	E_ACK_C,
	E_START_CARTO_C,
	E_STOP_CARTO_C,
    E_ASK_4_DATA_C,
	E_UPDATE_LIDAR_DATA_C,
	E_UPDATE_POSITION_DATA_C,
	E_STANDBY_C,
	E_NB_EVENT_C
}Cartographer_event_e;

static char * eventC[7] = 
	{"E_NOP_C",
	"E_ACK_C",
	"E_START_CARTO_C",
	"E_STOP_CARTO_C",
	"E_ASK_4_DATA_C",
	"E_UPDATE_LIDAR_DATA_C",
	"E_UPDATE_POSITION_DATA_C",
	"E_STANDBY_C"
	};


typedef struct{
	Cartographer_state_e destinationState;
	Cartographer_transistion_action_e actionToPerform;
} Transition_t;


/* \struct Mq_message_t
 * \brief Mq_message_t structure dedicated to pass the event between multiples threads
 */
typedef struct
{
	Cartographer_event_e event;
} Mq_message_t;

typedef struct{
	Cartographer_state_e mycartographereState;
	Lidar_data_t lidarDataToSend;
	Position_data_t positionDataToSend;

}Cartographer_t;



static Transition_t myTransition[NB_STATE_C][E_NB_EVENT_C] = {

	[S_IDLE_C][E_START_CARTO_C] = {S_RUNNING_C, T_START_CARTO_C},

	[S_RUNNING_C][E_ASK_4_DATA_C] = {S_ASK_4_DATA_C, T_ASK_4_DATA_C},
	[S_RUNNING_C][E_ASK_4_DATA_C] = {S_ASK_4_DATA_C, T_ASK_4_DATA_C},
	[S_RUNNING_C][E_STANDBY_C] = {S_STANDBY_C, T_NOP_C},

	[S_STANDBY_C][E_NOP_C] = {S_STANDBY_C, T_NOP_C},
	[S_STANDBY_C][E_ASK_4_DATA_C] = {S_RUNNING_C, T_ASK_4_DATA_C},
	[S_STANDBY_C][E_ACK_C] = {S_STANDBY_C, T_NOP_C},
	[S_STANDBY_C][E_UPDATE_POSITION_DATA_C] = {S_STANDBY_C, T_NOP_C},
	[S_STANDBY_C][E_UPDATE_LIDAR_DATA_C] = {S_STANDBY_C, T_NOP_C},
	[S_STANDBY_C][E_STANDBY_C] = {S_STANDBY_C, T_NOP_C},

	[S_ASK_4_DATA_C][E_ACK_C] = {S_RUNNING_C, T_NOP_C},
	[S_ASK_4_DATA_C][E_ASK_4_DATA_C] = {S_RUNNING_C, T_NOP_C},
	[S_ASK_4_DATA_C][E_UPDATE_POSITION_DATA_C] = {S_ASK_4_DATA_C, T_NOP_C},
	[S_ASK_4_DATA_C][E_UPDATE_LIDAR_DATA_C] = {S_ASK_4_DATA_C, T_NOP_C},

	[S_RUNNING_C][E_UPDATE_LIDAR_DATA_C] = {S_SET_LIDAR_DATA_C, T_UPDATE_LIDAR_DATA_C},
	[S_SET_LIDAR_DATA_C][E_ACK_C] = {S_RUNNING_C, T_NOP_C},
	[S_SET_LIDAR_DATA_C][E_UPDATE_POSITION_DATA_C] = {S_SET_LIDAR_DATA_C, T_NOP_C},
	[S_SET_LIDAR_DATA_C][E_UPDATE_LIDAR_DATA_C] = {S_SET_LIDAR_DATA_C, T_NOP_C},
	[S_SET_LIDAR_DATA_C][E_ASK_4_DATA_C] = {S_SET_LIDAR_DATA_C, T_NOP_C},

	[S_RUNNING_C][E_UPDATE_POSITION_DATA_C] = {S_SET_POSITION_DATA_C, T_UPDATE_POSITION_DATA_C},
	[S_SET_POSITION_DATA_C][E_ACK_C] = {S_RUNNING_C, T_NOP_C},
	[S_SET_POSITION_DATA_C][E_UPDATE_POSITION_DATA_C] = {S_SET_POSITION_DATA_C, T_NOP_C},
	[S_SET_POSITION_DATA_C][E_UPDATE_LIDAR_DATA_C] = {S_SET_POSITION_DATA_C, T_NOP_C},
	[S_SET_POSITION_DATA_C][E_ASK_4_DATA_C] = {S_SET_POSITION_DATA_C, T_NOP_C},

	[S_RUNNING_C][E_STOP_CARTO_C] = {S_DEATH_C, T_STOP_CARTO_C},
	[S_IDLE_C][E_STOP_CARTO_C] = {S_DEATH_C, T_STOP_CARTO_C},
};


static pthread_t mythread;						   //ID thread
static const char queue_name[] = "/cartographer"; //name of mailbox
static pthread_mutex_t myMutex=PTHREAD_MUTEX_INITIALIZER;
static mqd_t id_bal;	

static Cartographer_t * myCartographer;
static DATA_to_jump_t actualData; //Actual data lidar and position
static uint8_t CARTO_START = 0;


/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

static void cartographer_mq_init();
static void cartographer_mq_send(Cartographer_event_e cartographer_event);
static Mq_message_t cartographer_mq_receive();
static void cartographer_mq_done();

static void *cartographer_run();
static void cartographer_waitTaskTermination();
static void cartographer_performAction(Cartographer_transistion_action_e action);
static void cartographer_setState(Cartographer_state_e state);
static Cartographer_state_e cartographer_getState();

static void perform_signal_start();
static void perform_signal_stop();
static void perform_ask_4_data();
static void perform_updatePositionData();
static void perform_updateLidarData();


/**********************************  PUBLIC FUNCTIONS ************************************************/


extern void cartographer_start(){

    myCartographer = NULL;
    myCartographer = calloc(1, sizeof(Cartographer_t) );
    if (myCartographer == NULL){
        perror("Error calloc");
    }
	cartographer_setState(S_IDLE_C);

    cartographer_mq_init();

	int return_thread = pthread_create(&mythread, NULL, (void *)&cartographer_run, NULL);
	assert(return_thread == 0 && "Error Pthread_create cartographer\n");

	fprintf(stderr, "cartographer_start\n");

}


extern void cartographer_signal_start(){
	
    cartographer_mq_send(E_START_CARTO_C);
	CARTO_START = 1;
}

/**
 * @brief Function called by dispatcher
 */
extern void cartographer_signal_ask4data(){
    cartographer_mq_send(E_ASK_4_DATA_C);
}

/**
 * @brief Function called by mapper
 */
extern void cartographer_signal_setLidarData(){
	fprintf(stderr, "cartographer_signal_setLidarData()\n");
    cartographer_mq_send(E_UPDATE_LIDAR_DATA_C);
}

/**
 * @brief Function called by adminpositioning
 */
extern void cartographer_signal_setPositionData(){
	fprintf(stderr, "cartographer_signal_setPositionData()\n");
    cartographer_mq_send(E_UPDATE_POSITION_DATA_C);
}


extern void cartographer_signal_stop(){
    cartographer_mq_send(E_STOP_CARTO_C);
}

extern void cartographer_signal_stopCartographie(){
    cartographer_mq_send(E_STANDBY_C);
}

extern uint8_t cartographer_getStartState(){
	return CARTO_START;
}





/*********************************** MAILBOX FUNCTION *************************************************************/


static void cartographer_mq_init()
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
	assert(id_bal != -1 && "Error mq_open cartographer\n");

}


/** \fn static void cartographer_mq_send(Event cartographer_event)
 *  \brief Function dedicated to send a special stop message to the mailbox
 *  \param Event the event to send through the mailbox
 */

static void cartographer_mq_send(Cartographer_event_e cartographer_event)
{

	pthread_mutex_lock(&myMutex);
	//***** RACE CONDITION *****//
	Mq_message_t cartographer_msg;
	cartographer_msg.event = cartographer_event;
	
	mqd_t bal_send = mq_send(id_bal, (const char *)&cartographer_msg, sizeof(Mq_message_t), 0); //Priority 0 to 31 (highest priority first)
	assert(id_bal != -1 && "Error mq_send cartographer\n");
	//***** END RACE CONDITION *****//
	pthread_mutex_unlock(&myMutex);

}

/** \fn static Mq_message_t cartographer_mq_receive()
 *  \brief Function dedicated to receive a message from the mailbox
 *  \return Mq_message_t the message received in the mailbox
 */

static Mq_message_t cartographer_mq_receive()
{
	/*Reception of a message from the BAL*/

	Mq_message_t cartographer_msg;
	ssize_t bal_receive_msg = mq_receive(id_bal, (char *)&cartographer_msg, sizeof(Mq_message_t), NULL); //Null priority => mailbox use as a FIFO
	assert(bal_receive_msg != -1 && "Error mq_receive cartographer\n");

	return cartographer_msg;
}

/** \fn static void cartographer_mq_done ()
 *  \brief Function dedicated to destroy the mailbox
 */
static void cartographer_mq_done()
{
	/* close the mailbox */
	mqd_t bal_close = (mq_close(id_bal));
	assert(bal_close != -1 && "x>>> mq_close cartographer\n");

	/* destruct the mailbox */
	mqd_t bal_destruct = mq_unlink((const char *)queue_name);
	assert(bal_destruct != -1 && "x>>> mq_unlink cartographer\n");
}

/********************************* END OF MAILBOX FUNCTION ********************************************************/

static void cartographer_setState(Cartographer_state_e state){
	myCartographer->mycartographereState = state;
}

static Cartographer_state_e cartographer_getState(){
	return myCartographer->mycartographereState;
}

static void cartographer_waitTaskTermination(){
	int error_code = pthread_join(mythread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}

static void perform_signal_start(){
    //Start other active classes
	fprintf(stderr, "perform_signal_start\n");

	adminpositioning_signal_start();
	mapper_signal_start();

}

static void perform_signal_stop(){
	//cartographer_waitTaskTermination();

    //Stop other active classes
    adminpositioning_signal_stop();
    mapper_signal_stop();

	cartographer_mq_done();
    free(myCartographer);
}

static void perform_updateLidarData(){
	mapper_setLidaraData(&myCartographer->lidarDataToSend);
	mapper_signal_ackLidarData();
}

static void perform_updatePositionData(){
	adminpositioning_setPositionData(&myCartographer->positionDataToSend);
	adminpositioning_signal_ackPositionData();
}

static void perform_ask_4_data(){
	actualData.lidarData = myCartographer->lidarDataToSend;
	actualData.positionData = myCartographer->positionDataToSend;
	proxy_mapviewer_send_data(&actualData);
}


static void *cartographer_run(){
	Mq_message_t cartographer_msg;
	Cartographer_transistion_action_e an_action = T_NOP_C;

	while (myCartographer->mycartographereState != S_DEATH_C)
	{
		cartographer_msg = cartographer_mq_receive();

		/*
		fprintf(stderr, "cartographer_run() : state : %s \n", stateC[cartographer_getState()]);
		fprintf(stderr, "cartographer_run() : event : %s \n", eventC[cartographer_msg.event]);
		*/

		if (myTransition[ cartographer_getState() ][cartographer_msg.event].destinationState != S_DEATH_C)
		{
			an_action = myTransition[ cartographer_getState() ][cartographer_msg.event].actionToPerform;
			cartographer_performAction(an_action);

			myCartographer->mycartographereState = myTransition[myCartographer->mycartographereState][cartographer_msg.event].destinationState;
		}
	}
	return NULL;
}



static void cartographer_performAction(Cartographer_transistion_action_e action)
{

    switch(action)
    {
            case T_NOP_C :
                break;

            case T_START_CARTO_C : 
				perform_signal_start();
                break;

            case T_STOP_CARTO_C :
				perform_signal_stop();
                break;

            case T_UPDATE_LIDAR_DATA_C :
				perform_updateLidarData();
				cartographer_mq_send(E_ACK_C);
                break;
            case T_UPDATE_POSITION_DATA_C :
				perform_updatePositionData();
				cartographer_mq_send(E_ACK_C);
                break;
			case T_ASK_4_DATA_C :
				perform_ask_4_data();
				cartographer_mq_send(E_ACK_C);
				break;

            default :
                break;
        
    }
}




/**********************************  EXTERN FUNCTIONS DECLARATIONS ************************************************/






