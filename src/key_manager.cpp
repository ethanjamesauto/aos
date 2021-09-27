#include "key_manager.h"
#include "ioport.h"

char scancode_table[] = {
0, //error
0, //escape
'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\b', //backspace
0, //tab
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
'\n', //enter
0, //lctrl
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
'`',
0, //lshift
'\\',
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
0, //rshift
'*', //keypad-*
0, //lalt
' ', //space
0, //capslock
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //fkeys
0, //numlock
0, //scrolllock
'7', //keypad-7
'8', //keypad-8
'9', //keypad-9
'-', //keypad--
'4', //keypad-4
'5', //keypad-5
'6', //keypad-6
'+', //keypad-+
'1', //keypad-1
'2', //keypad-2
'3', //keypad-3
'0', //keypad-0
'.', //keypad-.
0, //ralt
};

KeyManager::KeyManager() {
	buffer_loc = 0;
	chars_left = 0;
	key_pressed = false;
}

void KeyManager::update() {
	char key = port_byte_in(KEYBOARD_PORT);
	char pressed = (key & 0b10000000) ? 0 : (key & 0b01111111);
	if (pressed != key_pressed && pressed) {
		buffer[(buffer_loc + chars_left) % BUFFER_SIZE] = key & 0b01111111;
		chars_left++;
	}
	key_pressed = pressed;
}

char KeyManager::getKeyPress() {
	if (chars_left == 0) {
		return 0;
	}
	chars_left--;
	return scancode_table[buffer[buffer_loc++ % BUFFER_SIZE]];
}