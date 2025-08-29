.globl protect_mode_start

.set PROT_MODE_DSEG, 0x10

.section .text
.code32
protect_mode_start:
    movw  $PROT_MODE_DSEG, %ax
    movw  %ax, %ds
    movw  %ax, %es
    movw  %ax, %fs
    movw  %ax, %gs
    movw  %ax, %ss
    movl  $0x7000, %ebp
    movl  %ebp, %esp
    call  0x20000

loop:
    jmp loop
