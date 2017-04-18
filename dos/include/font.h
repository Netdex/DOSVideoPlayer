/*
 * font.h
 *
 *  Created on: Apr 17, 2017
 *      Author: netdex
 */

#ifndef INCLUDE_FONT_H_
#define INCLUDE_FONT_H_

#include "types.h"
#include "vga.h"

#define TEXT_WIDTH 4
#define TEXT_HEIGHT 6

extern byte *VGA_BUFFER;

void draw_text(int x, int y, const char *string, byte palette_color);

#endif /* INCLUDE_FONT_H_ */
