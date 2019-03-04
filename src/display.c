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
#include "memory.h"

#define X_RES (vbeInfo->xResolution)
#define Y_RES (vbeInfo->yResolution)
#define BYTE_PER_PIXEL (vbeInfo->bitsPerPixel / 8)

VBE_MODE_INFO *vbeInfo = (VBE_MODE_INFO *)VBE_INFO_ADDR;


void display_init(void)
{
}


inline void draw_pixel(int x, int y, RGB rgb)
{
	unsigned char *pixelAddr = (unsigned char *)((X_RES * y + x) * BYTE_PER_PIXEL + vbeInfo->physBasePtr);
	pixelAddr[0] = rgb.B;
	pixelAddr[1] = rgb.G;
	pixelAddr[2] = rgb.R;
}


/*
 * Clean the screen and reset the location of cursor
 * CAN NOT GET BACK DATA AFTER CLEANING
 */
void screen_clear(void)
{
	memset((void *)vbeInfo->physBasePtr, 0x00, X_RES * Y_RES * BYTE_PER_PIXEL);
}
