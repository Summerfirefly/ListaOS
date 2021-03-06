/*
 * intr_handler.h
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for handling interrupt
 *
 * Licenced under LGPLv3
 *
 */

#ifndef _INTR_HANDLER_H
#define _INTR_HANDLER_H

void irq_0x20_handler(void);
void irq_0x21_handler(void);
void irq_0x27_handler(void);

#endif
