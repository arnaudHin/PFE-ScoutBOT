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
#define SIZE_MSG_SIZE (2)
#define SIZE_MSG_BUFFER (1)

static void proxy_simple_message(Network_CMD buffer, CMD_e cmd)
{
    Network_msg new_msg;
    new_msg.size = SIZE_MSG_CMD + SIZE_MSG_SIZE + SIZE_MSG_BUFFER;
    new_msg.cmd = cmd;
    new_msg.bufferToSend[0] = buffer;
    postman_jumpC_send_msg(new_msg);
}

void proxy_set_direction(Direction direction)
{
    Network_CMD buffer = NOP;
    CMD_e cmd = NOP_CMD;
    switch (direction)
    {
    case FORWARD:
        buffer = DIR_FORWARD;
        cmd = ASK_TRY_DIR;
        proxy_simple_message(buffer, cmd);
        break;
    case BACKWARD:
        buffer = DIR_BACKWARD;
        cmd = ASK_TRY_DIR;
        proxy_simple_message(buffer, cmd);

        break;
    case LEFT:
        buffer = DIR_LEFT;
        cmd = ASK_TRY_DIR;
        proxy_simple_message(buffer, cmd);

        break;
    case RIGHT:
        buffer = DIR_RIGHT;
        cmd = ASK_TRY_DIR;
        proxy_simple_message(buffer, cmd);

        break;
    case STOP_IT:
        buffer = STOP;
        cmd = ASK_TRY_DIR;
        proxy_simple_message(buffer, cmd);
    
    default:
        buffer = NOP;
    }
}

// void proxy_signal_emergency_stop()
// {
//     Network_CMD cmd = STOP_EMERGENCY;
// }
