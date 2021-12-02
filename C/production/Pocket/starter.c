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
#include <network_pilot.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "stddef.h"
#include "string.h"
#include <math.h>
#include "util.h"
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
	robot_state_mutex_init();
	race_protocol_init();
	
	//map_raw_new();
	TRACE("map_raw_new(); OK\n");

	//display_start();
	//TRACE("display_start(); OK\n");
	// geographer_start();
	// TRACE("geographer_start(); OK\n");
	// obstacle_sender_start();
	// TRACE("obstacle_sender_start(); OK\n");

	// network_pilot_start();
	// TRACE("network_pilot_start(); OK\n");
	dispatcher_race_start();
	pilot_start();

	//display_send_txt_lidar_distance("Init en cours...");
	//display_send_txt_msg("Init en cours...");
	//display_send_txt_connection("Init en cours...");
	//display_send_txt_state("Init en cours...");

	dispatcher_remote_start();



	pilot_stop();
	dispatcher_remote_stop();
	dispatcher_race_stop();

	// network_pilot_stop();
	// geographer_stop();
	// obstacle_sender_stop();
	// map_raw_free();
	robot_state_mutex_destroy();


	printf("\n************************************************************************************************\n");
	printf("\n                                    Pocket stop\n\n");
	printf("************************************************************************************************\n\n");

	return 0;
}

/**************************************************************************************************************/
