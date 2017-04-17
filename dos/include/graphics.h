/*
 * graphics.h
 *
 *  Created on: Apr 17, 2017
 *      Author: netdex
 */

#ifndef INCLUDE_GRAPHICS_H_
#define INCLUDE_GRAPHICS_H_

#include "vga.h"

extern byte *VGA_BUFFER;

void draw_rect(byte color, int x, int y, int width, int height);
void draw_line(byte color, int x1, int y1, int x2, int y2);

#endif /* INCLUDE_GRAPHICS_H_ */
