#include "kernel/driver/sata.h"

#include "kernel/internal/mm_internal.h"
#include "stdio.h"
#include "string.h"


void sata_driver_init(mem_addr_t ahci_base)
{
    printf("Initialize SATA driver\n");
    for (int i = 0; i < 4; ++i)
    {
        kernel_page_mmap(
            ahci_base / 0x1000 + i,
            AHCI_MMIO_BASE / 0x1000 + i,
            PAGE_FLAG_PRESENT | PAGE_FLAG_KERNEL | PAGE_FLAG_RW | PAGE_FLAG_PCD | PAGE_FLAG_PWT
        );
    }

    memset((void *)AHCI_CMD_FIS_BASE, 0, 0x1000 * 64);

    HBA_MEM *hba_mem = (HBA_MEM *)AHCI_MMIO_BASE;
    hba_mem->ghc |= (1 << 31);
    uint32_t ports = hba_mem->pi;
    for (int i = 0; i < 32; i++)
    {
        if (ports & (1 << i))
        {
            hba_mem->ports[i].clb = AHCI_CMD_FIS_BASE + 0x2000 * i;
            hba_mem->ports[i].clbu = 0;
            hba_mem->ports[i].fb = AHCI_CMD_FIS_BASE + 0x2000 * i + 0x1000;
            hba_mem->ports[i].fbu = 0;

            uint32_t ssts = hba_mem->ports[i].ssts;
            int active_device = 0;
            if ((ssts & 0xf) == 0x3 && ((ssts >> 8) & 0xf) == 0x1)
            {
                active_device = 1;
            }

            printf("Initialized AHCI port %d, status: %s\n", i, active_device == 1 ? "Active" : "Inactive");
        }
    }
}
