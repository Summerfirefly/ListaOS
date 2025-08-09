/*
 * interrupt.h
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for interrupt handle and interrupt initialize
 *
 * Licenced under LGPLv3
 *
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#define M_PIC 0x20
#define M_DAT 0x21
#define S_PIC 0xa0
#define S_DAT 0xa1

typedef struct idt_entry
{
    unsigned short offset_l; // offset bits 0..15
    unsigned short selector; // a code segment selector in GDT or LDT
    unsigned char  zero;      // unused, set to 0
    unsigned char  type_attr; // type and attributes
    unsigned short offset_h; // offset bits 16..31
} IDT_Entry;

void idt_init(void);
void pic_init(void);
void set_idt_gate(int intr_num, unsigned int handler_addr, unsigned char seg_sec, unsigned char flag);

void irq_0x20(void);
// 0x21 ~ 0xff user
void irq_0x21(void);
void irq_0x27(void);

#endif
