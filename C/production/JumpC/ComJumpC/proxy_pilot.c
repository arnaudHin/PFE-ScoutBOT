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
#include <stdio.h>

#define SIZE_MSG_CMD (1)
#define SIZE_MSG_SIZE (2)
#define SIZE_MSG_BUFFER (1)

Message_to_pocket_t myMsgToPocket;



static void proxy_pilot_ask_try_direction();



extern void proxy_pilot_set_direction(Direction_e dir)
{
    myMsgToPocket.data.direction = dir;
    proxy_pilot_ask_try_direction();
}


static void proxy_pilot_ask_try_direction()
{
    myMsgToPocket.cmd = ASK_TRY_DIR;
    myMsgToPocket.sizeData = 1;
    postman_jumpC_send_msg(&myMsgToPocket);
}



extern void proxy_pilot_ask_quit(){
    
}








