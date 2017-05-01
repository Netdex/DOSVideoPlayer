/*
 * fileio.c
 *
 *  Created on: Apr 30, 2017
 *      Author: netdex
 */
#include "fileio.h"

void read_byte(FILE* fp, byte* i) {
	if (fread(i, sizeof(*i), 1, fp) != 1)
		raise_error(ERROR_FILE);
}

void read_word(FILE* fp, word *i) {
	if (fread(i, sizeof(*i), 1, fp) != 1)
		raise_error(ERROR_FILE);
}

void read_int(FILE* fp, int* i) {
	if (fread(i, sizeof(*i), 1, fp) != 1)
		raise_error(ERROR_FILE);
}

void read_bin(FILE* fp, void* array, size_t arrayBytes) {
	if (fread(array, 1, arrayBytes, fp) != arrayBytes)
		raise_error(ERROR_FILE);
}
