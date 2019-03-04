.code32
.text

.globl irq_0x20_handler, irq_0x21_handler, irq_0x27_handler

.extern irq_0x20, irq_0x21, irq_0x27

irq_0x20_handler:
    call irq_0x20
    iret

irq_0x21_handler:
    call irq_0x21
    iret

irq_0x27_handler:
    cli
    call irq_0x27
    sti
    iret
