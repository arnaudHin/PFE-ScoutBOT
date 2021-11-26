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
#include "../util.h"
#include "com_race/postman_race.h"
#include "com_race/proxy_robot.h"
#include "robot_state.h"

/************************************** DEFINE ****************************************************************/

#define MAX_AUTHORIZED_DIRECTION 6
#define MQ_MAX_MESSAGES (10)
#define PILOT_DEFAULT_DIRECTION 5 // BREAK DIRECTION
#define MAX_DIR_SIZE 12
#define MAX_PATH_LEN (30)

#define DIRECTION_STR(dir) \
	(FORWARD == dir ? "FORWARD" : (BACKWARD == dir ? "BACKWARD" : (LEFT == dir ? "LEFT" : (RIGHT == dir ? "RIGHT" : (BREAK == dir ? "BREAK" : (DEFAULT == dir ? "DEFAULT" : "unknown"))))))

#define STATE_GENERATION S(S_FORGET) \
S(S_NOT_INITIALISED) S(S_NOT_CONNECTED) S(S_NORMAL) S(S_DEATH) S(NB_STATE)
#define S(x) x,
typedef enum
{
	STATE_GENERATION STATE_NB
} State;
#undef S
#define S(x) #x,
const char *const state_pilot_name[] = {STATE_GENERATION};
#undef STATE_GENERATION
#undef S

#define TRANSITION_ACTION_GENERATION S(T_NOP_PILOT) \
S(T_END_INIT) S(T_START_SCAN) S(T_STOP_SCAN) S(T_START_AUTO_SCAN) S(T_STOP_AUTO_SCAN) S(T_UPDATE_DIR) S(T_SIGNAL_FORBIDDEN_PATH) S(T_TRY_PATH) S(T_SIGNAL_END_DISPLACEMENT) S(T_EMERGENCY_STOP) S(T_RESET_POSITION) S(T_EXIT)
#define S(x) x,
typedef enum
{
	TRANSITION_ACTION_GENERATION TRANSITION_ACTION_NB
} Transition_action;
#undef S
#define S(x) #x,
const char *const transition_action_pilot_name[] = {TRANSITION_ACTION_GENERATION};
#undef TRANSITION_ACTION_GENERATION
#undef S

#define EVENT_GENERATION S(E_END_INIT) \
S(E_SUCCESSFULL_CONNECTION) S(E_FAIL_CONNECTION) S(E_UPDATE_DIR) S(E_SIGNAL_END_DISPLACEMENT) S(E_RESET_POSITION) S(E_SIGNAL_FORBIDDEN_PATH) S(E_ASK_DESTINATION) S(E_EMERGENCY_STOP) S(E_EXIT) S(NB_ENTREE)
#define S(x) x,
typedef enum
{
	EVENT_GENERATION EVENT_ACTION_NB
} Event;
#undef S
#define S(x) #x,
const char *const event_pilot_name[] = {EVENT_GENERATION};
#undef EVENT_GENERATION
#undef S

/************************************** END DEFINE *************************************************************/

static const char queue_name[] = "/pilot_postman"; //name of mailbox
static mqd_t id_bal;							   //ID mailbox
static pthread_t id_thread;						   //ID thread
static Direction_e direction = PILOT_DEFAULT_DIRECTION;
//static bool authorized_direction[MAX_AUTHORIZED_DIRECTION] = {[0 ... MAX_AUTHORIZED_DIRECTION - 1] = false};
static bool authorized_direction[MAX_AUTHORIZED_DIRECTION] = {[0 ... MAX_AUTHORIZED_DIRECTION - 1] = true};
//static Odometry astar_path[MAX_PATH_LEN] = {0};

typedef struct char_direction
{
	char direction[MAX_DIR_SIZE];
} char_direction;

/* \struct Transition
 *
 * \brief Transition structure to determine the following state to reach after a specific event occur
 *
 */
typedef struct
{
	State destination_state;
	Transition_action action;
} Transition;

/* \struct Mb_message
 * \brief Mb_message structure dedicated to pass the event between multiples threads
 *
 */
typedef struct
{
	Event evenement;
	//Coord coord;
} Mb_message;

/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

static void pilot_perform_action(Transition_action an_action);
static void *pilot_run();
static void pilot_mq_init();
static Mb_message pilot_mq_receive();
static void pilot_mq_send(Event pilot_event);
static void pilot_mq_done();
static void pilot_check_authorized(Direction_e dir);
static void pilot_update_authorized(void);


/********************************** END OF STATIC FUNCTIONS DECLARATIONS *******************************************/

/***************************************** TRANSITION STRUCT *******************************************************/

static Transition mySm[NB_STATE][NB_ENTREE] =
	{
		[S_NOT_INITIALISED][E_END_INIT] = {S_NOT_CONNECTED, T_END_INIT},
		[S_NOT_INITIALISED][E_EXIT] = {S_DEATH, T_EXIT},

		[S_NOT_CONNECTED][E_SUCCESSFULL_CONNECTION] = {S_NORMAL, T_START_SCAN},

		[S_NORMAL][E_FAIL_CONNECTION] = {S_NOT_CONNECTED, T_STOP_SCAN},
		[S_NORMAL][E_UPDATE_DIR] = {S_NORMAL, T_UPDATE_DIR},

		[S_NORMAL][E_FAIL_CONNECTION] = {S_NOT_CONNECTED, T_STOP_SCAN},
		[S_NORMAL][E_UPDATE_DIR] = {S_NORMAL, T_UPDATE_DIR},

		[S_NORMAL][E_SIGNAL_FORBIDDEN_PATH] = {S_NORMAL, T_SIGNAL_FORBIDDEN_PATH},
		[S_NORMAL][E_ASK_DESTINATION] = {S_NORMAL, T_TRY_PATH},
		[S_NORMAL][E_SIGNAL_END_DISPLACEMENT] = {S_NORMAL, T_SIGNAL_END_DISPLACEMENT},

		[S_NORMAL][E_RESET_POSITION] = {S_NORMAL, T_RESET_POSITION},
		[S_NORMAL][E_EMERGENCY_STOP] = {S_NORMAL, T_EMERGENCY_STOP},

		[S_NORMAL][E_EXIT] = {S_DEATH, T_EXIT},
		[S_NOT_CONNECTED][E_EXIT] = {S_DEATH, T_EXIT},
};

/********************************** TEST TOOLS ********************************************************************/

static State a_state_test;
static Transition_action an_action_test;
#ifdef NTEST

static void set_state(State state)
{
	a_state_test = state;
}

static void set_action(Transition_action action)
{
	an_action_test = action;
}

#endif

/********************************** END TEST TOOLS ****************************************************************/

/********************************** END OF TRANSITION STRUCT ******************************************************/

/** \fn static void pilot_start ()
 *
 *  \brief Function dedicated to start the Pilot
 */
extern void pilot_start()
{
	pilot_mq_init();

	pilot_signal_connection_success();
	int return_thread = pthread_create(&id_thread, NULL, (void *)&pilot_run, NULL);
	TRACE("Pilot is starting");
	assert(return_thread == 0 && "Error Pthread_create pilot\n");
}

/*********************************** MAILBOX FUNCTION *************************************************************/

/** \fn static void pilot_mq_init ()
 *
 *  \brief Function dedicated to initialize the log queue
 */
static void pilot_mq_init()
{
	/* INIT BAL */
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MQ_MAX_MESSAGES;
	attr.mq_msgsize = sizeof(Mb_message);
	attr.mq_curmsgs = 0;

	/* Destruct a pre-existing mailbox */
	mq_unlink(queue_name);

	/* Create and open the mailbox */

	id_bal = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &attr); // Creation of a new mailbox if any was created before.
	assert(id_bal != -1 && "Error mq_open pilot\n");

	TRACE("Creation de la BAL: %s\nID:%d\n\n", queue_name, id_bal);
}

/** \fn static void pilot_mq_send(Event pilot_event)
 *
 *  \brief Function dedicated to send a special stop message to the mailbox
 *
 *  \param Event the event to send through the mailbox
 */

static void pilot_mq_send(Event pilot_event)
{
	Mb_message pilot_msg;
	pilot_msg.evenement = pilot_event;
	
	TRACE("Evenement de pilot : %d", pilot_msg);
	mqd_t bal_send = mq_send(id_bal, (const char *)&pilot_msg, sizeof(Mb_message), 0); //Priority 0 to 31 (highest priority first)
	assert(id_bal != -1 && "Error mq_send pilot\n");
}

/** \fn static Mb_message pilot_mq_receive()
 *
 *  \brief Function dedicated to receive a message from the mailbox
 *
 *  \return Mb_message the message received in the mailbox
 */

static Mb_message pilot_mq_receive()
{
	/*Reception of a message from the BAL*/

	Mb_message pilot_msg;
	ssize_t bal_receive_msg = mq_receive(id_bal, (char *)&pilot_msg, sizeof(Mb_message), NULL); //Null priority => mailbox use as a FIFO
	assert(bal_receive_msg != -1 && "Error mq_receive pilot\n");

	return pilot_msg;
}

/** \fn static void pilot_mq_done ()
 *
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
 *
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *
 *  \retval NULL
 */
static void *pilot_run()
{
	Mb_message pilot_msg;
	Transition_action an_action = T_NOP_PILOT;

	State my_state = S_NOT_CONNECTED;

	// display_send_txt_state("S_NOT_INITIALISED");

	while (my_state != S_DEATH)
	{
#ifdef NTEST
		set_state(my_state);
		set_action(an_action);
#endif

		pilot_msg = pilot_mq_receive();
		TRACE("\n\nState:%s\nEvent:%s\n", state_pilot_name[my_state], event_pilot_name[pilot_msg.evenement]);
		if (mySm[my_state][pilot_msg.evenement].destination_state != S_FORGET)
		{
			an_action = mySm[my_state][pilot_msg.evenement].action;
			pilot_perform_action(an_action);

			my_state = mySm[my_state][pilot_msg.evenement].destination_state;
		}
	}
	return NULL;
}

/** \fn static void pilot_perform_action(Transition_action an_action)
 *
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *
 *  \param  Transition_action an Action to perform, compute from the event receive in the mailbox
 */
static void pilot_perform_action(Transition_action an_action)
{

	switch (an_action)
	{
	case T_NOP_PILOT:
		TRACE("NOTHING TO PERFORM HERE\n");
		break;

	case T_END_INIT:
		TRACE("END OF PERIPHERAL INITIALISATION\n");
		// display_send_txt_state("MANUAL");
		// display_send_txt_connection("WAITING_FOR_CONNECTION");
		break;

	case T_UPDATE_DIR:
		TRACE("UPDATE & CHECK DIRECTION : %s\n", DIRECTION_STR(direction));
		//pilot_check_authorized(direction);
		DATA_to_race_t data;
		data.direction = direction;
		proxy_robot_try_dir( &data );

		break;


	case T_EMERGENCY_STOP:
		TRACE("T_EMERGENCY_STOP\n");
		//robot_urgent_break();
		break;

	case T_EXIT:
		TRACE("STOP THE PILOT\n");
		Event pilot_event = E_EXIT;
		pilot_mq_send(pilot_event);
		break;

	default:
		TRACE("\nError >>> Unknown action performed in pilot MAE\n"); //we shouldn't enter the default case
		break;
	}
}

/*********************************** END OF MAE PERFORMING ACTION MECANISM *****************************************************/

/** \fn static void pilot_wait_task_termination ()
 *
 *  \brief Function dedicated to wait until the end of the current task
 */
static void pilot_wait_task_termination()
{
	TRACE("thread number %ld\n\n", id_thread);
	int error_code = pthread_join(id_thread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}

/** \fn extern void pilot_stop()
 *
 *  \brief Function dedicated to stop the pilot
 */
extern void pilot_stop()
{
	TRACE("pilot stop\n\n");

	pilot_wait_task_termination();
	pilot_mq_done();
}

/********************* 	MAILBOX SEND FONCTION ***************************************************************************/



/** \fn extern void pilot_set_direction(Direction dir)
 *
 *  \brief Function dedicated to send an event in the mailbox to set the direction
 *
 *  \param  Direction the direction to send to the robot entity
 */
extern void pilot_set_direction(Direction_e dir)
{
	direction = dir;
	TRACE("Pilot : SET DIRECTION : %d\n", direction);
	pilot_mq_send(E_UPDATE_DIR);
}




/** \fn extern void pilot_signal_end_init()
 *
 *  \brief Function dedicated to send an event in the mailbox to signal the end of peripheral initialization of the robot
 */
extern void pilot_signal_end_init()
{
	pilot_mq_send(E_END_INIT);
}

/** \fn extern void pilot_signal_connection_success()
 *
 *  \brief Function dedicated to send an event in the mailbox to signal the connection between the android app and pocket
 */
extern void pilot_signal_connection_success()
{
	pilot_mq_send(E_SUCCESSFULL_CONNECTION);
}

/** \fn extern void pilot_signal_connection_failed()
 *
 *  \brief Function dedicated to send an event in the mailbox to signal the lost of connection between the android app and pocket
 */
extern void pilot_signal_connection_failed()
{
	pilot_mq_send(E_FAIL_CONNECTION);
}

/** \fn extern void pilot_signal_emergency_stop()
 *
 *  \brief Function dedicated to send an event in the mailbox to signal that the user asked for an emergency stop of the robot
 */
extern void pilot_signal_emergency_stop()
{
	pilot_mq_send(E_EMERGENCY_STOP);
}

/** \fn extern void pilot_signal_robot_arrived()
 *
 *  \brief Function dedicated to send an event in the mailbox to signal that the robot is arrived to the desire position
 */
extern void pilot_signal_robot_arrived()
{
	pilot_mq_send(E_SIGNAL_END_DISPLACEMENT);
}



/** \fn extern void pilot_signal_restart_odometry()
 *
 *  \brief Function dedicated to send an event in the mailbox to ask for a reset of the current position + angle of the robot
 */
extern void pilot_signal_restart_odometry()
{
	pilot_mq_send(E_RESET_POSITION);
}

/** \fn extern void pilot_ask_destination(Coord coord_final)
 *
 *  \brief Function dedicated to send an event in the mailbox to ask the robot to go to the desire position (using A* algorithm)
 *
 *  \param  Coord the desire position which has been choosen by the user on the android app
 */





/********************* 	END OF MAILBOX SEND FONCTION *********************************************************************/

/** \fn static void pilot_check_authorized(Direction dir)
 *
 *  \brief Function dedicated to send a direction to the robot only if the direction is approved
 *
 *  \param  Direction the direction to send using the mailbox
 */
static void pilot_check_authorized(Direction_e dir)
{
	if (authorized_direction[dir] == true)
	{
		TRACE("Authorized direction\n");
		// display_send_txt_state("AUTO");
		//robot_try_mvmt(dir);
	}
	else
	{
		TRACE("Impossible direction\n");
		//display_send_txt_msg("DIRECTION_UNAUTHORIZED");
	}
}

/** \fn static void pilot_update_authorized()
 *
 *  \brief Function dedicated to update the list of approved direction thanks to sensor data
 */
static void pilot_update_authorized()
{

	Sensors_state *update_sensors_state = robot_state_get_sensors_state();
	if ((update_sensors_state->bumper_state_bl) | (update_sensors_state->bumper_state_br))
	{
		authorized_direction[BACKWARD] = false;

		authorized_direction[LEFT] = false;
		authorized_direction[RIGHT] = false;
	}
	else if ((update_sensors_state->bumper_state_fl) | (update_sensors_state->bumper_state_fr))
	{
		authorized_direction[FORWARD] = false;

		authorized_direction[LEFT] = false;
		authorized_direction[RIGHT] = false;
	}
	else
	{
		authorized_direction[LEFT] = true;
		authorized_direction[RIGHT] = true;

		authorized_direction[FORWARD] = true;
		authorized_direction[BACKWARD] = true;
		authorized_direction[BREAK] = true;
	}
}
