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
static void remote_protocol_encodeData(uint8_t *bufferWrite ,  Message_to_jump_t *messageToWrite);

static uint16_t remote_protocol_convert_two_bytes_into_uint16(uint8_t first, uint8_t second);
static void remote_protocol_convert_uint16_to_2_bytes(uint8_t * byte, uint16_t value);

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

    switch (messageToWrite->command)
    {
    case SET_DATA:
        remote_protocol_encodeData(bufferWrite, messageToWrite);
        break;
    
    default:
        break;
    }




}



static void remote_protocol_encodeData(uint8_t *bufferWrite ,  Message_to_jump_t *messageToWrite){
    uint16_t index = 0;
    uint16_t i=0;

    uint8_t cmd = (uint8_t) messageToWrite->command;
    memcpy( bufferWrite, &cmd, sizeof(uint8_t) );
    index++;

    uint8_t buf2[sizeof(uint16_t)];
    remote_protocol_convert_uint16_to_2_bytes(buf2, messageToWrite->size);
    memcpy( (bufferWrite + index), &buf2, sizeof(uint16_t) );
    index +=2;  

// DATA Positioning
    uint8_t buf4[sizeof(float)];
    memcpy( buf4, &messageToWrite->data.positionData.x, sizeof(float)  );
    memcpy( (bufferWrite+index), buf4, sizeof(float) );
    index += sizeof(float);
    
    memcpy( buf4, &messageToWrite->data.positionData.y, sizeof(float)  );
    memcpy( (bufferWrite+index), buf4, sizeof(float) );
    index += sizeof(float);   

    cmd = (uint8_t) messageToWrite->data.positionData.room;
    memcpy( (bufferWrite+index), &cmd, sizeof(uint8_t) );
    index++;
//DATA Mapping
    for (i = 0; i < LIDAR_TOTAL_DEGREE; i++)
    {
        remote_protocol_convert_uint16_to_2_bytes(buf2, (int16_t) messageToWrite->data.lidarData.X_buffer[i]);
        memcpy( (bufferWrite + index), &buf2, sizeof(int16_t) );
        index += sizeof(int16_t);         
    }

    for (i = 0; i < LIDAR_TOTAL_DEGREE; i++)
    {
        remote_protocol_convert_uint16_to_2_bytes(buf2, (int16_t) messageToWrite->data.lidarData.Y_buffer[i]);
        memcpy( (bufferWrite + index), &buf2, sizeof(int16_t) );
        index += sizeof(int16_t);         
    }
    

}



static void remote_protocol_decodeCommandSize(uint8_t *bufferRead ,  Message_from_jump_t *messageToRead){
    fprintf(stderr, "\n ---DECODE CMD+SIZE --- \n");
    for (size_t i = 0; i < 3; i++)
    {
        fprintf(stderr, "buffToRead[%ld]=%d\n", i, (uint8_t) *(bufferRead+i) );
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

static void remote_protocol_convert_uint16_to_2_bytes(uint8_t * byte, uint16_t value){

	byte[0] = (value >> 8) & 0xFF;
	byte[1] = value & 0xFF;
}


static void remote_protocol_convert_float_to_4_bytes(uint8_t * byte, uint16_t value){

	byte[0] = (value >> 32) & 0xFFFF;
	byte[1] = (value >> 16) & 0xFFFF;
	byte[2] = (value >> 8) & 0xFFFF;
	byte[3] = value & 0xFF;
}





