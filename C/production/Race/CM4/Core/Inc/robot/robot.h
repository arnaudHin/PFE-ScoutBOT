/*
 * robot.h
 *
 *  Created on: Nov 19, 2021
 *      Author: Arnaud
 */

#ifndef INC_ROBOT_ROBOT_H_
#define INC_ROBOT_ROBOT_H_


#include "utils.h"

typedef enum{
	E_STOP=0,
	E_TRY_DIR,
	E_START_ROBOT,
	E_DESTROY,
	NB_ENTREE
}Robot_event_e;


typedef enum{
	STOP=0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	NB_DIRECTION
}Robot_direction_e;

typedef struct{
	Robot_direction_e directionToPerform;
	Robot_event_e event;
}Robot_mq_t;




/**
 * @fn ScoutBOT_Status_e robot_init()
 * @brief
 * @return : ScoutBOT_Status_e
 */
extern ScoutBOT_Status_e robot_init();

/**
 * @fn ScoutBOT_Status_e robot_free()
 * @brief
 * @return
 */
extern ScoutBOT_Status_e robot_free();

/**
 * @fn void robot_run(Robot_mq_t)
 * @brief
 * @param message
 */
extern void robot_run(Robot_mq_t message);





#endif /* INC_ROBOT_ROBOT_H_ */
