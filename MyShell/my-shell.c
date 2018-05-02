#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "my-shell.h"

int main() {

  int resp;
  printf("		Menu\n");
  printf("		1 - > out\n");
  printf("		2 - >> append \n");
  printf("		3 - 2> error\n");
  printf("		4 - 2>> error append\n");
  fflush(stdin);
  scanf("%d",&resp);

  switch(resp)
  {
    case 1: func_out();
      break;
    case 2: func_append();
      break;
    case 3: func_error();
      break;
    case 4: func_error_append();
      break;
    default: exit(1);
  }

  return 0;
}
