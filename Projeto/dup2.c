#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  char * filename = "cores.txt";
  int fd = open(filename, O_RDONLY, 0);
  dup2(fd, 0);
  close(fd);
  char * params[] = {"more", NULL};
  execvp("more",params);

  return 0;
}
