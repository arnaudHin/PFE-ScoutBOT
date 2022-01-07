/**
 * @file  postman_race_test.c
 * @author BRIENT Nathan
 * @version 1.0
 * @date 
 * @brief test postman_race.c
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

static void test_postman_race_send_message(){ //Litle Endian format


}

static const struct CMUnitTest tests[] =
{
   cmocka_unit_test(test_postman_race_send_message),

};


int com_race_postman_run_tests(){
    return cmocka_run_group_tests_name("test of postman",tests,set_up,tear_down);
}
