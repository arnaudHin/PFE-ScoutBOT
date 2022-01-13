/**
 * @file  dispacther_race_test.c
 * @author Brient Nathan
 * @version 1.0
 * @date 
 * @brief test dispacther_race.c
 */

#include "../../src/com_race/dispatcher_race.h"

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
 * @brief Test suite for the module.
 * 
 */
static const struct CMUnitTest tests[] =
	{


};

int dispatcher_race_run_tests(){
    return cmocka_run_group_tests_name("test of proxy_robot_test",tests,set_up,tear_down);
}
