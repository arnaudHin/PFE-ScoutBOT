/**
 * @file protocol_jump.c
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "commun.h"
#include "protocol_jump.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define SIZE_MSG_CMD (1)
#define SIZE_MSG_SIZE (2)

static void protocol_jump_decodeCommandSize(uint8_t *bufferRead, Message_from_pocket_t *messageToRead);
static void protocol_jump_dataPositions(uint8_t *bufferRead, Message_from_pocket_t *messageToRead);
static uint16_t protocol_jump_convert_two_bytes_into_uint16(uint8_t first, uint8_t second);

extern void protocol_jump_decode(uint8_t *bufferRead, Message_from_pocket_t *messageToRead, __ssize_t byteToDecode)
{
    switch (byteToDecode)
    {
    case 3:
        protocol_jump_decodeCommandSize(bufferRead, messageToRead);
        break;
    case 1449:
        protocol_jump_dataPositions(bufferRead, messageToRead);
        break;
    default:
        break;
    }
}

extern void protocol_jump_encode(uint8_t *bufferWrite, Message_to_pocket_t *messageToWrite, __ssize_t byteToEncode)
{
}

static void protocol_jump_decodeCommandSize(uint8_t *bufferRead, Message_from_pocket_t *messageToRead)
{
    fprintf(stderr, "\n ---DECODE CMD+SIZE --- \n");
    for (size_t i = 0; i < SIZE_MSG_CMD + SIZE_MSG_SIZE + LIDAR_TOTAL_DATA * 2; i++)
    {
        //fprintf(stderr, "buffToRead[%d]=%d\n", i, (uint8_t) * (bufferRead + i));
        fprintf(stderr, "buffToRead[%ld]=%d\n", i, (uint8_t)(bufferRead[i]));
    }

    uint16_t index = 0;

    memcpy((CMD_from_pocket_e *)&messageToRead->command, (uint8_t *)bufferRead, sizeof(uint8_t));
    index++;

    messageToRead->size = protocol_jump_convert_two_bytes_into_uint16(*(bufferRead + 1), *(bufferRead + 2));
    index += 2;

    fprintf(stderr, "\n ---END DECODE CMD+SIZE --- \n");
}

static void protocol_jump_dataPositions(uint8_t *bufferRead, Message_from_pocket_t *messageToRead)
{
    uint16_t index = 0;
    fprintf(stderr, "\n ---DECODE DATA --- \n");
    memcpy(&messageToRead->data.positionData.x, (uint8_t *)bufferRead, sizeof(float));
    index += sizeof(float);

    memcpy(&messageToRead->data.positionData.y, (uint8_t *)bufferRead, sizeof(float));
    index += sizeof(float);

    memcpy((uint8_t)&messageToRead->data.positionData.room, (uint8_t *)bufferRead, sizeof(uint8_t));
    index++;
    for (size_t i = 0; i < LIDAR_TOTAL_DEGREE; i++)
    {
        memcpy((CMD_from_pocket_e *)&messageToRead->data.lidarData.X_buffer, (uint8_t *)bufferRead + index, sizeof(int16_t));
        index += sizeof(int16_t);
    }
    for (size_t i = 0; i < LIDAR_TOTAL_DEGREE; i++)
    {
        memcpy((CMD_from_pocket_e *)&messageToRead->data.lidarData.Y_buffer, (uint8_t *)bufferRead + index, sizeof(int16_t));
        index += sizeof(int16_t);
    }
    fprintf(stderr, "\nCMDreaden : %d | ", messageToRead->command);
    fprintf(stderr, "SIZEreaden : %d\n", messageToRead->size);

    for (size_t i = 0; i < LIDAR_TOTAL_DEGREE; i++)
    {
        fprintf(stderr, "Data lidar x[%ld] : %hn \n", i, messageToRead->data.lidarData.X_buffer);
        fprintf(stderr, "Data lidar y[%ld] : %hn \n", i, messageToRead->data.lidarData.Y_buffer);
    }
    fprintf(stderr, "\n ---END DECODE DATA --- \n");
}

static uint16_t protocol_jump_convert_two_bytes_into_uint16(uint8_t first, uint8_t second)
{
    uint16_t res = 0x0000;
    res = first;
    res = res << 8;
    res |= second;
    return res;
}