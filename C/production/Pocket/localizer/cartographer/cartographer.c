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



/************************************** DEFINE ****************************************************************/

/************************************** STATIC VARIABLE *******************************************************/



/************************************** END DEFINE *************************************************************/


/**********************************  STATIC FUNCTIONS DECLARATIONS ************************************************/

/**********************************  PUBLIC FUNCTIONS ************************************************/

extern void cartographer_start(){
    adminPositioning_start();
}
extern void cartographer_stop(){
    adminPositioning_stop();
}