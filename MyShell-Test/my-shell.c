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
  	int fdstd[3];

    fdstd[0] = dup(fileno(stdin));
    fdstd[1] = dup(fileno(stdout));
    fdstd[2] = dup(fileno(stderr));

    show_prompt();
    command = read_command();
    if (command == -2)
      exit(0);
  }

  return 0;
}