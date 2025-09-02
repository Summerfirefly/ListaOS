#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_FONT_WIDTH 8
#define CONSOLE_FONT_HEIGHT 16

void console_init(void);
void print_char(char ch);
void toggle_cursor(void);

#endif
