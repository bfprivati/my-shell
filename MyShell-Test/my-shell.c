#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "my-shell.h"

int main() {
  char command;

  do{
    clear_input();
    io_rdrct(entrada, saida);
    show_prompt();
    command = read_command();
  } while (command != 0);

  return 0;
}
