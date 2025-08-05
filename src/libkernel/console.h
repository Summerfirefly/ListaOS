#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_WIDTH  128
#define CONSOLE_HEIGHT 48

void console_init(void);
void print_char(char ch);
void toggle_cursor(void);

#endif