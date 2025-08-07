#include "mm.h"
#include "stdio.h"
#include "io.h"

static PHYS_MEM_PAGE *physMemMap = (PHYS_MEM_PAGE *) 0x100000;
static unsigned int pageNum = 0;
static int mmInitialed = 0;

//unsigned int *paging_dir = (unsigned int *) 0x11000;


//void *alloc_4k_phys(unsigned int num)


unsigned long long get_free_page_num(void)
{
    unsigned long long freeNum = 0;

    for (unsigned long long i = 0; i < pageNum; ++i)
    {
        if (physMemMap[i].state == 0) freeNum++;
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

/*
void _set_paging()
{
    int i = 0;
    for (i = 0; i < 1024; ++i)
    {
        ((unsigned int *)(0x13000))[i] = (unsigned int) ((0x1000 * i) | 4 | 2 | 1);
    }

    for (i = 0xb8; i <= 0xc0; ++i)
    {
        ((unsigned int *)(0x14000))[i - 0xb8] = (unsigned int)((0x1000 * i) | 3);
    }

    paging_dir[0] = 0x13000 | 3;
    paging_dir[1] = 0x14000 | 3;
    _enable_paging(0x11000);
    printf("CR0 = 0x%X, CR3 = 0x%X\n", _get_cr0(), _get_cr3());

    char *test_ptr = (char *)0x400000;
    char *log_str = "Paging Enabled!";
    for (i = 0; log_str[i] != '\0'; ++i)
    {
        *(test_ptr++) = log_str[i];
        *(test_ptr++) = 0x2;
    }
}
*/