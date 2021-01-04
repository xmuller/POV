cmake_minimum_required(VERSION 3.16.2)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/avr.toolchain.cmake")

project(POV)

add_library(common-pov STATIC
  src/common/encoder.cpp
  src/common/led_spi.cpp
  src/common/serial.cpp
  src/common/timer.cpp
  src/common/shell.cpp
)
target_compile_definitions(common-pov PUBLIC
  -D__DELAY_BACKWARD_COMPATIBLE__
)
target_link_libraries(common-pov PUBLIC ATMEGA328P::CPP20_OPTIMIZED_BUILD)
target_include_directories(common-pov PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
)

foreach(BINARY_NAME  hall_PD2 led_PD6 pov serial test_spi test_timer shell)
  add_avr_executable(${BINARY_NAME} src/${BINARY_NAME}.cpp)
  target_link_libraries(${BINARY_NAME} PRIVATE common-pov)
endforeach()
