/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file starter.c
 * @author Debus Alexy & Marbeuf Pierre
 * @version 3.0
 * @date 13/05/2020
 * @brief starter entity source file
 *
 */

#include <pocket/com_Pocket/dispatcher_pocket.h>
#include <pocket/com_Race/dispatcher_pilot.h>
#include <pocket/com_Race/proxy_robot.h>
// #include <pocket/copilot.h>
// #include <pocket/display.h>
// #include <pocket/geographer.h>
// #include <pocket/obstacle_sender.h>
#include <pocket/network_pilot.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "stddef.h"
#include "string.h"
#include <math.h>
#include "util.h"
#include "assert.h"

#define AVEC_VUART
//#define SANS_VUART

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
	map_raw_new();
	TRACE("map_raw_new(); OK\n");

	//display_start();
	//TRACE("display_start(); OK\n");
	// geographer_start();
	// TRACE("geographer_start(); OK\n");
	// obstacle_sender_start();
	// TRACE("obstacle_sender_start(); OK\n");

	// network_pilot_start();
	// TRACE("network_pilot_start(); OK\n");
	dispatcher_pilot_start();
	TRACE("dispatcher_pilot_start(); OK\n");

	//display_send_txt_lidar_distance("Init en cours...");
	//display_send_txt_msg("Init en cours...");
	//display_send_txt_connection("Init en cours...");
	//display_send_txt_state("Init en cours...");

	//TRACE("test envoi A7->M4\n");
	//postman_send_message()

#ifdef AVEC_VUART
	dispatcher_pocket_start();
	TRACE("dispatcher_pocket_start(); OK\n");
#endif
#ifdef SANS_VUART
	new_dispatcher_pocket_start();
	TRACE("new_dispatcher_pocket_start(); OK\n");
#endif

	// geographer_ask_map();

	dispatcher_pocket_stop();
	dispatcher_pilot_stop();

	//display_stop();

	// network_pilot_stop();
	// geographer_stop();
	// obstacle_sender_stop();
	// map_raw_free();
	robot_state_mutex_destroy();

	/*************************************** TEST ****************************************************************/
//		Odometry odometry_begin = {12,10,0};
//		Coord coord_final = {40,10}; //X Y has to be interverted
//		TRACE("x:%d,y:%d",coord_final.x,coord_final.y);
//
//		assert(coord_final.x <map_raw_get_width()-1  && "FORBIDDEN WIDTH\n"); // Halt the execution of the thread until it achieves his execution
//		assert(coord_final.y <map_raw_get_height()-1 && "FORBIDDEN HEIGHT\n"); // Halt the execution of the thread until it achieves his execution
//
//		if (is_available(coord_final)){
//			int my_path_length = 0;
//			memcpy(astar_path,calculate_path(odometry_begin,coord_final,&my_path_length,map_get_matrix(),map_raw_get_width(),map_raw_get_height()),my_path_length*sizeof(Odometry));
//
//			printf("len path : %d\n",my_path_length);
//
//			for (int i=0;i<my_path_length;i++){
//				printf("x:%d,y:%d\n",astar_path[i].x,astar_path[i].y);
//			}
//
//		}else{
//			TRACE("FORBIDDEN COORD\n");
//		}

	/**************************************** END TEST ***********************************************************/


	printf("\n************************************************************************************************\n");
	printf("\n                                    Pocket stop\n\n");
	printf("************************************************************************************************\n\n");

	return 0;
}

/**************************************************************************************************************/
