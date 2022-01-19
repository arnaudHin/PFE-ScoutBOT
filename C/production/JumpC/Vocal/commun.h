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

#define CMD_SIZE_BYTE 1

#define LIDAR_POINTS_PER_DEGREE 2
#define LIDAR_TOTAL_DEGREE 360
#define LIDAR_SIZE_BYTE_DATA (LIDAR_POINTS_PER_DEGREE) * (LIDAR_TOTAL_DEGREE)
#define LIDAR_TOTAL_DATA 720
#define POSITION_NB_BEACONS_BLE 9
#define POSITION_SIZE_BYTE_DATA (POSITION_NB_BEACONS_BLE + 1) * 2 * (sizeof(int16_t))

#define MAX_SIZE_BYTE_DATA_TO_SEND (1449)

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
	NO_ROOM = 0,
	ROOM_A,
	ROOM_B,
} Room_e;

typedef enum
{
	DEFAULT = 0,
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD,
	STOP,
	BREAK
} Direction_e;

typedef enum
{
	NOP = 0,
	ASK_TRY_DIR,
	ASK_SET_MODE,
	ASK_CHECK_CONNECTION,
	ASK_QUIT,
	ASK_4_DATA,
	ASK_STOP_CALIBRATION,
	NB_CMD_FROM_JUMP
} CMD_e;

typedef enum
{
	CHECK_CONNECTION = 0,
	SET_ASK_QUIT,
	SET_CHECK_CONNECTION,
	SET_DATA,
	NOP_CMD,
	NB_CMD_TO_JUMP
} CMD_from_pocket_e;

typedef struct
{
	float x;
	float y;
	Room_e room;
} Position_data_t;

typedef struct
{
	int16_t X_buffer[LIDAR_TOTAL_DEGREE];
	int16_t Y_buffer[LIDAR_TOTAL_DEGREE];
} Lidar_data_t;

typedef struct
{
	Lidar_data_t lidarData;
	Position_data_t positionData;
} DATA_from_pocket_t;

typedef struct
{
	Direction_e direction;

} DATA_to_pocket_t;

typedef struct
{
	CMD_e cmd;
	uint16_t sizeData;
	DATA_to_pocket_t data;
} Message_to_pocket_t;

typedef struct
{
	CMD_from_pocket_e command;
	uint16_t size;
	DATA_from_pocket_t data;
} Message_from_pocket_t;

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