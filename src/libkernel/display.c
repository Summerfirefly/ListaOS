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

#include "string.h"

#ifdef DEBUG
#include "stdio.h"
#endif

#define X_RES (vbeInfo->xResolution)
#define Y_RES (vbeInfo->yResolution)
#define BYTE_PER_PIXEL (vbeInfo->bitsPerPixel / 8)

VBE_MODE_INFO *vbeInfo = (VBE_MODE_INFO *)VBE_INFO_ADDR;


void display_init(void)
{
    screen_clear();
#ifdef DEBUG
    unsigned int xAddr = *((unsigned int *)0xf00e);
    unsigned short *x = (unsigned short *)(((xAddr&0xffff0000) >> 12) + (xAddr & 0x0000ffff));
    int availableMode = 0;
    for (int i = 0; x[i] != 0xffff; ++i)
    {
        VBE_MODE_INFO *vbeModes = (VBE_MODE_INFO *)(0x30000 + i * 0x100);

        if ((vbeModes->modeAttributes & 0x98) != 0x98)
            continue;
        // Video memory requirement should less than 8MB
        if (vbeModes->xResolution * vbeModes->yResolution * vbeModes->bitsPerPixel / 8 > 8 * 1024 * 1024)
            continue;
        // Only print direct color mode
        if (vbeModes->memoryModel != 6)
            continue;
        // Only print 24-bits and 32-bits color mode
        if (vbeModes->bitsPerPixel != 24 && vbeModes->bitsPerPixel != 32)
            continue;

        printf("%X, %ux%u, %d-bits color",
                x[i], vbeModes->xResolution, vbeModes->yResolution, vbeModes->bitsPerPixel);
        availableMode++;
        if (availableMode % 4 == 0)
            printf("\n");
        else
            printf("\t");
    }

    printf("%c\n", availableMode % 4 == 0 ? '\0' : '\n');
#endif
}


inline void draw_pixel(int x, int y, RGB rgb)
{
    unsigned char *pixelAddr = (unsigned char *)((X_RES * y + x) * BYTE_PER_PIXEL + vbeInfo->physBasePtr);
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
    memset((void *)vbeInfo->physBasePtr, 0x00, X_RES * Y_RES * BYTE_PER_PIXEL);
}
