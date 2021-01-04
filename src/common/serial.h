#pragma once

#include <stdint.h>
#include "ring_buffer.h"

/**
 * @namespace common::serial
 * Implementation of the USART0 serial driver.
 * @see atmega328p.pdf p143-165
 */
namespace pov::serial {
  enum class Mode : char {
      ASYNCHRONOUS_NORMAL,
      ASYNCHRONOUS_DOUBLE_SPEED,
      SYNCHRONOUS_MASTER,
  };

  struct Config {
     long int BAUD_RATE;
     long int BAUD_RATE_TOL;
     Mode MODE;
     long int PRINT_BUFFER_SIZE;
  };

  void init();
  void transmit(char data);
  void transmit(const char *s);

  void printf(const char* fmt, ...);

  char receive();
  bool dataAvailable();

  inline RingBuffer<char, 100> io_buffer;

namespace internal {
  /**
   * @brief calculateUBRR
   * @see atmega328p.pdf p146,163
   */
  consteval unsigned int calculateUBRR(unsigned int baud, Mode mode = Mode::ASYNCHRONOUS_NORMAL) {
    unsigned int ubrr = 1 << 12; // invalid UBRR;
    switch (mode) {
    case Mode::ASYNCHRONOUS_NORMAL:
        ubrr = (unsigned int)(F_CPU/16/baud) - 1;
        break;
    case Mode::ASYNCHRONOUS_DOUBLE_SPEED:
        ubrr = (unsigned int)(F_CPU/8/baud) - 1;
        break;
    case Mode::SYNCHRONOUS_MASTER:
        ubrr = (unsigned int)(F_CPU/2/baud) - 1;
        break;
    }
    return ubrr;
  }
}
}
