#include "keyboard.h"

#include <stdbool.h>
#include "stdio.h"

unsigned char keymap[4][0x80] = {
    { /*capsLock = false, shiftPressing = false*/
        0,
        0/*Esc*/, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
        '\n', 0/*L-Ctrl*/, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
        '`', 0x2a/*L-Shift*/, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x36/*R-Shift*/, '*',
        0/*L-Alt*/, ' ', 0x3a/*CapsLock*/,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0/*F1~F10*/,
        0/*NumLock*/, 0/*ScrollLock*/,
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
        0, 0, 0/*Reserved*/,
        0, 0/*F11~F12*/
    },
    { /*capsLock = false, shiftPressing = true*/
        0,
        0/*Esc*/, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
        '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
        '\n', 0/*L-Ctrl*/, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
        '~', 0x2a/*L-Shift*/, '|',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0x36/*R-Shift*/, '*',
        0/*L-Alt*/, ' ', 0x3a/*CapsLock*/,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0/*F1~F10*/,
        0/*NumLock*/, 0/*ScrollLock*/,
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
        0, 0, 0/*Reserved*/,
        0, 0/*F11~F12*/
    },
    { /*capsLock = true, shiftPressing = false*/
        0,
        0/*Esc*/, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
        '\n', 0/*L-Ctrl*/, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'',
        '`', 0x2a/*L-Shift*/, '\\',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0x36/*R-Shift*/, '*',
        0/*L-Alt*/, ' ', 0x3a/*CapsLock*/,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0/*F1~F10*/,
        0/*NumLock*/, 0/*ScrollLock*/,
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
        0, 0, 0/*Reserved*/,
        0, 0/*F11~F12*/
    },
    { /*capsLock = true, shiftPressing = true*/
        0,
        0/*Esc*/, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}',
        '\n', 0/*L-Ctrl*/, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"',
        '~', 0x2a/*L-Shift*/, '|',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0x36/*R-Shift*/, '*',
        0/*L-Alt*/, ' ', 0x3a/*CapsLock*/,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0/*F1~F10*/,
        0/*NumLock*/, 0/*ScrollLock*/,
        '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
        0, 0, 0/*Reserved*/,
        0, 0/*F11~F12*/
    }
};

unsigned char status = 0; /*0b00 capsLock, shiftPressing*/

unsigned char buffer[bufferSize] = {0};
unsigned short bufferHead = 0;
unsigned short bufferTail = 0;
unsigned short bufferUsed = 0;

void keyboard_buffer_clear(void)
{
    while (bufferUsed > 0)
    {
        unsigned char code = keyboard_buffer_out();

        if (code == 0x2a || code == 0x36)
        {
            status = (status & 2) + 1;
            continue;
        }
        else if (code == 0x3a)
        {
            status ^= 2;
            continue;
        }
        else if (code == 0xaa || code == 0xb6)
        {
            status &= 2;
            continue;
        }

        if (code < 0x80)
            if (keymap[status][code] != 0)
                putc(keymap[status][code]);
    }
}

void keyboard_buffer_in(unsigned char code)
{
    if (bufferUsed >= bufferSize || keymap[0][code] == 0)
        return;

    if (code == 0x2a || code == 0x36)
    {
        if (code == buffer[bufferTail]) return;
    }

    bufferTail = (bufferTail + 1) % bufferSize;
    buffer[bufferTail] = code;
    ++bufferUsed;
}

unsigned char keyboard_buffer_out(void)
{
    if (bufferUsed == 0)
        return 0;

    bufferHead = (bufferHead + 1) % bufferSize;
    --bufferUsed;

    return buffer[bufferHead];
}
