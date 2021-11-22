/**
 *  @file : robot.c
 *
 *
 *  Created on: Nov 19, 2021
 *      Author: Arnaud
 */



#include <robot/robot.h>
#include "utils.h"
#include "stdlib.h"
#include "lib/MOTORS.h"
#include "lib/TIMERS.h"


/*********************************************************************************/
/**									Private structures							**/
/*********************************************************************************/

typedef enum{
	S_IDLE=0,
	S_RUNNING,
	S_STAND_BY,
	S_DEATH,
	NB_STATE
}Robot_state_e;


typedef enum{
	T_START_ROBOT=0,
	T_TRY_DIR,
	T_QUIT
}Robot_transition_action_e;


typedef struct{

	Robot_state_e destinationState;
    Robot_transition_action_e actionToPerform;

}Robot_tab_transition_action_t;

static Robot_tab_transition_action_t myTransistion[NB_STATE][NB_ENTREE] ={
	 [S_IDLE][E_START_ROBOT]   	= {S_RUNNING,T_START_ROBOT},
	 [S_RUNNING][E_TRY_DIR]    	= {S_RUNNING,T_TRY_DIR},

	 [S_RUNNING][E_DESTROY]		= {S_DEATH,T_QUIT},
	 [S_IDLE][E_DESTROY]       	= {S_DEATH,T_QUIT}
};



/**
 * @struct Robot_t
 * @brief
 *
 */
typedef struct{
	Robot_state_e myState;
	Robot_direction_e myDirection;
}Robot_t;



/*********************************************************************************/
/**									Private variables							**/
/*********************************************************************************/

static Robot_t * myRobot;
static Robot_mq_t * myMessageQueue;

/*********************************************************************************/
/**									Private prototypes							**/
/*********************************************************************************/


/**
 * @fn void robot_perform_action(Robot_transition_action_e, Robot_mq_t)
 * @brief
 * @param action
 * @param message
 */
static void robot_perform_action(Robot_transition_action_e action, Robot_mq_t message);

/**
 * @fn void robot_set_direction(Robot_direction_e)
 * @brief
 * @param dir :
 */
static void robot_set_direction(Robot_direction_e dir);


/**
 * @fn void robot_set_state(Robot_state_e)
 * @brief
 * @param dir :
 */
static void robot_set_state(Robot_state_e state);

/**
 * @fn void robot_try_direction(Robot_direction_e)
 * @brief
 * @param direction
 */
static void robot_try_direction(Robot_direction_e direction);



/*********************************************************************************/
/**									Public functions							**/
/*********************************************************************************/

extern ScoutBOT_Status_e robot_init(){

	myRobot = NULL;
	myRobot = calloc(1, sizeof(Robot_t) );
	if(myRobot == NULL){
		return ERROR_BOT;
	}

	myMessageQueue = NULL;
	myMessageQueue = calloc(1, sizeof(Robot_mq_t) );
	if(myMessageQueue == NULL){
		return ERROR_BOT;
	}

	myRobot->myDirection = STOP;
	myRobot->myState = S_IDLE;

	TIMER4_STOP();

	return OK_BOT;
}


extern ScoutBOT_Status_e robot_free(){
	free(myRobot);
	return OK_BOT;
}

extern void robot_run(Robot_mq_t message){

	Robot_transition_action_e action;
	Robot_event_e event = message.event;

	if(myTransistion[myRobot->myState][event].destinationState != S_DEATH){
		action = myTransistion[myRobot->myState][event].actionToPerform;
		robot_perform_action(action, message);
		myRobot->myState = myTransistion[myRobot->myState][event].destinationState;
	}
}



/*********************************************************************************/
/**									Private functions							**/
/*********************************************************************************/


static void robot_perform_action(Robot_transition_action_e action, Robot_mq_t message){


	switch (action) {
		case T_START_ROBOT:
			TIMER4_RUN();
			robot_try_direction(STOP);
			robot_set_state(S_RUNNING);

			break;

		case T_TRY_DIR:
			robot_set_direction(message.directionToPerform);
			robot_try_direction(myRobot->myDirection);

			break;

		case T_QUIT:
			robot_set_direction(STOP);
			robot_try_direction(myRobot->myDirection);
			TIMER4_STOP();

			break;


		default:
			break;
	}

}


static void robot_set_direction(Robot_direction_e dir){
	myRobot->myDirection = dir;
}

static void robot_set_state(Robot_state_e state){
	myRobot->myState = state;
}




static void robot_try_direction(Robot_direction_e direction){

	switch (direction) {
		case FORWARD:
			MOTORS_forward();
			break;
		case BACKWARD:
			MOTORS_backward();
			break;
		case LEFT:
			MOTORS_turn_left();
			break;
		case RIGHT:
			MOTORS_turn_right();
			break;
		case STOP:
			MOTORS_stop();
			break;

		default:
			break;
	}
}


