/**
 * @file  postman_test.c
 * @author Debus Alexy
 * @version 1.0
 * @date 14/06/2020
 * @brief test postman.c
 */

#include "../../src/com_race/postman_race.c"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../CMocka/include/cmocka.h"



static int set_up(void **state) {
	return 0;
}

static int tear_down(void **state) {

	return 0;
}

//Negative 16 bit values are forbidden

static void test_function_1(){ //Litle Endian format



	uint16_t result = 0;
	uint16_t result2 = 0;


	assert_int_equal(result,result2);
}

static const struct CMUnitTest tests[] =
{
   cmocka_unit_test(test_function_1),

};


int com_race_postman_run_tests(){
    return cmocka_run_group_tests_name("test of postman",tests,set_up,tear_down);
}
