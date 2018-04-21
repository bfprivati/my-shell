#include <stdio.h>
#include <unistd.h>

#define IN 0
#define OUT 1

int main()
{
  int fd[2];//Descritor de Arquivo

  pipe(fd);
  //Entrada em fdo
  //Saíde em fd1

  int pid = fork();

  if(pid == 0)//Se for = a 0 é o filho
  {
    close(fd[IN]);//
    char * buf_child = "Hello";
    write(fd[OUT], buf_child, sizeof(buf_child));
  }else{//Se não é o pai
    close(fd[OUT]);// Filho fecha a entrada
    char buf_parent[1024];
    int i;
    for(i=0;i<1024;i++)
      buf_parent[i] = 0;
    read(fd[IN], buf_parent, 1024);
    printf("%s\n", buf_parent);
  }

  return 0;
}
