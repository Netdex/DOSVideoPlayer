/*
 * wav.h
 *
 *  Created on: Apr 30, 2017
 *      Author: netdex
 *
 *      For reading Microsoft 8-bit unsigned mono WAV
 */

#ifndef INCLUDE_WAV_H_
#define INCLUDE_WAV_H_

#include "fileio.h"
#include "types.h"

struct wav_header {
	byte ChunkID[4];		// "RIFF"
	int ChunkSize;
	byte Format[4];			// "WAVE"
	byte Subchunk1ID[4];	// "fmt "
	int Subchunk1Size;
	word AudioFormat;
	word NumChannels;
	int SampleRate;
	int ByteRate;
	word BlockAlign;
	word BitsPerSample;
	byte Subchunk2ID[4];	// "data"
	int Subchunk2Size;
};

struct wav_header wav_readheader(FILE *file);
int wav_readdata(FILE *file, byte *buf, int offset, int count);

#endif /* INCLUDE_WAV_H_ */
