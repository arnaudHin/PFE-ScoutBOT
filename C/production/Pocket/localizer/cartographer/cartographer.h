/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  cartographer.h
 * @author Brient Nathan
 * @version 2.0
 * @date 1/12/2021
 * @brief cartographer entity header file
 *
 */


#ifndef SRC_POCKET_V1_CARTOGRAPHER_H_
#define SRC_POCKET_V1_CARTOGRAPHER_H_


#include "../../commun.h"
#include "../positioning/adminPositioning.h"
#include "../mapping/mapper.h"

/************************ PROTOTYPE DECLARATION **************************************/

/** \fn static void cartographer_start ()
 *  \brief Function dedicated to start the cartographer
 */
extern void cartographer_start();

/** \fn extern void cartographer_stop()
 *  \brief Function dedicated to stop the cartographer
 */
extern void cartographer_stop();

/************************ END OF PROTOTYPE DECLARATION **************************************/

#endif /* SRC_POCKET_V1_CARTOGRAPHER_H_ */