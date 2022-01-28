/**
 * @file protocol_jump.h
 * @author Adrien LE ROUX (adrien.leroux@reseau.eseo.fr)
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "commun.h"

/**
 * @brief 
 * 
 * @param bufferRead 
 * @param messageToRead 
 * @param byteToDecode 
 */
extern void protocol_jump_decode(uint8_t *bufferRead, Message_from_pocket_t *messageToRead, __ssize_t byteToDecode);


/**
 * @brief 
 * 
 * @param bufferWrite 
 * @param messageToWrite 
 * @param byteToEncode 
 */
extern void protocol_jump_encode(uint8_t *bufferWrite ,  Message_to_pocket_t *messageToWrite, __ssize_t byteToEncode);