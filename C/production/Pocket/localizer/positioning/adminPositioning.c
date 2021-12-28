/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  adminPositioning.c
 * @author Brient Nathan
 * @version 2.0
 * @date 01/122021
 * @brief Make positioning calcul
 *
 * The purpose of the adminPositioning is to make calcul and estimation of the position
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
#include "adminPositioning.h"
#include "../../utils/util.h"
#include "../../utils/Watchdog/watchdog.h"


/************************************** DEFINE ****************************************************************/
#define TAILLE_MAX 20 //nombre de caractères maximums a lire 

/************************************** STATIC VARIABLE *******************************************************/

static Position_data_t robotPosition;
static pthread_t mythread;						   //ID thread
static Watchdog* myWatchdog;
static pthread_mutex_t myMutex=PTHREAD_MUTEX_INITIALIZER;
static bool stopRunning = false ;
/************************************** END DEFINE *************************************************************/


/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

static void* adminPositioning_run();
static void adminPositioning_waitTaskTermination();
static void time_out();



/** \fn static adminPositioning_setPosition(Position_data_t * position)
 *  \brief set position
 */
static void adminPositioning_setPosition(Position_data_t * position)
{
    pthread_mutex_lock(&myMutex);
    robotPosition.x = position->x;
    robotPosition.y = position->y;
    robotPosition.room = position->room;
    pthread_mutex_unlock(&myMutex);
}

static void time_out(){
    printf("je suis dans time_out \n");
    FILE* fichier = NULL;
        Position_data_t actualPosition;
        system("AnnexPositioningFiles/RSSI_scan.sh");
        printf("on ouvre le fichier \n");
        fichier = fopen("position.txt", "r");
        if (fichier != NULL){
            fscanf(fichier, "%f %f", &(actualPosition.x), &(actualPosition.y));  
            adminPositioning_setPosition(&actualPosition);
            printf("la position est x = %f y = %f",robotPosition.x,robotPosition.y);          
            fclose(fichier);
        }else{
            // On affiche un message d'erreur si on veut
            printf("Impossible d'ouvrir le fichier position.txt \n");
        }
    // Watchdog_start(myWatchdog);

}

/** \fn static void *adminPositioning_run()
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *  \retval NULL
 */
static void* adminPositioning_run(){
    
    // Watchdog_start(myWatchdog);
    FILE* fichier = NULL;
    while(stopRunning==false){
        Position_data_t actualPosition;
        system("./Indoor_positioning/bash/RSSI_scan.sh");
        printf("on ouvre le fichier \n");
        fichier = fopen("Indoor_positioning/results/data_indoor_pos.txt", "r");
        if (fichier != NULL){
            fscanf(fichier, "%f %f %d", &(actualPosition.x), &(actualPosition.y),  &(actualPosition.room));  
            adminPositioning_setPosition(&actualPosition);
            printf("la position est x = %f y = %f room = %d",robotPosition.x,robotPosition.y,robotPosition.room);          
            fclose(fichier);
        }else{
            // On affiche un message d'erreur si on veut
            printf("Impossible d'ouvrir le fichier position.txt \n");
        }
        
    }
    return NULL;
}

/** \fn static void adminPositioning_waitTaskTermination ()
 *  \brief Function dedicated to wait until the end of the current task
 */
static void adminPositioning_waitTaskTermination()
{
    //Watchdog_cancel(myWatchdog);
	int error_code = pthread_join(mythread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}
/**********************************  PUBLIC FUNCTIONS ************************************************/


extern void adminPositioning_new(){
    printf("je suis dans adminPositioning_new \n");
    stopRunning = false ;
    pthread_mutex_init(&myMutex, NULL);
    //myWatchdog = Watchdog_construct(1000000, (WatchdogCallback) time_out);
}

extern void adminPositioning_free(){
    printf("je suis dans adminPositioning_free \n");
    stopRunning = true ;
    pthread_mutex_destroy(&myMutex);
    //Watchdog_destroy(myWatchdog);
}

extern void adminPositioning_start(){
    adminPositioning_new();
    printf("je suis dans adminPositioning_start \n");
    int return_thread = pthread_create(&mythread, NULL, &adminPositioning_run, NULL);
	assert(return_thread == 0 && "Error Pthread_create adminPositioning\n");
}

extern void adminPositioning_stop(){
    printf("je suis dans adminPositioning_stop\n");
    adminPositioning_free();
    adminPositioning_waitTaskTermination();
}

extern Position_data_t * adminPositioning_getPosition(){
    Position_data_t actualPosition;
    pthread_mutex_lock(&myMutex);

    actualPosition.x = robotPosition.x;
    actualPosition.y = robotPosition.y;
    
    pthread_mutex_unlock(&myMutex);
    return &actualPosition;
}
