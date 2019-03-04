AS = as
ASFLAGS = --32
CC = gcc
CFLAGS = -m32 -Wall -nostdinc -std=c99 -fno-builtin -fno-pic -fno-stack-protector
LD = ld
LDFLAGS = -m elf_i386 --oformat binary -nostdlib

VPATH = src:src/bootloader

libKernelObj = console.o color.o display.o div64.o font.o io.o memory.o mm.o stdio.o interrupt.o intr_handler.o keyboard.o timer.o

.PHONY: all clean

all: boot.bin init.bin kernel.sys

boot.bin: boot.o
	$(LD) $(LDFLAGS) -Ttext=0x7c00 boot.o -o boot.bin

init.bin: start.o
	$(LD) $(LDFLAGS) -Ttext=0x8200 start.o -o init.bin
	
kernel.sys: main.o libkernel.a
	$(LD) $(LDFLAGS) -Ttext=0x20000 -e main main.o -o kernel.sys -L. -lkernel

libkernel.a: $(libKernelObj)
	ar rcs libkernel.a $(libKernelObj)

clean:
	rm *.o
