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

#include <stdarg.h>

#define NULL 0

int putc(char ch);
int puts(const char *str);
int printf(const char *format, ...);
int vprintf(const char *format, va_list args);
int sprintf(char *str, const char *format, ...);
