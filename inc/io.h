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


typedef struct idt_desc
{
    unsigned short size;
    unsigned int addr;
}__attribute__((packed)) IDT_DESC;
 
void _cli(void);
void _sti(void);

unsigned char _inb(unsigned short port);
void _load_idt(IDT_DESC idtDescData);
void _outb(unsigned short port, unsigned char data);

void _enable_paging(int dir_addr);
