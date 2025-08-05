/*
 * display.h
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for screen display and cursor control
 *
 * Licenced under LGPLv3
 *
 */
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "color.h"

#define VBE_INFO_ADDR 0x7e00

typedef struct vbe_mode_info
{
	unsigned short modeAttributes;
	unsigned char  winAattributes;
	unsigned char  winBattributes;
	unsigned short winGranularity;
	unsigned short winSize;
	unsigned short winAsegment;
	unsigned short winBsegment;
	unsigned int   winfuncptr;
	unsigned short bytesPerScanLine;
	
	unsigned short xResolution;
	unsigned short yResolution;
	unsigned char  xCharSize;
	unsigned char  yCharSize;
	unsigned char  numberOfPlanes;
	unsigned char  bitsPerPixel;
	unsigned char  numberOfBanks;
	unsigned char  memoryModel;
	unsigned char  bankSize;
	unsigned char  numberOfImagePages;
	unsigned char  reserved1;
	
	unsigned char  redMaskSize;
	unsigned char  redFieldPosition;
	unsigned char  greenMaskSize;
	unsigned char  greenFieldPosition;
	unsigned char  blueMaskSize;
	unsigned char  blueFieldPosition;
	unsigned char  rsvdMaskSize;
	unsigned char  rsvdFieldPosition;
	unsigned char  directColorModeInfo;
	
	unsigned int   physBasePtr;
	unsigned int   offscreenMemOffset;
	unsigned short offscreenMemSize;
	unsigned char  reserved2[206];
} VBE_MODE_INFO;

void display_init(void);
void draw_pixel(int x, int y, RGB rgb);
void screen_clear(void);

#endif
