/**
 * @file : race_protocol.c
 * @author : Arnaud Hincelin
 * @date : 25/11/2021
 * @brief : 
 */


#include "postman_race.h"
#include "race_protocol.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


static pthread_mutex_t mutexDecode;
static pthread_mutex_t mutexEncode;



static void convert_uint16_to_2_bytes(uint8_t * byte, uint16_t value);



extern void race_protocol_init(){
    int check = pthread_mutex_init(&mutexDecode, NULL);
	if (check == -1){
		perror("ERROR INIT MUTEX");
	}

	check = pthread_mutex_init(&mutexEncode, NULL);
	if (check == -1){
		perror("ERROR INIT MUTEX");
	}
}

//encode messageToEncode et le met dans bufferToEncode
extern void race_protocol_encode(Message_with_race_t * messageToEncode, uint8_t * bufferToEncode){

	pthread_mutex_lock(&mutexEncode);

    uint16_t index = 0;

    uint8_t cmd = (uint8_t) messageToEncode->command;
    memcpy( bufferToEncode, &cmd, sizeof(uint8_t) ); //dans le premier octet on met la commande
    index++;


    // uint8_t * size = calloc(1, sizeof(uint16_t));
    // *size = (uint8_t) messageToEncode->sizeData; 
    // memcpy( (bufferToEncode + index), &size, sizeof(uint16_t) );
    // free(size);
    // index += sizeof(uint16_t);
    uint8_t buf[2];
    convert_uint16_to_2_bytes(buf, messageToEncode->sizeData); //On converti la taille de la data de uint16 a 2bit 
    memcpy( (bufferToEncode + index), &buf, sizeof(uint16_t) ); //on met la taille dans les 2 eme et 3 eme bit
    index +=2;

    switch (messageToEncode->command)
    {
        case TRY_DIR:
            if(1){}
            uint8_t data = (uint8_t) messageToEncode->data.direction;
            memcpy( (bufferToEncode+index), &data, sizeof(uint8_t) );
            
            break;
        
        default:
            break;
    }

	pthread_mutex_unlock(&mutexEncode);
}



extern void race_protocol_decode(Message_with_race_t * messageToDecode, uint8_t * bufferToDecode){

}





/**
 *  \fn void convert_int16_to_2_bytes(unsigned char * byte, uint16_t value)
 *
 *  \brief Function dedicated to convert an uin16_t to an array of bytes (size 2)
 *
 *  \param byte (unsigned char *) : byte where the data will be stored
 *
 *  \param value (uint16_t) : value to convert
 *
 */

static void convert_uint16_to_2_bytes(uint8_t * byte, uint16_t value){

	//value = floor(value * MAP_RATIO -0.5);
	byte[0] = (value >> 8) & 0xFF;
	byte[1] = value & 0xFF;
}






