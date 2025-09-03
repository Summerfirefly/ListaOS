.globl page_fault_handler, irq_0x20_handler, irq_0x21_handler, irq_0x27_handler
.extern page_fault, irq_0x20, irq_0x21, irq_0x27

.section .text
.code32
page_fault_handler:
    cli
    movl  %cr2, %eax
    pushl %eax
    call  page_fault
    addl  $8, %esp
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
