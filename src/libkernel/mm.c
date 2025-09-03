#include "mm.h"
#include "mm_internal.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"


uint32_t *physMemBitmap = (uint32_t *)PHY_MEM_BITMAP_BASE; // RAM pages bitmap, set bit for free page, clear for used
uint32_t physMemBitmapSize = 0;
bool mmInitialed = false;


void mm_init(void)
{
    if (mmInitialed)
    {
        return;
    }

    // Unmap temporarily map in bootloader init stage
    kernel_page_unmap(0x8);
    kernel_page_unmap(0x10);

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


void kernel_page_mmap(uint32_t phy_page_index, uint32_t vpage_index)
{
    uint32_t *pageTable = (uint32_t *)PAGE_TABLE_VBASE;
    pageTable[vpage_index] = (phy_page_index << 12) | 0x7;
    __asm__(
        "invlpg (%0)\n\t"
        :
        : "r"(vpage_index * 0x1000)
        : "memory");
}


void kernel_page_unmap(uint32_t vpage_index)
{
    uint32_t *pageTable = (uint32_t *)PAGE_TABLE_VBASE;
    pageTable[vpage_index] = 0;
    __asm__(
        "invlpg (%0)\n\t"
        :
        : "r"(vpage_index * 0x1000)
        : "memory");
}
