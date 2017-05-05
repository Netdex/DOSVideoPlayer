/*
 * modex.h
 *
 *  Created on: May 5, 2017
 *      Author: netdex
 */

#ifndef INCLUDE_MODEX_H_
#define INCLUDE_MODEX_H_

#include <sys/nearptr.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <pc.h>
#include <string.h>

#define INPUT_STATUS_1     	0x03da
#define VRETRACE           	0x08

#define NUM_COLORS         	256       /* number of colors in mode 0x13 */
#define RESERVED_PALETTE 		16		  /* # of palette colors which can't be touched by set palette */

#define SEQU_ADDR 0x3c4
#define CRTC_ADDR 0x3d4
#define GRAC_ADDR 0x3ce

#define VGA_TEXT_MODE 		0x01
#define VGAX_320x200x256 	0x02
#define VGAX_320x240x256 	0x03

extern int VGA_WIDTH, VGA_HEIGHT, VGA_WIDTH_BYTES;
extern unsigned VGAX_ACTIVE_START, VGAX_VISIBLE_START;

void vga_init();
void vga_set_mode(int mode);
void vga_wait_retrace();
void vga_set_palette_index(int index, int r, int g, int b);
void vga_set_palette(byte *palette, byte palette_size);

void vgax_set_active_start(unsigned offset);
void vgax_set_visible_start(unsigned offset);
void vgax_set_active_page(int page);
void vgax_set_visible_page(int page);
void vgax_set_pixel(int x, int y, char color);
char vgax_get_pixel(int x, int y);



#endif /* INCLUDE_MODEX_H_ */
