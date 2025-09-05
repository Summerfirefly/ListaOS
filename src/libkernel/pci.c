#include "kernel/pci.h"

#include "kernel/io.h"
#include "kernel/internal/mm_internal.h"
#include "stdio.h"


void pci_init(void)
{
    PCI_FUNC *pci_func_list = pci_get_func_list();
    size_t count = 0;
    for (int i = 0; i <= 0xff; ++i)
    {
        for (int j = 0; j <= 0x1f; ++j)
        {
            uint16_t vendor = pci_read_config(i, j, 0, 0) & 0xffff;
            if (vendor == 0xffff)
            {
                continue;
            }

            uint8_t headerType = (pci_read_config(i, j, 0, 3) >> 16) & 0xff;
            int funcCount = 1;
            if ((headerType & 0x80) != 0)
            {
                // Multi-function PCI device
                funcCount = 8;
            }

            for (int k = 0; k < funcCount; ++k)
            {
                uint32_t reg0 = pci_read_config(i, j, k, 0);
                if ((reg0 & 0xffff) != 0xffff)
                {
                    pci_func_list[count].bus = i;
                    pci_func_list[count].dev = j;
                    pci_func_list[count].fun = k;
                    pci_func_list[count].class_info = pci_read_config(i, j, k, 2);
                    count++;
                }
            }
        }
    }

    pci_func_list[count].bus = 0xff;
    pci_func_list[count].dev = 0xff;
    pci_func_list[count].fun = 0xff;
    pci_func_list[count].class_info = 0xffffffff;
}


uint32_t pci_read_config(uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg)
{
    uint32_t addr = (uint32_t)bus << 16;
    addr |= (uint32_t)(dev & 0x1f) << 11;
    addr |= (uint32_t)(func & 0x7) << 8;
    addr |= (uint32_t)(reg << 2);
    addr |= 0x80000000;

    outl(0xcf8, addr);
    return inl(0xcfc);
}


PCI_FUNC *pci_get_func_list(void)
{
    return (PCI_FUNC *)PCI_FUNC_LIST_BASE;
}
