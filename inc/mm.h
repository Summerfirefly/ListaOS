#ifndef _MM_H
#define _MM_H

#define MAX_PAGE_NUM 0x100000
#define MM_BASE_PAGE 0x100

typedef struct mem_info
{
    unsigned long long addr;
    unsigned long long size;
    unsigned int type;
}__attribute__((packed)) MEMINFO;

typedef struct phys_mem_page
{
    unsigned long long addr;
    unsigned int state;
} PHYS_MEM_PAGE;

void mm_init(void);

#endif
