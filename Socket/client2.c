#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9010

void write_text(int socked_fd, char* text)
{
  int lenght = strlen(text)+1;
  write(socked_fd, &lenght, sizeof(lenght));
  write(socked_fd, text, lenght);
}

int main(int argc, char* argv[])
{
  char* socket_name = argv[1];
  char* message = argv[2];
  int socket_fd;
  struct sockaddr_in name;

  socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  name.sin_family = AF_INET;
  name.sin_port = htons(PORT);


  if(connect(socket_fd, (struct sockaddr*) &name, sizeof(struct sockaddr_in)) == -1)
  {
    perror("connect");
    return 1;
  }

  write_text(socket_fd, message);
  close(socket_fd);

  return 0;
}
