#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void func_append(char * file, char * params)
{
  // char * filename = "cores.txt";
  int fd = open(file, O_RDWR, 0);
  trabalho1(fd, 0);
  close(fd);

  if ()

  char * params[] = {"ls", NULL};
  execvp("ls",params);
}

int main()
{
  char * filename = "cores.txt";
  int fd = open(filename, O_RDONLY, 0); // Abertura de Arquivo
                                        //O_RDONLY = Abre apenas no modo Leitura
  //dup2(fd, 0);
  trabalho1(fd, 0);
  close(fd);
  char * params[] = {"ls", NULL};
  func_append(&filename, &params);

  // execvp("ls",params);
  return 0;
}

// >
// >>
// 2>
// 2>>
