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

    # Temporarily map FAT16 header and init bin to 0x8000
    pushl $0x8
    pushl $0x1
    pushl $0x8
    call  memory_page_map
    addl  $12, %esp

    # Temporarily map IDT/GDT to 0x10000
    pushl $0x10
    pushl $0x1
    pushl $0x10
    call  memory_page_map
    addl  $12, %esp

    # Map kernel to 0xc0000000
    pushl $0xc0000
    pushl $0x10
    pushl $0x20
    call  memory_page_map
    addl  $12, %esp

    # Map IDT/GDT to 0xc0010000
    pushl $0xc0010
    pushl $0x1
    pushl $0x10
    call  memory_page_map
    addl  $12, %esp

    # Map VBE info to 0xc0011e00
    pushl $0xc0011
    pushl $0x1
    pushl $0x7
    call  memory_page_map
    addl  $12, %esp

    # Map int 15H memory info to 0xc0012000
    pushl $0xc0012
    pushl $0x1
    pushl $0xe
    call  memory_page_map
    addl  $12, %esp

    # Map MM bitmap to 0xc0600000
    pushl $0xc0600
    pushl $0x200
    pushl $0x600
    call  memory_page_map
    addl  $12, %esp

    # Map kernel stack to 0xffbf8000
    pushl $0xffbf8
    pushl $0x7
    pushl $0x0
    call  memory_page_map
    addl  $12, %esp

    # Map kernel paging directory/table to 0xffbff000
    pushl $0xffbff
    pushl $0x401
    pushl $0x1ff
    call  memory_page_map
    addl  $12, %esp

    # Init new GDT data before paging
    movl  $0x00010800, %eax
    movl  $0x00000000, (%eax)
    movl  $0x00000000, 4(%eax)
    movl  $0x0000ffff, 8(%eax)
    movl  $0x00cf9a00, 12(%eax)
    movl  $0x0000ffff, 16(%eax)
    movl  $0x00cf9200, 20(%eax)
    movw  $23, 24(%eax)
    movl  $0x00010800, 26(%eax)
    lgdt  24(%eax)
    movl  $0xc0010800, 26(%eax)

    # Enable paging
    movl  $PAGE_DIRECTORY_BASE, %eax
    movl  %eax, %cr3
    movl  %cr0, %eax
    orl   $0x80000001, %eax
    movl  %eax, %cr0

    # Reload GDT data
    movl  $0xc0010800, %eax
    lgdt  24(%eax)

    movl  $0xffbff000, %ebp
    movl  %ebp, %esp
    call  0xc0000000

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
    shll $2, %edx
    shll $12, %eax
    addl $PAGE_TABLE_BASE, %edx
1:  cmpl $0, %ecx
    je   memory_page_map_return
    movl %eax, (%edx)
    orl  $0x7, (%edx)
    addl $4, %edx
    decl %ecx
    addl $0x1000, %eax
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
