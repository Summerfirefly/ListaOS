#ifndef MM_INTERNAL_H
#define MM_INTERNAL_H

#define BIOS_MEM_MAP_BASE 0xc0012000

#define PAGE_DIR_VBASE 0xffbff000
#define PAGE_TABLE_VBASE 0xffc00000

// Address below this value is reserved by kernel
// For now it is lower 8MB RAM
#define USABLE_PHY_ADDR_BASE 0x800000

#define PHY_MEM_BITMAP_BASE 0xc0600000

#include <stddef.h>
#include <stdint.h>


typedef struct bios_mem_info
{
    uint64_t addr;
    uint64_t size;
    uint32_t type;
}__attribute__((packed)) BIOS_MEMINFO;


void *alloc_4k_phys();
void free_4k_phys(void *addr);
unsigned long long get_free_page_num(void);
void kernel_page_mmap(uint32_t phy_page_index, uint32_t vpage_index);
void kernel_page_unmap(uint32_t vpage_index);

#endif
