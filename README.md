# ListaOS
A simple operating system for learning perpose

## Physical Memory Map

| Size |        Address        |                   Use for                   |
| ---: | :-------------------: | :------------------------------------------ |
|  28K | 0x00000000-0x00006fff | Kernel Stack                                |
| 0.5K | 0x00007e00-0x00007fff | Current VBE Mode Info                       |
|   4K | 0x0000e000-0x0000efff | Memory Info(int 15H)                        |
| 0.5K | 0x0000f000-0x0000f1ff | SVGA Info                                   |
|   2K | 0x00010000-0x000107ff | IDT                                         |
|  64K | 0x00020000-0x0002ffff | Kernel                                      |
|  64K | 0x00050000-0x0005ffff | Console Text Buffer                         |
|   4K | 0x001ff000-0x001fffff | Kernel Paging Directory                     |
|   4M | 0x00200000-0x005fffff | Kernel Paging Table                         |
|   2M | 0x00600000-0x007fffff | Physical RAM 4K page bitmap for management  |

## Virtual Address Map

| Size |        Address        |                   Use for                   |
| ---: | :-------------------: | :------------------------------------------ |
|   3G | 0x00000000-0xbfffffff | Free to use                                 |
|  64K | 0xc0000000-0xc000ffff | Kernel                                      |
|  28K | 0xffbf8000-0xffbfefff | Kernel Stack                                |
|   4K | 0xffbff000-0xffbfffff | Paging Directory                            |
|   4M | 0xffc00000-0xffffffff | Paging Table                                |
