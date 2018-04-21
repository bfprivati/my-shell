#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(){

  struct stat res;
  char* target = "petiscos.txt";

  int retorno = stat(target, &res);

  printf("%d", res.st_size );

  return retorno;
}
