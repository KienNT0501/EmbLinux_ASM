#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Bind to IP/Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen for connections
    listen(server_fd, 3);
    printf("Server listening...\n");

    // Accept client connection
    socklen_t addrlen = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
    printf("Client connected!\n");

    // Communicate
    read(client_fd, buffer, 1024);
    printf("Received: %s\n", buffer);
    send(client_fd, "Hello from server", 17, 0);

    // Close
    close(client_fd);
    close(server_fd);
}
