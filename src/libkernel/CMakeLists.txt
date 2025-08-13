cmake_minimum_required(VERSION 3.20)

add_library(kernel STATIC
    color.c console.c
    display.c
    font.c
    interrupt.c intr_handler.s io.c
    keyboard.c
    memory.c mm.c
    pci.c
    stdio.c
    timer.c)
target_link_libraries(kernel
    PRIVATE
        gcc)
target_include_directories(kernel
    PRIVATE
        ${CMAKE_SOURCE_DIR}/inc)
