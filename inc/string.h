#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
char *strcpy(char *restrict dst, const char *restrict src);
size_t strlen(const char *s);

#endif
