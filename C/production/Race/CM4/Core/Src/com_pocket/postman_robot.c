/*
 * postman_robot.c
 *
 *  Created on: 21 oct. 2021
 *      Author: martin & Arnaud
 */


#include <com_pocket/postman_robot.h>
#include <com_pocket/dispatcher_robot.h>
#include <com_pocket/intercore_protocol.h>
#include <utils.h>
#include "commun.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


/*********************************************************************************/
/**									Private variables							**/
/*********************************************************************************/

static __IO FlagStatus flag = RESET;
Message_with_race_t * pMessageFromPocket;
uint8_t buffer_read[MAX_RECEPTION_VUART_SIZE];



/*********************************************************************************/
/**									Private prototypes							**/
/*********************************************************************************/

static void postman_robot_read_message();


/*********************************************************************************/
/**									Public functions							**/
/*********************************************************************************/

extern ScoutBOT_Status_e postman_robot_init(){

	/* Init pMessageFromPocket */
	pMessageFromPocket = NULL;
	pMessageFromPocket = calloc(1, sizeof(Message_with_race_t) );
	if(pMessageFromPocket == NULL){
		return ERROR_BOT;
	}

	/* Init buffer_read */
	memset(buffer_read, 0x00, sizeof(buffer_read) );

	return OK_BOT;
}



extern ScoutBOT_Status_e postman_robot_free(){

	/* free pMessageFromPocket */
	free(pMessageFromPocket);

	return OK_BOT;
}


extern void postman_robot_run(){

	OPENAMP_check_for_message(); //check if message on VUART channels

	if (get_RX_msg_0() == RESET) { //if no message on VUART channel 0, quit function
		return;
	}

	postman_robot_read_message();
	set_RX_msg_0(flag);
	intercore_protocole_decode_message(buffer_read, pMessageFromPocket);

	dispatcher_robot_set_msg(*pMessageFromPocket);

}


/*********************************************************************************/
/**									Private functions							**/
/*********************************************************************************/

static void postman_robot_read_message(){

	printf("Postman read \n");

	memcpy(buffer_read, VIRT_UART_read_message_0(), MAX_RECEPTION_VUART_SIZE);

}



