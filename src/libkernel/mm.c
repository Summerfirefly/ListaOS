#include "mm.h"

#include <stdbool.h>
#include "stdio.h"
#include "io.h"

#define PAGE_DIR_BASE 0x1f0000
#define PAGE_TABLE_BASE 0x200000

#define PHY_MEM_BITMAP_BASE 0x800000

uint32_t *physMemBitmap = (uint32_t *)PHY_MEM_BITMAP_BASE; // RAM pages bitmap, set bit for free page, clear for used
uint32_t physMemBitmapSize = 0;
bool mmInitialed = false;

uint32_t *paging_dir = (uint32_t *)PAGE_DIR_BASE;
uint32_t *paging_table = (uint32_t *)PAGE_TABLE_BASE;


//void *alloc_4k_phys(unsigned int num)


unsigned long long get_free_page_num(void)
{
    unsigned long long freeNum = 0;

    for (uint32_t i = 0; i < physMemBitmapSize; ++i)
    {
        uint32_t tmp = physMemBitmap[i];
        for (int j = 0; j < 32; ++j)
        {
            if ((tmp & 1) == 1)
            {
                freeNum++;
            }

            tmp = tmp >> 1;
        }
    }

    return freeNum;
}


void mm_init(void)
{
    if (mmInitialed)
    {
        return;
    }

    int *infoNum = (int *)0xe000;
    MEMINFO *memInfo = (MEMINFO *)0xe004;

    for (int i = 0; i < *infoNum; ++i)
    {
        printf("%llu bytes from %llx with type %u\n", memInfo[i].size, memInfo[i].addr, memInfo[i].type);

        uint64_t addr = memInfo[i].addr;
        uint64_t size = (int64_t)memInfo[i].size;
        if ((addr & ~(0xfff)) != addr)
        {
            // Start addr is not 4K aligned, discard
            uint64_t newAddr = (addr & 0xfff) + 0x1000;
            if (newAddr - addr >= size)
            {
                continue;
            }

            size -= newAddr - addr;
            addr = newAddr;
        }

        while (size >= 0x1000) // Only use 4K aligned memory
        {
            uint32_t pageIndex = addr / 0x1000;
            uint32_t mapIndex = pageIndex / 32;
            uint8_t bitPos = pageIndex % 32;

            while (mapIndex >= physMemBitmapSize)
            {
                physMemBitmap[physMemBitmapSize++] = 0;
            }

            uint32_t mask = memInfo[i].type == 1 ? 1 << bitPos : 0;
            physMemBitmap[mapIndex] |= mask;
            addr += 0x1000;
            size -= 0x1000;
        }
    }

    printf("Free RAM: %llu Bytes\n\n", get_free_page_num() * 0x1000);
    mmInitialed = true;
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
