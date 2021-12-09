/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  cartographer.c
 * @author Brient Nathan
 * @version 2.0
 * @date 01/122021
 * @brief Make cartography
 *
 * The purpose of the cartographer is to make calcul and estimation of the position
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "cartographer.h"
#include "../../utils/util.h"
#include "../../utils/Watchdog/watchdog.h"



static pthread_t mythread;						   //ID thread
static const char queue_name[] = "/pilot_postman"; //name of mailbox
static mqd_t id_bal;	

static Watchdog* myWatchdog;



static DATA_to_jump_t DADAToJump;



/************************************** DEFINE ****************************************************************/



/************************************** STATIC VARIABLE *******************************************************/





/**********************************  PUBLIC FUNCTIONS ************************************************/


extern void cartographer_new(){
    adminPositioning_new();
    //mapper_new();
}


extern void cartographer_free(){
    adminPositioning_free();
    //mapper_free();

}

extern void cartographer_start(){
    // adminPositioning_start();
    mapper_start();

}
extern void cartographer_stop(){
    // adminPositioning_stop();
    mapper_stop();

}





/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

static void time_out();

static void *cartographer_run(){

}


static void adminPositioning_waitTaskTermination(){

}


static void cartographer_getPositionData(){


}

static void cartographer_getLidarData(){


}