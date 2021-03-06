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

typedef enum
{
     NOP_DIR = 0,
     LEFT,
     RIGHT,
     FORWARD,
     BACKWARD,
     STOP_IT,
     BREAK,
     STOP_EMERGENCY,
} Direction;

/**
     * Send a specific direction to the Pilot.
     * @param direction Direction the direction to communication to Pilot.
     */
extern void proxy_set_direction(Direction direction);

/**
     * Ask Pilot to immediately stop the robot.
     */
extern void proxy_signal_emergency_stop(void);
