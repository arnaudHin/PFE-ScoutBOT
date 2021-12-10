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

#define LIDAR_POINTS_PER_DEGREE			2
#define LIDAR_TOTAL_DEGREE				360
#define LIDAR_DATA_TYPE					int16_t
#define LIDAR_SIZE_BYTE_DATA			(LIDAR_POINTS_PER_DEGREE)*(LIDAR_TOTAL_DEGREE)*(sizeof(LIDAR_DATA_TYPE))

#define POSITION_SIZE_BYTE_DATA 		(2*sizeof(float) )+1

#define MAX_SIZE_BYTE_DATA_TO_SEND		(LIDAR_SIZE_BYTE_DATA)+(POSITION_SIZE_BYTE_DATA)
#define MAX_SIZE_BYTE_DATA_TO_RECEIVE	(1)

#define CMD_SIZE_BYTE					1
#define DATA_SIZE_BYTE					sizeof(uint16_t)
#define BUFF_SIZE_TO_RECEIVE			(MAX_SIZE_BYTE_DATA_TO_RECEIVE) + (CMD_SIZE_BYTE) + ( DATA_SIZE_BYTE )
#define BUFF_SIZE_TO_SEND				(MAX_SIZE_BYTE_DATA_TO_SEND) + (CMD_SIZE_BYTE) + ( DATA_SIZE_BYTE )


#define SOCKET_CLOSED_R "\0"

#define SIZE_BUFFER_MAX 361


typedef enum
{
	NOP_ERROR = 0,
	UNAUTHORIZED_MOVE,
	OBSTACLE
}Network_error;


typedef enum{
	NO_ROOM=0,
	ROOM_A,
	ROOM_B,
}Room_e;

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


typedef struct{
	int16_t X_buffer[LIDAR_TOTAL_DEGREE];
	int16_t Y_buffer[LIDAR_TOTAL_DEGREE];
}Lidar_data_t;

typedef struct{
	float x;
	float y;
	Room_e room;
}Position_data_t;

typedef enum{
	MODE_MANUAL=0,
	MODE_AUTO
}MODE_e;

typedef enum
{
	NOP = 0,
	ASK_TRY_DIR,
	ASK_SET_MODE,
	ASK_CHECK_CONNECTION,
	ASK_QUIT,
	ASK_4_DATA,
	NB_CMD_FROM_JUMP
}CMD_from_jump_e;


typedef enum
{
	CHECK_CONNECTION,
	SET_ASK_QUIT,
	SET_CHECK_CONNECTION,
	SET_DATA,
	NB_CMD_TO_JUMP
}CMD_to_jump_e;


typedef struct{
	Direction_e direction;
	MODE_e mode;
}DATA_from_jump_t;


typedef struct{
	Lidar_data_t lidarData;
	Position_data_t positionData;
}DATA_to_jump_t;

typedef struct{
	CMD_from_jump_e command;
	uint16_t sizeData;
	DATA_from_jump_t data;
}Message_from_jump_t;


typedef struct{
	CMD_to_jump_e command;
	uint16_t size;
	DATA_to_jump_t data;
}Message_to_jump_t;




#endif /* SRC_POCKET_COMMUN_H_ */
