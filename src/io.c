/*
 * io.c
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for port input/output
 *
 * Licenced under LGPLv3
 *
 */

#include "io.h"

void _cli(void)
{
	__asm__("cli");
}

void _sti(void)
{
	__asm__("sti");
}

unsigned char _inb(unsigned short port)
{
	unsigned char retData = 0;
    __asm__("inb   %1, %0"
			: "=a"(retData)
			: "d"(port));
	return retData;
}

void _outb(unsigned short port, unsigned char data)
{
    __asm__("outb  %1, %0\n\t"
			:
			: "d"(port), "a"(data));
}

void _load_idt(IDT_DESC idtDescData)
{
	__asm__("lidt %0"
	        :
			: "m"(idtDescData)
			:);
}

void _enable_paging(int dir_addr)
{
    __asm__("movl  %0, %%cr3\n\t"
            "movl  %%cr0, %%eax\n\t"
            "orl   $0x80000000, %%eax\n\t"
            "movl  %%eax, %%cr0\n\t"
			:
			: "a"(dir_addr));
}