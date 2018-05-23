#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define PORT 3001

int server (int client_socket) {
    while (1) {
        int length;
        char *text;

        if (read(client_socket, &length, sizeof(length)) == 0)
            return 0;

        text = (char*)malloc(length);

        read(client_socket, text, length);
        printf("%s\n", text);
        if (!strcmp(text, "quit")) {
            free(text);
            return 1;
        } else 
            free(text);
    }
}

int main (int argc, char* argv[]) {
    char * socket_name = argv[1];
    int socket_fd;
    struct sockaddr_in name;
    int client_sent_quit_message;
    struct hostent* hostinfo;
    name.sin_port = htons(PORT);

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    name.sin_family = AF_INET;
    hostinfo = gethostbyname(argv[1]);
    bind(socket_fd, (struct sockaddr*) &name, sizeof(struct sockaddr_in));
    listen(socket_fd, 5);

    do {
        struct sockaddr_in client_name;
        socklen_t client_name_len;
        int client_socket_fd;

        client_name_len =  sizeof(client_name);
        client_socket_fd = accept(socket_fd, (struct sockaddr*) &client_name, &client_name_len);

    } while (!client_sent_quit_message);

    close(socket_fd);
    unlink(socket_name);

    return 0;
}