cmake_minimum_required(VERSION 3.20)

project(ListaOS)
enable_language(ASM C)

set(CMAKE_ASM_COMPILER "i686-elf-gcc")
set(CMAKE_ASM_FLAGS "-nostdlib")
set(CMAKE_C_COMPILER "i686-elf-gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c11 -ffreestanding -fno-builtin -fno-pic -fno-stack-protector")
set(CMAKE_AR "i686-elf-ar")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
