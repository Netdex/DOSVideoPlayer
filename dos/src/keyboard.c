#include "keyboard.h"


struct {																// information for the keyboard handler
	char enabled;														// hook enabled
} keyb_handler_info;

volatile char keyb[256];												// all keys if pressed or not

unsigned char scancode2ascii[256] = {									// lookup table to convert scancodes to ascii
  0, 0, 49, 50, 51, 52, 53, 54, 55, 56,								// 0-9
  57, 48, 45, 0, 0, 0, 113, 119, 101, 114,								// 10-19
  116, 121, 117, 105, 111, 112, 0, 0, 0, 0,							// 20-29
  97, 115, 100, 102, 103, 104, 106, 107, 108, 0,						// 30-39
  0, 0, 0, 0, 122, 120, 99, 118, 98, 110,								// 40-49
  109, 44, 46, 47, 0, 0, 0, 32, 0, 0,									// 50-59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0};

unsigned char last_keys[256];

_go32_dpmi_seginfo old_keyb_handler_seginfo, new_keyb_handler_seginfo;

void keyb_handler() {													// called by the system if a key is pressed, stores the key in the last_keys array and the keyb array
	unsigned char key;													// temporarily stores the key
	static char extended;												// saves if the last character used the 8th bit
	int c1;																// iteration variable

	key = inportb(0x60);												// 0x60 seems to be the keyboard port

	if (key == 0xe0)													// next key is a special extended key
		extended = 1;
	else {
		if (extended == 0) {
			if ( (key & 0x80) == 0) {									// key is pressed
				keyb[key & 0x7f] = 1;									// scan code is all but hightést bit, so mark this key as pressed
				for (c1 = 48; c1 > 0; c1--)							// each character in the last_keys array
					last_keys[c1] = last_keys[c1 - 1];					// has to go one step back to
				last_keys[0] = scancode2ascii[key & 0x7f];				// make place for the new character
			}
			else
				keyb[key & 0x7f] = 0;									// mark as not pressed
		}
		else {
			if ( (key & 0x80) == 0) {									// extended key handling
				keyb[(key & 0x7f) + 0x80] = 1;							// are saved at upper positions in array
				for (c1 = 48; c1 > 0; c1--)
					last_keys[c1] = last_keys[c1 - 1];
				last_keys[0] = scancode2ascii[(key & 0x7f) + 0x80];
			}
			else
				keyb[(key & 0x7f) + 0x80] = 0;
		}
		extended = 0;
	}
	outportb(0x20, 0x20);												// handled?? or iret??
}
void keyb_handler_end() {}												// marks the end of the code of keyb_handler()

char hook_keyb_handler(void) {											// enables the keyboard hook
	if (keyb_handler_info.enabled == 0) {								// only if not already enabled
		_go32_dpmi_lock_data( (char *)&keyb, 
			sizeof(keyb) );											// locks the memory region of the keyb array
		_go32_dpmi_lock_code(keyb_handler, 
			(unsigned long)keyb_handler_end - 
			(unsigned long)keyb_handler);								// locks the code region of keyb_handler
		_go32_dpmi_get_protected_mode_interrupt_vector(9, 
			&old_keyb_handler_seginfo);									// store old keyboard handler
		new_keyb_handler_seginfo.pm_offset = 
			(int)keyb_handler;											// function to call if a key is pressed is keyb_handler
		if (_go32_dpmi_allocate_iret_wrapper(
			&new_keyb_handler_seginfo) != 0)							// register this function
			return 1;
		if (_go32_dpmi_set_protected_mode_interrupt_vector(9, 
			&new_keyb_handler_seginfo) != 0) {							// another part of it
			_go32_dpmi_free_iret_wrapper(&new_keyb_handler_seginfo);
			return 1;
		}
		keyb_handler_info.enabled = 1;									// keyboard hook is now enabled
		memset(last_keys, 0, sizeof(last_keys) );						// clear the last_keys array
	}
	return 0;
}

void remove_keyb_handler(void) {										// disables the interrupt handler
	if (keyb_handler_info.enabled == 1) {
		_go32_dpmi_set_protected_mode_interrupt_vector(9, 
			&old_keyb_handler_seginfo);									// restore old handler
		_go32_dpmi_free_iret_wrapper(
			&new_keyb_handler_seginfo);									// free interrupt capturing
		keyb_handler_info.enabled = 0;									// handler is disabled
	}
}

char key_pressed(unsigned char key) {									// returns if a key is pressed
	return keyb[key];
}
