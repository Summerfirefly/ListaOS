/*
 * timer.c
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for timer, use PIT
 *
 * Licenced under LGPLv3
 *
 */

#include "timer.h"
#include "io.h"

unsigned int count = 0;

void timer_init(void)
{
    _outb(0x43, 0x36); //00110110
    _outb(0x40, 0x9c); //Low
    _outb(0x40, 0x2e); //High
    // 1.193182 MHz / 0x2e9c
    // About 1 interrupt per 10ms
}
