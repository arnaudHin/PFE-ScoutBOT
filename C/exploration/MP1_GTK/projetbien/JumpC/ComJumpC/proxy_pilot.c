/**
 * @file proxy_pilot.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "postman_jump.h"
#include "proxy_pilot.h"
#include "commun.h"

#define SIZE_MSG_CMD (1)
#define SIZE_MSG_SIZE (3)

static void proxy_simple_message(Network_CMD cmd)
{
    int size = SIZE_MSG_CMD + SIZE_MSG_SIZE;
    unsigned char byte[size];

    // Command
    byte[0] = (uint16_t)cmd;

    // Size
    for (int i = 1; i < size; i++)
    {
        byte[i] = 0x00;
    }
    // Send
    postman_jumpC_send_msg(byte, size);
}

void proxy_set_direction(Direction direction)
{
    Network_CMD cmd = NOP;
    switch (direction)
    {
    case FORWARD:
        cmd = DIR_FORWARD;
        proxy_simple_message(cmd);
        break;
    case BACKWARD:
        cmd = DIR_BACKWARD;
        proxy_simple_message(cmd);
        break;
    case LEFT:
        cmd = DIR_LEFT;
        proxy_simple_message(cmd);
        break;
    case RIGHT:
        cmd = DIR_RIGHT;
        proxy_simple_message(cmd);
        break;
    case STOP_IT:
        cmd = STOP;
        proxy_simple_message(cmd);
    default:
        cmd = NOP;
    }
}

// void proxy_signal_emergency_stop()
// {
//     Network_CMD cmd = STOP_EMERGENCY;
// }
