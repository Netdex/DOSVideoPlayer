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
#include "font.h"

#define MSPERSEC 1000

extern byte *VGA_BUFFER;

int main() {
	hook_keyb_handler();
	vga_init();
	vga_set_mode(VGA_256_COLOR_MODE);

	FILE* video_file = fopen("badapple.bin", "rb");
	struct video_header hd = decode_video_header(video_file);
	byte palette[768];
	byte palette_size;
	decode_video_reset();

	FILE* audio_file = fopen("audio.bin", "rb");
	struct audio_header ah = decode_audio_header(audio_file);
	struct audio_frame af;
	word audio_event = 0;

	uclock_t last_frame = uclock();
	int frame_ratio = ah.frame_rate / hd.frame_rate;
	int framems = (MSPERSEC / hd.frame_rate) / frame_ratio;

	for (int f = 0; f < hd.frame_count * frame_ratio; f++) {
		last_frame = uclock();
		if (f % frame_ratio == 0) {
			// consume next video frame
			decode_video_frame(video_file, palette, &palette_size, VGA_BUFFER);
			vga_wait_retrace();
			vga_set_palette(palette, palette_size);
			draw_text(320 - TEXT_WIDTH * 9, 0, "BAD APPLE", 0xF);
			vga_swap(0);
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

		uclock_t now = uclock();
		uclock_t diff = now - last_frame;
		int del = max(0, framems - (int) (MSPERSEC * diff / UCLOCKS_PER_SEC));
		if (!key_pressed(0x1d)) { // frame skip
			delay(del);
		} else {
			printf("\r>>");
		}
	}
	fclose(video_file);
	fclose(audio_file);

	nosound();
	vga_set_mode(TEXT_MODE);
	return 0;
}
