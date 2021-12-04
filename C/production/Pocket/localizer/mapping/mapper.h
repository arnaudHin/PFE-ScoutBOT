/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  mapper.c
 * @author ARONDEL Martin
 * @version 2.0
 * @date 01/122021
 * @brief Make lidar calcul
 */


#ifndef SRC_POCKET_V1_MAPPER_H_
#define SRC_POCKET_V1_MAPPER_H_


#include "../../commun.h"


/************************ PROTOTYPE DECLARATION **************************************/

/** \fn static void mapper_start ()
 *  \brief Function dedicated to start the mapperPosition
 */
extern void mapper_start();

/** \fn extern void mapper_stop()
 *  \brief Function dedicated to stop the mapperPosition
 */
extern void mapper_stop();

/************************ END OF PROTOTYPE DECLARATION **************************************/

#endif /* SRC_POCKET_V1_MAPPER_H_ */
