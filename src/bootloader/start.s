.set PROT_MODE_CSEG, 0x8
.set PROT_MODE_DSEG, 0x10

.globl _start
_start:
.code16
	xor  %ax, %ax
	movw %ax, %ds
	movw %ax, %es

	movb $0x03, %al
	movb $0x00, %ah
	int  $0x10

	movw SVGAInfoAddr, %di
	movw $0x0000, %ax
	movw %ax, %es
	movw $0x4f00, %ax
	int  $0x10
	
	#Get VBE Information
	movw $0x0000, %ax
	movw %ax, %es
	movw VBEModeInfoAddr, %di
	movw $0x4f01, %ax
	movw $0x0118, %cx
	int  $0x10
    
	#Get All Video Mode
	xorl %ebx, %ebx
	movw SVGAInfoAddr, %bx
	addl $0x0000000e, %ebx
	movl (%ebx), %ebx
	movw %bx, %dx
	andl $0xffff0000, %ebx
	andl $0x0000ffff, %edx
	shr  $12, %ebx
	addl %edx, %ebx
	movw $0x3000, %ax
	movw %ax, %es
	movw $0x0000, %di
2:	movw $0x4f01, %ax
	movw (%ebx), %cx
	cmp  $0xffff, %cx
	je   2f
	int  $0x10
	addw $0x0002, %bx
	addw $0x0100, %di
	jmp  2b
    
	#Set Video Mode 0x118(1024x768 True Color)
2:	movw $0x4118, %bx
	movw $0x4f02, %ax
	int  $0x10
	cmp  $0x00, %ah
	jnz  vesa_error
	jmp  continue

vesa_error:
	movw $msgVESAError, %ax
	movw %ax, %bp
	movw $0x0010, %cx
	movw $0x000f, %bx
	movw $0x1301, %ax
	movb $0x00, %dl
	movb $0x00, %dh
	int  $0x10
	jmp  vesa_error
msgVESAError: .ascii "VESA Not Support"

continue:
	inb  $0x92, %al
	orb  $0x02, %al
	outb %al, $0x92

	#Get Memory Info with INT15H-E820
	#Start At Addr 0xe000
	movl $0, 0xe000
	xorl %ebx, %ebx
	movw $0x0000, %ax
	movw %ax, %es
	movw $0xe004, %di
start_probe:
	movw $0xe820, %ax
	movl $0x00000014, %ecx
	movl $0x534D4150, %edx
	int  $0x15
	jnc  count
	movl $0xffffffff, 0xe000
	jmp finish_probe
count:
	addw $0x14, %di
	incl 0xe000
	cmpl $0x0, %ebx
	jnz start_probe
finish_probe:

	cli
	data32 addr32 lgdt gdtdesc

	movl %cr0, %eax
	orl  $0x1, %eax
	mov  %eax, %cr0
	data32 ljmp $PROT_MODE_CSEG, $1f

.code32
1:	movw  $PROT_MODE_DSEG, %ax
	movw  %ax, %ds
	movw  %ax, %es
	movw  %ax, %fs
	movw  %ax, %gs
	movw  %ax, %ss
	movl  $0x7e00, %esp
	movl  $0, %ebp
	#call  main
	call  0x20000

loop:
	jmp loop

VBEModeInfoAddr:	.word 0x7e00
SVGAInfoAddr:		.word 0xf000

#### GDT
	.align 8
gdt:
	.quad 0x0000000000000000	# Null segment.  Not used by CPU.
	.quad 0x00cf9a000000ffff	# System code, base 0, limit 4 GB.
	.quad 0x00cf92000000ffff	# System data, base 0, limit 4 GB.

gdtdesc:
	.word	gdtdesc - gdt - 1	# Size of the GDT, minus 1 byte.
	.long	gdt					# Address of the GDT.
