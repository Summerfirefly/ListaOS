/*
 * stdio.h
 *
 * The standard C library for Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for standard input/output
 *
 * Licenced under LGPLv3
 *
 */
#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

int putc(char ch);
int puts(const char *str);
int printf(const char *format, ...);
int vprintf(const char *format, va_list args);
int sprintf(char *str, const char *format, ...);

#endif
