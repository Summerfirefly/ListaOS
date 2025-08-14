cmake_minimum_required(VERSION 3.20)

add_subdirectory(bootloader)
add_subdirectory(libkernel)

add_executable(kernel.sys
    main.c)
target_include_directories(kernel.sys
    PRIVATE
        ${CMAKE_SOURCE_DIR}/inc)
target_link_libraries(kernel.sys
    PRIVATE
        kernel)
target_link_options(kernel.sys
    PRIVATE
        -nostdlib
        -ffreestanding
        -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.lds)

add_custom_target(image
    COMMAND ${CMAKE_SOURCE_DIR}/makeimg.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(image PROPERTIES EXCLUDE_FROM_ALL ON)
