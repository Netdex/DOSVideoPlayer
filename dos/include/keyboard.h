#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <dpmi.h>

char hook_keyb_handler();
void remove_keyb_handler();
char key_pressed(unsigned char key);

#endif