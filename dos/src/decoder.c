#include "decoder.h"

struct header_data decode_header(FILE *file){
	struct header_data hd;
	fread(&hd.frame_count, sizeof(hd.frame_count), 1, file);
	fread(&hd.frame_rate, sizeof(hd.frame_rate), 1, file);
	return hd;
}

struct frame_data decode_frame(FILE *file, byte *dest){
	struct frame_data fd;
	fread(&fd.chunk_count, sizeof(fd.chunk_count), 1, file);
	
	int offset = 0;
	for(int c = 0; c < fd.chunk_count; c++){
		struct chunk_data cd;
		fread(&cd.data, sizeof(cd.data), 1, file);
		fread(&cd.repeat, sizeof(cd.repeat), 1, file);
		memset(dest + offset, cd.data, cd.repeat);
		offset += cd.repeat;
	}
	return fd;
}