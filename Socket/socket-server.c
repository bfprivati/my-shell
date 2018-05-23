#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

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
    struct sockaddr_un name;
    int client_sent_quit_message;

    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socket_name);
    bind(socket_fd, (struct sockaddr*) &name, SUN_LEN(&name));
    listen(socket_fd, 5);

    do {
        struct sockaddr_un client_name;
        socklen_t client_name_len;
        int client_socket_fd;

        client_name_len =  sizeof(client_name);
        client_socket_fd = accept(socket_fd, (struct sockaddr*) &client_name, &client_name_len);

    } while (!client_sent_quit_message);

    close(socket_fd);
    unlink(socket_name);

    return 0;
}