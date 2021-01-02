# AVR toolchain file

# OPTIONS:
# - TOOLCHAIN_CUSTOM_AVR_GCC_PATH   : Use is to use a given GCC build.

## Use a downloaded gcc-avr.
set(TOOLCHAIN_CUSTOM_AVR_GCC_PATH "" CACHE PATH
    "Set custom avr-gcc path. Clear your build folder and `cmake .. -DCUSTOME_AVR_GCC_PATH=your/custom/path`. PATH is used by default."
)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSS_COMPILING 1)
if(TOOLCHAIN_CUSTOM_AVR_GCC_PATH STREQUAL "")
  set(CMAKE_CXX_COMPILER avr-g++)
  set(CMAKE_C_COMPILER avr-gcc)
  set(CMAKE_ASM_COMPILER avr-gcc)
  set(CMAKE_OBJDUMP avr-objdump)
  set(CMAKE_OBJCOPY avr-objcopy)
else()
  set(CMAKE_CXX_COMPILER ${TOOLCHAIN_CUSTOM_AVR_GCC_PATH}/bin/avr-g++)
  set(CMAKE_C_COMPILER ${TOOLCHAIN_CUSTOM_AVR_GCC_PATH}/bin/avr-gcc)
  set(CMAKE_ASM_COMPILER ${TOOLCHAIN_CUSTOM_AVR_GCC_PATH}/bin/avr-gcc)
  set(CMAKE_OBJDUMP ${TOOLCHAIN_CUSTOM_AVR_GCC_PATH}/bin/avr-objdump)
  set(CMAKE_OBJCOPY ${TOOLCHAIN_CUSTOM_AVR_GCC_PATH}/bin/avr-objcopy)
endif()

# Add AVR's include files, for completion only !
if(TOOLCHAIN_CUSTOM_AVR_GCC_PATH STREQUAL "")
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES /usr/avr/include/ CACHE PATH "The AVR include directory, only used for completion.")
else()
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${TOOLCHAIN_CUSTOM_AVR_GCC_PATH}/avr/include CACHE PATH "The AVR include directory, only used for completion.")
endif()

# Add extension consistency
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")


# ATMEGA328P platform
if(NOT _ATMEGA328P_BUILD_SET)
  set(_ATMEGA328P_BUILD_SET ON)
  add_library(ATMEGA328P_CPP20_OPTIMIZED_BUILD INTERFACE)
  add_library(ATMEGA328P::CPP20_OPTIMIZED_BUILD ALIAS ATMEGA328P_CPP20_OPTIMIZED_BUILD )

  target_link_options(ATMEGA328P_CPP20_OPTIMIZED_BUILD INTERFACE -mmcu=atmega328p )
  target_compile_definitions(ATMEGA328P_CPP20_OPTIMIZED_BUILD INTERFACE
    -DF_CPU=13000000U
    -D__AVR_ATmega328P__
  )

  target_compile_options(ATMEGA328P_CPP20_OPTIMIZED_BUILD INTERFACE
    -std=c++20
    -Os
    -Wall
    -Wextra # reasonable and standard
    -Werror
    -Wundef
    -Wshadow # warn the user if a variable declaration shadows one from a parent context
    -Wcast-align # warn for potential performance problem casts
    -Wunused # warn on anything being unused
    -Wpedantic # warn if non-standard C++ is used
    -Wconversion # warn on type conversions that may lose data
    -Wsign-conversion # warn on sign conversions
    -Wnull-dereference # warn if a null dereference is detected
    -Wdouble-promotion # warn if float is implicit promoted to double
    -Wformat=2 # warn on security issues around functions that format output (ie printf)
    -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
    -Wduplicated-cond # warn if if / else chain has duplicated conditions
    -Wduplicated-branches # warn if if / else branches have duplicated code
    -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
    -Wuseless-cast # warn if you perform a cast to the same type
    -fno-common
    -fno-exceptions -ffunction-sections -fdata-sections
  )

  function(add_avr_executable target_name)
#message(STATUS "AVR TOOLCHAIN: add_avr_executable available options:\n\
#  → GENERATE_BINARY\n\
#  → GENERATE_FLASH_CMD\n\
#    ")
    add_executable(${target_name} ${ARGN})
#    set(options
#       GENERATE_BINARY
#       GENERATE_FLASH_CMD
#     )
#    cmake_parse_arguments(AVR_EXE_OPTIONS "" "${options}" "${multiValueArgs}" "${ARGN}")

#  if(AVR_EXE_OPTIONS_GENERATE_BINARY)
    add_custom_command(
      TARGET ${target_name}
      POST_BUILD COMMAND
      ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE_NAME:${target_name}> ${target_name}.bin
    )
#  endif()

#  if(AVR_EXE_OPTIONS_GENERATE_FLASH_CMD)
    add_custom_target(flash-${target_name}
      COMMAND avrdude -p m328p -c usbasp -U flash:w:${target_name}.bin
      DEPENDS ${target_name}
    )
#  endif()
  endfunction()
endif()
