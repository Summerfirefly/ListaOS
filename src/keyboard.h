#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define bufferSize 256

void keyboard_buffer_clear(void);
void keyboard_buffer_in(unsigned char code);
unsigned char keyboard_buffer_out(void);

#endif
