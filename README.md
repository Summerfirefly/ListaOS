# ListaOS
A simple operating system for learning perpose

## Physical Memory Map

| Size |        Address        |                   Use for                   |
| ---: | :-------------------: | :------------------------------------------ |
|  28K | 0x00000000-0x00006fff | Kernel Stack                                |
| 0.5K | 0x00007e00-0x00007fff | Current VBE Mode Info                       |
|   4K | 0x0000e000-0x0000efff | Memory Info(int 15H)                        |
|   2K | 0x00010000-0x000107ff | IDT                                         |
|   2K | 0x00010800-0x00010fff | GDT                                         |
|  64K | 0x00020000-0x0002ffff | Kernel                                      |
|   4K | 0x001ff000-0x001fffff | Kernel Paging Directory                     |
|   4M | 0x00200000-0x005fffff | Kernel Paging Table                         |
|   2M | 0x00600000-0x007fffff | Physical RAM 4K page bitmap for management  |

## Virtual Address Map

| Size |        Address        |                   Use for                   |
| ---: | :-------------------: | :------------------------------------------ |
|   3G | 0x00000000-0xbfffffff | Free to use                                 |
|  64K | 0xc0000000-0xc000ffff | Kernel                                      |
|   2K | 0xc0010000-0xc00107ff | IDT                                         |
|   2K | 0xc0010800-0xc0010fff | GDT                                         |
| 0.5K | 0xc0011e00-0xc0011fff | Current VBE Mode Info                       |
|   4K | 0xc0012000-0xc0012fff | Memory Info(int 15H)                        |
|   2M | 0xc0600000-0xc07fffff | Physical RAM 4K page bitmap for management  |
|   8M | 0xc1000000-0xc17fffff | Video Memory (Up to 1920x1080 32-bit color) |
|  28K | 0xffbf8000-0xffbfefff | Kernel Stack                                |
|   4K | 0xffbff000-0xffbfffff | Paging Directory                            |
|   4M | 0xffc00000-0xffffffff | Paging Table                                |
