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

static Position robotPosition;
static pthread_t mythread;						   //ID thread
static Watchdog* myWatchdog;
static pthread_mutex_t mutexPosition=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexLidar=PTHREAD_MUTEX_INITIALIZER;
static bool stopRunning = false ;

/************************************** DEFINE ****************************************************************/

/************************************** STATIC VARIABLE *******************************************************/



/************************************** END DEFINE *************************************************************/


/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

/** \fn static void *adminPositioning_run()
 *  \brief Function dedicated to run in the thread
 */
static void *adminPositioning_run();

/** \fn static void adminPositioning_waitTaskTermination()
 *  \brief Function dedicated to wait the end of the thread and end it
 */
static void adminPositioning_waitTaskTermination();


/** \fn static void time_out()
 *  \brief Function called when watchdog timer is over
 */
static void time_out();

/** \fn static Position getPosition()
 *  \brief Function called to get actual position of the robot
 */
static void getPositionData();

/** \fn static void time_out()
 *  \brief Function called when watchdog timer is over
 */
static void time_out();

static void *adminPositioning_run(){

}


static void adminPositioning_waitTaskTermination(){

}

static void time_out(){


}

static void getPositionData(){
    pthread_mutex_lock(&mutexPosition);
    robotPosition =adminPositioning_getPosition();
    pthread_mutex_unlock(&mutexPosition);

}
static void getLidarData(){
    pthread_mutex_lock(&mutexLidar);
    robotPosition =adminPositioning_getPosition();
    pthread_mutex_unlock(&mutexLidar);
}
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
