#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define PORT 3001

void write_text(int socket_fd, char* text) {
    int length = strlen(text) + 1;
    write(socket_fd, &length, sizeof(length));
    write(socket_fd, text, length);
}

int main (int argc, char* argv[]) {
    char* socket_name = argv[1];
    char* message = argv[2];
    int socket_fd;
    struct sockaddr_in name;
    name.sin_port = htons(PORT);

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    name.sin_family = AF_INET;
    connect(socket_fd, (struct sockaddr*) &name, sizeof(struct sockaddr_in));

    write_text(socket_fd, message);
    close(socket_fd);
    return 0;
}