/**
 * @file : proxy_map.c
 * @author : Arnaud Hincelin
 * @date : 29/11/2021
 * @brief : 
 */


#include "../utils/util.h"
#include "stdlib.h"
#include "remote_protocol.h"
#include "postman_remote.h"
#include "proxy_mapviewer.h"
#include "../commun.h"
#include "stdio.h"


static Message_to_jump_t messageToJumpc;


extern void proxy_mapviewer_send_data(DATA_to_jump_t * dataToSend){
    messageToJumpc.command = SET_DATA;
    messageToJumpc.size = MAX_SIZE_BYTE_DATA_TO_SEND; //720*2+ 2*4+1 = 
    messageToJumpc.data = *dataToSend;

    uint16_t bytesToSend = 1 + 2 + messageToJumpc.size; 
    uint8_t bufferToSend[bytesToSend]; //1+2+(720*2)
    //memset(bufferToSend, 0x01, sizeof(bufferToSend) );

    remote_protocol_encode(bufferToSend, &messageToJumpc, bytesToSend);

    fprintf(stderr, "total bytes sent : %d \n", bytesToSend );

    postman_remote_write(bufferToSend, bytesToSend);
}








