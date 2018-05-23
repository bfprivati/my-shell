#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9010

int server (int client_socket)
{
  while(1)
  {
    int lenght;
    char *text;

    if (read(client_socket, &lenght, sizeof(lenght)) == 0)
      return 0;

    text = (char*)malloc(lenght);

    read(client_socket, text, lenght);
    printf("%s\n", text);
    if (!strcmp(text, "quit"))
    {
      free(text);
      return 1;
    } else {
      free(text);
    }
  }
}

int main(int argc, char* argv[])
{
  char* socket_name = argv[1];
  int socket_fd;
  struct sockaddr_in name;
  int client_sent_quit_message;

  socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  name.sin_family = AF_INET;
  name.sin_port = htons(PORT);
  bind(socket_fd, (struct sockaddr*) &name, sizeof(struct sockaddr_in)); //vincular o descritor de arquivos com o endereço
  listen(socket_fd, 5);

  do{
    struct sockaddr_in client_name;
    socklen_t client_name_len;
    int client_socket_fd;

    //função que bloqueia no descriptor de arquivo?
    client_name_len = sizeof(client_name);
    client_socket_fd = accept(socket_fd, (struct sockaddr*) &client_name, &client_name_len);

    client_sent_quit_message = server(client_socket_fd);
    close (client_socket_fd);
  }while(!client_sent_quit_message);

  close(socket_fd);
  unlink(socket_name);

  return 0;
}
