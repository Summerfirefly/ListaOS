#include "string.h"

void *memcpy(void *dst, const void *src, size_t n)
{
	__asm__(
		"memcpy_start:\n\t"
		"movb (%%ebx), %%dl\n\t"
		"movb %%dl, (%%eax)\n\t"
		"incl %%ebx\n\t"
		"incl %%eax\n\t"
		"decl %%ecx\n\t"
		"jg   memcpy_start"
		:
		: "a"(dst), "b"(src), "c"(n)
		: "dl");

	return dst;
}


void *memset(void *s, int c, size_t n)
{
	__asm__(
		"memset_start:\n\t"
		"cld\n\t"
		"rep\n\t"
		"stosb\n\t"
		:
		: "a"(c), "D"(s), "c"(n)
		: "memory");

	return s;
}


char *strcpy(char *restrict dst, const char *restrict src)
{
	memcpy(dst, src, strlen(src));
	return dst;
}


size_t strlen(const char *s)
{
	size_t len = 0;
	while (s[len] != '\0')
	{
		len++;
	}

	return len;
}
