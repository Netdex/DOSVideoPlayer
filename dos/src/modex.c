/*
 * modex.c
 *
 *  Created on: May 5, 2017
 *      Author: netdex
 */

#include "modex.h"

unsigned char* vga = (unsigned char *) 0xA0000;

/* width and height should specify the mode dimensions.  widthBytes
 specify the width of a line in addressable bytes. */

int VGA_WIDTH, VGA_HEIGHT, VGA_WIDTH_BYTES;

/* actStart specifies the start of the page being accessed by
 drawing operations.  visStart specifies the contents of the Screen
 Start register, i.e. the start of the visible page */

unsigned VGAX_ACTIVE_START, VGAX_VISIBLE_START;

void vga_set_text();
void vgax_set_320x200x256();
void vgax_set_320x240x256();

void vga_init() {
	if (__djgpp_nearptr_enable() == 0) {
		printf("Couldn't get access to first 640K of memory.\n");
		exit(-1);
	}
	vga += __djgpp_conventional_base;
	// __djgpp_nearptr_disable();
	return;
}

void vga_set_mode(int mode) {
	switch (mode) {
	case VGA_TEXT_MODE:
		vga_set_text();
		break;
	case VGAX_320x200x256:
		vgax_set_320x200x256();
		break;
	case VGAX_320x240x256:
		vgax_set_320x240x256();
		break;
	}
}

void vga_wait_retrace() {
	while ((inp(INPUT_STATUS_1) & VRETRACE))
		;
	while (!(inp(INPUT_STATUS_1) & VRETRACE))
		;
}

void vga_set_palette_index(int index, int r, int g, int b) {
	outp(0x03c8, index);
	outp(0x03c9, r >> 2);
	outp(0x03c9, g >> 2);
	outp(0x03c9, b >> 2);
}

void vga_set_palette(byte *palette, byte palette_size) {
	outp(0x03c8, 0);
	if(palette_size > NUM_COLORS - RESERVED_PALETTE)
		raise_error("Invalid Palette Size");

	for (int i = 0; i < RESERVED_PALETTE * 3; i++) {
		outp(0x03c9, DEF_PALETTE[i]);
	}
	for (int i = 0; i < palette_size * 3; i++) {
		outp(0x03c9, palette[i] >> 2);
	}
}

void vga_set_text() {
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = 0x03;
	int86(0x10, &regs, &regs);
}

/*
 * set320x200x256_X()
 *      sets mode 13h, then turns it into an unchained (planar), 4-page
 *      320x200x256 mode.
 */
void vgax_set_320x200x256() {
	union REGS r;

	/* Set VGA BIOS mode 13h: */
	r.x.ax = 0x0013;
	int86(0x10, &r, &r);

	/* Turn off the Chain-4 bit (bit 3 at index 4, port 0x3c4): */
	outportw(SEQU_ADDR, 0x0604);

	/* Turn off word mode, by setting the Mode Control register
	 of the CRT Controller (index 0x17, port 0x3d4): */
	outportw(CRTC_ADDR, 0xE317);

	/* Turn off doubleword mode, by setting the Underline Location
	 register (index 0x14, port 0x3d4): */
	outportw(CRTC_ADDR, 0x0014);

	/* Clear entire video memory, by selecting all four planes, then
	 writing 0 to the entire segment. */
	outportw(SEQU_ADDR, 0x0F02);
	memset(vga + 1, 0, 0xffff); /* stupid size_t exactly 1 too small */
	vga[0] = 0;

	/* Update the global variables to reflect the dimensions of this
	 mode.  This is needed by most future drawing operations. */
	VGA_WIDTH = 320;
	VGA_HEIGHT = 200;

	/* Each byte addresses four pixels, so the width of a scan line
	 in *bytes* is one fourth of the number of pixels on a line. */
	VGA_WIDTH_BYTES = VGA_WIDTH / 4;

	/* By default we want screen refreshing and drawing operations
	 to be based at offset 0 in the video segment. */
	VGAX_ACTIVE_START = VGAX_VISIBLE_START = 0;
}
/*
 * setActiveStart() tells our graphics operations which address in video
 * memory should be considered the top left corner.
 */

void vgax_set_active_start(unsigned offset) {
	VGAX_ACTIVE_START = offset;
}

/*
 * setVisibleStart() tells the VGA from which byte to fetch the first
 * pixel when starting refresh at the top of the screen.  This version
 * won't look very well in time critical situations (games for
 * instance) as the register outputs are not synchronized with the
 * screen refresh.  This refresh might start when the high byte is
 * set, but before the low byte is set, which produces a bad flicker.
 * I won't bother with this now.
 */

void vgax_set_visible_start(unsigned offset) {
	VGAX_VISIBLE_START = offset;
	outportb(CRTC_ADDR, 0x0C); /* set high byte */
	outportb(CRTC_ADDR + 1, VGAX_VISIBLE_START >> 8);
	outportb(CRTC_ADDR, 0x0D); /* set low byte */
	outportb(CRTC_ADDR + 1, VGAX_VISIBLE_START & 0xff);
}

/*
 * setXXXPage() sets the specified page by multiplying the page number
 * with the size of one page at the current resolution, then handing the
 * resulting offset value over to the corresponding setXXXStart()
 * function.  The first page number is 0.
 */

void vgax_set_active_page(int page) {
	vgax_set_active_start(page * VGA_WIDTH_BYTES * VGA_HEIGHT);
}

void vgax_set_visible_page(int page) {
	vgax_set_visible_start(page * VGA_WIDTH_BYTES * VGA_HEIGHT);
}
void vgax_set_pixel(int x, int y, char color) {

	/* Each address accesses four neighboring pixels, so set
	 Write Plane Enable according to which pixel we want
	 to modify.  The plane is determined by the two least
	 significant bits of the x-coordinate: */
	outportb(0x3c4, 0x02);
	outportb(0x3c5, 0x01 << (x & 3));

	/* The offset of the pixel into the video segment is
	 offset = (width * y + x) / 4, and write the given
	 color to the plane we selected above.  Heed the active
	 page start selection. */
	vga[(unsigned) (VGA_WIDTH_BYTES * y) + (x / 4) + VGAX_ACTIVE_START] = color;
}

char vgax_get_pixel(int x, int y) {
	/* Select the plane from which we must read the pixel color: */
	outportb(GRAC_ADDR, 0x04);
	outportb(GRAC_ADDR + 1, x & 3);

	return vga[(unsigned) (VGA_WIDTH_BYTES * y) + (x / 4) + VGAX_ACTIVE_START];
}
void vgax_set_320x240x256() {
	/* Set the unchained version of mode 13h: */
	vgax_set_320x200x256();

	/* Modify the vertical sync polarity bits in the Misc. Output
	 Register to achieve square aspect ratio: */
	outportb(0x3C2, 0xE3);

	/* Modify the vertical timing registers to reflect the increased
	 vertical resolution, and to center the image as good as
	 possible: */
	outportw(0x3D4, 0x2C11); /* turn off write protect */
	outportw(0x3D4, 0x0D06); /* vertical total */
	outportw(0x3D4, 0x3E07); /* overflow register */
	outportw(0x3D4, 0xEA10); /* vertical retrace start */
	outportw(0x3D4, 0xAC11); /* vertical retrace end AND wr.prot */
	outportw(0x3D4, 0xDF12); /* vertical display enable end */
	outportw(0x3D4, 0xE715); /* start vertical blanking */
	outportw(0x3D4, 0x0616); /* end vertical blanking */

	/* Update mode info, so future operations are aware of the
	 resolution: */
	VGA_HEIGHT = 240;
}
