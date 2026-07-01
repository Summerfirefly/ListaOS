cmake_minimum_required(VERSION 3.20)

set(GNU_TOOLCHAIN_DIR "$ENV{HOME}/.local/software/gnu-toolchain/i686-elf/16.1.0" CACHE PATH "GNU Toolchain dir")

set(CMAKE_ASM_COMPILER "${GNU_TOOLCHAIN_DIR}/bin/i686-elf-gcc")
set(CMAKE_ASM_FLAGS "-nostdlib")
set(CMAKE_C_COMPILER "${GNU_TOOLCHAIN_DIR}/bin/i686-elf-gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c11 -ffreestanding -fno-builtin -fno-pic -fno-stack-protector")
set(CMAKE_AR "${GNU_TOOLCHAIN_DIR}/bin/i686-elf-ar")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(ListaOS)
enable_language(ASM C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEBUG_BUILD ON)
if(${DEBUG_BUILD})
    add_compile_definitions(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE "")
endif()

add_subdirectory(src)
