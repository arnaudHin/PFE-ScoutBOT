/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  adminPositioning.c
 * @author Brient Nathan
 * @version 2.0
 * @date 01/122021
 * @brief Make positioning calcul
 *
 * The purpose of the adminPositioning is to make calcul and estimation of the position
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include "adminPositioning.h"
#include "../../utils/util.h"
#include "../../utils/Watchdog/watchdog.h"
#include "../cartographer/cartographer.h"


/************************************** DEFINE ****************************************************************/
#define TAILLE_MAX 20 //nombre de caractères maximums a lire 

/*****
/************************************** DEFINE ****************************************************************/
#define DISPLAY 1
/************************************** STATIC VARIABLE *******************************************************/


#define MQ_MAX_MESSAGES (10)


typedef enum{
	S_IDLE_P=0,
	S_POSITIONING_P,
    S_SET_POSITION_DATA_P,
	S_DEATH_P,
	NB_STATE_P
}adminpositioning_state_e;

typedef enum{
	T_NOP_P = 0,
	T_STOP_POSITIONING_P,
    T_POSITIONING_P,
	T_SET_POSITION_DATA_P,
	T_ACK_POSITION_DATA_P,
	T_NB_TRANS_P
}adminpositioning_transistion_action_e;


typedef enum{
    E_NOP_P=0,
    E_START_POSITIONING_P,
	E_STOP_POSITIONING_P,
	E_POSITIONING_P,
    E_SET_POSITION_DATA_P,
	E_ACK_POSITION_DATA_P,
	E_NB_EVENT_P
}adminpositioning_event_e;



/**
 * @brief : 
 * @struct : 
 */
typedef struct{
	adminpositioning_state_e destinationState;
	adminpositioning_transistion_action_e actionToPerform;
} Transition_t;




/** 
 * @struct Mq_message_t
 * @brief Mq_message_t structure dedicated to pass the event between multiples threads
 */
typedef struct
{
	adminpositioning_event_e event;
} Mq_message_t;



/**
 * @brief : 
 * @struct : 
 */
typedef struct{
	adminpositioning_state_e myAdminpositioningState;
	Position_data_t positionData;

}adminpositioning_t;



static Transition_t myTransition[NB_STATE_P][E_NB_EVENT_P] = {

	[S_IDLE_P][E_START_POSITIONING_P] = {S_POSITIONING_P, T_POSITIONING_P},

	[S_POSITIONING_P][E_POSITIONING_P] = {S_POSITIONING_P, T_POSITIONING_P},
	[S_POSITIONING_P][E_SET_POSITION_DATA_P] = {S_SET_POSITION_DATA_P, T_SET_POSITION_DATA_P},

	[S_SET_POSITION_DATA_P][E_ACK_POSITION_DATA_P] = {S_POSITIONING_P, T_ACK_POSITION_DATA_P},
	[S_SET_POSITION_DATA_P][E_POSITIONING_P] = {S_SET_POSITION_DATA_P, T_NOP_P},
	[S_SET_POSITION_DATA_P][E_SET_POSITION_DATA_P] = {S_SET_POSITION_DATA_P, T_NOP_P},
	
	[S_POSITIONING_P][E_STOP_POSITIONING_P] = {S_DEATH_P, T_STOP_POSITIONING_P},
	[S_IDLE_P][E_STOP_POSITIONING_P] = {S_DEATH_P, T_STOP_POSITIONING_P},
	[S_SET_POSITION_DATA_P][E_STOP_POSITIONING_P] = {S_DEATH_P, T_STOP_POSITIONING_P},

};


/************************************** DECLARATIONS *******************************************************/


static const char queue_name[] = "/adminpositioning"; //name of mailbox
static mqd_t id_bal;	
static pthread_t mythread;
static pthread_mutex_t myMutex=PTHREAD_MUTEX_INITIALIZER;
static adminpositioning_t * myAdminpositioning;



/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/
static void adminpositioning_mq_init();
static void adminpositioning_mq_send();
static Mq_message_t adminpositioning_mq_receive();
static void adminpositioning_mq_done();

static void *adminpositioning_run();
static void adminpositioning_waitTaskTermination();
static void adminpositioning_BLE_positioning();
static void adminpositioning_performAction(adminpositioning_transistion_action_e action);

static void adminpositioning_start();
static void adminpositioning_stop();
static void adminpositioning_perform_setPositionData();


/**********************************  PUBLIC FUNCTIONS ************************************************/
/**
 * @fn : 
 * @brief : called by cartographer
 */
extern void adminpositioning_signal_start(){
	adminpositioning_start();
}

/**
 * @fn : 
 * @brief : called by cartographer
 */
extern void adminpositioning_signal_ackPositionData(){
	fprintf(stderr, "adminpositioning_signal_ackPositionData() \n");
    adminpositioning_mq_send(E_ACK_POSITION_DATA_P);
}

/**
 * @fn : 
 * @brief : called by cartographer
 */
extern void adminpositioning_signal_stop(){
    adminpositioning_mq_send(E_STOP_POSITIONING_P);
}

//called by cartographer
extern void adminpositioning_setPositionData(Position_data_t * posData){
	*posData = myAdminpositioning->positionData;
	//memcpy(lidarData, &myMapper->lidarData, sizeof(Lidar_data_t) );
}



/**********************************  PRIVATE FUNCTIONS ************************************************/


/** 
 * @fn static void *adminpositioning_run()
 * @brief Function run executed by the thread of adminpositioning
 * @retval NULL
 */
static void * adminpositioning_run(){

	Mq_message_t adminpositioning_msg;
	adminpositioning_transistion_action_e an_action = T_NOP_P;

	while (myAdminpositioning->myAdminpositioningState != S_DEATH_P)
	{
		adminpositioning_msg = adminpositioning_mq_receive();
		if (myTransition[myAdminpositioning->myAdminpositioningState ][adminpositioning_msg.event].destinationState != S_DEATH_P)
		{
			an_action = myTransition[myAdminpositioning->myAdminpositioningState][adminpositioning_msg.event].actionToPerform;
			adminpositioning_performAction(an_action);

			myAdminpositioning->myAdminpositioningState = myTransition[myAdminpositioning->myAdminpositioningState][adminpositioning_msg.event].destinationState;
		}
	}
	return NULL;
}




static void adminpositioning_performAction(adminpositioning_transistion_action_e action)
{

    switch(action)
    {
            case T_NOP_P :
                break;
                
			case T_POSITIONING_P :
				adminpositioning_BLE_positioning();
				adminpositioning_mq_send(E_SET_POSITION_DATA_P);	
				break;			

            case T_SET_POSITION_DATA_P :
				adminpositioning_perform_setPositionData();
                break;

			case T_ACK_POSITION_DATA_P:
				adminpositioning_mq_send(E_POSITIONING_P);
				break;

            case T_STOP_POSITIONING_P :
				adminpositioning_stop();
                break;

            default :
                break;
        
    }
}


static void adminpositioning_BLE_positioning(){
    
    /* BEGIN Call python positioning script */

    /* END Call python positioning script */
	sleep(5);

}



/** @fn extern void adminpositioning_start()
 *  @brief Function dedicated to execute the adminpositioning thread
 *  @retval NULL
 */
static void adminpositioning_start(){

	fprintf(stderr, "adminpositioning_start\n");

    myAdminpositioning = NULL;
    myAdminpositioning = calloc(1, sizeof(adminpositioning_t) );
    if (myAdminpositioning == NULL)
    {
        perror("Error calloc");
    }
    myAdminpositioning->myAdminpositioningState = S_IDLE_P;

    adminpositioning_mq_init();

    int return_thread = pthread_create(&mythread, NULL, &adminpositioning_run, NULL);
	assert(return_thread == 0 && "Error Pthread_create adminPositioning\n");

	adminpositioning_mq_send(E_START_POSITIONING_P);

	fprintf(stderr, "adminpositioning_start\n");
}

/** \fn static void adminpositioning_stop()
 *  \brief Function dedicated to stop the adminpositioning thread
 *  \retval NULL
 */
static void adminpositioning_stop(){
    fprintf(stderr,"adminpositioning stop\n\n");

	adminpositioning_mq_done();

    free(myAdminpositioning);
}



static void adminpositioning_perform_setPositionData(){
	fprintf(stderr, "adminpositioning_perform_setPositionData\n");
	//** BEGIN Call adminpositioning function **
	cartographer_signal_setPositionData();
	//** END Call adminpositioning function **
	fprintf(stderr, "adminpositioning_perform_setPositionData\n");

}


/** \fn static void adminpositioning_waitTaskTermination ()
 *  \brief Function stop executed by the thread of adminpositioning
 */
static void adminpositioning_waitTaskTermination()
{
	int error_code = pthread_join(mythread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}


/*********************************** MAILBOX FUNCTION *************************************************************/


static void adminpositioning_mq_init()
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
	assert(id_bal != -1 && "Error mq_open adminpositioning\n");

}


/** @fn static void adminpositioning_mq_send(Event adminpositioning_event)
 *  @brief Function dedicated to send a special stop message to the mailbox
 *  @param Event the event to send through the mailbox
 */

static void adminpositioning_mq_send(adminpositioning_event_e adminpositioning_event)
{
	pthread_mutex_lock(&myMutex);
	//***** RACE CONDITION *****//
	Mq_message_t adminpositioning_msg;
	adminpositioning_msg.event = adminpositioning_event;
	
	mqd_t bal_send = mq_send(id_bal, (const char *)&adminpositioning_msg, sizeof(Mq_message_t), 0); //Priority 0 to 31 (highest priority first)
	assert(id_bal != -1 && "Error mq_send adminpositioning\n");
	//***** END RACE CONDITION *****//
	pthread_mutex_unlock(&myMutex);
}

/** \fn static Mq_message_t adminpositioning_mq_receive()
 *  \brief Function dedicated to receive a message from the mailbox
 *  \return Mq_message_t the message received in the mailbox
 */

static Mq_message_t adminpositioning_mq_receive()
{
	/*Reception of a message from the BAL*/

	Mq_message_t adminpositioning_msg;
	ssize_t bal_receive_msg = mq_receive(id_bal, (char *)&adminpositioning_msg, sizeof(Mq_message_t), NULL); //Null priority => mailbox use as a FIFO
	assert(bal_receive_msg != -1 && "Error mq_receive adminpositioning\n");

	return adminpositioning_msg;
}

/** \fn static void adminpositioning_mq_done ()
 *  \brief Function dedicated to destroy the mailbox
 */
static void adminpositioning_mq_done()
{
	/* close the mailbox */
	mqd_t bal_close = (mq_close(id_bal));
	assert(bal_close != -1 && "x>>> mq_close adminpositioning\n");

	/* destruct the mailbox */
	mqd_t bal_destruct = mq_unlink((const char *)queue_name);
	assert(bal_destruct != -1 && "x>>> mq_unlink adminpositioning\n");
}

