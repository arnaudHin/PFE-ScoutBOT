/**
 * @file  proxy_robot_test.c
 * @author Brient Nathan
 * @version 1.0
 * @date 
 * @brief test proxy_robot.c
 */

#include "../../src/com_race/proxy_robot.c"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../CMocka/include/cmocka.h"
#include "../util.h"


static int set_up(void **state)
{
	return 0;
}

static int tear_down(void **state)
{
	return 0;
}


/**
 * @struct ParametersTestCalculDistancePosition
 *
 * @brief 
 */
typedef struct {
    DATA_to_race_t dataToRace;            
    Message_with_race_t myMessageToRace;    
} ParametersProxyDir;

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
static ParametersProxyDir parametersProxyDir[] = {
    //                                                                     | <---------X---------> | <---------Y---------> |
    {.dataToRace = {.direction= DEFAULT}, .myMessageToRace = {.command = TRY_DIR }},
    {.dataToRace = {.direction= LEFT}, .myMessageToRace = {.command = TRY_DIR }},
    {.dataToRace = {.direction= RIGHT}, .myMessageToRace = {.command = TRY_DIR }},
    {.dataToRace = {.direction= FORWARD}, .myMessageToRace = {.command = TRY_DIR }},
    {.dataToRace = {.direction= BACKWARD}, .myMessageToRace = {.command = TRY_DIR }},
    {.dataToRace = {.direction= STOP}, .myMessageToRace = {.command = TRY_DIR }},
    {.dataToRace = {.direction= BREAK}, .myMessageToRace = {.command = TRY_DIR }}
};

/**
 * @brief Test function of the proxyTourDirector_setCurrentTourData function
 * 
 */
static void test_proxy_robot_try_dir(void **state)
{
    ParametersProxyDir * param =(ParametersProxyDir*) * state;

    DATA_to_race_t dataToRace = param->dataToRace;
	myMessageToRace.sizeData = sizeof(dataToRace);
	myMessageToRace.data = dataToRace;

	uint8_t bufferToSend[4];
	memset(bufferToSend, 0x00, sizeof(bufferToSend) );
	//spy
	expect_function_call(__wrap_race_protocol_encode);
	expect_value(__wrap_race_protocol_encode, __messageToEncode, &myMessageToRace);
	//expect_value(__wrap_race_protocol_encode, __bufferToEncode, bufferToSend);

	// //spy
	expect_function_call(__wrap_postman_race_send_message);
	// expect_value(__wrap_postman_race_send_message, __bufferToWrite.direction, dataToRace.direction);
	// expect_value(__wrap_postman_race_send_message, __bytesToSend, myMessageToRace.sizeData);

	proxy_robot_try_dir(&dataToRace);

}


/**
 * @brief Test suite for the module.
 * 
 */
static const struct CMUnitTest tests[] =
	{
   	cmocka_unit_test_prestate(test_proxy_robot_try_dir, &(parametersProxyDir[0])),
    cmocka_unit_test_prestate(test_proxy_robot_try_dir, &(parametersProxyDir[1])),
    cmocka_unit_test_prestate(test_proxy_robot_try_dir, &(parametersProxyDir[2])),
    cmocka_unit_test_prestate(test_proxy_robot_try_dir, &(parametersProxyDir[3])),
    cmocka_unit_test_prestate(test_proxy_robot_try_dir, &(parametersProxyDir[4])),
    cmocka_unit_test_prestate(test_proxy_robot_try_dir, &(parametersProxyDir[5])),
    cmocka_unit_test_prestate(test_proxy_robot_try_dir, &(parametersProxyDir[6])),

};

int proxy_robot_run_tests(){
    return cmocka_run_group_tests_name("test of proxy_robot_test",tests,set_up,tear_down);
}
