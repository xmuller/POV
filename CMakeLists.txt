cmake_minimum_required(VERSION 3.16.2)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/avr.toolchain.cmake")

project(POV)

add_avr_executable(pov
  src/common/encoder.cpp
  src/common/led_spi.cpp
  src/common/serial.cpp
  src/common/timer.cpp
  src/common/shell.cpp
  src/main.cpp
)
target_compile_definitions(pov PUBLIC
  -D__DELAY_BACKWARD_COMPATIBLE__
)
target_link_libraries(pov PUBLIC ATMEGA328P::CPP20_OPTIMIZED_BUILD)
target_include_directories(pov PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
)
