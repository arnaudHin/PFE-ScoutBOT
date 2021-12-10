/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  maper.c
 * @author ARONDEL Martin
 * @version 2.0
 * @date 01/122021
 * @brief Make lidar calcul
 *
 * The purpose of the mapper is to retrieve objects positions with the lidar
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
#include "mapper.h"
#include "../../utils/util.h"

/************************************** DEFINE ****************************************************************/
#define DISPLAY 1
/************************************** STATIC VARIABLE *******************************************************/
static pthread_t mythread;

/************************************** EXTERN VARIABLE *******************************************************/

static Lidar_data_t lidarData;
/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/
static void *mapper_run();
static void mapper_waitTaskTermination();
static void fileAcquisition();
static uint8_t stopMapper = 0;


static void fileAcquisition(){
    FILE* fichier = NULL;
    uint32_t buf_index = 0;
    int16_t x;
    int16_t y;
    fprintf(stderr, "Ouverture du fichier lidar.sh\n");
    system("./lidar.sh");
    fichier = fopen("position_gtk.txt","r");
    // fichier = fopen("data_lidar.txt","r");
    if (fichier != NULL)
    {
        do{
            int a = fscanf(fichier, "%hd,%hd,", &x, &y);
            lidarData.X_buffer[buf_index] = x;
            lidarData.Y_buffer[buf_index] = y;  
            buf_index++;
        }while( !feof(fichier) );
        fclose(fichier);
    }
    else{
        fprintf(stderr,"Error! opening file 'position_gtk.txt'\n");
    }
}

/** \fn static void *mapper_run()
 *  \brief Function run executed by the thread of mapper
 *  \retval NULL
 */
static void *mapper_run()
{   
    while(stopMapper != 1){
        fileAcquisition();

        #if DISPLAY
        fprintf(stderr,"\nBUFFER X : \n");
        for(int i = 0 ; i< SIZE_BUFFER_MAX ; i++){
            fprintf(stderr,"%hd;", lidarData.X_buffer[i]);
        }

        fprintf(stderr,"\nBUFFER Y : \n");
        for(int i = 0 ; i< SIZE_BUFFER_MAX ; i++){
            fprintf(stderr,"%hd;", lidarData.Y_buffer[i]);
        }
        #endif
    }
}

/** \fn static void mapper_waitTaskTermination ()
 *  \brief Function stop executed by the thread of mapper
 */
static void mapper_waitTaskTermination()
{
	int error_code = pthread_join(mythread, NULL);
	assert(error_code != -1 && "ERROR Joining current thread\n"); // Halt the execution of the thread until it achieves his execution
}

/**********************************  PUBLIC FUNCTIONS ************************************************/

/** \fn extern void mapper_start()
 *  \brief Function dedicated to execute the mapper thread
 *  \retval NULL
 */
extern void mapper_start(){

    int return_thread = pthread_create(&mythread, NULL, &mapper_run, NULL);
	fprintf(stderr,"mapperstart is starting");
	assert(return_thread == 0 && "Error Pthread_create adminPositioning\n");
}

/** \fn static void mapper_stop()
 *  \brief Function dedicated to stop the mapper thread
 *  \retval NULL
 */
extern void mapper_stop(){
    fprintf(stderr,"pilot stop\n\n");
    //mapper_waitTaskTermination(); //on utilise pas de bal...
    stopMapper = 1;
}


extern void mapper_new(){

}

extern void mapper_free(){

}

extern Lidar_data_t mapper_getLidarData(){

    
}