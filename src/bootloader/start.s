.set PROT_MODE_CSEG, 0x8
.set PROT_MODE_DSEG, 0x10

.globl _start
_start:
.code16
    movb %dl, DriverNumber
    xor  %ax, %ax
    movw %ax, %ds
    movw %ax, %es

    # Calculate FAT16 related sector number
    movw %bx, %ax
    addw $0x0005, %ax
    movw %ax, FAT1StartSector

    movw $0x8000, %bx
    xor  %ax, %ax
    movb 0x0d(%bx), %al
    movb %al, SectorsPerCluster

    movb 0x10(%bx), %al
    movw 0x16(%bx), %cx
    mul  %cx
    addw FAT1StartSector, %ax
    movw %ax, RootDirStartSector

    movw 0x11(%bx), %cx
    movw 0x20, %ax
    mul  %cx
    movw 0x0b(%bx), %cx
    movw %cx, BytesPerSector
    divw %cx
    cmp  0x0000, %dx
    je   no_remainder
    inc  %ax
no_remainder:
    movw %ax, RootEntrySectorCount
    addw RootDirStartSector, %ax
    movw %ax, DataStartSector

    # Find kernel.sys in root directory
    movw $KernelFileName, %si
    call find_file
    cmpw $0x0000, %ax
    je   file_missed_error

    # Load kernel.sys from 0x20000
    movw $0x0000, off
    movw $0x2000, seg
    call load_file

    movb $0x03, %al
    movb $0x00, %ah
    int  $0x10

    movw SVGAInfoAddr, %di
    movw $0x0000, %ax
    movw %ax, %es
    movw $0x4f00, %ax
    int  $0x10

    #Get VBE Information
    movw $0x0000, %ax
    movw %ax, %es
    movw VBEModeInfoAddr, %di
    movw $0x4f01, %ax
    movw $0x0118, %cx
    int  $0x10

    #Get All Video Mode
    xorl %ebx, %ebx
    movw SVGAInfoAddr, %bx
    addl $0x0000000e, %ebx
    movl (%ebx), %ebx
    movw %bx, %dx
    andl $0xffff0000, %ebx
    andl $0x0000ffff, %edx
    shr  $12, %ebx
    addl %edx, %ebx
    movw $0x3000, %ax
    movw %ax, %es
    movw $0x0000, %di
2:  movw $0x4f01, %ax
    movw (%ebx), %cx
    cmp  $0xffff, %cx
    je   2f
    int  $0x10
    addw $0x0002, %bx
    addw $0x0100, %di
    jmp  2b

    #Set Video Mode 0x118(1024x768 True Color)
2:  movw $0x4118, %bx
    movw $0x4f02, %ax
    int  $0x10
    cmp  $0x00, %ah
    jnz  vesa_error
    jmp  continue

vesa_error:
    movw $msgVESAError, %ax
    movw %ax, %bp
    movw $0x0010, %cx
    movw $0x000f, %bx
    movw $0x1301, %ax
    movb $0x00, %dl
    movb $0x00, %dh
    int  $0x10
    jmp  vesa_error
msgVESAError: .ascii "VESA Not Support"

continue:
    inb  $0x92, %al
    orb  $0x02, %al
    outb %al, $0x92

    #Get Memory Info with INT15H-E820
    #Start At Addr 0xe000
    movl $0, 0xe000
    xorl %ebx, %ebx
    movw $0x0000, %ax
    movw %ax, %es
    movw $0xe004, %di
start_probe:
    movw $0xe820, %ax
    movl $0x00000014, %ecx
    movl $0x534D4150, %edx
    int  $0x15
    jnc  count
    movl $0xffffffff, 0xe000
    jmp finish_probe
count:
    addw $0x14, %di
    incl 0xe000
    cmpl $0x0, %ebx
    jnz start_probe
finish_probe:

    cli
    data32 addr32 lgdt gdtdesc

    movl %cr0, %eax
    orl  $0x1, %eax
    mov  %eax, %cr0
    data32 ljmp $PROT_MODE_CSEG, $1f

fin:
    jmp  fin

load_error:
    movw $msgLoadError, %ax
    movw %ax, %bp
    movw $0x000b, %cx
    jmp  print_msg

file_missed_error:
    movw $msgFileMissed, %ax
    movw %ax, %bp
    movw $0x000c, %cx
    jmp  print_msg

print_msg:
    movw $0x000f, %bx
    movw $0x1301, %ax
    movb $0x00, %dl
    movb $0x00, %dh
    int  $0x10
    jmp  fin

# Function find_file
#   Find file in FAT16 filesystem root directory
#   FAT table will be loaded from 0x9000
#   Params:
#       %si: filename
#   Return:
#       %ax: Memory address of directory enrty in root directory
find_file:
    movw $0x9000, off
    movw $0x0000, seg
    movw RootEntrySectorCount, %cx
    pushw %cx
    movw RootDirStartSector, %ax
    movw %ax, start_sector
    movw %si, %bx
find_start:
    # Load 1 sector of root directory table
    call read_sector
    movw off, %di
    movw BytesPerSector, %dx
2:  movw %bx, %si
    movb $0x0b, %ch
    cld
1:  movb (%di), %ah
    lodsb
    cmp  %ah, %al
    jne  next_file              # 逐字符比较文件名，有不同则跳到下一个文件
    inc  %di
    dec  %ch
    jnz  1b
    andw $0xfff0, %di
    movw %di, %ax
    ret
next_file:
    andw $0xfff0, %di
    addw $0x0020, %di
    subw $0x0020, %dx
    jnz  2b
    popw %cx
    dec  %cx                    # 此扇区内无文件
    jz   find_not_found         # 根目录无内核文件，抛出错误提示
    pushw %cx
    addw $0x0001, start_sector
    jmp  find_start             # 读取根目录表下一个扇区
find_not_found:
    movw $0x0000, %ax
    ret

# TODO: NEED REIMPLEMENT DUE TO CODE REFACTOR
# Function load_file
#   Load file from root directory of FAT16 filesystem
#   Note: this function will use a sector size of memory from 0x9000 to store FAT table
#   Params:
#       %ax: Load address offset
#       %dx: Load address segment
#       %bx: FAT entry address of the file to load
load_file:
    movw 26(%bx), %ax           # 将起始簇号放入%ax
    xorl %ecx, %ecx
    movw seg, %cx
    shl  $4, %ecx
    addw off, %cx               # %ecx中存放加载地址
2:  movw %ax, %bx
    movw %ax, %di
    subw $2, %bx                # FAT 16的数据区簇号从2开始，因此先减2
    andl $0x0000ffff, %ebx
    shl  $2, %ebx
    addl DataStartSector, %ebx  # 计算得到待读簇的首扇区号，存放在%ebx
    andw $0x00ff, %di
    shl  $1, %di
    addw $0x8000, %di
    shr  $8, %ax
    addw FAT1StartSector, %ax   # 此时%ax存有所需FAT表项存放的扇区号
read_fat:
    andl $0x0000ffff, %eax
    movw $0x0001, count
    movl %eax, start_sector
    movw $0x8000, off           # FAT记录会放在0x8000 - 0x8200, 注意
    movw $0x0000, seg
    call read_sector
# Load File
    movw $0x0004, count
    movl %ebx, start_sector
    movw %cx, off
    xorw %cx, %cx
    shr  $4, %ecx
    movw %cx, seg
    call read_sector
    xorl %ecx, %ecx
    movw seg, %cx
    shl  $4, %ecx
    addw off, %cx
    addl $0x00000800, %ecx
    movw (%di), %ax
    cmp  $0xfff0, %ax
    jb   2b
    ret

read_sector:
    movb $0x42, %ah             # 读磁盘功能号
    movb DriverNumber, %dl
    movw $info_packet, %si
    int  $0x13
    cmp  $0x00, %ah
    jne  load_error
    ret

info_packet:
size:           .byte 0x10          # size of packet
reverse:        .byte 0x00          # not used
count:          .word 0x0001        # amount to read
off:            .word 0x0000        # memary address offset
seg:            .word 0x1000        # memary address segment
start_sector:   .int  0x00000000
                .int  0x00000000

msgLoadError:           .ascii  "Load error!"
msgFileMissed:          .ascii  "File missed!"
FAT1StartSector:        .word   0x0000
RootEntrySectorCount:   .word   0x0000
RootDirStartSector:     .int    0x00000000
DataStartSector:        .int    0x00000000
SectorsPerCluster:      .byte   0x20
BytesPerSector:         .word   0x0200
DriverNumber:           .byte   0x00
KernelFileName:         .ascii  "KERNEL  SYS"

.code32
1:	movw  $PROT_MODE_DSEG, %ax
    movw  %ax, %ds
    movw  %ax, %es
    movw  %ax, %fs
    movw  %ax, %gs
    movw  %ax, %ss
    movl  $0x7e00, %esp
    movl  $0, %ebp
    #call  main
    call  0x20000

loop:
    jmp loop

VBEModeInfoAddr:    .word 0x7e00
SVGAInfoAddr:       .word 0xf000

#### GDT
    .align 8
gdt:
    .quad 0x0000000000000000    # Null segment.  Not used by CPU.
    .quad 0x00cf9a000000ffff    # System code, base 0, limit 4 GB.
    .quad 0x00cf92000000ffff    # System data, base 0, limit 4 GB.

gdtdesc:
    .word   gdtdesc - gdt - 1   # Size of the GDT, minus 1 byte.
    .long   gdt                 # Address of the GDT.
