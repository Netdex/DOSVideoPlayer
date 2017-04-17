#include "decoder.h"

LZ4_streamDecode_t lz4StreamDecode_body;
LZ4_streamDecode_t* lz4StreamDecode = &lz4StreamDecode_body;

char decBuf[2][BLOCK_BYTES];
int decBufIndex = 0;

char cmpBuf[LZ4_COMPRESSBOUND(BLOCK_BYTES)];

size_t read_int(FILE* fp, int* i) {
	return fread(i, sizeof(*i), 1, fp);
}

size_t read_bin(FILE* fp, void* array, size_t arrayBytes) {
	return fread(array, 1, arrayBytes, fp);
}

struct video_header decode_video_header(FILE *file) {
	struct video_header hd;
	fread(&hd.frame_count, sizeof(hd.frame_count), 1, file);
	fread(&hd.frame_rate, sizeof(hd.frame_rate), 1, file);
	return hd;
}

void decode_video_reset() {
	LZ4_setStreamDecode(lz4StreamDecode, NULL, 0);
	decBufIndex = 0;
}

void decode_video_frame(FILE *file, byte *palette, byte *dest) {
	int cmpBytes = 0;
	// decode palette
	fread(palette, sizeof(byte), 768, file);

	// decode frame data
	const size_t readCount0 = read_int(file, &cmpBytes);
	if (readCount0 != 1 || cmpBytes <= 0) {
		printf("decode error");
		exit(-1);
	}

	const size_t readCount1 = read_bin(file, cmpBuf, (size_t) cmpBytes);
	if (readCount1 != (size_t) cmpBytes) {
		printf("decode error");
		exit(-1);
	}

	char* const decPtr = decBuf[decBufIndex];
	const int decBytes = LZ4_decompress_safe_continue(lz4StreamDecode, cmpBuf,
			decPtr, cmpBytes, BLOCK_BYTES);
	if (decBytes <= 0) {
		printf("decode error");
		exit(-1);
	}
	memcpy(dest, decPtr, decBytes);

	decBufIndex = (decBufIndex + 1) % 2;
}

struct audio_header decode_audio_header(FILE *file) {
	struct audio_header ah;
	fread(&ah.frame_count, sizeof(ah.frame_count), 1, file);
	fread(&ah.frame_rate, sizeof(ah.frame_rate), 1, file);
	return ah;
}

struct audio_frame decode_audio_frame(FILE *file) {
	struct audio_frame af;
	fread(&af.frame, sizeof(af.frame), 1, file);
	fread(&af.action, sizeof(af.action), 1, file);
	if (af.action)
		fread(&af.frequency, sizeof(af.frequency), 1, file);
	return af;
}

struct lyric_header decode_lyric_header(FILE *file) {
	struct lyric_header lh;
	fscanf(file, "%d", &lh.frame_count);
	return lh;
}

struct lyric_frame decode_lyric_frame(FILE *file) {
	struct lyric_frame lf;
	fscanf(file, "%d ", &lf.delay);
	char* text = (char*) malloc(32);
	fgets(text, 32, file);
	int l = strlen(text);
	if (text[l - 1] == '\n')
		text[l - 1] = '\0';
	lf.text = text;
	return lf;
}
