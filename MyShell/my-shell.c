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

  do {
    // show_prompt();
    command = read_command();
    switch(command){
      case 1:
      break;
      case 2:
      break;
      default: 
        printf("command not found\n");
      break;
    }
  } while (command != 0);

  return 0;
}
