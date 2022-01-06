/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  proxy_robot.h
 * @author Debus Alexy
 * @version 0.2
 * @date 01/05/2020
 * @brief Proxy_robot entity source file
 *
 */

#include "proxy_robot.h"
#include "race_protocol.h"
#include "postman_race.h"
#include "../utils/util.h"
#include "stdlib.h"

#include "../commun.h"


static Message_with_race_t myMessageToRace;




extern void proxy_robot_try_dir(DATA_to_race_t * dataToRace){

	myMessageToRace.command = TRY_DIR;
	myMessageToRace.sizeData = 1;
	myMessageToRace.data = *dataToRace;

	uint8_t bufferToSend[4];
	memset(bufferToSend, 0x00, sizeof(bufferToSend) );

	fprintf(stderr, "\nproxy_robot_try_dir : data dir=%d\n", (uint8_t) myMessageToRace.data.direction);

	//encode
	race_protocol_encode(&myMessageToRace, bufferToSend);

	for (size_t i = 0; i < 4; i++)
	{
		fprintf(stderr, "%d,", bufferToSend[i] ); 
	}
	fprintf( stderr, "\n" ); 
	
	
	postman_race_send_message(bufferToSend, sizeof(bufferToSend) );
}
