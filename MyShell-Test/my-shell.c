#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "my-shell.h"


int main() {
  char command;

  while(1) {
    clear_input();
    //io_rdrct(entrada, saida);
    show_prompt();
    command = read_command();
    if (command == -2)
      exit(0);
  }
  
  return 0;
}