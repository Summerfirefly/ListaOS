cmake_minimum_required(VERSION 3.20)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_COMPILER_TARGET x86_64-unknown-uefi)

project(bootloader-uefi)

add_executable(bootx64.efi
    main.c)

target_include_directories(bootx64.efi
    PRIVATE
    /usr/include)

target_link_options(bootx64.efi
    PRIVATE
    -nostdlib)
