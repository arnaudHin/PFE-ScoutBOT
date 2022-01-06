/**
 * @file proxy_cartographer.c
 * @author AdrienLE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "postman_jump.h"
#include "protocol_jump.h"
#include "commun.h"
#include <stddef.h>

static Message_to_pocket_t messageToPocket;

extern void proxyCartographer_ask4data()
{
    messageToPocket.cmd = ASK_4_DATA;
    messageToPocket.sizeData = 0;
    messageToPocket.data.direction = DEFAULT;

    uint16_t bytesToSend = 1 + 2 + messageToPocket.sizeData;
    uint8_t bufferToSend[bytesToSend]; //1+2+(720*2)
    //memset(bufferToSend, 0x01, sizeof(bufferToSend) );

    protocol_jump_encode(bufferToSend, &messageToPocket, bytesToSend);

    //postman_jumpC_send_msg(bufferToSend);
}

extern void proxyCartographer_signal_start()
{
    messageToPocket.cmd = ASK_4_DATA;
    messageToPocket.sizeData = 0;
    messageToPocket.data.direction = DEFAULT;

    uint16_t bytesToSend = 1 + 2 + messageToPocket.sizeData;
    uint8_t bufferToSend[bytesToSend]; //1+2+(720*2) + = 1443 
    //memset(bufferToSend, 0x01, sizeof(bufferToSend) );

    protocol_jump_encode(bufferToSend, &messageToPocket, bytesToSend);

    //postman_jumpC_send_msg(bufferToSend);
}

extern void proxyCartographer_signal_stop()
{
    messageToPocket.cmd= ASK_QUIT;
    messageToPocket.sizeData = 0;
    messageToPocket.data.direction = DEFAULT;

    uint16_t bytesToSend = 1 + 2 + messageToPocket.sizeData;
    uint8_t bufferToSend[bytesToSend]; //1+2+(720*2)
    //memset(bufferToSend, 0x01, sizeof(bufferToSend) );

    protocol_jump_encode(bufferToSend, &messageToPocket, bytesToSend);
    //postman_jumpC_send_msg(bufferToSend);
}