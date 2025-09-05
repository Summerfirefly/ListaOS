cmake_minimum_required(VERSION 3.20)

file(GLOB LIBKERNEL_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/driver/*.c")

add_library(kernel STATIC
    ${LIBKERNEL_SOURCE})
target_link_libraries(kernel
    PRIVATE
        gcc)
target_include_directories(kernel
    PRIVATE
        ${CMAKE_SOURCE_DIR}/inc)
