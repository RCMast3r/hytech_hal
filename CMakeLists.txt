cmake_minimum_required(VERSION 3.2)
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi-gcc.cmake)
project(test LANGUAGES CXX C ASM)

# set(CMAKE_BUILD_TYPE Debug)

find_package(libopencm3_stm32f4 REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(hello_world main.cpp)

target_link_libraries(hello_world PUBLIC libopencm3_stm32f4)

set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

target_compile_options(hello_world PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -fno-exceptions
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wconversion
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>)

target_link_options(hello_world PRIVATE
    # $<$<VERSION_GREATER:$<C_COMPILER_VERSION>,10.3.1>:-Wl,--no-warn-rwx-segments>
    -Wl,--start-group
    # --static
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage)


stm32_add_linker_script(hello_world PRIVATE STM32F446RETX_RAM.ld)

add_custom_target(hello_world.bin ALL DEPENDS hello_world)
add_custom_command(
    TARGET hello_world.bin
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${CMAKE_CURRENT_BINARY_DIR}/hello_world.elf ${CMAKE_CURRENT_BINARY_DIR}/hello_world.bin
)
include(GNUInstallDirs)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hello_world.elf
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)
include(GNUInstallDirs)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hello_world.bin
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)

