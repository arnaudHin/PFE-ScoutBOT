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


extern void intercore_protocole_decode_message(uint8_t * bufferToDecode, Pilot_message_r * message){

	// Get values
	uint16_t counterMemory = 0;
	memcpy(&message->start_flag, (bufferToDecode + counterMemory), FLAG_SIZE); 		//  START_FLAG
	counterMemory += FLAG_SIZE;
	memcpy(&message->slave_addr, (bufferToDecode + counterMemory), SLAVE_ADDR_SIZE);	//  SLAVE_ADDR
	counterMemory += SLAVE_ADDR_SIZE;
	memcpy(&message->order_r, (bufferToDecode + counterMemory), CMD_SIZE);				//  CMD
	counterMemory += CMD_SIZE;
	memcpy(&message->max_data, (bufferToDecode + counterMemory), MAX_DATA);

}




