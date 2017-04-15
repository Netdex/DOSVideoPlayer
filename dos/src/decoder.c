#include "decoder.h"

struct video_header decode_video_header(FILE *file){
	struct video_header hd;
	fread(&hd.frame_count, sizeof(hd.frame_count), 1, file);
	fread(&hd.frame_rate, sizeof(hd.frame_rate), 1, file);
	return hd;
}

struct video_frame decode_video_frame(FILE *file, byte *dest){
	struct video_frame fd;
	fread(&fd.chunk_count, sizeof(fd.chunk_count), 1, file);
	
	int offset = 0;
	for(int c = 0; c < fd.chunk_count; c++){
		struct video_chunk_data cd;
		fread(&cd.data, sizeof(cd.data), 1, file);
		fread(&cd.repeat, sizeof(cd.repeat), 1, file);
		memset(dest + offset, cd.data, cd.repeat);
		offset += cd.repeat;
	}
	return fd;
}

struct audio_header decode_audio_header(FILE *file){
	struct audio_header ah;
	fread(&ah.frame_count, sizeof(ah.frame_count), 1, file);
	fread(&ah.frame_rate, sizeof(ah.frame_rate), 1, file);
	return ah;
}

struct audio_frame decode_audio_frame(FILE *file){
	struct audio_frame af;
	fread(&af.frame, sizeof(af.frame), 1, file);
	fread(&af.action, sizeof(af.action), 1, file);
	if(af.action)
		fread(&af.frequency, sizeof(af.frequency), 1, file);
	return af;
}

struct lyric_header decode_lyric_header(FILE *file){
	struct lyric_header lh;
	fscanf(file, "%d", &lh.frame_count);
	return lh;
}

struct lyric_frame decode_lyric_frame(FILE *file){
	struct lyric_frame lf;
	fscanf(file, "%d ", &lf.delay);
	char* text = (char*) malloc(32);
	fgets(text, 32, file);
	int l = strlen(text);
	if(text[l - 1] == '\n')
		text[l - 1] = '\0';
	lf.text = text;
	return lf;
}