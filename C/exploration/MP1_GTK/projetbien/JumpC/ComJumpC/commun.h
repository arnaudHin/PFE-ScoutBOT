/**
 * @file commun.h
 * @author your name (you@domain.com)
 * @brief commun data file that we need to use in different files
 * @version 0.1
 * @date 2021-10-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SRC_POCKET_COMMUN_H_
#define SRC_POCKET_COMMUN_H_

#include <stdint.h>

//#include "../pocket/map_raw.h"
/*
 * Orders that can be received from the Android app
 */
#define INDICATE_R "IN"
#define FORWARD_R "DF"
#define BACKWARD_R "DB"
#define LEFT_R "DL"
#define RIGHT_R "DR"
#define BREAK_R "BR"
#define END_R "EN"
#define SOCKET_CLOSED_R "\0"


typedef enum
{
	NOP_ERROR = 0,
	UNAUTHORIZED_MOVE,
	OBSTACLE
}Network_error;


/*
 * Network_msg : expected structure to receive
 */

typedef enum
{
	// FROM Jump
	NOP = 0,
	DIR_FORWARD,
	DIR_BACKWARD,
	DIR_LEFT,
	DIR_RIGHT,
	STOP,
	CLOSE,
	REINIT_POS,
	POSITION_ROBOT_DEST,
	START_ODOMETRY,
	STOP_ODOMETRY,
	START_OBSTACLE_SENDER,
	STOP_OBSTACLE_SENDER,
	EMERGENCY_STOP,

	// FROM-TO jump
	INDICATE = 128,

	// TO Jump
	POSITION_ROBOT_CURRENT = 129,
	SEND_MAP = 130,
	ERROR = 131,
	AUTO = 132,
	MANUAL = 133,
	SEND_OBSTACLE = 134,
	SEND_COORD = 135
}Network_CMD;



typedef struct
{
	Network_CMD cmd;
	int size;
	//Coord coord;
}Network_msg;


#endif /* SRC_POCKET_COMMUN_H_ */

// typedef enum
// {
//     PLAY = 0,
//     PAUSE
// } RemoteState;

// /**
//  * @brief Is the remote connected to the car ?
//  * 
//  */
// typedef enum
// {
//     CONNECTED = 0,
//     UNCONNECTED
// } Connexion;

// /**
//  * @brief Car direction
//  * 
//  */
// typedef enum
// {
//     LEFT = 0,
//     RIGHT,
//     FORWARD,
//     MOVEBACK
// } Direction;

// typedef struct
// {
//     Direction dir;
// } Data;