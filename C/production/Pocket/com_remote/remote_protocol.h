/**
 * @file : remote_protocol.c
 * @author : Arnaud Hincelin
 * @date : 25/11/2021
 * @brief : 
 */



#ifndef REMOTE_PROTOCOL_H__
#define REMOTE_PROTOCOL_H__


#include "../commun.h"






extern void remote_protocol_decode(uint8_t *bufferRead ,  Message_from_jump_t *messageToRead, __ssize_t byteToDecode);














#endif /* REMOTE_PROTOCOL_H__ */


