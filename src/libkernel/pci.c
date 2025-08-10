#include "pci.h"

#include "io.h"
#include "stdio.h"


void pci_init(void)
{
    uint32_t count = 0;
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
                funcCount = 8;
            }

            for (int k = 0; k < funcCount; ++k)
            {
                uint32_t reg0 = pci_read_config(i, j, k, 0);
                if ((reg0 & 0xffff) != 0xffff)
                {
                    uint32_t reg2 = pci_read_config(i, j, k, 2);
                    uint32_t reg3 = pci_read_config(i, j, k, 3);
                    uint8_t classCode = reg2 >> 24;
                    uint8_t subclass = reg2 >> 16 & 0xff;
                    printf("PCI Bus %d, Device %d, Function %d\n", i, j, k);
                    printf("\tPCI Device ID: 0x%X, Vendor ID: 0x%X, ", reg0 >> 16, reg0 & 0xffff);
                    printf("Header Type: 0x%X, ", reg3 >> 16 & 0xff);
                    printf("Class Code: 0x%X, Subclass: 0x%X\n", classCode, subclass);
                    count++;
                }
            }
        }
    }

    printf("%d PCI device functions found\n\n", count);
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
