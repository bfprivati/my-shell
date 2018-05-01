#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> //Biblioteca da function OPEN
#include <sys/stat.h> //Biblioteca da function OPEN
#include <fcntl.h> //Biblioteca da function OPEN
#include <unistd.h>

#define INN 0
#define OUTT 1

// > : Sobescreve o arquivo
//void func1()
//{
  //char * params[] = {"ls", NULL};
  //execvp("ls",params);
//}

// >> : Escreve no final da página
/*void func_append()
{

}*/

int main()
{
  char * in = "cores.txt"; // Arquivo de entrada
  char * out = "saida.txt";// Arquivo de saída

  //Abertura de Arquivos
  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY, 0);

  //dup2(fdin, 0);
  //dup2(fdout, 1);

  /*
  close(fdin);
  close(fdout);
  
  char * params[] = {"ls", ">>", "" NULL};
  
  execvp("ls",params);
  */

  int fd[2];//Descritor de Arquivo

  pipe(fd); //o que esse pipe esta fazendo aqui

  int pid = fork();

  //Se o Processo filho foi criado
  if (pid == 0) {
    close(fd[INN]);
    dup2(fdout, OUTT);
    close(fdout);
    char* params[] = {"ls", NULL};
    execvp("ls", params);
  } else {
    close(fdout);
    dup2(fdin, INN);
    close(fdin);
    char* params[] = {"ls", NULL};
    execvp("ls", params);
  }

  return 0;
}
