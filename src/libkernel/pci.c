#include "pci.h"

#include "io.h"
#include "stdio.h"


void pci_init(void)
{
    for (int i = 0; i <= 0xff; ++i)
    {
        for (int j = 0; j <= 0x1f; ++j)
        {
            for (int k = 0; k <= 0x7; ++k)
            {
                uint32_t config = pci_read_config(i, j, k, 0);
                uint16_t vendor = config & 0xffff;
                if (vendor != 0xffff)
                {
                    uint8_t headerType = (pci_read_config(i, j, k, 3) >> 16) & 0xff;
                    uint32_t deviceClass = pci_read_config(i, j, k, 2);
                    uint8_t classCode = deviceClass >> 24;
                    uint8_t subclass = (deviceClass >> 16) & 0xff;
                    printf("PCI Bus %d, Device %d, Function %d\n", i, j, k);
                    printf("\tPCI Device ID: 0x%X, Vendor ID: 0x%X, Header Type: 0x%X, ", config >> 16, vendor, headerType & 0x7f);
                    printf("Class Code: 0x%X, Subclass: 0x%X\n", classCode, subclass);
                }
            }
        }
    }
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
