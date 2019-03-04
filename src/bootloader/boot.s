.code16
.byte 0xeb, 0x3c, 0x90 # 跳转指令jmp _start; nop
.ascii "MSDOS5.0"      # OEM名，8字节，不足以空格填充
.word 0x0200           # 每扇区字节数
.byte 0x04             # 每簇扇区数
.word 0x0004           # 保留扇区数
.byte 0x02             # FAT表记录数
.word 0x0200           # 最大根目录文件数
.word 0x0000           # 逻辑扇区总数
.byte 0xf8             # 媒体描述符
.word 0x00f2           # 每个FAT占用扇区数
.word 0x003f           # 每个磁道扇区数
.word 0x00ff           # 磁头数
.int  0x00000000       # 隐藏扇区数
.int  0x0003c800       # 若逻辑扇区总数记录是0，则在这里记录扇区总数
.byte 0x80             # 中断0x13的驱动器号
.byte 0x00             # 未使用
.byte 0x29             # 扩展引导标志
.int  0x64668707       # 卷序列号
.ascii "Lista OS   "   # 卷标，11字节，不足以空格填充
.ascii "FAT16   "      # 文件系统类型，8字节，不足以空格填充

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
	
	# 在根目录寻找并载入kernel.sys
	# 载入位置为内存地址0x20000
	movw $KernelFileName, %si
	movw $0x8200, off
	call find_file
	movw $0x0000, off
	movw $0x2000, seg
	call load_file
	
	# 在根目录寻找并载入init.bin
	# 载入位置为内存地址0x8200
	movw $InitFileName, %si
	movw $0x8200, off
	call find_file
	movw $0x8200, off
	movw $0x0000, seg
	call load_file
	
	# 开始执行init.bin
	call 0x8200
	
fin:
	jmp  fin
    
load_error:
	movw $msgLoadError, %ax
	movw %ax, %bp
	movw $0x000b, %cx
	jmp  print_msg
	
file_missed_error:
	movw $msgFileMissed, %ax
	movw %ax, %bp
	movw $0x000c, %cx
	jmp  print_msg

print_msg:
	movw $0x000f, %bx
	movw $0x1301, %ax
	movb $0x00, %dl
	movb $0x00, %dh
	int  $0x10
	jmp  fin


# 函数find_file
# 用于在FAT 16根目录寻找文件
# 调用前设置的参数：
# si: 文件名；info_packet.off：根目录表加载内存地址
find_file:
	movw $0x0000, seg
	movb $0x20, %cl
	movl RootDirStartSector, %eax
	movl %eax, start_sector
	movw %si, %bx
find_start:
	# 加载一个根目录表扇区
	call read_sector
	movw off, %di
	movw $0x0200, %dx
2:	movw %bx, %si
	movb $0x0b, %ch
1:	movb (%di), %ah
	lodsb
	cmp  %ah, %al
	jne  next_file				# 逐字符比较文件名，有不同则跳到下一个文件
	inc  %di
	dec  %ch
	jnz  1b
	andw $0xfff0, %di
	ret
next_file:
	andw $0xfff0, %di
	addw $0x0020, %di
	subw $0x0020, %dx
	jnz  2b
	dec  %cl					# 此扇区内无文件
	jz   file_missed_error		# 根目录无内核文件，抛出错误提示
	addl $1, start_sector
	jmp  find_start				# 读取根目录表下一个扇区	


# 函数load_file
# 用于加载find_file找出的文件，必须紧跟find_file之后调用
# find_file调用完成后(%di)中存放着目标文件的文件描述符
# 注意：FAT记录会占用内存0x8000-0x8200，避免使用这段内存
# 调用前设置的参数：
# info_packet的off, seg：用于指定加载的内存地址，16 * seg + off
# di:指向存储下一簇号的内存单元
load_file:
	movw 26(%di), %ax			# 将起始簇号放入%ax
	xorl %ecx, %ecx
	movw seg, %cx
	shl  $4, %ecx
	addw off, %cx				# %ecx中存放加载地址
2:	movw %ax, %bx
	movw %ax, %di
	subw $2, %bx				# FAT 16的数据区簇号从2开始，因此先减2
	andl $0x0000ffff, %ebx
	shl  $2, %ebx
	addl DataStartSector, %ebx	# 计算得到待读簇的首扇区号，存放在%ebx
	andw $0x00ff, %di
	shl  $1, %di
	addw $0x8000, %di
	shr  $8, %ax
	addw FAT1StartSector, %ax	# 此时%ax存有所需FAT表项存放的扇区号
read_fat:
	andl $0x0000ffff, %eax
	movw $0x0001, count
	movl %eax, start_sector
	movw $0x8000, off			# FAT记录会放在0x8000 - 0x8200, 注意
	movw $0x0000, seg
	call read_sector
# Load File
	movw $0x0004, count
	movl %ebx, start_sector
	movw %cx, off
	xorw %cx, %cx
	shr  $4, %ecx
	movw %cx, seg
	call read_sector
	xorl %ecx, %ecx
	movw seg, %cx
	shl  $4, %ecx
	addw off, %cx
	addl $0x00000800, %ecx
	movw (%di), %ax
	cmp  $0xfff0, %ax
	jb   2b
	ret
	
	
read_sector:
	movb $0x42, %ah			# 读磁盘功能号
	movb DriverNumber, %dl
	movw $info_packet, %si
	int  $0x13
	cmp  $0x00, %ah
	jne  load_error
	ret
	
info_packet:
size:			.byte 0x10			# size of packet
reverse:		.byte 0x00			# not used
count:			.word 0x0001		# amount to read
off:			.word 0x8200		# memary address offset
seg:			.word 0x0000		# memary address segment
start_sector:	.int  0x00000000
				.int  0x00000000

msgLoadError:		.ascii	"Load error!"
msgFileMissed:	    .ascii	"File missed!"
FAT1StartSector:	.word	4
RootDirStartSector:	.int	488
DataStartSector:	.int	520
DriverNumber:		.byte	0x00
KernelFileName:		.ascii	"KERNEL  SYS"
InitFileName:		.ascii	"INIT    BIN"

.org 510
.byte 0x55, 0xaa
