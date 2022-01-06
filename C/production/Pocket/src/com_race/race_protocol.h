/**
 * @file : race_protocol.h
 * @author : Arnaud Hincelin
 * @date : 25/11/2021
 * @brief : 
 */

#ifndef RACE_PROTOCOL_H_
#define RACE_PROTOCOL_H_

#include <stdint.h>
#include <unistd.h>
#include "../commun.h"
#include "race_protocol.h"



typedef enum{
	TRY_DIR=1
}CMD_to_race_e;


typedef struct{
	Direction_e direction;
}DATA_to_race_t;

typedef struct{
	int a;
}DATA_from_race_t;

typedef struct{
	CMD_to_race_e command;
	uint16_t sizeData;
	DATA_to_race_t data;
}Message_with_race_t;



extern void race_protocol_init();
extern void race_protocol_encode(Message_with_race_t * messageToEncode, uint8_t * bufferToEncode);
extern void race_protocol_decode(Message_with_race_t * messageToDecode, uint8_t * bufferToDecode);




#endif

