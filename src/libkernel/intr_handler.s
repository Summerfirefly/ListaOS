.globl page_fault_handler, irq_0x20_handler, irq_0x21_handler, irq_0x27_handler
.extern page_fault, irq_0x20, irq_0x21, irq_0x27

.section .text
.code32

# CPU will push these data to stack before entering interrupt handler
# From stack top: Error Code -> EIP -> CS -> EFLAGS
# If from user mode to kernel: Error Code -> EIP -> CS -> EFLAGS -> ESP -> SS
# Error Code is optional and need to pop before iret

page_fault_handler:
    cli
    pushl %ebp
    movl  %esp, %ebp
    pushal
    movl  4(%ebp), %eax
    pushl %eax
    movl  %cr2, %eax
    pushl %eax
    call  page_fault
    addl  $8, %esp
    popal
    popl  %ebp
    addl  $4, %esp      # Clear error code
    sti
    iret

irq_0x20_handler:
    cli
    pushal
    call  irq_0x20
    popal
    sti
    iret

irq_0x21_handler:
    cli
    pushal
    call  irq_0x21
    popal
    sti
    iret

irq_0x27_handler:
    cli
    pushal
    call  irq_0x27
    popal
    sti
    iret
