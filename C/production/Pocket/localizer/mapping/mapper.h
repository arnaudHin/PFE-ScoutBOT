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
/** \fn extern void mapper_new()
 *  \brief Function dedicated to initialize the mapper
 */
extern void mapper_new();

/** \fn extern void mapper_free()
 *  \brief Function dedicated to free the mapper
 */
extern void mapper_free();

/** \fn extern void mapper_start ()
 *  \brief Function dedicated to start the mapper
 */

/** \fn static void mapper_start ()
 *  \brief Function dedicated to start the mapperPosition
 */
extern void mapper_start();

/** \fn extern void mapper_stop()
 *  \brief Function dedicated to stop the mapperPosition
 */
extern void mapper_stop();

/** \fn extern void mapper_getPosition()
 *  \brief Function dedicated to obtain the actual lidar data
 * \return return lidarData
 */
extern Lidar_data_t * mapper_getLidarData();
/************************ END OF PROTOTYPE DECLARATION **************************************/

#endif /* SRC_POCKET_V1_MAPPER_H_ */
