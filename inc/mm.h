#ifndef MM_H
#define MM_H

#include <stdint.h>

typedef struct mem_info
{
    uint64_t addr;
    uint64_t size;
    uint32_t type;
}__attribute__((packed)) MEMINFO;

void mm_init(void);
void set_paging(void);

#endif
