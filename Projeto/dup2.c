#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
/*
void func1()
{

}
*/
int main()
{
  char * in = "cores.txt";
  char * out = "saida.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);

  close(fdin);
  close(fdout);
  
  char * params[] = {"ls", NULL};
  execvp("ls",params);

  return 0;
}
