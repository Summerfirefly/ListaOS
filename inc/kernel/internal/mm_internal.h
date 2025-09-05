#ifndef MM_INTERNAL_H
#define MM_INTERNAL_H

#define IDT_BASE            0xc0010000
#define BIOS_MEM_MAP_BASE   0xc0012000
#define PCI_FUNC_LIST_BASE  0xc0020000
#define AHCI_MMIO_BASE      0xc00a0000
#define AHCI_CMD_FIS_BASE   0xc00a4000
#define PHY_MEM_BITMAP_BASE 0xc0600000
#define VRAM_BASE           0xc1000000
#define PAGE_DIR_VBASE      0xffbff000
#define PAGE_TABLE_VBASE    0xffc00000

// Address below this value is reserved by kernel
// For now it is lower 8MB RAM
#define USABLE_PHY_ADDR_BASE 0x800000


#include <stddef.h>
#include <stdint.h>
#include "kernel/types.h"


#define PAGE_FLAG_PRESENT   0x1
#define PAGE_FLAG_RW        0x2
#define PAGE_FLAG_RO        0x0
#define PAGE_FLAG_USER      0x4
#define PAGE_FLAG_KERNEL    0x0
#define PAGE_FLAG_PWT       0x8
#define PAGE_FLAG_PCD       0x10


typedef struct bios_mem_info
{
    uint64_t addr;
    uint64_t size;
    uint32_t type;
}__attribute__((packed)) BIOS_MEMINFO;


void *alloc_4k_phys();
void free_4k_phys(void *addr);
unsigned long long get_free_page_num(void);
void kernel_page_mmap(mem_addr_t phy_page_index, mem_addr_t vpage_index, uint8_t flags);
void kernel_page_unmap(mem_addr_t vpage_index);

#endif
