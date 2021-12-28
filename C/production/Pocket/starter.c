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
	race_protocol_init();


	dispatcher_race_start();
	pilot_start();
	cartographer_start();
	dispatcher_remote_start();


	pilot_stop();
	dispatcher_remote_stop();
	dispatcher_race_stop();



	printf("\n************************************************************************************************\n");
	printf("\n                                    Pocket stop\n\n");
	printf("************************************************************************************************\n\n");

	return 0;
}

/**************************************************************************************************************/
