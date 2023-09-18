cmake_minimum_required(VERSION 3.2)
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi-gcc.cmake)
project(test LANGUAGES CXX C ASM)

find_package(libopencm3_stm32f4 REQUIRED)

add_library(hytech_hal INTERFACE)

target_include_directories(hytech_hal 
  INTERFACE 
  $<INSTALL_INTERFACE:include/hytech_hal>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/hytech_hal>
  )
# add_library(stm32f4 PUBLIC )

# target_include_directories(stm32f4 
#   PUBLIC
#   $<INSTALL_INTERFACE:include/stm32f4>
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/stm32f4>
#   )

# hytech_hal interface library 
# (the only thing that a user code library should include essentially) 
include(GNUInstallDirs)
install(TARGETS hytech_hal
    EXPORT hytech_halTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT hytech_halTargets
        FILE hytech_halTargets.cmake
        NAMESPACE hytech_hal::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hytech_hal
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hytech_halConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/hytech_halConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hytech_hal
)
install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/hytech_halConfig.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hytech_hal
)

# install all of the headers for the libraries
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    FILES_MATCHING PATTERN "*.h" 
    )

