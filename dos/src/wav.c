/*
 * wav.c
 *
 *  Created on: Apr 30, 2017
 *      Author: netdex
 */

#include "wav.h"

struct wav_header wav_readheader(FILE *file){
	struct wav_header wh;
	read_bin(file, wh.ChunkID, 4);
	read_int(file, &wh.ChunkSize);
	read_bin(file, wh.Format, 4);
	read_bin(file, wh.Subchunk1ID, 4);
	read_int(file, &wh.Subchunk1Size);
	read_word(file, &wh.AudioFormat);
	read_word(file, &wh.NumChannels);
	read_int(file, &wh.SampleRate);
	read_int(file, &wh.ByteRate);
	read_word(file, &wh.BlockAlign);
	read_word(file, &wh.BitsPerSample);
	read_bin(file, wh.Subchunk2ID, 4);
	read_int(file, &wh.Subchunk2Size);
	return wh;
}

int wav_readdata(FILE *file, byte *buf, int offset, int count){
	return fread(buf + offset, 1, count, file);
}
