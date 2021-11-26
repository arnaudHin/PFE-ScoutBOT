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

#define MAX_SIZE_BYTE_DATA_TO_SEND	(1)

typedef enum
{
	NOP_ERROR = 0,
	UNAUTHORIZED_MOVE,
	OBSTACLE
} Network_error;

/*
 * Network_msg : expected structure to receive
 */


typedef enum
{
	DEFAULT=0,
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD,
	STOP,
	BREAK
} Direction_e;

typedef enum
{
	NOP_CMD = 0,
	ASK_TRY_DIR,
	ASK_SET_MODE,
	ASK_CHECK_CONNECTION,
	ASK_QUIT,
	ASK_LIDAR_MAPPING,
	ASK_POSITION,
	NB_CMD_FROM_JUMP
} CMD_e;

typedef struct{
	Direction_e direction;

}DATA_to_pocket_t;

typedef struct
{
	CMD_e cmd;
	uint16_t sizeData;
	DATA_to_pocket_t data;
} Message_to_pocket_t;

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