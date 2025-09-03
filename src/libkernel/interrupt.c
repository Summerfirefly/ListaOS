/*
 * interrupt.c
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for interrupt handle and interrupt initialize
 *
 * Licenced under LGPLv3
 *
 */

#include "interrupt.h"

#include <stdint.h>
#include "intr_handler.h"
#include "io.h"
#include "keyboard.h"
#include "mm_internal.h"
#include "timer.h"


static IDT_Entry *idt_gate = (IDT_Entry *) 0xc0010000;


void idt_init(void)
{
    IDT_DESC idtDescData = {0x7ff, 0xc0010000};

    for (int i = 0; i < 256; ++i)
    {
        set_idt_gate(i, 0, 0, 0);
    }

    // Page Fault
    set_idt_gate(0x0e, (unsigned int)page_fault_handler, 0x08, 0x8e);

    // System call
    set_idt_gate(0x20, (unsigned int)irq_0x20_handler, 0x08, 0x8e);
    set_idt_gate(0x21, (unsigned int)irq_0x21_handler, 0x08, 0x8e);
    set_idt_gate(0x27, (unsigned int)irq_0x27_handler, 0x08, 0x8e);

    load_idt(idtDescData);
}


void pic_init(void)
{
    outb(M_DAT, 0xff);
    outb(S_DAT, 0xff);

    outb(M_PIC, 0x11);  //ICW1
    outb(S_PIC, 0x11);  //ICW1

    outb(M_DAT, 0x20);  //ICW2, IRQ0~IRQ7 Interrupt 0x20~0x27
    outb(S_DAT, 0x28);  //ICW2, IRQ8~IRQ15 Interrupt 0x28~0x2f

    outb(M_DAT, 0x04);  //ICW3, Connect IR2 to S-PIC
    outb(S_DAT, 0x02);

    outb(M_DAT, 0x01);  //ICW4, 80x86 Mode
    outb(S_DAT, 0x01);  //ICW4, 80x86 Mode

    outb(M_DAT, 0x7c);  //01111100 Enable keyboard interrupt and timer interrupt
    outb(S_DAT, 0xff);

    sti();
}


void set_idt_gate(int intr_num, unsigned int handler_addr, unsigned char seg_sec, unsigned char flag)
{
    idt_gate[intr_num].zero = 0;
    idt_gate[intr_num].offset_l = (unsigned short) (handler_addr & 0xffff);
    idt_gate[intr_num].offset_h = (unsigned short) ((handler_addr >> 16) & 0xffff);
    idt_gate[intr_num].selector = seg_sec;
    idt_gate[intr_num].type_attr = flag;
}


void page_fault(uint32_t addr, uint32_t err)
{
    if ((err & 0x1) == 0)
    {
        void *newPage = alloc_4k_phys();
        kernel_page_mmap((uint32_t)newPage / 0x1000, addr / 0x1000);
    }
}


void irq_0x20(void)
{
    outb(M_PIC, 0x60);
    ++count;
}


// 0x21 ~ 0xff user
void irq_0x21(void)
{
    outb(M_PIC, 0x61);
    keyboard_buffer_in(inb(0x60));
}


void irq_0x27(void)
{
    ;
}
