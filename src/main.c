#include "kernel/console.h"
#include "kernel/display.h"
#include "kernel/driver/sata.h"
#include "kernel/interrupt.h"
#include "kernel/keyboard.h"
#include "kernel/mm.h"
#include "kernel/pci.h"
#include "kernel/timer.h"
#include "stdio.h"
#include <stddef.h>
#include <stdint.h>


int main(void)
{
    // Screen Init
    console_init();
    display_init();

    // Interrupt & Memory Management Init
    timer_init();
    idt_init();
    pic_init();
    mm_init();

    printf("Lista OS by Kayo Hikawa\n\n");
#ifdef DEBUG
    printf("printf float test: %f\n", 3.141592653589793);
#endif

    pci_init();

    printf("Boot completed!\n");
    printf("%d-bits Mode\n\n", sizeof(void *) * 8);

#ifdef DEBUG
    // Page Fault Test
    uint32_t *ptr = (uint32_t *)0x123;
    *ptr = 0x321;
    printf("Page Fault Test: PTE = 0x%x, data at %p is 0x%x\n", *((uint32_t *)0xffc00000), ptr, *ptr);

    // PCI Enumerating Result
    PCI_FUNC *pci_func_list = pci_get_func_list();
    for (size_t i = 0; pci_func_list[i].fun != 0xff; ++i)
    {
        uint8_t bus = pci_func_list[i].bus;
        uint8_t dev = pci_func_list[i].dev;
        uint8_t fun = pci_func_list[i].fun;
        uint32_t reg0 = pci_read_config(bus, dev, fun, 0);
        uint32_t reg3 = pci_read_config(bus, dev, fun, 3);
        uint8_t class_code = pci_func_list[i].class_info >> 24;
        uint8_t subclass = (pci_func_list[i].class_info >> 16) & 0xff;
        uint8_t prog = (pci_func_list[i].class_info) >> 8 & 0xff;
        printf("PCI Bus %d, Device %d, Function %d\n", bus, dev, fun);
        printf("\tPCI Device ID: 0x%X, Vendor ID: 0x%X, Prog If: 0x%X, ", reg0 >> 16, reg0 & 0xffff, prog);
        printf("Header Type: 0x%X, ", reg3 >> 16 & 0xff);
        printf("Class Code: 0x%X, Subclass: 0x%X\n", class_code, subclass);

        if (class_code == 0x1 && subclass == 0x6)
        {
            uint32_t ahci_bar5 = pci_read_config(bus, dev, fun, 9);
            printf("AHCI Controller found with base address at 0x%x\n", ahci_bar5);
            sata_driver_init(ahci_bar5);
        }
    }
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
