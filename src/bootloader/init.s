.set PROT_MODE_CSEG, 0x8

.globl _start
.extern set_suitable_video_mode
.extern protect_mode_start

.section .text
.code16
_start:
    movb %dl, DriverNumber
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es

    # Calculate FAT16 related sector number
    movw %bx, %ax
    addw $0x0004, %ax               # 4 hidden sector for FAT16
    movw %ax, FAT1StartSector

    movw $0x8000, %bx
    movb 0x0d(%bx), %al
    movb %al, SectorsPerCluster

    xorw %ax, %ax
    movb 0x10(%bx), %al
    movw 0x16(%bx), %cx
    mulw %cx
    addw FAT1StartSector, %ax
    movw %ax, RootDirStartSector

    movw 0x11(%bx), %cx             # FAT16 root directory entries count
    movw $0x20, %ax                 # 32 bytes per entry
    mulw %cx
    movw 0x0b(%bx), %cx
    movw %cx, BytesPerSector
    divw %cx
    cmp  $0x0000, %dx
    je   no_remainder
    inc  %ax
no_remainder:
    movw %ax, RootEntrySectorCount
    addw RootDirStartSector, %ax
    movw %ax, DataStartSector

    xorw %ax, %ax
    movb SectorsPerCluster, %al
    mulw BytesPerSector
    movw %ax, %bx
    andl $0x0000ffff, %ebx
    movw %dx, %ax
    shll $16, %eax
    addl %ebx, %eax
    movl %eax, BytesPerCluster

    # Find kernel.sys in root directory
    pushw $KernelFileName
    call find_file
    cmpw $0x0000, %ax
    je   file_missed_error

    # Load kernel.sys from 0x20000
    movw %ax, %bx
    movw $0x0000, %ax
    movw $0x2000, %dx
    call load_file

    movb $0x03, %al
    movb $0x00, %ah
    int  $0x10

    call set_suitable_video_mode
    cmpw $0, %ax
    jne  vesa_error

    # A20 Gate Control
    inb  $0x92, %al
    orb  $0x02, %al
    outb %al, $0x92

    # Get Memory Info with INT15H-E820
    # Start At Addr 0xe000
    movw MemMapAddr, %bx
    movl $0, (%bx)
    leaw 4(%bx), %di
    xorl %ebx, %ebx
    movw %bx, %es
start_probe:
    movl $0x0000e820, %eax
    movl $0x00000014, %ecx
    movl $0x534D4150, %edx
    int  $0x15
    jc   finish_probe
    addw $0x14, %di
    incl MemMapLen
    cmpl $0x0, %ebx
    jnz start_probe
finish_probe:
    movw MemMapAddr, %bx
    movl MemMapLen, %eax
    movl %eax, (%bx)

    cli
    data32 addr32 lgdt gdtdesc

    movl %cr0, %eax
    orl  $0x1, %eax
    mov  %eax, %cr0
    data32 ljmp $PROT_MODE_CSEG, $protect_mode_start

fin:
    jmp  fin

vesa_error:
    movw $msgVESAError, %ax
    movw %ax, %bp
    jmp  print_error

load_error:
    movw $msgLoadError, %ax
    movw %ax, %bp
    jmp  print_error

file_missed_error:
    movw $msgFileMissed, %ax
    movw %ax, %bp
    jmp  print_error

print_error:
    movw $0x000f, %bx
    movw $0x1301, %ax
    movw $0x000c, %cx
    movb $0x00, %dl
    movb $0x00, %dh
    int  $0x10
    jmp  fin

# Function find_file
#   Find file in FAT16 filesystem root directory
#   FAT table will be loaded from 0x9000
#   Params:
#       4(%bp): pointer to filename
#   Return:
#       %ax: Memory address of directory entry in root directory
find_file:
    pushw %bp
    movw %sp, %bp
    subw $2, %sp
    pushw %si
    pushw %di
    movw $0x9000, off
    movw $0x0000, seg
    movw RootEntrySectorCount, %cx
    movw %cx, -2(%bp)
    movw RootDirStartSector, %ax
    movw %ax, start_sector
    movw $1, sector_count       # Load 1 sector of root directory table
find_start:
    call read_sector
    movw off, %di
    movw BytesPerSector, %dx
2:  movw 4(%bp), %si
    movw $0x000b, %cx
    cld
    repe cmpsb
    jne  next_file              # Compare filename, if different, jump to next file
    andw $0xfff0, %di
    movw %di, %ax
    jmp  end_find_file
next_file:
    andw $0xfff0, %di
    addw $0x0020, %di
    subw $0x0020, %dx
    jnz  2b
    decw -2(%bp)                # File not found in loaded sector
    jz   file_not_found         # All root entry table sectors have been checked, file not found
    addw $0x0001, start_sector
    jmp  find_start             # Start to search next sector of root entry table
file_not_found:
    movw $0x0000, %ax
end_find_file:
    popw %di
    popw %si
    movw %bp, %sp
    popw %bp
    ret

# Function load_file
#   Load file from root directory of FAT16 filesystem
#   Note: this function will use a sector size of memory from 0x9000 to store FAT table
#   Params:
#       %ax: Load address offset
#       %dx: Load address segment
#       %bx: FAT entry address of the file to load
load_file:
    pushw %bp
    movw %sp, %bp
    subw $6, %sp
    xorl %ecx, %ecx
    movw %dx, %cx
    shl  $4, %ecx
    addw %ax, %cx
    movl %ecx, -4(%bp)          # Save load address into stack
    movw 0x1a(%bx), %ax         # Save file start cluster number into %ax
2:  movw %ax, %bx
    movw %ax, -6(%bp)
    subw $2, %bx                # Data cluster number is start from 2 in FAT16 filesystem, minus 2 first
    xorw %ax, %ax
    movb SectorsPerCluster, %al
    mul  %bx
    movw %dx, %bx
    shl  $16, %ebx
    movw %ax, %bx
    addl DataStartSector, %ebx  # Calculate and save file data sector into %ebx
    movw $2, %ax
    xorw %dx, %dx
    mulw -6(%bp)
    divw BytesPerSector
    addw FAT1StartSector, %ax   # Calculate and save FAT table sector into %ax
read_fat:
    andl $0x0000ffff, %eax
    movw $0x0001, sector_count
    movl %eax, start_sector
    movw $0x9000, off
    movw $0x0000, seg
    call read_sector
# Load File
    xorw %ax, %ax
    movb SectorsPerCluster, %al
    movw %ax, sector_count
    movl %ebx, start_sector
    movl -4(%bp), %ecx
    movw %cx, off
    andl $0xffff0000, %ecx
    shr  $4, %ecx
    movw %cx, seg
    call read_sector
    movl BytesPerCluster, %ecx
    addl %ecx, -4(%bp)
    movw $2, %ax
    xorw %dx, %dx
    mulw -6(%bp)
    divw BytesPerSector
    movw %dx, %bx
    movw 0x9000(%bx), %ax
    cmpw $0xfff8, %ax
    jb   2b
    movw %bp, %sp
    popw %bp
    ret

read_sector:
    pushw %bp
    movw %sp, %bp
    pushw %si
    movb $0x42, %ah             # Read disk
    movb DriverNumber, %dl
    movw $info_packet, %si
    int  $0x13
    cmp  $0x00, %ah
    jne  load_error
    popw %si
    popw %bp
    ret

.section .data

MemMapAddr:         .word 0xe000
MemMapLen:          .int  0x00000000

info_packet:
size:           .byte 0x10          # size of packet
reverse:        .byte 0x00          # not used
sector_count:   .word 0x0001        # amount to read
off:            .word 0x0000        # memary address offset
seg:            .word 0x1000        # memary address segment
start_sector:   .int  0x00000000
                .int  0x00000000

msgVESAError:           .ascii  "VESA error! "
msgLoadError:           .ascii  "Load error! "
msgFileMissed:          .ascii  "File missed!"
FAT1StartSector:        .word   0x0000
RootEntrySectorCount:   .word   0x0000
RootDirStartSector:     .int    0x00000000
DataStartSector:        .int    0x00000000
SectorsPerCluster:      .byte   0x20
BytesPerSector:         .word   0x0200
BytesPerCluster:        .int    0x00004000
DriverNumber:           .byte   0x00
KernelFileName:         .ascii  "KERNEL  SYS"

#### GDT
    .align 8
gdt:
    .quad 0x0000000000000000    # Null segment.  Not used by CPU.
    .quad 0x00cf9a000000ffff    # System code, base 0, limit 4 GB.
    .quad 0x00cf92000000ffff    # System data, base 0, limit 4 GB.

gdtdesc:
    .word   gdtdesc - gdt - 1   # Size of the GDT, minus 1 byte.
    .long   gdt                 # Address of the GDT.
