cmake_minimum_required(VERSION 3.2)
project(test)

find_package(libopencm3_stm32f4 REQUIRED)
add_executable(hello_world main.cpp)

message(STATUS ${CMAKE_MODULE_PATH})
message(STATUS ${CMAKE_MODULE_PATH})

message(STATUS ${CMAKE_MODULE_PATH})

message(STATUS ${CMAKE_MODULE_PATH})

message(STATUS ${CMAKE_MODULE_PATH})
target_link_libraries(hello_world PUBLIC libopencm3::libopencm3_stm32f4)