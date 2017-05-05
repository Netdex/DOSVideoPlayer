/*
 * Thank you based http://www.brackeen.com/vga/ for
 * teaching me VGA
 */

#ifndef VGA_H_
#define VGA_H_

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <sys/nearptr.h>
#include <string.h>

#include "types.h"
#include "error.h"

#define VIDEO_INT           	0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           	0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            	0x00      /* BIOS func to set the video mode. */

#define VGA_256_COLOR_MODE  	0x13      /* use to set 256-color mode. */
#define TEXT_MODE           	0x03      /* use to set 80x25 text mode. */

#define VGA_WIDTH           	320       /* width in pixels of mode 0x13 */
#define VGA_HEIGHT         	200       /* height in pixels of mode 0x13 */
#define NUM_COLORS         	256       /* number of colors in mode 0x13 */

#define INPUT_STATUS_1     	0x03da
#define VRETRACE           	0x08

#define VGA_CLEAR				0x1
#define VGA_WAIT_RETRACE		0x2

#define RESERVED_PALETTE 		16		  /* # of palette colors which can't be touched by set palette */

void vga_init();
void vga_set_mode(byte mode);
void vga_plot(int x, int y, byte color);
void vga_clear(byte color);
void vga_swap(int flags);
void vga_wait_retrace();
void vga_set_palette_index(int index, int r, int g, int b);
void vga_set_palette(byte *palette, byte palette_size);

#endif
