.code16
.globl _start
_start:
    movb %dl, DriverNumber
    xor  %ax, %ax
    movw %ax, %ds
    movw %ax, %es

    cli
    movw %ax, %ss
    movw $0x7c00, %sp
    sti

    # Load GPT partition table to 0x8000
    movw $0x0002, start_sector
    call read_sector

    # Find Boot FAT16 filesystem start sector to bx
    movw $0x8000, %bx
    movw 0x20(%bx), %ax

    # Load init.bin from hidden sector
    movw %ax, start_sector
    movw $0x0004, count         # 4 hidden sector for FAT16
    call read_sector

    movw $msgCallInit, %ax
    movw %ax, %bp
    movw $0x000b, %cx
    call print_msg

    # Jump to init.bin code
    movb DriverNumber, %dl
    movw start_sector, %bx
    movw %sp, %bp
    call 0x8200

fin:
    jmp  fin

load_error:
    movw $msgLoadError, %ax
    movw %ax, %bp
    movw $0x000b, %cx
    call print_msg
    jmp  fin

print_msg:
    movw $0x000f, %bx
    movw $0x1301, %ax
    movb $0x00, %dl
    movb $0x00, %dh
    int  $0x10
    ret

read_sector:
    movb $0x42, %ah             # Function to read disk
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
off:            .word 0x8000        # memary address offset
seg:            .word 0x0000        # memary address segment
start_sector:   .int  0x00000000
                .int  0x00000000

msgLoadError:   .ascii  "Load error!"
msgCallInit:    .ascii  "Call  init!"
DriverNumber:   .byte   0x00

.org 446

gpt_protective:
.word 0x0000
.word 0x0002
.int  0xffffffee
.int  0x00000001
.int  0xffffffff

.org 510
.byte 0x55, 0xaa
