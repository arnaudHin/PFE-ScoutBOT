/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  robot_state.h
 * @author Debus Alexy & Marbeuf Pierre
 * @version 3.0
 * @date 29/04/2020
 * @brief robot_state entity header file
 *
 */

#ifndef SRC_POCKET_ROBOT_STATE_H_
#define SRC_POCKET_ROBOT_STATE_H_

#define BUMPER_NB (4)
#define NB_SENSOR (3)

#include <stdbool.h>
#include <stdint.h>

/* \struct Odometry
 * \brief Odometry structure with the current position of the robot (x,y) & the current angle
 *
 */
typedef struct {
	int32_t x;		// Unity : mm
	int32_t y;		// Unity : mm
	int32_t teta;	// UNity : rad
}Odometry;

/* \struct Sensors_state
 * \brief Sensors_state structure with the current value of the 4 bumpers : front_left , front_right, back_left and back_right
 * TRUE : The robot shocked an obstacle with the bumper
 * FALSE : The robot didn't shocked an obstacle with the bumper
 */
typedef struct {
	bool bumper_state_fl; // Bumper front_left
	bool bumper_state_fr; // Bumper front_right

	bool bumper_state_bl; // Bumper back_left
	bool bumper_state_br; // Bumper back_right
}Sensors_state;

/* \struct Lidar_state
 * \brief Lidar_state structure with the current value of the lidar sensors
 */
typedef struct{
	uint8_t rangeStatus[NB_SENSOR];
	uint16_t distance[NB_SENSOR];
	uint16_t signalRate[NB_SENSOR];
	uint16_t ambientRate[NB_SENSOR];
	int32_t servoAngle;
}Lidar_state;

/** \fn extern void robot_state_set_mapping(bool mapping)
 *  \brief Function dedicated to set the value of a boolean dedicated to indicate to start or stop a mapping sequence
 *  \param  bool a boolean to launch or stop a mapping sequence
 */
extern void robot_state_set_mapping(bool mapping);

/** \fn extern Odometry robot_state_get_odometry ()
 *  \brief Function dedicated to obtain the current position(x,y) + angle of the robot
 *  \retval Odometry pointer on the current position + angle of the robot
 */
extern Odometry robot_state_get_odometry ();

/** \fn extern void robot_state_set_odometry(Odometry robot_odometry)
 *  \brief Function dedicated to set the current odometry
 *  \param Odometry the position(x,y) + angle(teta) to set
 */
extern void robot_state_set_odometry(Odometry robot_odometry);

/** \fn extern Sensors_state* robot_state_get_sensors_state ()
 *  \brief Function dedicated to obtain the current value (true or false) of the 4 bumpers
 *  \retval Sensors_state* pointer on the current value of the 4 different bumpers
 */
extern Sensors_state* robot_state_get_sensors_state ();

/** \fn extern void robot_state_set_sensors_state(Sensors_state sensors_state)
 *  \brief Function dedicated to set the current value (true or false) of the 4 bumpers
 *  \param Sensors_state the value of the 4 different bumpers to set
 */
extern void robot_state_set_sensors_state(Sensors_state sensors_state);

/*
 *  \brief Function dedicated to get the value of the ToF sensors as well as the value of the angle of the lidar's servomotor
 *  \retval robot_state_lidar_sensors_state the value of the ToF sensors and angle of the servomotor
 */
extern Lidar_state robot_state_get_lidar_sensors_state();

/*
 *  \brief Function dedicated to set the value of the ToF sensors as well as the value of the angle of the lidar's servomotor
 *  \param Lidar_state the value of the ToF sensors and angle of the servomotor
 */
extern void robot_state_set_lidar_sensors_state(Lidar_state lidar_state);

/** \fn extern void robot_state_mutex_init()
 *  \brief Function dedicated to initialize a mutex
 */
extern void robot_state_mutex_init();

/** \fn extern void robot_state_mutex_destroy()
 *  \brief Function dedicated to destroy a mutex
 */
extern void robot_state_mutex_destroy();

#endif /* SRC_POCKET_V1_robot_STATE_H_ */
