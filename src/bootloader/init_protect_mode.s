.globl protect_mode_start

.set PROT_MODE_DSEG, 0x10
.set PAGE_DIRECTORY_BASE, 0x001ff000
.set PAGE_TABLE_BASE, 0x00200000

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

    call  init_kernel_paging
    pushl $0
    pushl $0x100000
    pushl $0
    call  memory_page_map
    addl  $12, %esp

    movl  $PAGE_DIRECTORY_BASE, %eax
    movl  %eax, %cr3
    movl  %cr0, %eax
    orl   $0x80000001, %eax
    movl  %eax, %cr0

    call  0x20000

loop:
    jmp loop

# void memclr(uint32_t start_addr, uint32_t size)
memclr:
    pushl %ebp
    pushl %edi
    movl %esp, %ebp
    xorl %eax, %eax
    movl 12(%ebp), %edi
    movl 16(%ebp), %ecx
    cld
    rep stosb
    popl %edi
    popl %ebp
    ret

# void memory_page_map(uint32_t start_page_index, uint32_t page_count, uint32_t target_start_page_index)
memory_page_map:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %eax
    movl 12(%ebp), %ecx
    movl 16(%ebp), %edx
    shll $12, %edx
    shll $2, %eax
    addl $PAGE_TABLE_BASE, %eax
1:  cmpl $0, %ecx
    je   memory_page_map_return
    movl %edx, (%eax)
    orl  $0x7, (%eax)
    addl $4, %eax
    decl %ecx
    addl $0x1000, %edx
    jmp  1b
memory_page_map_return:
    popl %ebp
    ret

# void init_kernel_paging(void)
init_kernel_paging:
    pushl %ebp
    movl %esp, %ebp
    subl $4, %esp
    movl $0, -4(%ebp)
    movl $PAGE_DIRECTORY_BASE, %ecx
    movl %ecx, %eax
    addl $0x1000, %eax
1:  movl %eax, %edx
    orl  $0x7, %edx
    movl %edx, (%ecx)
    addl $0x1000, %eax
    addl $4, %ecx
    addl $1, -4(%ebp)
    cmpl $0x400, -4(%ebp)
    jne  1b
    pushl $0x00400000
    pushl $PAGE_TABLE_BASE
    call memclr
    addl $12, %esp
    popl %ebp
    ret
