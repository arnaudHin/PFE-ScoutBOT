/**
 * @file proxy_pilot.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "commun.h"



/**
     * Send a specific direction to the Pilot.
     * @param direction Direction the direction to communication to Pilot.
     */
extern void proxy_pilot_set_direction(Direction_e dir);




/**
     * Ask Pilot to immediately stop the robot.
     */
extern void proxy_signal_emergency_stop(void);
