/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  pilot.c
 * @author Debus Alexy & Marbeuf Pierre
 * @version 3.0
 * @date 5/06/2020
 * @brief robot_state entity source file
 *
 * The purpose of the robot_state entity is to gather all data send by the robot (depth sensor and bumper) in order to be used by all the entities
 *
 */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <robot_state.h>
#include "../utils/util.h"



/************************************** STATIC FUNCTION *************************************************************/

static bool robot_state_is_mapping();
static void robot_state_mutex_lock();
static void robot_state_mutex_unlock();

/**************************************END OF STATIC FUNCTION ********************************************************/


static bool robot_state_mapping = false;
static Odometry robot_state_odometry;
static Sensors_state robot_state_sensors_state;
static Lidar_state robot_state_lidar_sensors_state;

static pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER; ////pthread_mutex initializer

/*
 *  \brief Function dedicated to set the value of a boolean dedicated to indicate to start or stop a mapping sequence
 *  \param  bool a boolean to launch or stop a mapping sequence
 */
extern void robot_state_set_mapping(bool mapping){
	robot_state_mutex_lock();
	robot_state_mapping = mapping;
	robot_state_mutex_unlock();
}

/** \fn static bool robot_state_is_mapping()
 *  \brief Function dedicated to indicate if the robot is scanning the environment
 *  \return bool a boolean to specify if the robot is in a sequence mapping
 */
static bool robot_state_is_mapping() { //Used only in V2
	return robot_state_mapping;
}

/************************************** GETTER/SETTER ******************************************************************/

/*
 *  \brief Function dedicated to obtain the current position(x,y) + angle of the robot
 *  \retval Odometry* pointer on the current position + angle of the robot
 */
extern Odometry robot_state_get_odometry (){
	return (robot_state_odometry);
}

/*
 *  \brief Function dedicated to set the current odometry
 *  \param Odometry the position(x,y) + angle(teta) to set
 */
extern void robot_state_set_odometry(Odometry robot_odometry){
	robot_state_mutex_lock();
	robot_state_odometry.x = robot_odometry.x;
	robot_state_odometry.y = robot_odometry.y;
	robot_state_odometry.teta = robot_odometry.teta;
	robot_state_mutex_unlock();
}

/*
 *  \brief Function dedicated to obtain the current value (true or false) of the 4 bumpers
 *  \retval Sensors_state* pointer on the current value of the 4 different bumpers
 */
extern Sensors_state * robot_state_get_sensors_state (){
	return (&robot_state_sensors_state);
}

/*
 *  \brief Function dedicated to set the current value (true or false) of the 4 bumpers
 *  \param Sensors_state the value of the 4 different bumpers to set
 */
extern void robot_state_set_sensors_state(Sensors_state sensors_state){
	robot_state_mutex_lock();
	robot_state_sensors_state.bumper_state_bl = sensors_state.bumper_state_bl;
	robot_state_sensors_state.bumper_state_br = sensors_state.bumper_state_br;
	robot_state_sensors_state.bumper_state_fl = sensors_state.bumper_state_fl;
	robot_state_sensors_state.bumper_state_fr = sensors_state.bumper_state_fr;
	robot_state_mutex_unlock();
}

/*
 *  \brief Function dedicated to get the value of the ToF sensors as well as the value of the angle of the lidar's servomotor
 *  \retval robot_state_lidar_sensors_state the value of the ToF sensors and angle of the servomotor
 */
extern Lidar_state robot_state_get_lidar_sensors_state(){
	return (robot_state_lidar_sensors_state);
}

/*
 *  \brief Function dedicated to set the value of the ToF sensors as well as the value of the angle of the lidar's servomotor
 *  \param Lidar_state the value of the ToF sensors and angle of the servomotor
 */
extern void robot_state_set_lidar_sensors_state(Lidar_state lidar_state){
	robot_state_mutex_lock();
	for(uint8_t num_sensor=0 ;num_sensor<NB_SENSOR ;num_sensor++){
		robot_state_lidar_sensors_state.distance[num_sensor] = lidar_state.distance[num_sensor];
	}
	robot_state_lidar_sensors_state.servoAngle = lidar_state.servoAngle;
	robot_state_mutex_unlock();
}

/************************************** MUTEX_FUNCTION ********************************************************************/
/*
 *  \brief Function dedicated to initialize a mutex
 */
extern void robot_state_mutex_init(){
	int mutex_init;
	mutex_init = pthread_mutex_init(&mutex_thread,NULL);
	assert(mutex_init == 0 && "Error to init mutex\n");
	//_unused(mutex_init);

}

/** \fn static void robot_state_mutex_lock()
 *  \brief Function dedicated to lock a mutex
 */
static void robot_state_mutex_lock(){
	int mutex_lock;
	mutex_lock = pthread_mutex_lock(&mutex_thread);
	assert(mutex_lock == 0 && "Error to lock mutex\n");
	//_unused(mutex_lock);

}

/** \fn static void robot_state_mutex_unlock()
 *  \brief Function dedicated to unlock a mutex
 */
static void robot_state_mutex_unlock(){
	int mutex_unlock;
	mutex_unlock = pthread_mutex_unlock(&mutex_thread);
	assert(mutex_unlock == 0 && "Error to unlock mutex\n");
	//_unused(mutex_unlock);

}

/*
 *  \brief Function dedicated to destroy a mutex
 */
extern void robot_state_mutex_destroy(){
	int mutex_destroy;
	mutex_destroy = pthread_mutex_destroy(&mutex_thread);
	assert(mutex_destroy == 0 && "Error to destroy mutex\n");
	//_unused(mutex_destroy);

}

/************************************** END OF MUTEX_FUNCTION ********************************************************************/
