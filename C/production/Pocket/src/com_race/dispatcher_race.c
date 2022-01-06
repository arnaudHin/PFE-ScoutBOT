/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  dispatcher_pilot.c
 * @author Debus Alexy
 * @version 2.0
 * @date 07/06/2020
 * @brief Dispatcher_pilot entity source file
 *
 */

#include "dispatcher_race.h"
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "../utils/util.h"
#include "postman_race.h"
#include "race_protocol.h"
#include "proxy_robot.h"

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

#define RPMSG_BUFFER_SIZE	FLAG_SIZE + CMD_SIZE + SENSORS_SIZE + ODOMETRY_SIZE + LIDAR_DISTANCE_SIZE + SERVO_ANGLE_SIZE + FLAG_SIZE

/************************************** END DEFINE *************************************************************/






typedef struct{
	Message_with_race_t myMessageToRace;
}Message_mq_t;


static const char queue_name[] = "/dispatcher_race"; //nom BAL
static mqd_t id_bal; //ID BAL
static pthread_t id_thread; //ID thread

static Message_mq_t myMessageMq;



/**********************************  STATIC FUNCTIONS DECLARATION ************************************************/

static void * dispatcher_race_run();
static void dispatcher_race_mq_init ();
static Message_mq_t dispatcher_race_mq_receive();
static void dispatcher_race_mq_send(Message_mq_t messageMq);
static void dispatcher_race_mq_done ();
static void dispatcher_race_check_for_message();

/********************************** END OF STATIC FUNCTIONS DECLARATION *******************************************/





/*********************************** MAILBOX FUNCTION ****************************************************************/

/*
 * \brief Function dedicated to start the dispatcher_pilot (Initialize and open the mailbox + create a thread)
 */
extern void dispatcher_race_start(){

	postman_race_start();
	
	dispatcher_race_mq_init();

	if (pthread_create(&id_thread, NULL, &dispatcher_race_run, NULL)==-1){
			perror("ERROR CREATING THREAD...\n");
	}
}


/**
 *  @fn static void dispatcher_pilot_mq_init ()
 *  @brief Function dedicated to initializes the log queue
 */
static void dispatcher_race_mq_init(){
	/* INIT BAL */
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MQ_MAX_MESSAGES;
	attr.mq_msgsize = sizeof(Message_mq_t);
	attr.mq_curmsgs = 0;

	/* Destruct a pre-existing mailbox */
	mq_unlink(queue_name);
	//* Create and open the mailbox */

	id_bal = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &attr); // Creation of a new mailbox if any was create before.
	if (id_bal == -1){
		perror("Error opening BAL\n");
	}
}


static void * dispatcher_race_run(){

	Message_with_race_t msg_to_disptach;
	while (1)
	{
		dispatcher_race_check_for_message();
		msg_to_disptach = myMessageMq.myMessageToRace;

		switch (msg_to_disptach.command)
		{
		case TRY_DIR:
			proxy_robot_try_dir(&msg_to_disptach.data);
			break;
		
		default:
			break;
		}
		
	}
	return NULL;

}





/** \fn static void dispatcher_pilot_mq_send(Event dispatcher_event,Pilot_message msg)
 *  \brief Function dedicated to send a special stop message on the log queue
 *  \param Event the event to send through the mailbox
 *  \param Pilot_message the message received on the serial port to be passed through multiple threads using the mailbox
 */
static void dispatcher_race_mq_send(Message_mq_t messageMq){

	mqd_t BAL_send = mq_send(id_bal,(const char *)&messageMq,sizeof(Message_mq_t),0); //Priority 0 to 31 (highest priority first)
	if (BAL_send == -1){
		perror("Error sending message using the mailbox\n");
	}
}

/** \fn static Mb_message dispatcher_pilot_mq_receive()
 *  \brief Function dedicated to receive a message from the log queue
 *  \return Mb_message the message received in the mailbox
 */
static Message_mq_t dispatcher_race_mq_receive(){
    /*Reception of a message fron the BAL*/

    Message_mq_t dispatcher_msg;
	ssize_t BAL_receiveMsg = mq_receive(id_bal, (char*)&dispatcher_msg, sizeof(Message_mq_t), NULL); //Nul priority => Mailbox use as a FIFO
	if (BAL_receiveMsg == -1){
		perror("Error receiving message \n");
	}
	return dispatcher_msg;
}

/** \fn static void Dispatcher_pilot_mq_done ()
 *  \brief Function dedicated to destroys the log queue
 */
static void dispatcher_race_mq_done (){
	/* close the BAL */
	mqd_t bal_close = (mq_close(id_bal));
	if (bal_close == -1){
		perror("ERROR closing the bal");
	}
	/* destruct the BAL */
	if(mq_unlink((const char *)queue_name)==-1){
		perror("Error to unlink");
	}
}


static void dispatcher_race_check_for_message(){

	myMessageMq = dispatcher_race_mq_receive();

	


	

}


/********************************* END OF MAILBOX FUNCTION **************************************************************/

/*********************************** MAE PERFORMING ACTION MECANISM *****************************************************/

/*********************************** END OF MAE PERFORMING ACTION MECANISM *****************************************************/
/** \fn static void waitTaskTermination ()
 *  \brief Function dedicated to wait until the end of the current task
 */
static void wait_task_termination()
{
	if (pthread_join(id_thread,NULL)==-1){ // Halt the exécution of the thread until he achieves his execution
		perror("ERROR Joining current thread\n");
	}
}



/** \fn extern void Dispatcher_pilot_stop()
 *  \brief Function dedicated to stop the Pilot
 */
extern void dispatcher_race_stop(){
	wait_task_termination();
	dispatcher_race_mq_done();
	postman_race_stop();
}

/********************* 	MAILBOX SEND FONCTION ***************************************************************************/


extern void dispatcher_race_set_msg(){

}

/********************* 	END OF MAILBOX SEND FONCTION *********************************************************************/

