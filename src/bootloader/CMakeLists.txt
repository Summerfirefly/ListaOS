cmake_minimum_required(VERSION 3.20)

add_executable(boot.bin
    boot.s)
target_link_options(boot.bin
    PRIVATE
        -T ${CMAKE_CURRENT_SOURCE_DIR}/boot.lds)

add_executable(init.bin
    start.s)
target_link_options(init.bin
    PRIVATE
        -T ${CMAKE_CURRENT_SOURCE_DIR}/init.lds)
