#include "mm.h"
#include "mm_internal.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"
#include "io.h"


uint32_t *physMemBitmap = (uint32_t *)PHY_MEM_BITMAP_BASE; // RAM pages bitmap, set bit for free page, clear for used
uint32_t physMemBitmapSize = 0;
bool mmInitialed = false;

uint32_t *paging_dir = (uint32_t *)PAGE_DIR_BASE;
uint32_t *paging_table = (uint32_t *)PAGE_TABLE_BASE;


void mm_init(void)
{
    if (mmInitialed)
    {
        return;
    }

    uint32_t infoNum = *(uint32_t *)BIOS_MEM_MAP_BASE;
    BIOS_MEMINFO *memInfo = (BIOS_MEMINFO *)(BIOS_MEM_MAP_BASE + 4);

    for (uint32_t i = 0; i < infoNum; ++i)
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

            uint32_t mask = (memInfo[i].type == 1 && addr >= USABLE_PHY_ADDR_BASE) ? 1 << bitPos : 0;
            physMemBitmap[mapIndex] |= mask;
            addr += 0x1000;
            size -= 0x1000;
        }
    }

    printf("Free RAM: %llu Bytes\n\n", get_free_page_num() * 0x1000);
    mmInitialed = true;

    set_paging();
}

/********************************** Internal **********************************/

void *alloc_4k_phys()
{
    for (uint32_t i = 0; i < physMemBitmapSize; ++i)
    {
        if (physMemBitmap[i] == 0)
        {
            continue;
        }

        uint32_t addr = i * 0x1000 * 32;
        size_t bitPos = 0;
        while ((physMemBitmap[i] & (1 << bitPos)) == 0)
        {
            bitPos++;
        }

        addr += bitPos * 0x1000;
        physMemBitmap[i] &= ~(1 << bitPos);
        return (void *)addr;
    }

    return NULL;
}


void free_4k_phys(void *addr)
{
    uint32_t pageIndex = (uint32_t)addr / 0x1000;
    uint32_t mapIndex = pageIndex / 32;
    uint8_t bitPos = pageIndex % 32;
    physMemBitmap[mapIndex] |= 1 << bitPos;
}


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
