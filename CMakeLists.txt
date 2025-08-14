cmake_minimum_required(VERSION 3.20)

set(CMAKE_ASM_COMPILER "$ENV{HOME}/.local/software/i686-elf/bin/i686-elf-gcc")
set(CMAKE_ASM_FLAGS "-nostdlib")
set(CMAKE_C_COMPILER "$ENV{HOME}/.local/software/i686-elf/bin/i686-elf-gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c11 -ffreestanding -fno-builtin -fno-pic -fno-stack-protector")
set(CMAKE_AR "$ENV{HOME}/.local/software/i686-elf/bin/i686-elf-ar")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(ListaOS)
enable_language(ASM C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
