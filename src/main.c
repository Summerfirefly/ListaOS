#include "stdio.h"
#include "kernel/console.h"
#include "kernel/display.h"
#include "kernel/interrupt.h"
#include "kernel/keyboard.h"
#include "kernel/mm.h"
#include "kernel/pci.h"
#include "kernel/timer.h"
#include <stdint.h>


int main(void)
{
    console_init();
    display_init();
    printf("Lista OS by Kayo Hikawa\n\n");

#ifdef DEBUG
    printf("printf float test: %f\n", 3.141592653589793);
#endif

    idt_init();
    pic_init();
    timer_init();
    mm_init();
    pci_init();
    printf("Boot completed!\n");
    printf("%d-bits Mode\n\n", sizeof(void *) * 8);

#ifdef DEBUG
    // Page Fault Test
    uint32_t *ptr = (uint32_t *)0x123;
    *ptr = 0x321;
    printf("Page Fault Test: PTE = 0x%x, data at %p is 0x%x\n", *((uint32_t *)0xffc00000), ptr, *ptr);
#endif

    for (;;)
    {
        if (count % 5 == 0)
        {
            keyboard_buffer_clear();
        }

        if (count % 50 == 0)
        {
            toggle_cursor();
        }

        __asm__("hlt":::);
    }
}
