cmake_minimum_required(VERSION 3.2)
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi-gcc.cmake)
project(test LANGUAGES CXX C ASM)

find_package(cmake_macros REQUIRED)
find_package(libopencm3_stm32f4 REQUIRED)

add_library(hytech_hal INTERFACE)

target_include_directories(hytech_hal 
  INTERFACE 
  $<INSTALL_INTERFACE:include/hytech_hal>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/hytech_hal>
  )

add_library(stm32f4 STATIC src/init.cpp)

target_include_directories(stm32f4
  PUBLIC 
  $<INSTALL_INTERFACE:include/stm32f4>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/stm32f4>
  )

target_link_libraries(stm32f4 PUBLIC libopencm3_stm32f4 hytech_hal)

# include(cmake/create_package.cmake)
include(create_package)
make_cmake_package(hytech_hal hytech_hal)
make_cmake_package(stm32f4 hytech_hal)
# install all of the headers for the libraries
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    FILES_MATCHING PATTERN "*.h" 
    )

