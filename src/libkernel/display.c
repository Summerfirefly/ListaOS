/*
 * display.c
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for screen display and cursor control
 *
 * Licenced under LGPLv3
 *
 */

#include "display.h"

#include "mm_internal.h"
#include "string.h"
#include <stdint.h>

#ifdef DEBUG
#include "stdio.h"
#endif

#define X_RES (vbeInfo->xResolution)
#define Y_RES (vbeInfo->yResolution)
#define BYTE_PER_PIXEL (vbeInfo->bitsPerPixel / 8)
#define VRAM_BASE 0xc1000000
#define VRAM_SIZE 0x00800000

VBE_MODE_INFO *vbeInfo = (VBE_MODE_INFO *)VBE_INFO_ADDR;


void display_init(void)
{
    for (uint32_t i = 0; i < VRAM_SIZE / 0x1000; ++i)
    {
        kernel_page_mmap(vbeInfo->physBasePtr / 0x1000 + i, VRAM_BASE / 0x1000 + i);
    }

    screen_clear();
#ifdef DEBUG
    printf("Selected Video Mode: %ux%u, %d-bits color\n", X_RES, Y_RES, BYTE_PER_PIXEL * 8);
#endif
}


inline void draw_pixel(int x, int y, RGB rgb)
{
    unsigned char *pixelAddr = (unsigned char *)((X_RES * y + x) * BYTE_PER_PIXEL + VRAM_BASE);
    pixelAddr[0] = rgb.B;
    pixelAddr[1] = rgb.G;
    pixelAddr[2] = rgb.R;
}


void get_display_info(DISPLAY_INFO *display)
{
    display->width = X_RES;
    display->height = Y_RES;
    display->bytePerPixel = BYTE_PER_PIXEL;
}


/*
 * Clean the screen and reset the location of cursor
 * CAN NOT GET BACK DATA AFTER CLEANING
 */
void screen_clear(void)
{
    memset((void *)VRAM_BASE, 0x00, X_RES * Y_RES * BYTE_PER_PIXEL);
}
