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
    pushl %eax
    movl  4(%esp), %eax
    pushl %eax
    movl  %cr2, %eax
    pushl %eax
    call  page_fault
    addl  $8, %esp
    popl  %eax
    addl  $4, %esp
    sti
    iret

irq_0x20_handler:
    cli
    call  irq_0x20
    sti
    iret

irq_0x21_handler:
    cli
    call  irq_0x21
    sti
    iret

irq_0x27_handler:
    cli
    call  irq_0x27
    sti
    iret
