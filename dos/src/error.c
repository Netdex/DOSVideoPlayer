/*
 * error.c
 *
 *  Created on: Apr 17, 2017
 *      Author: netdex
 */

#include "error.h"

void raise_error(const char *code){
	vga_set_mode(TEXT_MODE);
	printf("FATAL ERROR: %s\n"
			"\n"
			"The program has terminated.\n", code);
	exit(-1);
}
