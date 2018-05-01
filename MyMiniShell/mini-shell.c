#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// >
void func_out(){
  char * in = "in.txt";
  char * out = "saida.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);

  close(fdin);
  close(fdout);
 
  char * params[] = {"sort",NULL};
  execvp("sort",params);
}

// >>
void func_append(){
  char * in = "in.txt";
  char * out = "saida.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY|O_APPEND, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);

  close(fdin);
  close(fdout);
 
  char * params[] = {"sort",NULL};
  execvp("sort",params);
}

/*
// 2> 
void func_error(){

}

// 2>>
void func_error_append(){

} 
*/

int main()
{
  func_out();
  func_append();
  func_error();
  func_error_append();

  return 0;
}
