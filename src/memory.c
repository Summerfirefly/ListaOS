#include "memory.h"

void *memcpy(void *dest, void *src, unsigned int n)
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
		: "a"(dest), "b"(src), "c"(n)
		: "dl");

	return dest;
}


void *memset(void *s, int c, unsigned int n)
{
	__asm__(
		"memset_start:\n\t"
		"movb %%al, (%%ebx)\n\t"
		"incl %%ebx\n\t"
		"decl %%ecx\n\t"
		"jg   memset_start"
		:
		: "a"(c), "b"(s), "c"(n)
		: );

	return s;
}
