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
#include "intr_handler.h"
#include "io.h"
#include "timer.h"
#include "keyboard.h"

static IDT_Entry *idt_gate = (IDT_Entry *) 0x10000;

void idt_init(void)
{
    IDT_DESC idtDescData = {0x7ff, 0x10000};

    for (int i = 0; i < 256; ++i)
    {
        set_idt_gate(i, 0, 0, 0);
    }

    // System call
    set_idt_gate(0x20, (unsigned int)irq_0x20_handler, 0x08, 0x8e);
    set_idt_gate(0x21, (unsigned int)irq_0x21_handler, 0x08, 0x8e);
    set_idt_gate(0x27, (unsigned int)irq_0x27_handler, 0x08, 0x8e);

    _load_idt(idtDescData);
}


void pic_init(void)
{
    _outb(M_DAT, 0xff);
    _outb(S_DAT, 0xff);

    _outb(M_PIC, 0x11);  //ICW1
    _outb(S_PIC, 0x11);  //ICW1

    _outb(M_DAT, 0x20);  //ICW2, IRQ0~IRQ7 Interrupt 0x20~0x27
    _outb(S_DAT, 0x28);  //ICW2, IRQ8~IRQ15 Interrupt 0x28~0x2f

    _outb(M_DAT, 0x04);  //ICW3, Connect IR2 to S-PIC
    _outb(S_DAT, 0x02);

    _outb(M_DAT, 0x01);  //ICW4, 80x86 Mode
    _outb(S_DAT, 0x01);  //ICW4, 80x86 Mode

    _outb(M_DAT, 0x7c);  //01111100 Enable keyboard interrupt and timer interrupt
    _outb(S_DAT, 0xff);

    _sti();
}

void set_idt_gate(int intr_num, unsigned int handler_addr, unsigned char seg_sec, unsigned char flag)
{
    idt_gate[intr_num].zero = 0;
    idt_gate[intr_num].offset_l = (unsigned short) (handler_addr & 0xffff);
    idt_gate[intr_num].offset_h = (unsigned short) ((handler_addr >> 16) & 0xffff);
    idt_gate[intr_num].selector = seg_sec;
    idt_gate[intr_num].type_attr = flag;
}

void irq_0x20(void)
{
    _cli();
    _outb(M_PIC, 0x60);
    ++count;
    _sti();
}

// 0x21 ~ 0xff user
void irq_0x21(void)
{
    _cli();
    _outb(M_PIC, 0x61);
    keyboard_buffer_in(_inb(0x60));
    _sti();
}

void irq_0x27(void)
{
    ;
}
