#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
// >
void func_out(){
  char * params[] = {"ls", NULL};
  execvp("ls",params);
}

// >>
void func_append(){
  char * params[] = {"ls", NULL};
  execvp("ls",params);
}

// 2> 
void func_append_error(){

}

// 2>>
void func_append_error2(){

} */

int main()
{
  char * in = "in.txt";
  char * out = "saida.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_APPEND, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);

  close(fdin);
  close(fdout);
 
  char * params[] = {"sort",NULL};
  execvp("sort",params);

  return 0;
}
