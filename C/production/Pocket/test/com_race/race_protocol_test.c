/**
 * @file  race_protocol_test.c
 * @author Brient Nathan
 * @version 1.0
 * @date 
 * @brief test race_protocol.c
 */

#include "../../src/com_race/race_protocol.c"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../CMocka/include/cmocka.h"
#include "../util.h"

#define BITS_SIZE_16 2

/**
 * @struct ParametersTestCalculDistancePosition
 *
 * @brief Structure des donnees passees en parametre des fonctions de test pour la conversion des Position
 * en tableau d'octet.
 */
typedef struct {
    Message_with_race_t messageToEncode;            /**< Le message a encoder*/
    uint8_t expectedBuffer[4];    /**< Le buffer attendue */
} ParametersTestEncode;

	// 	DEFAULT=0, 1010
	// LEFT,1011
	// RIGHT,1012
	// FORWARD,1013
	// BACKWARD,1014
	// STOP,1015
	// BREAK, 1016
/**
 * @brief Tableau contenant les donnees de test pour l'encodage.
 *
 */
static ParametersTestEncode parametersTestEncode[] = {
    //                                                                     | <---------X---------> | <---------Y---------> |
    {.messageToEncode = {.command = 1,.data={.direction= DEFAULT},.sizeData=1} , .expectedBuffer = {1,0,1,0}},
    {.messageToEncode = {.command = 1,.data={.direction= LEFT},.sizeData=1} , .expectedBuffer = {1,0,1,1}},
    {.messageToEncode = {.command = 1,.data={.direction= RIGHT},.sizeData=1} , .expectedBuffer = {1,0,1,2}},
    {.messageToEncode = {.command = 1,.data={.direction= FORWARD},.sizeData=1} , .expectedBuffer = {1,0,1,3}},
    {.messageToEncode = {.command = 1,.data={.direction= BACKWARD},.sizeData=1} , .expectedBuffer = {1,0,1,4}},
    {.messageToEncode = {.command = 1,.data={.direction= STOP},.sizeData=1} , .expectedBuffer = {1,0,1,5}},
    {.messageToEncode = {.command = 1,.data={.direction= BREAK},.sizeData=1} , .expectedBuffer = {1,0,1,6}}
};

const int int16_max = 32767;
const int uint16_max = 65535;

static int set_up(void **state) {
	race_protocol_init();
	return 0;
}

static int tear_down(void **state) {

	return 0;
}


static void test_convert_uint16_to_2_bytes_null(){ //Litle Endian format

	unsigned char bits_null[BITS_SIZE_16];
	unsigned char return_null[BITS_SIZE_16] = {0x00,0x00};

	uint16_t result = 0;
	convert_uint16_to_2_bytes(bits_null,result);

	assert_int_equal(bits_null[0],return_null[0]);
	assert_int_equal(bits_null[1],return_null[1]);
}


static void test_convert_uint16_to_2_bytes_max(){ //Litle Endian format

	unsigned char bits_max[BITS_SIZE_16];
	unsigned char return_max[BITS_SIZE_16] = {0xFF,0xFF};

	uint16_t result = uint16_max;
	convert_uint16_to_2_bytes(bits_max,result);

	assert_int_equal(bits_max[0],return_max[0]);
	assert_int_equal(bits_max[1],return_max[1]);

}

static void test_convert_uint16_to_2_bytes_positive(){ //Litle Endian format

	unsigned char bits_return_1[BITS_SIZE_16] = {0x00,0x0F}; // assuming char is 8 bits
	unsigned char bits_return_2[BITS_SIZE_16] = {0x00,0xFF}; // assuming char is 8 bits
	unsigned char bits_return_3[BITS_SIZE_16] = {0x0F,0xFF} ; // assuming char is 8 bits
	unsigned char bits_return_4[BITS_SIZE_16] = {0x7F,0xFF} ; // assuming char is 8 bits
	unsigned char bits_return_5[BITS_SIZE_16] = {0xFF,0xFE} ; // assuming char is 8 bits

	unsigned char bits_sample_1[BITS_SIZE_16]; // assuming char is 8 bits
	unsigned char bits_sample_2[BITS_SIZE_16]; // assuming char is 8 bits
	unsigned char bits_sample_3[BITS_SIZE_16]; // assuming char is 8 bits
	unsigned char bits_sample_4[BITS_SIZE_16]; // assuming char is 8 bits
	unsigned char bits_sample_5[BITS_SIZE_16]; // assuming char is 8 bits


	uint16_t value_sample_1 = 15, value_sample_2 = 255, value_sample_3 = 4095, value_sample_4 = int16_max, value_sample_5 = uint16_max;
	convert_uint16_to_2_bytes(bits_sample_1,value_sample_1);
	convert_uint16_to_2_bytes(bits_sample_2,value_sample_2);
	convert_uint16_to_2_bytes(bits_sample_3,value_sample_3);
	convert_uint16_to_2_bytes(bits_sample_4,value_sample_4);
	convert_uint16_to_2_bytes(bits_sample_5,value_sample_5);

	assert_int_equal(bits_sample_1[0] ,bits_return_1[0]);
	assert_int_equal(bits_sample_1[1] ,bits_return_1[1]);

	assert_int_equal(bits_sample_2[0] ,bits_return_2[0]);
	assert_int_equal(bits_sample_2[1] ,bits_return_2[1]);

	assert_int_equal(bits_sample_3[0] ,bits_return_3[0]);
	assert_int_equal(bits_sample_3[1] ,bits_return_3[1]);

	assert_int_equal(bits_sample_4[0] ,bits_return_4[0]);
	assert_int_equal(bits_sample_4[1] ,bits_return_4[1]);

	assert_int_equal(bits_sample_5[0] ,bits_return_5[0]);
	assert_int_equal(bits_sample_5[1] ,bits_return_5[1]+1);

}

//test de race_protocol_encode(Message_with_race_t * messageToEncode, uint8_t * bufferToEncode)

static void test_race_protocol_encode_direction_message(void** state){ //Litle Endian format
	// Inputs
		// Pilot_message_r my_pilot_message_test;

	ParametersTestEncode* param =(ParametersTestEncode*) * state;


	//Initialisation
	uint8_t bufferResult[4];
	memset(bufferResult, 0x00, sizeof(bufferResult) );

	//Call function
	race_protocol_encode(&param->messageToEncode,bufferResult);
	//Assert
	assert_int_equal(bufferResult[0],param->expectedBuffer[0]);
	assert_int_equal(bufferResult[1],param->expectedBuffer[1]);
	assert_int_equal(bufferResult[2],param->expectedBuffer[2]);
	assert_int_equal(bufferResult[3],param->expectedBuffer[3]);

	

}

static const struct CMUnitTest tests[] =
{
	//test de conversion de uint16 a 2 octets
	cmocka_unit_test(test_convert_uint16_to_2_bytes_null),
   	cmocka_unit_test(test_convert_uint16_to_2_bytes_max),
   	cmocka_unit_test(test_convert_uint16_to_2_bytes_positive),
   
	//test de l'encodage des messages a envoyer a race
   	cmocka_unit_test_prestate(test_race_protocol_encode_direction_message, &(parametersTestEncode[0])),
   	cmocka_unit_test_prestate(test_race_protocol_encode_direction_message, &(parametersTestEncode[1])),
   	cmocka_unit_test_prestate(test_race_protocol_encode_direction_message, &(parametersTestEncode[2])),
   	cmocka_unit_test_prestate(test_race_protocol_encode_direction_message, &(parametersTestEncode[3])),
   	cmocka_unit_test_prestate(test_race_protocol_encode_direction_message, &(parametersTestEncode[4])),
   	cmocka_unit_test_prestate(test_race_protocol_encode_direction_message, &(parametersTestEncode[5])),
   	cmocka_unit_test_prestate(test_race_protocol_encode_direction_message, &(parametersTestEncode[6])),
   

};


int com_race_protocol_run_tests(){
    return cmocka_run_group_tests_name("test of race_protocol",tests,set_up,tear_down);
}
