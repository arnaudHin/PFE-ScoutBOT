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



/************************************** DEFINE ****************************************************************/
#define TAILLE_MAX 20 //nombre de caractères maximums a lire 

/************************************** STATIC VARIABLE *******************************************************/

static Position robotPosition;
static pthread_t mythread;						   //ID thread
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

/************************************** END DEFINE *************************************************************/


/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

static void *adminPositioning_run();
static void adminPositioning_waitTaskTermination();


/** \fn static adminPositioning_setPosition(Position * position)
 *  \brief set position
 */
static void adminPositioning_setPosition(Position * position)
{
    pthread_mutex_lock(&mutex);
    robotPosition.x = position->x;
    robotPosition.y = position->y;
    pthread_mutex_unlock(&mutex);
}

/** \fn static void *adminPositioning_run()
 *  \brief Function dedicated to perform an action when a message is received in the mailbox
 *  \retval NULL
 */
static void *adminPositioning_run()
{
    FILE* fichier = NULL;
    char chaine[TAILLE_MAX] = "";
    Position actualPosition;
    while(1){
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
    }
}

/** \fn static void adminPositioning_waitTaskTermination ()
 *  \brief Function dedicated to wait until the end of the current task
 */
static void adminPositioning_waitTaskTermination()
{
	int error_code = pthread_join(mythread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}
/**********************************  PUBLIC FUNCTIONS ************************************************/


extern void adminPositioning_start(){

    int return_thread = pthread_create(&mythread, NULL, &adminPositioning_run, NULL);
	TRACE("adminPositioning is starting");
	assert(return_thread == 0 && "Error Pthread_create adminPositioning\n");
}

extern void adminPositioning_stop(){
    TRACE("pilot stop\n\n");
    adminPositioning_waitTaskTermination();
}

extern Position adminPositioning_getPosition(){
    Position actualPosition;
    pthread_mutex_lock(&mutex);
    actualPosition.x = robotPosition.x;
    actualPosition.y = robotPosition.y;
    pthread_mutex_unlock(&mutex);
    return actualPosition;
}
