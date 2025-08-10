#ifndef PCI_H
#define PCI_H

#include <stdint.h>

void pci_init(void);
uint32_t pci_read_config(uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg);

#endif
