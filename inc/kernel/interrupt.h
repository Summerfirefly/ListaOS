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

#include <stdint.h>


#define M_PIC 0x20
#define M_DAT 0x21
#define S_PIC 0xa0
#define S_DAT 0xa1


typedef struct idt_entry
{
    uint16_t offset_l;  // offset bits 0..15
    uint16_t selector;  // a code segment selector in GDT or LDT
    uint8_t  zero;      // unused, set to 0
    uint8_t  type_attr; // type and attributes
    uint16_t offset_h;  // offset bits 16..31
} IDT_Entry;


void idt_init(void);
void pic_init(void);
void set_idt_gate(uint32_t intr_num, void (*handler)(void), uint16_t seg_sec, uint8_t flag);

void page_fault(uint32_t addr, uint32_t err);

void irq_0x20(void);
void irq_0x21(void);
void irq_0x27(void);

#endif
