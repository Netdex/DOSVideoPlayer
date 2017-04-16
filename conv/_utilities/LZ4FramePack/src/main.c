// LZ4 streaming API example : double buffer
// Copyright : Takayuki Matsuoka

#ifdef _MSC_VER    /* Visual Studio */
#  define _CRT_SECURE_NO_WARNINGS
#  define snprintf sprintf_s
#endif
#include "lz4.h"
#include "lz4hc.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum {
	BLOCK_BYTES = 320 * 200
};

size_t write_int(FILE* fp, int i) {
	return fwrite(&i, sizeof(i), 1, fp);
}

size_t write_short(FILE* fp, short i) {
	return fwrite(&i, sizeof(i), 1, fp);
}

size_t write_byte(FILE* fp, unsigned char i) {
	return fwrite(&i, sizeof(i), 1, fp);
}

size_t write_bin(FILE* fp, const void* array, size_t arrayBytes) {
	return fwrite(array, 1, arrayBytes, fp);
}

size_t read_int(FILE* fp, int* i) {
	return fread(i, sizeof(*i), 1, fp);
}

size_t read_short(FILE* fp, unsigned short* i) {
	return fread(i, sizeof(*i), 1, fp);
}

size_t read_byte(FILE* fp, unsigned char* i) {
	return fread(i, sizeof(*i), 1, fp);
}

size_t read_bin(FILE* fp, void* array, size_t arrayBytes) {
	return fread(array, 1, arrayBytes, fp);
}

void lz4_frame_pack(FILE* outFp, FILE* inpFp) {
	LZ4_streamHC_t lz4Stream_body;
	LZ4_streamHC_t* lz4Stream = &lz4Stream_body;

	char inpBuf[2][BLOCK_BYTES];
	int inpBufIndex = 0;

	// copy header
	unsigned short frame_count;
	unsigned char fps;
	read_short(inpFp, &frame_count);
	read_byte(inpFp, &fps);
	write_short(outFp, frame_count);
	write_byte(outFp, fps);

	char paletteBuf[768];

	LZ4_resetStreamHC(lz4Stream, 9);

	for (int f = 0; f < frame_count; f++) {
		// write palette information
		read_bin(inpFp, paletteBuf, 768);
		write_bin(outFp, paletteBuf, 768);

		char* const inpPtr = inpBuf[inpBufIndex];
		const int inpBytes = (int) read_bin(inpFp, inpPtr, BLOCK_BYTES);
		if (0 == inpBytes) {
			printf("file ended early!\n");
			break;
		}
		{
			char cmpBuf[LZ4_COMPRESSBOUND(BLOCK_BYTES)];
			const int cmpBytes = LZ4_compress_HC_continue(lz4Stream, inpPtr,
					cmpBuf, inpBytes, sizeof(cmpBuf));
			if (cmpBytes <= 0) {
				printf("file ended early!\n");
				break;
			}
			write_int(outFp, cmpBytes);
			write_bin(outFp, cmpBuf, (size_t) cmpBytes);
		}
		inpBufIndex = (inpBufIndex + 1) % 2;
	}

	write_int(outFp, 0);
}

int main(int argc, char* argv[]) {
	char inpFilename[256] = { 0 };
	char lz4Filename[256] = { 0 };

	if (argc < 2) {
		printf("Please specify input filename\n");
		return 0;
	}

	snprintf(inpFilename, 256, "%s", argv[1]);
	snprintf(lz4Filename, 256, "%s.lz4hc-%d", argv[1], BLOCK_BYTES);

	printf("inp = [%s]\n", inpFilename);
	printf("lz4 = [%s]\n", lz4Filename);

	// compress
	{
		FILE* inpFp = fopen(inpFilename, "rb");
		FILE* outFp = fopen(lz4Filename, "wb");

		printf("compress : %s -> %s\n", inpFilename, lz4Filename);
		lz4_frame_pack(outFp, inpFp);
		printf("compress : done\n");

		fclose(outFp);
		fclose(inpFp);
	}

	return 0;
}
