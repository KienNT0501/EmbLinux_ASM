#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[128];

    // Remove old socket file if exists
    unlink(SOCKET_PATH);

    // Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Bind to a file path
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen and accept
    listen(server_fd, 5);
    printf("Server waiting for connection...\n");

    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected.\n");

    // Communicate
    read(client_fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    write(client_fd, "Hello from UNIX server", 23);

    // Close and clean up
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
