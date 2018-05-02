#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// >
void func_out(){
  char * in = "in.txt";
  char * out = "out.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);

  close(fdin);
  close(fdout);

  char * params[] = {"ls",NULL};
  execvp("ls",params);
}

// >>
void func_append(){
  char * in = "in.txt";
  char * out = "out.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY|O_APPEND, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);

  close(fdin);
  close(fdout);

  char * params[] = {"sort",NULL};
  execvp("sort",params);
}

// 2>
void func_error(){
  char * in = "in.txt";
  char * out = "out.txt";
  char * error = "error.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY|O_APPEND, 0);
  int fderror = open(error, O_WRONLY, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);
  dup2(fderror, 2);

  close(fdin);
  close(fdout);
  close(fderror);

  char * params[] = {"rm", "saidaaaaa.txt", NULL};
  execvp("rm",params);
}

// 2>>
void func_error_append(){
  char * in = "in.txt";
  char * out = "out.txt";
  char * error = "error.txt";

  int fdin = open(in, O_RDONLY, 0);
  int fdout = open(out, O_WRONLY|O_APPEND, 0);
  int fderror = open(error, O_WRONLY|O_APPEND, 0);

  dup2(fdin, 0);
  dup2(fdout, 1);
  dup2(fderror, 2);

  close(fdin);
  close(fdout);
  close(fderror);

  char * params[] = {"rm", "saidaaaaa.txt", NULL};
  execvp("rm",params);
}

int main()
{
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
