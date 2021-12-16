/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  adminPositioning.h
 * @author Brient Nathan
 * @version 2.0
 * @date 1/12/2021
 * @brief adminPositioning entity header file
 *
 */


#ifndef SRC_POCKET_V1_ADMINPOSITIONING_H_
#define SRC_POCKET_V1_ADMINPOSITIONING_H_


#include "../../commun.h"


/************************ PROTOTYPE DECLARATION **************************************/

/** \fn extern void adminPositioning_new()
 *  \brief Function dedicated to initialize the adminPositioning
 */
extern void adminPositioning_new();

/** \fn extern void adminPositioning_free()
 *  \brief Function dedicated to free the adminPositioning
 */
extern void adminPositioning_free();

/** \fn extern void adminPositioning_start ()
 *  \brief Function dedicated to start the adminPositioning
 */
extern void adminPositioning_start();

/** \fn extern void adminPositioning_stop()
 *  \brief Function dedicated to stop the adminPositioning
 */
extern void adminPositioning_stop();

/** \fn extern void adminPositioning_getPosition()
 *  \brief Function dedicated to obtain the actual position of the robot
 * \return retourne la position du robot
 */
extern Position_data_t * adminPositioning_getPosition();

/************************ END OF PROTOTYPE DECLARATION **************************************/

#endif /* SRC_POCKET_V1_ADMINPOSITIONING_H_ */
