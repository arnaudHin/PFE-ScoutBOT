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


/** \fn extern void adminpositioning_signal_start ()
 *  \brief Function dedicated to start the adminPositioning
 */
extern void adminpositioning_signal_start();

/** \fn extern void adminpositioning_signal_stop()
 *  \brief Function dedicated to stop the adminPositioning
 */
extern void adminpositioning_signal_stop();

/** \fn extern void adminpositioning_signal_ackPositionData()
 *  \brief Function dedicated to obtain the actual position of the robot
 * \return retourne la position du robot
 */
extern void adminpositioning_signal_ackPositionData();

/**
 * @brief 
 * 
 * @param posData 
 */
extern void adminpositioning_setPositionData(Position_data_t * posData);



/************************ END OF PROTOTYPE DECLARATION **************************************/

#endif /* SRC_POCKET_V1_ADMINPOSITIONING_H_ */
