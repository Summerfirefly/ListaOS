#ifndef MM_H
#define MM_H

#define MAX_PAGE_NUM 0x100000
#define MM_BASE_PAGE 0x100

#include <stdint.h>

typedef struct mem_info
{
    uint64_t addr;
    uint64_t size;
    uint32_t type;
}__attribute__((packed)) MEMINFO;

typedef struct phys_mem_page
{
    unsigned long long addr;
    unsigned int state;
} PHYS_MEM_PAGE;

void mm_init(void);
void set_paging(void);

#endif
