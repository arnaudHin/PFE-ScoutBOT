// SPDX-identifier: GPL-2.0
/*
 * Copyright (C) STMicroelectronics SA 2018
 *
 * Authors: Jean-Christophe Trotin <jean-christophe.trotin@st.com>
 *
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int i =11;

	printf("\nUser space example: hello world from STMicroelectronics\n");
	setbuf(stdout,NULL);
	while (i--) {
		printf("%i ", i);
		sleep(1);
	}
	printf("\nUser space example: goodbye from STMicroelectronics\n");

	return(0);
}
