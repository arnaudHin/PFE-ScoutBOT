/*
 * postman_robot.c
 *
 *  Created on: 21 oct. 2021
 *      Author: martin
 */


#include <com_pocket/postman_robot.h>
#include <com_pocket/dispatcher_robot.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static __IO FlagStatus flag = RESET;

#define FLAG_SIZE (1)
#define SLAVE_ADDR_SIZE (1)
#define MAX_DATA  (1)
#define CMD_SIZE  (1)
#define SIZE_DATA (4)

#define ODOMETRY_SIZE (6)

#define MAX_RECEPTION_VUART_SIZE (FLAG_SIZE + SLAVE_ADDR_SIZE + CMD_SIZE + MAX_DATA + (30 * SIZE_DATA) + FLAG_SIZE)

/************************************** STATIC FUNCTIONS*************************************************************/

static Pilot_message_r postman_robot_read_message();
static void postman_dispatch (unsigned char * buffer_read, Pilot_message_r * pilot_message );

/**************************************END OF STATIC FUNCTIONS *******************************************************/


extern void postman_robot_run(){
	VIRT_UART_Callback_0();
	if (get_RX_msg_0()) {
		printf("Message received from A7\n");
		set_RX_msg_0(flag);
		Pilot_message_r pilot_message = postman_robot_read_message();
		dispatcher_robot_set_msg(pilot_message);
	}
}


/* \fn static Pilot_message postman_robot_read_message()
 * \brief Function dedicated to read message on VUART (huart 0)
 * \return Pilot_message the structure which contain a read message
 */
static Pilot_message_r postman_robot_read_message(){

	printf("Postman read \n");

	Pilot_message_r pilot_message;
	memset(&pilot_message,0,sizeof(Pilot_message_r));

	unsigned char buffer_read [MAX_RECEPTION_VUART_SIZE];//[FLAG_SIZE + SLAVE_ADDR_SIZE + CMD_SIZE + MAX_DATA];
	memset(buffer_read,0x00,sizeof(buffer_read));

	memcpy(buffer_read, VIRT_UART_read_message_0(), MAX_RECEPTION_VUART_SIZE);

	postman_dispatch(buffer_read, &pilot_message);

	return pilot_message;
}


static void postman_dispatch (unsigned char * buffer_read, Pilot_message_r * pilot_message ){

	printf("Init value data received \n");
	pilot_message->start_flag = 0;
	pilot_message->slave_addr = 0;
	pilot_message->order_r = 0;
	pilot_message->max_data = 0;
	pilot_message->stop_flag = 0;

	// Get values
	memcpy(&pilot_message->start_flag, &buffer_read[0], FLAG_SIZE); 		//  START_FLAG
	memcpy(&pilot_message->slave_addr, &buffer_read[1], SLAVE_ADDR_SIZE);	//  SLAVE_ADDR
	memcpy(&pilot_message->order_r, &buffer_read[2], CMD_SIZE);				//  CMD
	memcpy(&pilot_message->max_data, &buffer_read[3], MAX_DATA);			//  MAX_DATA

	printf("START FLAG : %d\n",pilot_message->start_flag);
	printf("SLAVE ADDR : %d\n",pilot_message->slave_addr);
	printf("CMD_SIZE : %d\n",pilot_message->order_r);
	printf("MAX_DATA : %d\n",pilot_message->max_data);

}





