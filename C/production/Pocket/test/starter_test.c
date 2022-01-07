/*
 * main_test.c
 *
 *  @file main_test.c
 *  @brief Testsuite main
 *  @author Debus Alexy
 *  @date 29/04/20
 *  @version 0.1
 */

/**
 * \file main_test.c
 *
 */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// define #define NDEBUG (1) in src/util.h

/** Number of test suites to run.. */
#define NB_SUITE_TESTS 1


extern int com_race_postman_run_tests(void);
extern int com_race_protocol_run_tests(void);

/** List of test suites to run. */
int (* suite_tests[])(void) =
{
	// Call here the different test suites

	/*pilot_run_tests,
	robot_state_run_tests,
	display_run_tests,
	geographer_run_tests,
	copilot_run_tests,*/
	// com_pocket_dispatcher_pocket_run_tests,
	com_race_protocol_run_tests,
	com_race_postman_run_tests

	/*network_pilot_run_tests // put this one in last (one of the tests takes time)
	*/
	

};


/*
 * Generic code for launching test suites.
 */

static void test_runner();
static void display_help(char * nom);

/**
 * Fonction principal du programme de test.
 */
int main(int argc, char * argv[])
{
	if ( (argc > 1) )
	{
		if (! strcmp(argv[1], "-text")) {
			cmocka_set_message_output(CM_OUTPUT_STDOUT);
		} else
		if (! strcmp(argv[1], "-subunit")) {
			cmocka_set_message_output(CM_OUTPUT_SUBUNIT);
		} else
		if (! strcmp(argv[1], "-tap")) {
			cmocka_set_message_output(CM_OUTPUT_TAP);
		} else
		if (! strcmp(argv[1], "-xml")) {
			cmocka_set_message_output(CM_OUTPUT_XML);
		} else {
			display_help(argv[0]);
		}
	}

	test_runner();
	return 0;
}


/**
 * Lancement de chacune des suites de test avec CMocka.
 */
static void test_runner()
{
	int i;

	printf("Test suite for the program to be tested \n");
	for (i = 0; i < NB_SUITE_TESTS; i++)
	{
		suite_tests[i]();
	}
}


/**
 * Affichage d'une aide sur la sortie standard pour utiliser le programme de test.
 */
static void display_help(char * nom)
{
	printf("Use : %s [-text] [-subunit] [-tap] [-xml]\n", nom);

	exit(0);
}

