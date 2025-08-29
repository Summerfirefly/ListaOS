.globl set_suitable_video_mode
.set VBE_MODE_INFO_ADDR, 0x7e00
.set MAX_VRAM_SIZE_BIT,  0x00800000  # 8MB VRAM (64Mb)

.section .text
.code16
# uint16_t set_suitable_video_mode(void)
set_suitable_video_mode:
    pushw %bp
    pushw %di
    movw %sp, %bp
    subw $2, %sp

    # uint16_t videoMode = 0xffff
    movw $0xffff, -2(%bp)

    # Get VGA Info, store at 0xf000
    movw $0xf000, %di
    movw $0x0000, %ax
    movw %ax, %es
    movw $0x4f00, %ax
    int  $0x10

    # Iterate All Video Mode
    # %edx point to next video mode number (uint16_t)
    movl $0x0000f00e, %edx
    movl (%edx), %edx
    movw %dx, %ax
    andl $0xffff0000, %edx
    andl $0x0000ffff, %eax
    shr  $12, %edx
    addl %eax, %edx

    # Use 0x7e00-0x7fff as video mode info temp
    # ax = get_vesa_vbe_mode_info(*edx, VBE_MODE_INFO_ADDR)
2:  pushw $VBE_MODE_INFO_ADDR
    movw (%edx), %ax
    pushw %ax
    call get_vesa_vbe_mode_info
    addw $4, %sp
    # ax = is_suitable_video_mode(VBE_MODE_INFO_ADDR)
    pushl %edx
    pushw $VBE_MODE_INFO_ADDR
    call is_suitable_video_mode
    addw $2, %sp
    popl %edx
    # ax == 1?
    cmpw $1, %ax
    jne  set_suitable_video_mode_check_next_mode
    # videoMode = *edx
    movw (%edx), %ax
    movw %ax, -2(%bp)
set_suitable_video_mode_check_next_mode:
    addl $2, %edx
    cmpw $0xffff, (%edx)
    jne  2b

    cmpw $0xffff, -2(%bp)
    je   set_suitable_video_mode_error
    movw -2(%bp), %ax
    pushw %ax
    call set_vesa_video_mode
    addw $2, %sp
    cmpw $0, %ax
    jne  set_suitable_video_mode_error

    pushw $VBE_MODE_INFO_ADDR
    movw -2(%bp), %ax
    pushw %ax
    call get_vesa_vbe_mode_info
    addw $4, %sp
    cmpw $0, %ax
    jne  set_suitable_video_mode_error

set_suitable_video_mode_success:
    movw $0, %ax
    jmp  set_suitable_video_mode_return
set_suitable_video_mode_error:
    movw $1, %ax
set_suitable_video_mode_return:
    addw $2, %sp
    popw %di
    popw %bp
    ret

# uint16_t is_suitable_video_mode(uint16_t info_addr)
is_suitable_video_mode:
    pushw %bp
    pushw %bx
    movw %sp, %bp
    subw $2, %sp
    movw 6(%bp), %bx
    movw (%bx), %ax
    andw $0x98, %ax
    cmpw $0x98, %ax
    jne  not_suitable_video_mode
    cmpb $0x06, 27(%bx)
    jne  not_suitable_video_mode
    cmpb $24, 25(%bx)
    jl   not_suitable_video_mode
    xorl %eax, %eax
    xorl %edx, %edx
    movb 25(%bx), %al
    movw 18(%bx), %cx
    mulw %cx
    shl  $16, %edx
    addl %edx, %eax
    xorl %edx, %edx
    movw 20(%bx), %cx
    andl $0x0000ffff, %ecx
    mull %ecx
    movl $8, %ecx
    divl %ecx
    cmpl $MAX_VRAM_SIZE_BIT, %eax            # Max 8MB (64Mb) video memory
    jg   not_suitable_video_mode
    cmpl SuitableModeVRAM, %eax
    jng  not_suitable_video_mode
    movl %eax, SuitableModeVRAM
    movw $1, %ax
    jmp  is_suitable_video_mode_return
not_suitable_video_mode:
    movw $0, %ax
is_suitable_video_mode_return:
    addw $2, %sp
    popw %bx
    popw %bp
    ret

# uint16_t get_vesa_vbe_mode_info(uint16_t video_mode, uint16_t info_addr)
get_vesa_vbe_mode_info:
    pushw %bp
    pushw %di
    movw %sp, %bp
    movw $0x0000, %ax
    movw %ax, %es
    movw 8(%bp), %di
    movw $0x4f01, %ax
    movw 6(%bp), %cx
    int  $0x10
    cmp  $0x004f, %ax
    jne  get_vesa_vbe_mode_info_error
    movw $0x0000, %ax
    jmp  get_vesa_vbe_mode_info_return
get_vesa_vbe_mode_info_error:
    movw $0x0001, %ax
get_vesa_vbe_mode_info_return:
    popw %di
    popw %bp
    ret

# uint16_t set_vesa_video_mode(uint16_t video_mode)
set_vesa_video_mode:
    pushw %bp
    pushw %bx
    movw %sp, %bp
    movw 6(%bp), %bx
    addw $0x4000, %bx
    movw $0x4f02, %ax
    int  $0x10
    cmp  $0x00, %ah
    jne  set_vesa_video_mode_error
    movw $0x0000, %ax
    jmp  set_vesa_video_mode_return
set_vesa_video_mode_error:
    movw $0x0001, %ax
set_vesa_video_mode_return:
    popw %bx
    popw %bp
    ret

.section .data

SuitableModeVRAM:   .long 0x00000000
