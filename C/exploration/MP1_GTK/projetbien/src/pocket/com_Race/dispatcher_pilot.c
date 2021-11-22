/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  dispatcher_pilot.c
 * @author Debus Alexy
 * @version 2.0
 * @date 07/06/2020
 * @brief Dispatcher_pilot entity source file
 *
 */

#include "../../pocket/com_Race/dispatcher_pilot.h"

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "../../util.h"
#include "../com_Race/postman_pilot.h"

#include "../../pocket/com_Race/proxy_robot.h"

/************************************** DEFINE ****************************************************************/

#define MQ_MAX_MESSAGES (10)

#define STATE_GENERATION S(S_FORGET) S(S_RUN) S(S_DEATH) S(NB_STATE)
#define S(x) x,
typedef enum {STATE_GENERATION STATE_NB} State;
#undef S
#define S(x) #x,
const char * const State_Dispatcher_Name[] = {STATE_GENERATION};
#undef STATE_GENERATION
#undef S

#define TRANSITION_ACTION_GENERATION S(T_NOP) S(T_RECEPTION) S(T_EXIT)
#define S(x) x,
typedef enum {TRANSITION_ACTION_GENERATION TRANSITION_ACTION_NB} Transition_action;
#undef S
#define S(x) #x,
const char * const Transition_action_Dispatcher_Name[] = {TRANSITION_ACTION_GENERATION};
#undef TRANSITION_ACTION_GENERATION
#undef S

#define EVENT_GENERATION S(E_POLL_MESSAGE) S(E_EXIT) S(NB_ENTREE)
#define S(x) x,
typedef enum {EVENT_GENERATION EVENT_ACTION_NB} Event;
#undef S
#define S(x) #x,
const char * const Event_Dispatcher_Name[] = {EVENT_GENERATION};
#undef EVENT_GENERATION
#undef S

/************************************** END DEFINE *************************************************************/

static const char queue_name[] = "/dispatcher_pilot_postman"; //nom BAL
static mqd_t id_bal; //ID BAL
static pthread_t id_thread; //ID thread

/* \struct Transition
 * \brief Transition structure to determine the following state to reach after a specific event occur
 */
typedef struct{
    State destination_state;
    Transition_action action;
}Transition;

/* \struct Mb_message
 * \brief Mb_message structure dedicated to pass the event between multiple threads
 */
typedef struct
{
	Event evenement;
	Pilot_message_r my_pilot_message;
} Mb_message;

/**********************************  STATIC FUNCTIONS DECLARATION ************************************************/

static void dispatcher_pilot_perform_action(Transition_action anAction,Pilot_message_r msg);
static void dispatcher_pilot_decode_msg(Pilot_message_r msg);
static void * dispatcher_pilot_run();

static void dispatcher_pilot_mq_init ();
static Mb_message dispatcher_pilot_mq_receive();
static void dispatcher_pilot_mq_send(Event dispatcher_event,Pilot_message_r msg);
static void dispatcher_pilot_mq_done ();

/********************************** END OF STATIC FUNCTIONS DECLARATION *******************************************/

/**********************************TRANSITION STRUCT***************************************************************/

static Transition mySm[NB_STATE][NB_ENTREE] =
{
		[S_RUN][E_POLL_MESSAGE] = {S_RUN,T_RECEPTION},
		[S_RUN][E_EXIT]         = {S_DEATH,T_EXIT},
};

/**********************************END OF TRANSITION STRUCT **********************************************************/

/*********************************** MAILBOX FUNCTION ****************************************************************/

/*
 * \brief Function dedicated to start the dispatcher_pilot (Initialize and open the mailbox + create a thread)
 */
extern void dispatcher_pilot_start(){
	postman_start();
	dispatcher_pilot_mq_init();

	if (pthread_create(&id_thread,NULL,(void*)&dispatcher_pilot_run,NULL)==-1){
			perror("ERROR CREATING THREAD...\n");
	}
}

/**
 *  @fn static void dispatcher_pilot_mq_init ()
 *  @brief Function dedicated to initializes the log queue
 */
static void dispatcher_pilot_mq_init(){
	/* INIT BAL */
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MQ_MAX_MESSAGES;
	attr.mq_msgsize = sizeof(Mb_message);
	attr.mq_curmsgs = 0;

	/* Destruct a pre-existing mailbox */
	mq_unlink(queue_name);
	//* Create and open the mailbox */

	id_bal = mq_open(queue_name,O_CREAT | O_RDWR,0666,&attr); // Creation of a new mailbox if any was create before.
	if (id_bal == -1){
		perror("Error opening BAL\n");
	}
	TRACE("Creation of BAL: %s\nID:%d\n",queue_name,id_bal);
}

/** \fn static void dispatcher_pilot_mq_send(Event dispatcher_event,Pilot_message msg)
 *  \brief Function dedicated to send a special stop message on the log queue
 *  \param Event the event to send through the mailbox
 *  \param Pilot_message the message received on the serial port to be passed through multiple threads using the mailbox
 */
static void dispatcher_pilot_mq_send(Event dispatcher_event,Pilot_message_r msg){
	Mb_message dispatcher_msg;
	dispatcher_msg.evenement = dispatcher_event;
	dispatcher_msg.my_pilot_message = msg;

	mqd_t BAL_send = mq_send(id_bal,(const char *)&dispatcher_msg,sizeof(Mb_message),0); //Priority 0 to 31 (highest priority first)
	if (BAL_send == -1){
		perror("Error sending message using the mailbox\n");
	}
}

/** \fn static Mb_message dispatcher_pilot_mq_receive()
 *  \brief Function dedicated to receive a message from the log queue
 *  \return Mb_message the message received in the mailbox
 */
static Mb_message dispatcher_pilot_mq_receive(){
    /*Reception of a message fron the BAL*/

    Mb_message dispatcher_msg;
	ssize_t BAL_receiveMsg = mq_receive(id_bal,(char*)&dispatcher_msg,sizeof(Mb_message),NULL); //Nul priority => Mailbox use as a FIFO
	if (BAL_receiveMsg == -1){
		perror("Error receiving message \n");
	}
	return dispatcher_msg;
}

/** \fn static void Dispatcher_pilot_mq_done ()
 *  \brief Function dedicated to destroys the log queue
 */
static void dispatcher_pilot_mq_done (){
	/* close the BAL */
	mqd_t bal_close = (mq_close(id_bal));
	if (bal_close == -1){
		TRACE("ERROR closing the bal\n");
	}
	/* destruct the BAL */
	if(mq_unlink((const char *)queue_name)==-1){
		perror("Error to unlink");
	}
}

/********************************* END OF MAILBOX FUNCTION **************************************************************/

/*********************************** MAE PERFORMING ACTION MECANISM *****************************************************/

/** \fn static void * dispatcher_pilot_run()
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *  \retval NULL
 */
static void * dispatcher_pilot_run(){
	Mb_message dispatcher_msg;
	Transition_action an_action;
	State my_state  = S_RUN;

	//Display_send_txt_state(NO_INITIALISED);

	while (my_state != S_DEATH){
		dispatcher_msg = dispatcher_pilot_mq_receive();
		TRACE("\n\nState:%s\nEvent:%s\n",State_Dispatcher_Name[my_state],Event_Dispatcher_Name[dispatcher_msg.evenement]);
		if (mySm[my_state][dispatcher_msg.evenement].destination_state!= S_FORGET){
			an_action = mySm[my_state][dispatcher_msg.evenement].action;
			dispatcher_pilot_perform_action(an_action,dispatcher_msg.my_pilot_message);

			my_state = mySm[my_state][dispatcher_msg.evenement].destination_state;
		}
	}
	return NULL;
}

/** \fn static void dispatcher_pilot_perform_action(Transition_action anAction,Pilot_message msg)
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *  \param Transition_action an Action to perform, compute from the event receive in the mailbox
 *  \param Pilot_message a structure with data for pilot or robot_state entity (Information message or sensor data)
 */
static void dispatcher_pilot_perform_action(Transition_action anAction,Pilot_message_r msg){
	switch(anAction)
	{
		case T_NOP:
			TRACE("NOTHNG TO PERFORM HERE\n");
			break;

		case T_RECEPTION:
			dispatcher_pilot_decode_msg(msg);
			TRACE("State: %s\n",Transition_action_Dispatcher_Name[anAction]);
			break;

		case T_EXIT:
			TRACE("STOP THE DISPATCHER\n");
			break;

		default:
			TRACE("\nError >>> Unknown action performed in Dispatcher MAE\n");
			break;
	}
}

/** \fn static void dispatcher_pilot_decode_msg(Pilot_message msg)
 *  \brief Function dedicated to look after the message type in order to execute a specific action
 *  \brief (update the odometry, update sensors state or indicate Race successfully initialized all peripherals)
 *  \param Pilot_message a structure with data for pilot or robot_state entity (Information message or sensor data)
 */
static void dispatcher_pilot_decode_msg(Pilot_message_r msg){
        switch(msg.order_r){
        case ASK_FOR_DEFAULT_ORDER_R:
			TRACE("DEFAULT ORDER MSG");
			break;
        case NOTIFY_END_INIT:
        	pilot_signal_end_init();
        	break;

        case SET_ODOMETRY:
        	robot_state_set_odometry(msg.my_current_position);
        	break;

        case SET_SENSORS:
        	robot_state_set_sensors_state(msg.my_sensors_state);
			break;

        case SET_LIDAR_SENSORS:
        	robot_state_set_lidar_sensors_state(msg.my_lidar_state);
        	break;

        default:
			TRACE("DEFAULT MSG\n");
			break;
        }
}

/*********************************** END OF MAE PERFORMING ACTION MECANISM *****************************************************/

/** \fn static void waitTaskTermination ()
 *  \brief Function dedicated to wait until the end of the current task
 */
static void wait_task_termination()
{
	TRACE("thread number %ld\n",id_thread);
	if (pthread_join(id_thread,NULL)==-1){ // Halt the exécution of the thread until he achieves his execution
		perror("ERROR Joining current thread\n");
	}
}

/** \fn extern void Dispatcher_pilot_stop()
 *  \brief Function dedicated to stop the Pilot
 */
extern void dispatcher_pilot_stop(){
	Event dispatcher_event = E_EXIT;

	Pilot_message_r msg = {.order_r = ASK_FOR_DEFAULT_ORDER_R};
	dispatcher_pilot_mq_send(dispatcher_event,msg);
	wait_task_termination();
	dispatcher_pilot_mq_done();
	postman_stop();
}

/********************* 	MAILBOX SEND FONCTION ***************************************************************************/

/*
 * \brief Function dedicated to send through the mailbox a message received through the serial port
 * \param Pilot_message the structure with the content of the message to send through the mailbox
 */
extern void dispatcher_pilot_set_msg(Pilot_message_r msg){
	dispatcher_pilot_mq_send(E_POLL_MESSAGE,msg);
}

/********************* 	END OF MAILBOX SEND FONCTION *********************************************************************/

