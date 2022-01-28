/*
 * intercore_protocol.c
 *
 *  Created on: Nov 19, 2021
 *      Author: Arnaud
 */





#include "com_pocket/postman_robot.h"
#include "com_pocket/intercore_protocol.h"
#include "string.h"


/*********************************************************************************/
/**									Public functions							**/
/*********************************************************************************/


extern void intercore_protocole_decode_message(uint8_t * bufferToDecode, Message_with_race_t * message){

	// Get values
	uint16_t counterMemory = 0;
	memcpy(&(message->command), (bufferToDecode + counterMemory), FLAG_SIZE); 		//  START_FLAG
	counterMemory += FLAG_SIZE;

	uint16_t size;
	memcpy(&size, (bufferToDecode + counterMemory), SLAVE_ADDR_SIZE);	//  SLAVE_ADDR
	message->sizeData = size;
	counterMemory += SLAVE_ADDR_SIZE;


	memcpy(&(message->data.direction), (bufferToDecode + counterMemory), 1);				//  CMD
	counterMemory += CMD_SIZE;

}




