TARGET=i686-elf

AS = $(TARGET)-as
CC = $(TARGET)-gcc
LD = $(TARGET)-gcc

CFLAGS = -Wall -Wextra -std=c11 -ffreestanding -fno-stack-protector
LDFLAGS = -nostdlib -lgcc -ffreestanding -Wl,--oformat=binary

VPATH = src:src/bootloader

libKernelObj = console.o color.o display.o div64.o font.o io.o memory.o mm.o stdio.o interrupt.o intr_handler.o keyboard.o timer.o

.PHONY: all clean

all: boot.bin init.bin kernel.sys

boot.bin: boot.o
	$(LD) -o boot.bin $(LDFLAGS) -Ttext=0x7c00 boot.o

init.bin: start.o
	$(LD) -o init.bin $(LDFLAGS) -Ttext=0x8200 start.o
	
kernel.sys: main.o libkernel.a
	$(LD) -o kernel.sys $(LDFLAGS) -Ttext=0x20000 -e main main.o -L. -lkernel

libkernel.a: $(libKernelObj)
	ar rcs libkernel.a $(libKernelObj)

clean:
	rm *.o
