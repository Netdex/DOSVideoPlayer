#include "decoder.h"

LZ4_streamDecode_t lz4StreamDecode_body;
LZ4_streamDecode_t* lz4StreamDecode = &lz4StreamDecode_body;

char decBuf[2][BLOCK_BYTES];
int decBufIndex = 0;

char cmpBuf[LZ4_COMPRESSBOUND(BLOCK_BYTES)];

void read_byte(FILE* fp, byte* i) {
	if (fread(i, sizeof(*i), 1, fp) != 1)
		raise_error(ERROR_DECODE);
}

void read_word(FILE* fp, word *i) {
	if (fread(i, sizeof(*i), 1, fp) != 1)
		raise_error(ERROR_DECODE);
}

void read_int(FILE* fp, int* i) {
	if (fread(i, sizeof(*i), 1, fp) != 1)
		raise_error(ERROR_DECODE);
}

void read_bin(FILE* fp, void* array, size_t arrayBytes) {
	if (fread(array, 1, arrayBytes, fp) != arrayBytes)
		raise_error(ERROR_DECODE);
}

struct video_header decode_video_header(FILE *file) {
	struct video_header hd;
	read_word(file, &hd.frame_count);
	read_byte(file, &hd.frame_rate);
	return hd;
}

void decode_video_reset() {
	LZ4_setStreamDecode(lz4StreamDecode, NULL, 0);
	decBufIndex = 0;
}

void decode_video_frame(FILE *file, byte *palette, byte* palette_size,
		byte *dest) {
	int cmpBytes = 0;
	// decode palette
	read_byte(file, palette_size);
	(*palette_size)++;
	read_bin(file, palette, 3 * *palette_size);

	// decode frame data
	read_int(file, &cmpBytes);
	read_bin(file, cmpBuf, (size_t) cmpBytes);

	char* const decPtr = decBuf[decBufIndex];
	const int decBytes = LZ4_decompress_safe_continue(lz4StreamDecode, cmpBuf,
			decPtr, cmpBytes, BLOCK_BYTES);
	if (decBytes <= 0) {
		raise_error(ERROR_DECODE);
	}
	memcpy(dest, decPtr, decBytes);

	decBufIndex = (decBufIndex + 1) % 2;
}

struct audio_header decode_audio_header(FILE *file) {
	struct audio_header ah;
	read_word(file, &ah.frame_count);
	read_byte(file, &ah.frame_rate);
	return ah;
}

struct audio_frame decode_audio_frame(FILE *file) {
	struct audio_frame af;
	read_word(file, &af.frame);
	read_byte(file, &af.action);
	if (af.action)
		read_word(file, &af.frequency);
	return af;
}
