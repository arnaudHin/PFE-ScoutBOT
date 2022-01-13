/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file starter.c
 * @author Debus Alexy & Marbeuf Pierre
 * @version 3.0
 * @date 13/05/2020
 * @brief starter entity source file
 *
 */

#include <com_remote/dispatcher_remote.h>
#include <com_race/race_protocol.h>
#include <com_race/dispatcher_race.h>
#include <com_race/proxy_robot.h>
#include "localizer/cartographer/cartographer.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "stddef.h"
#include "string.h"
#include <math.h>
#include "utils/util.h"
#include "assert.h"


/**************************************************************************************************************/

/*  fn int main (int argc, char *argv[])
 *  \brief Function dedicated to call the pocket components to be launched
 */
int main (int argc, char *argv[])
{

	printf("\n************************************************************************************************\n");
	printf("\n                                    Pocket start\n\n");
	printf("************************************************************************************************\n\n");
	// race_protocol_init();


	// dispatcher_race_start();
	// pilot_start();
	// cartographer_start();
	// dispatcher_remote_start();


	// pilot_stop();
	// dispatcher_remote_stop();
	// dispatcher_race_stop();
	static Message_with_race_t myMessageToRace;
	static DATA_to_race_t data;
	data.direction=DEFAULT;

	// 	DEFAULT=0, 1010
	// LEFT,1011
	// RIGHT,1012
	// FORWARD,1013
	// BACKWARD,1014
	// STOP,1015
	// BREAK, 1016
	myMessageToRace.command = TRY_DIR;
	myMessageToRace.sizeData = 1;
	myMessageToRace.data = data;

	uint8_t bufferToSend[4];
	memset(bufferToSend, 0x00, sizeof(bufferToSend) );
	fprintf(stderr, "\nproxy_robot_try_dir : data dir=%d\n", (uint8_t) myMessageToRace.data.direction);

	//encode
	race_protocol_encode(&myMessageToRace, bufferToSend);
	for(int i =0; i<4;i++){
		printf("%d",bufferToSend[i]);
	}
	//1012 RIGHT

	printf("\n************************************************************************************************\n");
	printf("\n                                    Pocket stop\n\n");
	printf("************************************************************************************************\n\n");

	return 0;
}

/**************************************************************************************************************/
