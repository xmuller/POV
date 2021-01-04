#include <avr/io.h>
#include <util/delay.h>

#include "common/shell.h"
#include "common/serial.h"
#include "avr/interrupt.h"

using namespace pov;

//const shell::ShellCommand shell::shell_commands[] = {
// {nullptr, nullptr }
//};

int main() {
  sei();
  shell::init();

  while (1) {
//    shell::shellTick();
//    while(serial::io_buffer.size() > 0) {
//      serial::transmit(serial::io_buffer.pop());
//    }
//    serial::transmit(serial::receive());
//    serial::transmit("c\n");
    _delay_ms(1000);
  }
  return 0;
}
