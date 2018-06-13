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
#include "my-shell2.h"


int main() {
  int p_in, p_out, p_err;
  char command;
  
  while(1) {
    // Redirecionar para saída/entrada padrão
    p_in = dup(0);
    p_out = dup(1);
    p_err = dup(2);
    
    show_prompt();
    read_command();
    // if (command == -2)
    //   exit(0);
  }

  return 0;
}