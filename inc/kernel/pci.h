#ifndef PCI_H
#define PCI_H

#include <stdint.h>


typedef struct pci_func
{
    uint8_t bus;
    uint8_t dev;
    uint8_t fun;
    uint32_t class_info;
} PCI_FUNC;


void pci_init(void);
uint32_t pci_read_config(uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg);
PCI_FUNC *pci_get_func_list(void);

#endif
