/*
 * intercore_protocol.h
 *
 *  Created on: Nov 19, 2021
 *      Author: Arnaud
 */

#ifndef INC_COM_POCKET_INTERCORE_PROTOCOL_H_
#define INC_COM_POCKET_INTERCORE_PROTOCOL_H_





#include "com_pocket/postman_robot.h"
#include "stdlib.h"
#include "commun.h"




#define FLAG_SIZE (1)
#define SLAVE_ADDR_SIZE (2)
#define MAX_DATA  (1)
#define CMD_SIZE  (1)
#define SIZE_DATA (4)
#define ODOMETRY_SIZE (6)
#define MAX_RECEPTION_VUART_SIZE (FLAG_SIZE + SLAVE_ADDR_SIZE + CMD_SIZE + MAX_DATA + (30 * SIZE_DATA) + FLAG_SIZE)



typedef enum{
	TRY_DIR=1
}CMD_from_pocket_e;


typedef struct{
	Direction_e direction;
}DATA_from_pocket_t;

typedef struct{
	int a;
}DATA_to_pocket_t;

typedef struct{
	CMD_from_pocket_e command;
	uint16_t sizeData;
	DATA_from_pocket_t data;
}Message_with_race_t;



extern void intercore_protocole_decode_message(uint8_t * bufferToDecode, Message_with_race_t * message);





#endif /* INC_COM_POCKET_INTERCORE_PROTOCOL_H_ */
