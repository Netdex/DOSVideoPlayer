#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "types.h"
#include "vga.h"
#include "util.h"
#include "decoder.h"

extern byte *VGA_BUFFER;

int main(){
	vga_init();
	vga_set_mode(VGA_256_COLOR_MODE);
	
	FILE* file = fopen("badapple.bin", "rb");
	struct header_data hd = decode_header(file);
	
	for(int f = 0; f < hd.frame_count; f++){
		struct frame_data fd = decode_frame(file, VGA_BUFFER);
		vga_swap(VGA_WAIT_RETRACE);
		delay(50);
	}
	
	vga_set_mode(TEXT_MODE);
	return 0;
}
