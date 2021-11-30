/**
 * @file : remote_protocol.c
 * @author : Arnaud Hincelin
 * @date : 25/11/2021
 * @brief : 
 */

#include "../commun.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



static void remote_protocol_decodeCommandSize(uint8_t *bufferRead ,  Message_from_jump_t *messageToRead);
static void remote_protocol_dataOne(uint8_t *bufferRead ,  Message_from_jump_t *messageToRead);
static uint16_t remote_protocol_convert_two_bytes_into_uint16(uint8_t first, uint8_t second);


/**
 * @brief : 
 * @param : 
 */
extern void remote_protocol_decode(uint8_t *bufferRead ,  Message_from_jump_t *messageToRead, __ssize_t byteToDecode){

    switch (byteToDecode){
    case 3:
        remote_protocol_decodeCommandSize(bufferRead, messageToRead);
        break;
    case 1:
        remote_protocol_dataOne(bufferRead, messageToRead);

    
    default:
        break;
    }
}





extern void remote_protocol_encode(uint8_t *bufferWrite ,  Message_to_jump_t *messageToWrite, __ssize_t byteToEncode){






}







static void remote_protocol_decodeCommandSize(uint8_t *bufferRead ,  Message_from_jump_t *messageToRead){
    fprintf(stderr, "\n ---DECODE CMD+SIZE --- \n");
    for (size_t i = 0; i < 3; i++)
    {
        fprintf(stderr, "buffToRead[%d]=%d\n", i, (uint8_t) *(bufferRead+i) );
    }
    
    uint16_t index = 0;
    
    memcpy( (CMD_from_jump_e*) &messageToRead->command, (uint8_t*) bufferRead, sizeof(uint8_t));
    index++;

    messageToRead->sizeData = remote_protocol_convert_two_bytes_into_uint16( *(bufferRead+1), *(bufferRead+2) );

    fprintf(stderr, "\nCMDreaden : %d | ", messageToRead->command);
    fprintf(stderr, "SIZEreaden : %d\n", messageToRead->sizeData);

    fprintf(stderr, "\n ---END DECODE CMD+SIZE --- \n");
}



static void remote_protocol_dataOne(uint8_t *bufferRead ,  Message_from_jump_t *messageToRead){
    fprintf(stderr, "\n ---DECODE DATA --- \n");
    fprintf(stderr, "buffToRead[0]=%d\n", (uint8_t) *(bufferRead) );

    memcpy( (Direction_e*) &messageToRead->data.direction, (uint8_t*) (bufferRead), sizeof(uint8_t));
    fprintf(stderr, "DIRreaden : %d\n", messageToRead->data.direction);
    fprintf(stderr, "\n ---END DECODE DATA --- \n");
}

static uint16_t remote_protocol_convert_two_bytes_into_uint16(uint8_t first, uint8_t second){

    uint16_t res = 0x0000;
    res = first;
    res = res << 8;
    res |= second;
    return res;
}