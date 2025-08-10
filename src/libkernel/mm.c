#include "mm.h"
#include "stdio.h"
#include "io.h"

static PHYS_MEM_PAGE *physMemMap = (PHYS_MEM_PAGE *) 0x800000;
static unsigned int pageNum = 0;
static int mmInitialed = 0;

#define PAGE_DIR_BASE 0x1f0000
#define PAGE_TABLE_BASE 0x200000

uint32_t *paging_dir = (uint32_t *)PAGE_DIR_BASE;
uint32_t *paging_table = (uint32_t *)PAGE_TABLE_BASE;


//void *alloc_4k_phys(unsigned int num)


unsigned long long get_free_page_num(void)
{
    unsigned long long freeNum = 0;

    for (unsigned long long i = 0; i < pageNum; ++i)
    {
        if (physMemMap[i].state == 0)
        {
            freeNum++;
        }
    }

    return freeNum;
}


void mm_init(void)
{
    if (mmInitialed) return;

    int *infoNum = (int *)0xe000;
    MEMINFO *memInfo = (MEMINFO *)0xe004;

    for (int i = 0; i < *infoNum; ++i)
    {
        printf("%llu bytes from %llx with type %u\n", memInfo[i].size, memInfo[i].addr, memInfo[i].type);
        if (memInfo[i].type == 1)
        {
            if (memInfo[i].addr + memInfo[i].size < 0x100000) continue;
            if (memInfo[i].addr < 0x100000)
            {
                memInfo[i].size -= 0x100000 - memInfo[i].addr;
                memInfo[i].addr = 0x100000;
                if (memInfo[i].size == 0) continue;
            }

            memInfo[i].size -= (memInfo[i].addr + 0xfff) / 0x1000 * 0x1000 - memInfo[i].addr;
            memInfo[i].size = memInfo[i].size / 0x1000 * 0x1000;

            unsigned int temp = pageNum;
            pageNum += memInfo[i].size / 0x1000;
            while (temp < pageNum)
            {
                physMemMap[temp].addr = memInfo[i].addr;
                physMemMap[temp].state = 0;
                memInfo[i].addr += 0x1000;
                ++temp;
            }
        }
    }

    for (unsigned int i = 0; i < (pageNum * sizeof(PHYS_MEM_PAGE) + 0xfff) / 0x1000; ++i)
    {
        physMemMap[i].state = 0xffffffff;
    }

    printf("Free RAM: %llu Bytes\n\n", get_free_page_num() * 0x1000);
    mmInitialed = 1;
}


void set_paging(void)
{
    for (int i = 0; i < 1024; ++i)
    {
        paging_dir[i] = (uint32_t)(paging_table + i * 1024) | 0x7;
        for (int j = 0; j < 1024; ++j)
        {
            (paging_table + i * 1024)[j] = (uint32_t)((0x1000 * (i * 1024 + j)) | 0x7);
        }
    }

    enable_paging(PAGE_DIR_BASE);
}
