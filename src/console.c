#include "console.h"
#include "display.h"
#include "font.h"

int row = 0;
int col = 0;
int cursorState = 0;

unsigned char consoleBuffer[CONSOLE_WIDTH * CONSOLE_HEIGHT] = {'\0'};

RGB BACK_COLOR;
RGB CURSOR_COLOR;
RGB FONT_COLOR;

void _draw_font(char ch, int col, int row, RGB fontColor, RGB backColor);

void console_init(void)
{
	FONT_COLOR = COLOR_WHITE;
	CURSOR_COLOR = COLOR_GREY;
	BACK_COLOR = COLOR_BLACK;
}


inline void print_char(char ch)
{
	_draw_font(' ', col, row, FONT_COLOR, BACK_COLOR);
	switch (ch)
	{
	case '\b':
		if (col > 0)
		{
			col--;
			_draw_font(' ', col, row, FONT_COLOR, BACK_COLOR);
		}
		break;
	case '\n':
		row++;
		col = 0;
		break;
	case '\r':
		col = 0;
		break;
	case '\t':
		row += (col + 4) / CONSOLE_WIDTH;
		col = (col + 4) % CONSOLE_WIDTH;
		break;
	default:
		_draw_font(ch, col, row, FONT_COLOR, BACK_COLOR);
		row += (col + 1) / CONSOLE_WIDTH;
		col = (col + 1) % CONSOLE_WIDTH;
	}

	if (row == CONSOLE_HEIGHT)
	{
		row--;
	}
}


void toggle_cursor(void)
{
	if (cursorState)
	{
		_draw_font(' ', col, row, FONT_COLOR, BACK_COLOR);
	}
	else
	{
		_draw_font('|', col, row, CURSOR_COLOR, BACK_COLOR);
	}

	cursorState = !cursorState;
}


inline void _draw_font(char ch, int col, int row, RGB fontColor, RGB backColor)
{
	col *= 8;
	row *= 16;

	for (int i = 0; i < 16; ++i)
	{
		unsigned char data = fontData[ch * 16 + i];

		for (int j = 0; j < 8; ++j)
		{
			if ((data & 0x80) != 0)
			{
				draw_pixel(col + j, row + i, fontColor);
			}
			else
			{
				draw_pixel(col + j, row + i, backColor);
			}

			data <<= 1;
		}
	}
}