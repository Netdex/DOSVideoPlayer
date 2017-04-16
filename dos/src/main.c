#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pc.h>
#include <time.h>
#include <string.h>

#include "types.h"
#include "vga.h"
#include "util.h"
#include "decoder.h"
#include "keyboard.h"

#define MSPERSEC 900

extern byte *VGA_BUFFER;

int main() {
	hook_keyb_handler();
	vga_init();
	vga_set_mode(VGA_256_COLOR_MODE);

	FILE* video_file = fopen("badapple.bin", "rb");
	struct video_header hd = decode_video_header(video_file);
	decode_video_reset();

	FILE* audio_file = fopen("audio.bin", "rb");
	struct audio_header ah = decode_audio_header(audio_file);
	struct audio_frame af;
	word audio_event = 0;

	FILE* lyric_file = fopen("lyrics.txt", "r");
	struct lyric_frame lf;
	struct lyric_header lh = decode_lyric_header(lyric_file);
	word lyric_event = 0;
	int last_lyric = 0;
	char lyric_buffer[256] = "";

	clock_t last_frame = clock();
	int frame_ratio = ah.frame_rate / hd.frame_rate;
	int framems = (MSPERSEC / hd.frame_rate) / frame_ratio;

	for (int f = 0; f < hd.frame_count * frame_ratio; f++) {
		last_frame = clock();
		if (f % frame_ratio == 0) {
			// consume next video frame
			decode_video_frame(video_file, VGA_BUFFER);
			vga_swap(VGA_WAIT_RETRACE);
			int cmsec = 1000 * f / ah.frame_rate;

			// consume next lyric frame
			if (last_lyric + lf.delay < cmsec && lyric_event < lh.frame_count) {
				if (lf.text) {
					strcat(lyric_buffer, lf.text);
					if ((int) strlen(lyric_buffer) > 32) {
						memset(lyric_buffer, 0, 256);
						strcpy(lyric_buffer, lf.text);
					}
				}
				last_lyric += lf.delay;
				lf = decode_lyric_frame(lyric_file);
				lyric_event++;
			}

			printf("\r%s", lyric_buffer);
		}

		// consume next audio frame
		if (af.frame < f && audio_event < ah.frame_count) {
			if (af.action) {
				sound((int) af.frequency);
			} else {
				nosound();
			}
			af = decode_audio_frame(audio_file);
			audio_event++;
		}

		if (key_pressed(0x01))	// exit condition
			break;

		clock_t now = clock();
		clock_t diff = now - last_frame;
		int del = max(0, framems - (int) (diff / CLOCKS_PER_SEC));
		if (!key_pressed(0x1d))	// frame skip
			delay(del);
	}
	fclose(video_file);
	fclose(audio_file);
	fclose(lyric_file);

	nosound();
	vga_set_mode(TEXT_MODE);
	return 0;
}
