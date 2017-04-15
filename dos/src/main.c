#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pc.h>
#include <time.h>

#include "types.h"
#include "vga.h"
#include "util.h"
#include "decoder.h"

#define AUDIO_REALTIME 8

extern byte *VGA_BUFFER;

int main(){
	vga_init();
	vga_set_mode(VGA_256_COLOR_MODE);
	
	FILE* video_file = fopen("badapple.bin", "rb");
	struct header_data hd = decode_header(video_file);
	
	FILE* audio_file = fopen("audio.bin", "rb");
	word audio_count = get_audio_event_count(audio_file);
	word audio_event = 0;
	
	int framems = 1000 / hd.frame_rate;
	struct audio_frame af;
	clock_t last_frame = clock();
	
	for(int f = 0; f < hd.frame_count; f++){
		struct frame_data fd = decode_frame(video_file, VGA_BUFFER);
		vga_swap(VGA_WAIT_RETRACE);
		
		clock_t now = clock();
		clock_t diff = now - last_frame;
		int del = max(0, framems - (int) diff);
		
		// process audio during sleep
		for(int d = 0; d < AUDIO_REALTIME; d++){
			int adjframe = f * AUDIO_REALTIME + d;
			if(af.frame < adjframe && audio_event < audio_count){
				af = decode_audio_frame(audio_file);
				audio_event++;
			}
			if(af.frame == adjframe){
				if(af.action){
					sound((int) af.frequency);
				}else{
					nosound();
				}
			}
			delay(del / AUDIO_REALTIME);
		}
		
		last_frame = now;
	}
	
	vga_set_mode(TEXT_MODE);
	return 0;
}
