#include "shell.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "config.h"

using namespace pov::serial;

namespace pov::shell {

void showAllCommands(unsigned int, char* []) {
  short i = 0;
  while(shell_local_commands[i].command_name != nullptr) {
      transmit(shell_local_commands[i].command_name);
      transmit('\n');
      i++;
  }
  i = 0;
  while(shell_commands[i].command_name != nullptr) {
     transmit(shell_commands[i].command_name);
     transmit('\n');
     i++;
  }
}

void showInfo(unsigned int, char* []) {
  transmit("POV équipe 1:\n"
           " - Vincent Ameeuw\n"
           " - Titouan Berardet\n"
           " - Xavier Muller\n"
           " - Edgard Pinget\n");
}

static void echo(unsigned int argc, char* argv[]) {
  for (unsigned int i = 0; i < argc - 1; i++)
  {
     transmit(argv[i]);
     transmit(' ');
  }
  transmit(argv[argc - 1]);
  transmit("\n");
}

const ShellCommand shell_local_commands[] = {
  {"help", showAllCommands},
  {"info", showInfo},
  {"echo", echo},
  {nullptr, nullptr}
};

void init() {
  serial::init();
  serial::transmit(config.PROMPT);
}

#include <util/delay.h>
static char buffer[config.MAX_LINE_SIZE];
static char* argv[config.MAX_ARGUMENTS+1];
void shellTick()
{
  int buf_pos = 0;
  if(serial::io_buffer.size() > 0) {
    char c = serial::io_buffer.pop();
    while(c != '\n') {
      buffer[buf_pos++] = c;
      c = serial::io_buffer.pop();
    }
  } else {
      if(warning_enabled)
        serial::printf("Error invalid shell call.\n");
      run_shell = false;
      return;
  }
  buffer[buf_pos] = '\0';

  char* save_ptr;
  unsigned int argc = 0;
  strtok_r(buffer, " ", &save_ptr);
  while ((argv[argc] = strtok_r(nullptr, " ", &save_ptr)) != nullptr
         && argc < config.MAX_ARGUMENTS) {
    *(argv[argc]-1) = '\0'; // replace ' ' → '\0' inside the buffer
    argc++;
  }

  if (save_ptr != nullptr)
    *(save_ptr - 1) = ' ';

  bool command_found = false;
  short i = 0;
  while( command_found == false && shell_local_commands[i].command_name != nullptr) {
     if (strcmp(shell_local_commands[i].command_name, buffer) == 0) {
         shell_local_commands[i].function(argc, argv);
         command_found = true;
     }
     i++;
  }
  i = 0;
  while(command_found == false && shell_commands[i].command_name != nullptr) {
     if (strcmp(shell_commands[i].command_name, buffer) == 0) {
         shell_commands[i].function(argc, argv);
         command_found = true;
         break;
     }
     i++;
  }
  if (!command_found) {
    serial::printf("Shell error: %s command not found...\n", buffer);
  }
  serial::transmit(config.PROMPT);

  run_shell = false;
}

}
