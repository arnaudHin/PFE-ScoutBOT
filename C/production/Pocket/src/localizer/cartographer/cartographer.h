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

/**
 * @brief 
 */
extern void cartographer_signal_ask4data();

extern void cartographer_signal_start();

extern void cartographer_signal_stop();

extern uint8_t cartographer_getStartState();

extern void cartographer_signal_setLidarData();

extern void cartographer_signal_setPositionData();



/************************ END OF PROTOTYPE DECLARATION **************************************/

#endif /* SRC_POCKET_V1_CARTOGRAPHER_H_ */
