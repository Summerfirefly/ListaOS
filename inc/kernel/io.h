/*
 * io.h
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for port input/output
 *
 * Licenced under LGPLv3
 *
 */
#ifndef IO_H
#define IO_H

#include <stdint.h>

typedef struct idt_desc
{
    unsigned short size;
    unsigned int addr;
}__attribute__((packed)) IDT_DESC;

void cli(void);
void sti(void);

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
void load_idt(IDT_DESC idtDescData);
void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);
void outl(uint16_t port, uint32_t data);

#endif
