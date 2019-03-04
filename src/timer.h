/*
 * timer.h
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for timer, use PIT
 *
 * Licenced under LGPLv3
 *
 */

#ifndef _TIMER_H
#define _TIMER_H

extern unsigned int count;

typedef struct timer_item
{
    unsigned int timeout;
} TimerItem;

void timer_init(void);

#endif
