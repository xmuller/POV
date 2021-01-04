#pragma once

#include "ring_buffer.h"

namespace pov::shell
{
struct Config {
  unsigned int MAX_ARGUMENTS;
  unsigned int MAX_LINE_SIZE;
  const char* NEW_LINE_STRING;
  const char* PROMPT;
};

using shellcmd_t = void (*)(unsigned int argc, char *argv[]);
struct ShellCommand
{
  const char* command_name;
  shellcmd_t  function;
};

void init();
void shellTick();

extern const ShellCommand shell_commands[];
extern const ShellCommand shell_local_commands[];
}
