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

#include "kernel/io.h"

void cli(void)
{
    __asm__("cli");
}

void sti(void)
{
    __asm__("sti");
}

uint8_t inb(uint16_t port)
{
    uint8_t retData = 0;
    __asm__("inb %1, %0"
	        : "=a"(retData)
	        : "d"(port));
    return retData;
}

uint16_t inw(uint16_t port)
{
    uint16_t retData = 0;
    __asm__("inw %1, %0"
            : "=a"(retData)
            : "d"(port));
    return retData;
}

uint32_t inl(uint16_t port)
{
    uint32_t retData = 0;
    __asm__("inl %1, %0"
            : "=a"(retData)
            : "d"(port));
    return retData;
}

void outb(uint16_t port, uint8_t data)
{
    __asm__("outb %1, %0\n\t"
            :
            : "d"(port), "a"(data));
}

void outw(uint16_t port, uint16_t data)
{
    __asm__("outw %1, %0\n\t"
            :
            : "d"(port), "a"(data));
}

void outl(uint16_t port, uint32_t data)
{
    __asm__("outl %1, %0\n\t"
            :
            : "d"(port), "a"(data));
}

void load_idt(IDT_DESC idtDescData)
{
    __asm__("lidt %0"
            :
            : "m"(idtDescData)
            :);
}
