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

#include "kernel/interrupt.h"

#include <stdint.h>
#include "kernel/intr_handler.h"
#include "kernel/io.h"
#include "kernel/keyboard.h"
#include "kernel/internal/mm_internal.h"
#include "kernel/timer.h"
#include "kernel/types.h"


static IDT_Entry *idt_gate = (IDT_Entry *)IDT_BASE;


void idt_init(void)
{
    IDT_DESC idtDescData = {0x7ff, IDT_BASE};

    for (int i = 0; i < 256; ++i)
    {
        set_idt_gate(i, 0, 0, 0);
    }

    // Page Fault
    set_idt_gate(0x0e, page_fault_handler, 0x08, 0x8f);

    // IRQ with offset 0x20
    set_idt_gate(0x20, irq_0x20_handler, 0x08, 0x8e);
    set_idt_gate(0x21, irq_0x21_handler, 0x08, 0x8e);
    set_idt_gate(0x27, irq_0x27_handler, 0x08, 0x8e);

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


void set_idt_gate(uint32_t intr_num, void (*handler)(void), uint16_t seg_sec, uint8_t flag)
{
    idt_gate[intr_num].zero = 0;
    idt_gate[intr_num].offset_l = (uint16_t)((mem_addr_t)handler & 0xffff);
    idt_gate[intr_num].offset_h = (uint16_t)(((mem_addr_t)handler >> 16) & 0xffff);
    idt_gate[intr_num].selector = seg_sec;
    idt_gate[intr_num].type_attr = flag;
}


void page_fault(uint32_t addr, uint32_t err)
{
    if ((err & 0x1) == 0)
    {
        void *newPage = alloc_4k_phys();
        kernel_page_mmap(
            (uint32_t)newPage / 0x1000,
            addr / 0x1000,
            PAGE_FLAG_PRESENT | PAGE_FLAG_USER | PAGE_FLAG_RW
        );
    }
}


void irq_0x20(void)
{
    outb(M_PIC, 0x60);
    ++count;
}


void irq_0x21(void)
{
    outb(M_PIC, 0x61);
    keyboard_buffer_in(inb(0x60));
}


void irq_0x27(void)
{
    ;
}
