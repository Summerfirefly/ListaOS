TARGET = i686-elf

AS = $(TARGET)-as
ASFLAGS =
CC = $(TARGET)-gcc
CFLAGS = -Wall -Wextra -std=c11 -ffreestanding -fno-builtin -fno-pic -fno-stack-protector -Isrc/libkernel
LD = $(TARGET)-ld
LDFLAGS = -nostdlib

VPATH = src

bootloaderSrc = boot.s start.s
libKernelSrc_c = color.c console.c \
                 display.c div64.c \
                 font.c \
                 interrupt.c io.c \
                 keyboard.c \
                 memory.c mm.c \
                 stdio.c \
                 timer.c
libkernelSrc_s = intr_handler.s

bootloaderObj := $(bootloaderSrc:%.s=build/bootloader/%.o)
libKernelObj_c := $(libKernelSrc_c:%.c=build/libkernel/%.o)
libKernelObj_s := $(libkernelSrc_s:%.s=build/libkernel/%.o)

.PHONY: all dir clean

all: dir boot.bin init.bin kernel.sys

dir:
	@mkdir -p build/
	@mkdir -p build/bootloader
	@mkdir -p build/libkernel

$(bootloaderObj) $(libKernelObj_s): build/%.o: %.s
	@echo -e "Building $@"
	@$(AS) $(ASFLAGS) -o $@ $<

$(libKernelObj_c): build/%.o: %.c
	@echo -e "Building $@"
	@$(CC) -c $(CFLAGS) -o $@ $<

build/main.o: main.c
	@echo -e "Building $@"
	@$(CC) -c $(CFLAGS) -o $@ $<

boot.bin: build/bootloader/boot.o
	@echo -e "Linking $@"
	@$(LD) $(LDFLAGS) -Ttext=0x7c00 $< -o build/bootloader/boot.elf
	@objcopy -O binary -j .text build/bootloader/boot.elf build/boot.bin

init.bin: build/bootloader/start.o
	@echo -e "Linking $@"
	@$(LD) $(LDFLAGS) -T init.lds $< -o build/init.bin

kernel.sys: build/main.o build/libkernel.a
	@echo -e "Linking $@"
	@$(CC) $(LDFLAGS) -ffreestanding -T kernel.lds -o build/kernel.sys build/main.o -Lbuild/ -lkernel -lgcc

build/libkernel.a: $(libKernelObj_c) $(libKernelObj_s)
	@echo -e "Archiving $@"
	@ar rcs $@ $(libKernelObj_c) $(libKernelObj_s)

clean:
	-rm -rf build/
