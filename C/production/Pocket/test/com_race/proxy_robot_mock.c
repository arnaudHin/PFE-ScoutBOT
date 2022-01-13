/**
 * @file  proxy_robot_mock.c
 * @author Brient Nathan
 * @version 1.0
 * @date 
 * @brief mock to test proxy_robot.c
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stddef.h> 
#include <stdint.h> 
#include <sys/socket.h>
#include <string.h>
#include "cmocka.h"

#include "../../src/com_race/race_protocol.h"

void __wrap_race_protocol_encode(Message_with_race_t * __messageToEncode, uint8_t * __bufferToEncode){
    function_called();
    check_expected(__messageToEncode);
    //check_expected(*__bufferToEncode);    
}

void __wrap_postman_race_send_message(uint8_t * __bufferToWrite, uint16_t __bytesToSend){
    function_called();
    // check_expected(__bufferToWrite);
    // check_expected(__bytesToSend);    
}