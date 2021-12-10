/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  pilot.c
 * @author Debus Alexy and Montet Julien
 * @version 2.0
 * @date 13/06/2020
 * @brief Pilot entity source file
 *
 * The purpose of the pilot is to control the robot thanks to directions send from an android app.
 * It must also gather data send from the robot to ensure the direction asked by the android app is allowed.
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
#include "pilot.h"
#include "../utils/util.h"
#include "../com_race/proxy_robot.h"
#include "../com_race/race_protocol.h"

#include "robot_state.h"

/************************************** DEFINE ****************************************************************/

#define MAX_AUTHORIZED_DIRECTION 6
#define MQ_MAX_MESSAGES (10)
#define PILOT_DEFAULT_DIRECTION 5 // BREAK DIRECTION
#define MAX_DIR_SIZE 12
#define MAX_PATH_LEN (30)

#define DIRECTION_STR(dir) \
	(FORWARD == dir ? "FORWARD" : (BACKWARD == dir ? "BACKWARD" : (LEFT == dir ? "LEFT" : (RIGHT == dir ? "RIGHT" : (BREAK == dir ? "BREAK" : (DEFAULT == dir ? "DEFAULT" : "unknown"))))))



typedef enum{

	S_IDLE_P=0,
	S_RUNNING_P,
	S_DEATH_P,
	NB_STATE_P
}Pilot_state_e;

typedef enum{
	T_NOP_P = 0,
	T_START_PILOT_P,
	T_UPDATE_DIR_P,
	T_EMERGENCY_STOP_P,
	T_EXIT_P,	
	T_NB_TRANS_P
}Pilot_transistion_action_e;


typedef enum{
	E_START_P=0,
	E_CONNECTION_OK_P,
	E_FAIL_CONNECTION_P,
	E_UPDATE_DIR_P,
	E_EXIT_P,
	E_NB_EVENT_P
}Pilot_event_e;


/************************************** END DEFINE *************************************************************/

static const char queue_name[] = "/pilot_postman"; //name of mailbox
static mqd_t id_bal;							   //ID mailbox
static pthread_t id_thread;						   //ID thread
static bool authorized_direction[MAX_AUTHORIZED_DIRECTION] = {[0 ... MAX_AUTHORIZED_DIRECTION - 1] = true};

// typedef struct char_direction
// {
// 	char direction[MAX_DIR_SIZE];
// } char_direction;

/* \struct Transition
 *
 * \brief Transition structure to determine the following state to reach after a specific event occur
 */
// typedef struct
// {
// 	State destination_state;
// 	Transition_action action;
// } Transition;

typedef struct{
	Pilot_state_e destinationState;
	Pilot_transistion_action_e actionToPerform;
} Transition_t;



/* \struct Mq_message_t
 * \brief Mq_message_t structure dedicated to pass the event between multiples threads
 */
typedef struct
{
	Pilot_event_e event;
} Mq_message_t;

typedef struct{
	Pilot_state_e myPiloteState;
	Direction_e myDirection;
}Pilot_t;

/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

static void pilot_perform_action(Pilot_transistion_action_e an_action);
static void *pilot_run();
static void pilot_mq_init();
static Mq_message_t pilot_mq_receive();
static void pilot_mq_send(Pilot_event_e pilot_event);
static void pilot_mq_done();
static void pilot_check_authorized(Direction_e dir);
static void pilot_update_authorized(void);


/********************************** END OF STATIC FUNCTIONS DECLARATIONS *******************************************/

/***************************************** TRANSITION STRUCT *******************************************************/

static Transition_t myTransition[NB_STATE_P][E_NB_EVENT_P] = {
	[S_IDLE_P][E_CONNECTION_OK_P] = {S_RUNNING_P, T_START_PILOT_P},
	[S_IDLE_P][E_FAIL_CONNECTION_P] = {S_IDLE_P, T_NOP_P},
	
	[S_RUNNING_P][E_UPDATE_DIR_P] = {S_RUNNING_P, T_UPDATE_DIR_P},
	[S_RUNNING_P][E_FAIL_CONNECTION_P] = {S_IDLE_P, T_EMERGENCY_STOP_P},

	[S_RUNNING_P][E_EXIT_P] = {S_DEATH_P, T_EXIT_P},
	[S_IDLE_P][E_EXIT_P] = {S_DEATH_P, T_EXIT_P},
};



static Pilot_t * mypilot;







/** \fn static void pilot_start ()
 *  \brief Function dedicated to start the Pilot
 */
extern void pilot_start()
{
	pilot_mq_init();

	mypilot == NULL;
	mypilot = calloc(1, sizeof(Pilot_t));

	mypilot->myDirection = BREAK;
	mypilot->myPiloteState = S_IDLE_P;

	int return_thread = pthread_create(&id_thread, NULL, (void *)&pilot_run, NULL);
	TRACE("Pilot is starting");
	assert(return_thread == 0 && "Error Pthread_create pilot\n");
}

/*********************************** MAILBOX FUNCTION *************************************************************/

/** \fn static void pilot_mq_init ()
 *  \brief Function dedicated to initialize the log queue
 */
static void pilot_mq_init()
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
	assert(id_bal != -1 && "Error mq_open pilot\n");

	TRACE("Creation de la BAL: %s\nID:%d\n\n", queue_name, id_bal);
}


/** \fn static void pilot_mq_send(Event pilot_event)
 *  \brief Function dedicated to send a special stop message to the mailbox
 *  \param Event the event to send through the mailbox
 */

static void pilot_mq_send(Pilot_event_e pilot_event)
{
	Mq_message_t pilot_msg;
	pilot_msg.event = pilot_event;
	
	TRACE("Evenement de pilot : %d", pilot_msg);
	mqd_t bal_send = mq_send(id_bal, (const char *)&pilot_msg, sizeof(Mq_message_t), 0); //Priority 0 to 31 (highest priority first)
	assert(id_bal != -1 && "Error mq_send pilot\n");
}

/** \fn static Mq_message_t pilot_mq_receive()
 *  \brief Function dedicated to receive a message from the mailbox
 *  \return Mq_message_t the message received in the mailbox
 */

static Mq_message_t pilot_mq_receive()
{
	/*Reception of a message from the BAL*/

	Mq_message_t pilot_msg;
	ssize_t bal_receive_msg = mq_receive(id_bal, (char *)&pilot_msg, sizeof(Mq_message_t), NULL); //Null priority => mailbox use as a FIFO
	assert(bal_receive_msg != -1 && "Error mq_receive pilot\n");

	return pilot_msg;
}

/** \fn static void pilot_mq_done ()
 *  \brief Function dedicated to destroy the mailbox
 */
static void pilot_mq_done()
{
	/* close the mailbox */
	mqd_t bal_close = (mq_close(id_bal));
	assert(bal_close != -1 && "x>>> mq_close pilot\n");

	/* destruct the mailbox */
	mqd_t bal_destruct = mq_unlink((const char *)queue_name);
	assert(bal_destruct != -1 && "x>>> mq_unlink pilot\n");
}

/********************************* END OF MAILBOX FUNCTION ********************************************************/

/***************************** MAE PERFORMING ACTION MECANISM *****************************************************/

/** \fn static void *pilot_run()
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *  \retval NULL
 */
static void *pilot_run()
{
	Mq_message_t pilot_msg;
	Pilot_transistion_action_e an_action = T_NOP_P;

	while (mypilot->myPiloteState != S_DEATH_P)
	{
		pilot_msg = pilot_mq_receive();
		//TRACE("\n\nState:%s\nEvent:%s\n", state_pilot_name[my_state], event_pilot_name[pilot_msg.evenement]);
		if (myTransition[mypilot->myPiloteState][pilot_msg.event].destinationState != S_DEATH_P)
		{
			an_action = myTransition[mypilot->myPiloteState][pilot_msg.event].actionToPerform;
			pilot_perform_action(an_action);

			mypilot->myPiloteState = myTransition[mypilot->myPiloteState][pilot_msg.event].destinationState;
		}
	}
	return NULL;
}

/** \fn static void pilot_perform_action(Transition_action an_action)
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *  \param  Transition_action an Action to perform, compute from the event receive in the mailbox
 */
static void pilot_perform_action(Pilot_transistion_action_e an_action)
{
	DATA_to_race_t data;

	switch (an_action)
	{
	case T_NOP_P:
		TRACE("NOTHING TO PERFORM HERE\n");
		break;

	case T_EXIT_P:
		TRACE("END OF PERIPHERAL INITIALISATION\n");
		
		break;

	case T_UPDATE_DIR_P:
		printf("\nPilot UPDATE DIR\n");
		data.direction = mypilot->myDirection;
		proxy_robot_try_dir( &data );
		break;

	case T_EMERGENCY_STOP_P:
		TRACE("T_EMERGENCY_STOP\n");
		//robot_urgent_break();
		break;

	case T_START_PILOT_P:
		TRACE("STOP THE PILOT\n");
		
		break;

	default:
		TRACE("\nError >>> Unknown action performed in pilot MAE\n"); //we shouldn't enter the default case
		break;
	}
}

/*********************************** END OF MAE PERFORMING ACTION MECANISM *****************************************************/

/** \fn static void pilot_wait_task_termination ()
 *  \brief Function dedicated to wait until the end of the current task
 */
static void pilot_wait_task_termination()
{
	TRACE("thread number %ld\n\n", id_thread);
	int error_code = pthread_join(id_thread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}

/** \fn extern void pilot_stop()
 *  \brief Function dedicated to stop the pilot
 */
extern void pilot_stop()
{
	TRACE("pilot stop\n\n");

	pilot_wait_task_termination();
	pilot_mq_done();

	free(mypilot);
}




/********************* 	MAILBOX SEND FONCTION ***************************************************************************/



/** @fn extern void pilot_set_direction(Direction dir)
 *  @brief Function dedicated to send an event in the mailbox to set the direction
 *  @param  Direction the direction to send to the robot entity
 */
extern void pilot_set_direction(Direction_e dir)
{
	mypilot->myDirection = dir;
	TRACE("Pilot : SET DIRECTION : %d\n", dir);
	pilot_mq_send(E_UPDATE_DIR_P);
}


/** \fn extern void pilot_signal_end_init()
 *  \brief Function dedicated to send an event in the mailbox to signal the end of peripheral initialization of the robot
 */
extern void pilot_signal_exit()
{
	pilot_mq_send(E_EXIT_P);
}

/** \fn extern void pilot_signal_connection_success()
 *  \brief Function dedicated to send an event in the mailbox to signal the connection between the android app and pocket
 */
extern void pilot_signal_connection_success()
{
	pilot_mq_send(E_CONNECTION_OK_P);
}

/** \fn extern void pilot_signal_connection_failed()
 *  \brief Function dedicated to send an event in the mailbox to signal the lost of connection between the android app and pocket
 */
extern void pilot_signal_connection_failed()
{
	pilot_mq_send(E_FAIL_CONNECTION_P);
}





/********************* 	END OF MAILBOX SEND FONCTION *********************************************************************/

