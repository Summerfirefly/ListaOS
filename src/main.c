#include "stdio.h"
#include "console.h"
#include "display.h"
#include "interrupt.h"
#include "keyboard.h"
#include "mm.h"
#include "timer.h"

/* 
 *         Simple Memory Map
 * 
 *  Size         Address         Use for
 *  31KB  0x00000000-0x00007e00  Stack
 * 0.5KB  0x00007e00-0x00008000  Current VBE Mode Info
 *   4KB  0x0000e000-0x0000f000  Memory Info(int 15H)
 * 0.5KB  0x0000f000-0x0000f200  SVGA Info
 *   2KB  0x00010000-0x00010800  IDT
 *  64KB  0x00020000-0x00030000  Kernel
 *  32KB  0x000b8000-0x000c0000  Text Mode VRAM
 * 0.39%  0x00100000-0x????????  RAM Map
 */


int main(void)
{
	console_init();
	display_init();
    screen_clear();
    printf("Lista OS by Kayo Hikawa\n\n");

    idt_init();
    pic_init();
    timer_init();
    mm_init();
    printf("Boot completed!\n");
	printf("%d-bits Mode\n\n", sizeof(unsigned int *) * 8);
    /*
    unsigned int xAddr = *((unsigned int *)0xf00e);
	unsigned short *x = (unsigned short *)(((xAddr&0xffff0000) >> 12) + (xAddr & 0x0000ffff));
	for (int i = 0; x[i] != 0xffff; ++i)
	{
	    VBE_MODE_INFO *vbeModes = (VBE_MODE_INFO *)(0x30000 + i * 0x100);
		if ((vbeModes->modeAttributes&0x98) != 0x98) continue;
		printf("%X, %ux%u, ",
		        x[i], vbeModes->xResolution, vbeModes->yResolution);
		switch (vbeModes->memoryModel)
		{
			case 0:
				printf("Text\n");
				break;
			case 4:
				printf("Packed pixel, %d-bits color\n", vbeModes->bitsPerPixel);
				break;
			case 6:
				printf("Direct color, %d-bits color\n", vbeModes->bitsPerPixel);
				break;
			default:
				printf("\n");
		}
	}*/

    for (;;)
    {
        if (count % 5 == 0)
        {
            keyboard_buffer_clear();
        }
		
		if (count % 50 == 0)
		{
			toggle_cursor();
		}

		__asm__("hlt":::);
    }
}
